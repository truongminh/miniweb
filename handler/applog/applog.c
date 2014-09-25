/* applog.c
 *
 * Copyright (c) 2013, EPI Technologies
 * written by Nguyen Truong Minh <nguyentrminh at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> /* proper header for S_IRUSR */
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

#include "applog.h"
#include "ds/safe_queue.h"
#include "ds/sds.h"
#include "ds/compile.h"
#include "sys/tlpi_hdr.h"
#include "ds/adlist.h"

static char *_uri = "/applog";
static char *log_dir = "/tmp/relog";

/*
 *  * Default file access permissions for new files.
 *   */

#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE (S_IRUSR | S_IWUSR | S_IXUSR)

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t bmlogKey;
static pthread_mutex_t fd_lock;

static reply* bmlogOK;
static int fd;
static unsigned int offset; /* must be locked */
static list* jobList;
static char fn[PATH_MAX];
static void *log_thread_function(void *arg);
static void open_new_log();
static struct tm* btime;
static int next_hour;

static int _handle(request *req, reply *rep);

static void destructor(void *b)
{
    listDelNode(jobList,(listNode*)b);    
}

#define BUFFER_SIZE (1024*1024) /* 1MB */
struct logbuffer {
    char data[BUFFER_SIZE];
    char *pos;
};

static void create_key(void)
{
    int s = pthread_key_create(&bmlogKey,destructor);
    if (unlikely(s)) {
        errMsg("pthread key create %s",_uri);
    }
    pthread_mutex_init(&fd_lock, NULL);
    bmlogOK = _reply_create();
    reply_stock(bmlogOK,reply_ok,NULL);
    offset = 0;
    jobList = listCreate();

    if(mkdir(log_dir,DIR_MODE) && errno != EEXIST) {
        errExit("Directory [%s]\n", log_dir);
    }
    time_t now = time(NULL);
    btime = malloc(sizeof(struct tm));
    localtime_r(&now, btime);
    open_new_log();
    pthread_t t1;
    s = pthread_create(&t1, NULL, log_thread_function, jobList);
    if (s != 0) {
        errMsg("pthread_create");
    }    
}

static void copy_to_disk(struct logbuffer *buffer)
{
    int to_write = buffer->pos - buffer->data;
    while(to_write > 0) {
        int nwritten = write(fd, buffer->data,to_write);
        /* Content */
        if (nwritten == -1) {
            if (errno == EAGAIN) {
                nwritten = 0;
                usleep(10000); /* sleep 10 ms before try again */
                continue;
            } else {
                errMsg("[%s] write to [%s], data will be discarded", _uri, fn);
                /* suffer loss of data */
                break;
            }
        }
        to_write -= nwritten;
    }
    buffer->pos = buffer->data;
}

void open_new_log()
{
    sprintf(fn,"%s/craftings_%d_%d_%d_%d.log",
            log_dir, btime->tm_year + 1900, btime->tm_mon + 1,
            btime->tm_mday, btime->tm_hour);
    if((fd = open(fn, O_RDWR | O_CREAT , FILE_MODE)) < 0) {
       errMsg("ufile open[%s]",fn);
    }
    next_hour = btime->tm_hour++;
}

static inline void process_string(struct logbuffer *buffer, sds str)
{
    int len = sdslen(str);
    if (likely(len > 0 && len < BUFFER_SIZE)) {
        if(buffer->pos - buffer->data > BUFFER_SIZE - len) {
            // Copy to disk
            copy_to_disk(buffer);            
        }
        memcpy(buffer->pos, str, len);
        buffer->pos += len;
    }
    sdsfree(str);
}

static inline int time_for_new_log()
{
    time_t now = time(NULL);
    localtime_r(&now, btime);
    return btime->tm_hour == next_hour;
}

static void *log_thread_function(void *arg)
{
    list *jobList = (list*) arg;
    int loop = 0;
    struct logbuffer *buffer = malloc(sizeof(struct logbuffer));
    buffer->pos = buffer->data;
    listIter li;
    listNode *ln;
    safeQueue *job;
    sds str;
    int done;
    while(1)
    {
        done = 0;
        listRewind(jobList,&li);
        while ((ln = listNext(&li)) != NULL) {
            job = listNodeValue(ln);
            while((str = safeQueuePop(job)) != NULL)
            {
                process_string(buffer,str);
                done++;
            }
        }
        loop++;

        if (loop == 900) { /* at most 15 minutes */
            loop = 0;
            if(time_for_new_log()) {
                copy_to_disk(buffer);
                open_new_log();
            }
        }
        if (done < 16) sleep(1);
    }
    return NULL;
}

int init(MODULE_TABLE *handlers)
{
    int s;
    s = pthread_once(&once,create_key);
    if (s != 0) {
        errMsg("pthread_once");
        return -1;
    }

    safeQueue *myjob = pthread_getspecific(bmlogKey);
    if (myjob == NULL) {
        myjob = safeQueueCreate();

        pthread_mutex_lock(&fd_lock);
        listAddNodeHead(jobList,myjob);
        pthread_mutex_unlock(&fd_lock);

        s = pthread_setspecific(bmlogKey, myjob);
        if (s != 0) {
            errMsg("pthread_setspecific");
            return -1;
        }
    }

    MODULE_TABLE_add_fixed(handlers,_uri,&_handle);

    return 0;
}

int _handle(request *req, reply *rep)
{
    safeQueue *myjob = (safeQueue*)pthread_getspecific(bmlogKey);
    if(likely(myjob&&req->content)) {
        safeQueuePush(myjob,req->content);
        req->content = NULL; /* don't free this data */
        reply_share_buffer(bmlogOK,rep);
        return 0;
    }
    return -1;
}

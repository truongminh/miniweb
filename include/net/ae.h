/* A simple event-driven programming library. Originally I wrote this code
 * for the Jim's event-loop (Jim is a Tcl interpreter) but later translated
 * it in form of a library for easy reuse.
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
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
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
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

#ifndef __AE_H__
#define __AE_H__

#include <sys/epoll.h>
//#include "lib/adlist.h"
#include "lib/list.h"
#include "lib/kfifo.h"
#include "ccache_config.h"

#define AE_OK 0
#define AE_ERR -1

#define AE_UNACTIVATED 0
#define AE_READABLE EPOLLIN
#define AE_WRITABLE EPOLLOUT

/* Macros */
#define AE_NOTUSED(V) ((void) V)

/* File event structure */
typedef struct ae_socket_event {
    struct epoll_event *ee;
    void *clientData;
} ae_socket_event;


/* State of an event based program */
typedef struct ae_ev_loop {
    int epfd; // epoll file descriptor. Note: fd is simply an int
    // our server has only one epoll instance.
    struct epoll_event newees[AE_MAX_EPOLL_EVENTS];
    int numfds;
    int stop;
    void *apidata; /* This is used for polling API specific data */
    /* for epoll apidata = {epoll fd, array of events} */
    struct list_head clients;
    int myid;
    int numworkers;
    char accepting_clients_buffer[MAX_REQUEST_PER_LOOP];
    struct kfifo *acceptingClients;
#ifdef AE_MAX_CLIENT_PER_WORKER
    unsigned int maxclients;
#endif
#ifdef AE_MAX_CLIENT_IDLE_TIME
    int maxidletime;
#endif
    char buf[CCACHE_IOBUF_LEN]; /* for temporary client data */
    unsigned int processed;
    unsigned int lastSecondProcessed;
    time_t lastSecond;
    unsigned int loop;
} ae_ev_loop;

/* Prototypes */
ae_ev_loop *aeCreateEventLoop(void);
void aeDeleteEventLoop(ae_ev_loop *eventLoop);
void aeStop(ae_ev_loop *eventLoop);
int aeCreateFileEvent(ae_ev_loop *eventLoop, int fd, void *clientData);
int aeModifyFileEvent(ae_ev_loop *eventLoop, int fd, int mask);
int aeDeleteFileEvent(ae_ev_loop *eventLoop, int fd);
void aeProcessEvents(ae_ev_loop *eventLoop);
void aeMain(ae_ev_loop *eventLoop);
char *aeGetApiName(void);

#endif

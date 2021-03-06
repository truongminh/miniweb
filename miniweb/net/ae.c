/* A simple event-driven programming library. Originally I wro  te this code
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

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "net/ae.h"
#include "net/anet.h"
#include "net/client.h"


static ae_socket_event *aeEvents = server.events;
static void workerBeforeSleep(struct ae_ev_loop *eventLoop);

/* We received a SIGTERM,  shuttingdown here in a safe way, as it is
 * not ok doing so inside the signal handler. */
void workerBeforeSleep(struct ae_ev_loop *eventLoop){
    /*if (server.shutdown_asap) {
        if (prepareForShutdown() == CCACHE_OK) exit(0);
        redisLog(CCACHE_WARNING,"SIGTERM received but errors trying to shut down the server, check the logs for more information");
    }
    */
    time_t now = time(NULL);
    if (now > eventLoop->lastSecond) {        
        eventLoop->lastSecond = now;
        eventLoop->loop++;
        eventLoop->lastSecondProcessed = eventLoop->processed;
        eventLoop->processed = 0;
#ifdef AE_MAX_CLIENT_IDLE_TIME    
        if (eventLoop->loop % eventLoop->maxidletime == 0) {
            closeTimedoutClients(eventLoop);
        }
#endif
    }
}

ae_ev_loop *aeCreateEventLoop(void) {
    ae_ev_loop *eventLoop;

    eventLoop = malloc(sizeof(*eventLoop));
    if (!eventLoop) return NULL;

    eventLoop->epfd = epoll_create(AE_MAX_EPOLL_EVENTS); /* 1024 is just a hint for the kernel */
    if (eventLoop->epfd < 0 ) {
        free(eventLoop);
        return NULL;
    }
    eventLoop->stop = 0;
    eventLoop->numfds = 0;
    LIST_HEAD_INIT(eventLoop->clients);
    eventLoop->clients.next = &(eventLoop->clients);
    eventLoop->clients.prev = &(eventLoop->clients);
    eventLoop->acceptingClients = malloc(sizeof(struct kfifo));
    kfifo_init(eventLoop->acceptingClients,eventLoop->accepting_clients_buffer,MAX_REQUEST_PER_LOOP);
#ifdef AE_MAX_CLIENT_PER_WORKER
    eventLoop->maxclients = AE_MAX_CLIENT_PER_WORKER;
#endif
#ifdef AE_MAX_CLIENT_IDLE_TIME
    eventLoop->maxidletime = AE_MAX_CLIENT_IDLE_TIME;
#endif    

    anetCreateSocketPair(NULL, eventLoop->socketpair_fds);
    aeCreateFileEvent(eventLoop, eventLoop->socketpair_fds[1], NULL);
    eventLoop->lastSecond = time(NULL);
    eventLoop->loop = 0;
    eventLoop->lastSecondProcessed = eventLoop->processed;
    eventLoop->processed = 0;
    return eventLoop;
}

void aeDeleteEventLoop(ae_ev_loop *eventLoop) {
    close(eventLoop->epfd);
    free(eventLoop->acceptingClients);
    free(eventLoop);
}

void aeStop(ae_ev_loop *eventLoop) {
    eventLoop->stop = 1;
}

int aeCreateFileEvent(ae_ev_loop *eventLoop, int fd, void *clientData)
{    
    if (fd >= AE_FD_SET_SIZE) return AE_ERR;
    ae_socket_event *fe = aeEvents + fd;
    fe->ee->events = AE_READABLE;
    fe->clientData = clientData;
    /* add/modify event associated with fd to event loop */
    if (epoll_ctl(eventLoop->epfd,EPOLL_CTL_ADD,fd,fe->ee))
        return AE_ERR;
    eventLoop->numfds++;
    return AE_OK;
}

int aeModifyFileEvent(ae_ev_loop *eventLoop, int fd, int mask)
{
    if (fd >= AE_FD_SET_SIZE) return AE_ERR;
    ae_socket_event *fe = aeEvents + fd;
    fe->ee->events = mask;
    /* add/modify event associated with fd to event loop */
    if (epoll_ctl(eventLoop->epfd,EPOLL_CTL_MOD,fd,fe->ee))
        return AE_ERR;
    return AE_OK;
}

int aeDeleteFileEvent(ae_ev_loop *eventLoop, int fd)
{
    ae_socket_event *fe = aeEvents + fd;
    fe->clientData = NULL;
    if (fe->ee->events == AE_UNACTIVATED)
        return AE_ERR; /* safe check */
    /* Note, Kernel < 2.6.9 requires a non null event pointer even for
     * EPOLL_CTL_DEL. */
    if(epoll_ctl(eventLoop->epfd,EPOLL_CTL_DEL,fd,fe->ee))
        return AE_ERR;
    fe->ee->events = AE_UNACTIVATED;
    eventLoop->numfds--;
    return AE_OK;
}

/*
static void aeGetTime(long *seconds, long *milliseconds)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    *seconds = tv.tv_sec;
    *milliseconds = tv.tv_usec/1000;
}
*/


static int acceptNewClient(ae_ev_loop *eventLoop) {
    char buf[1024];
    int nread;

    nread = read(eventLoop->socketpair_fds[1], buf, 1024);

    http_client *c;
    while (kfifo_len(eventLoop->acceptingClients)) {
        /* ... read it, one integer at a time */
        if (kfifo_out(eventLoop->acceptingClients, &c, HTTP_CLIENT_POINTER_SIZE) != HTTP_CLIENT_POINTER_SIZE) {
            printf("kfifo accepting clients %d\n", eventLoop->myid);
            break;
        }

#ifdef AE_MAX_CLIENT_PER_WORKER
        if(eventLoop->numfds > AE_MAX_CLIENT_PER_WORKER) {
            // TODO: write a http reply
            static char *max_client_err = "-ERR MAX CLIENT\r\n";
            /* That's a best effort error message, don't check write errors */
            if (write(c->fd,max_client_err,17) == -1) {
                /* Nothing to do, Just to avoid the warning... */
            }
            freeClient(c);
            continue;
#endif
        }

        c->el = eventLoop;
        list_add(&c->elNode,&eventLoop->clients);
        if (aeCreateFileEvent(eventLoop, c->fd, c) != AE_OK) freeClient(c);
    }

    return 1;
}

void aeProcessEvents(ae_ev_loop *eventLoop)
{

        int numevents = epoll_wait(eventLoop->epfd,eventLoop->newees,AE_MAX_EPOLL_EVENTS,100);
        if(numevents < 1) {
            /* No waiting client */
            usleep(10000);
            return;
        }
        struct epoll_event *newees = eventLoop->newees;
        struct epoll_event *fired_ee;
        int fd;
        ae_socket_event *fe;
        while(numevents--) {
            fired_ee = newees++;
            fd = fired_ee->data.fd;
            fe = aeEvents + fired_ee->data.fd;

            if (fd == eventLoop->socketpair_fds[1]) {                
                acceptNewClient(eventLoop);
            }

            else if (fired_ee->events & fe->ee->events) {
                switch(fired_ee->events) {
                case AE_READABLE:
                    readQueryFromClient(eventLoop,fd,fe->clientData);
                    break;
                case AE_WRITABLE:
                    sendReplyToClient(eventLoop,fd,fe->clientData);
                default:
                    break;
                    sendReplyToClient(eventLoop,fd,fe->clientData);
                }
            }
        };
}

void aeMain(ae_ev_loop *eventLoop) {
    eventLoop->stop = 0;
    while (!eventLoop->stop) {        
        workerBeforeSleep(eventLoop);
        aeProcessEvents(eventLoop);
    }
}

char *aeGetApiName(void) {
    return "epoll";
}

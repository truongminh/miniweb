#ifndef __CLIENT_H
#define __CLIENT_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include "miniweb_config.h"
#include "lib/sds.h"    /* Dynamic safe strings */
#include "lib/util.h"
#include "http/reply.h"
#include "http/request.h"
#include "net/http_server.h"
#include "lib/list.h"

extern httpServer server;

#define CLIENT_OK 0
#define CLIENT_ERR -1
#define CLIENT_NOTUSED(x) ((void)x)


/*-----------------------------------------------------------------------------
 * Data types
 *----------------------------------------------------------------------------*/

typedef struct http_client {
    int fd;    
    char *ip;
    int port;
    reply *rep;
    int bufpos;
    request *req;
    time_t lastinteraction; /* time of the last interaction, used for timeout */
    struct list_head elNode; /* point to the position this clients in its eventLoop's list of clients*/
    int blocked;
    ae_ev_loop *el;
} http_client;

#define HTTP_CLIENT_POINTER_SIZE sizeof(http_client*)

typedef struct {
    int fd;
    char *ip;
    int port;
    int wasAccepted;
} clientEndpoint;


/*-----------------------------------------------------------------------------
 * Functions prototypes
 *----------------------------------------------------------------------------*/

http_client *createClient(ae_ev_loop *el, int fd, const char *ip, int port);
#ifdef AE_MAX_CLIENT_IDLE_TIME
int closeTimedoutClients(ae_ev_loop *el);
#endif

void freeClient(http_client *c);
void resetClient(http_client *c);
void sendReplyToClient(ae_ev_loop *el, int fd, http_client *c);
void readQueryFromClient(ae_ev_loop *el, int fd, http_client *c);

void unblockClient(http_client *c, sds obuf);

/*
Use other mem allocator? NO

#if defined(__GNUC__)
void *calloc(size_t count, size_t size) __attribute__ ((deprecated));
void free(void *ptr) __attribute__ ((deprecated));
void *malloc(size_t size) __attribute__ ((deprecated));
void *realloc(void *ptr, size_t size) __attribute__ ((deprecated));
#endif
*/

#endif

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "net/ae.h"

typedef aeEventLoop* httpWorker;

typedef struct {
    int sport;
    char *sip;
    int sfd;
    httpWorker workers[CCACHE_NUM_WORKER_THREADS];
    int numworkers;
    aeFileEvent events[AE_FD_SET_SIZE]; /* Registered events */
} httpServer;

httpServer server;

void initServer(char *bindaddr, int port);
unsigned int numConcurrentFD();
unsigned int numRequests();

#endif // HTTP_SERVER_H

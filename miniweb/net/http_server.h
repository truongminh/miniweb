#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "net/ae.h"

typedef ae_ev_loop* httpWorker;

typedef struct {
    int sport;
    char *sip;
    int sfd;
    httpWorker workers[CCACHE_NUM_WORKER_THREADS];
    int numworkers;
    ae_socket_event events[AE_FD_SET_SIZE]; /* Registered events */
} httpServer;

httpServer server;

void initServer(char *bindaddr, int port);
unsigned int numConcurrentFD();
unsigned int numRequestsPerSecond();

#define SERVER_NOTUSED(x) ((void)x)

#endif // HTTP_SERVER_H

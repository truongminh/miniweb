#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include "net/anet.h"
#include "net/client.h"
#include "syslib/tlpi_hdr.h"
#include "net/http_server.h"
#include "http/request_handler.h"

static void *aeWorkerThread(void *eventloop);

void *aeWorkerThread(void *eventloop)
{
   ae_ev_loop *el = eventloop;   
   initRequestHandle(el->myid); /* initialize thread-specific data */
   aeMain(el);
   aeDeleteEventLoop(el);
   free(el);
   pthread_exit(NULL);
}

/* Main server loop, where to accept new clients */
static void acceptTcpHandler() {
    int cport, cfd;
    char cip[128];
    int sfd = server.sfd;
    httpWorker *workers = server.workers;

    /* Accept new connection */
    char neterr[ANET_ERR_LEN];
    while(1) {
        cfd = anetTcpAccept(neterr, sfd, cip, &cport);
        if (cfd == AE_ERR) {
            errMsg("Accepting client connection");
            continue;
        }
        /* Create a new client and add it to server.clients list */
        /* NOTICE: must be called via function */
        http_client *c;
        /* NOTICE: not fd but cfd. */
        /* This el is from the master.
         * We may Change el to one from workers.
         */
        //printf("accept %d %.2lf \n",cfd,(double)(clock()));
#if (MNW_NUM_WORKER_THREADS == 4)
        httpWorker nextEL = workers[cfd&0x03];
#else
        httpWorker nextEL = workers[cfd%MNW_NUM_WORKER_THREADS];
#endif
        if ((c = createClient(nextEL,cfd,cip,cport)) == NULL) {
            errMsg("create new clients [%d]",cip);
            continue;
        }
    }
}

void initServer(char *bindaddr, int port)
{    
    server.sport = port;    
    server.sip = strdup(bindaddr);
    server.numworkers = MNW_NUM_WORKER_THREADS;
    int i;
    /* Events with mask == AE_NONE are not set. So let's initialize the
     * vector with it. */
    for (i = 0; i < AE_FD_SET_SIZE; i++) {
        server.events[i].ee = malloc(sizeof(struct epoll_event));
        struct epoll_event *ee = server.events[i].ee;
        ee->data.u64 = 0; /* suppress union cause valgrin check warning */
        ee->data.fd = i;
        ee->events = AE_UNACTIVATED;
    }

    pthread_t threads[MNW_NUM_WORKER_THREADS];
    int rc;
    long t;
    int numworkers = server.numworkers;
    for(t=0; t < numworkers; t++){
      httpWorker worker = aeCreateEventLoop();
      worker->myid = t+1;
      server.workers[t] = worker;
      rc = pthread_create(&threads[t], NULL, aeWorkerThread, worker);
      if (rc){
         printf("ERROR: return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   char neterr[ANET_ERR_LEN];
   int sfd = anetTcpServer(neterr,port,bindaddr);
   if (sfd == ANET_ERR) {
       printf("ERROR: Opening port %d: %s",port, neterr);
       exit(1);
   }
   if (sfd > 0)
       printf("The server is now ready to accept connections on port %d\n",port);
   server.sfd = sfd;
   acceptTcpHandler();
}

static httpWorker *workers = server.workers;

unsigned int numConcurrentFD() {
    int numworkers = server.numworkers;
    int numfd = 0;
    while(numworkers--) {
        numfd += workers[numworkers]->numfds;
    }
    return numfd;
}

unsigned int numRequestsPerSecond() {
    int numworkers = server.numworkers;
    int numProcessed = 0;
    while(numworkers--) {
        numProcessed += workers[numworkers]->lastSecondProcessed;
    }
    return numProcessed;
}

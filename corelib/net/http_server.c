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
   initRequestHandle(); /* initialize thread-specific data */
   aeEventLoop *el = eventloop;
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
        httpClient *c;
        /* NOTICE: not fd but cfd. */
        /* This el is from the master.
         * We may Change el to one from workers.
         */
        //printf("accept %d %.2lf \n",cfd,(double)(clock()));
#if (CCACHE_NUM_WORKER_THREADS == 4)
        httpWorker nextEL = workers[cfd&0x03];
#else
        static int numworkers = server.numworkers;
        httpWorker nextEL = workers[cfd%numworkers];
#endif
        if ((c = createClient(nextEL,cfd,cip,cport)) == NULL) {            
            errMsg("Cannot allocate resource for new clients");
            continue;
        }
    #ifdef AE_MAX_CLIENT_PER_WORKER
        /* Check for max client */
        if (listLength(nextEL->clients) > AE_MAX_CLIENT_PER_WORKER) {
            static char *max_client_err = "-ERR MAX CLIENT\r\n";
            /* That's a best effort error message, don't check write errors */
            if (write(cfd,max_client_err,17) == -1) {
                /* Nothing to do, Just to avoid the warning... */
            }
            freeClient(c);
            return;
        }
    #endif
    }
}

void initServer(char *bindaddr, int port)
{
    server.sport = port;    
    server.sip = strdup(bindaddr);
    server.numworkers = CCACHE_NUM_WORKER_THREADS;
    int i;
    /* Events with mask == AE_NONE are not set. So let's initialize the
     * vector with it. */
    for (i = 0; i < AE_FD_SET_SIZE; i++) {
        struct epoll_event *ee = malloc(sizeof(struct epoll_event));
        ee->data.u64 = 0; /* suppress union cause valgrin check warning */
        ee->data.fd = i;
        ee->events = AE_UNACTIVATED;
        server.events[i].ee = ee;
    }

    pthread_t threads[CCACHE_NUM_WORKER_THREADS];
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
        numfd += workers[numworkers]->maxclients;
    }
    return numfd;
}

unsigned int numRequests() {
    int numworkers = server.numworkers;
    int numProcessed = 0;
    while(numworkers--) {
        numProcessed += workers[numworkers]->processed;
    }
    return numProcessed;
}
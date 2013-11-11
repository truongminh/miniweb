#include <sys/uio.h>
#include "net/client.h"
#include "malloc.h"
#include "net/anet.h"   /* Networking the easy way */
#include "pthread.h"
#include "http/request_handler.h"
#include "lib/util.h"
#include "net/http_server.h"
#include "syslib/tlpi_hdr.h"

static int _installWriteEvent(aeEventLoop *el, httpClient *c);


httpClient *createClient(aeEventLoop *el, int fd, const char* ip, int port) {
    httpClient *c = malloc(sizeof(*c));
    if(!c) return NULL;
    /* Set the socket to nonblock as the default state set by the kernel is blocking or waiting */
    anetNonBlock(NULL,fd);
    anetTcpNoDelay(NULL,fd);
    c->fd = fd;
    c->rep = replyCreate();
    c->bufpos = 0;
    c->req = requestCreate();
    c->lastinteraction = el->lastSecond;
    c->ip = strdup(ip);
    c->port = port;
    c->blocked = 0;
    if(kfifo_in(el->acceptingClients,&c,HTTP_CLIENT_POINTER_SIZE) != HTTP_CLIENT_POINTER_SIZE) {
        free(c);
        return NULL;
    }
    return c;
}


void readQueryFromClient(aeEventLoop *el, int fd, httpClient *c) {
    char *buf = el->buf;
    int nread;
    nread = read(fd, buf, CCACHE_IOBUF_LEN);
    if (nread == -1) {
        if (errno == EAGAIN) { /* try again */
            return;
        } else {
            errMsg("Read data from client");
            freeClient(c);
            return;
        }
    } else if (nread == 0) {                
        freeClient(c);
        return;
    }
    else {
        //printf("Read Request: %.2lf \n", (double)(clock()));
        c->lastinteraction = el->lastSecond;
        list_move_tail(&c->elNode, &el->clients);
        /* NOTICE: nread or nread-1 */
        switch(requestParse(c->req,buf,buf+nread)){
        case parse_not_completed:
            break;
        case parse_completed:
        {
            requestHandle(c->req,c->rep);
            el->processed++;
            if (_installWriteEvent(el, c) != CLIENT_OK) {
                freeClient(c);
                return;
            }
            /* For HANDLE_OK there is nothing to do */
            // if(handle_result == HANDLER_ERR) requestHandleError(c->req,c->rep);
                break;
        }
        case parse_error:
            el->processed++;
            if (_installWriteEvent(el, c) != CLIENT_OK) {
                freeClient(c);
                return;
            }
            requestHandleError(c->req,c->rep);
            break;
        default:
            break;
        };
    }
}

void sendReplyToClient(aeEventLoop *el, int fd, httpClient *c) {
    int nwritten = 0;

    CLIENT_NOTUSED(el);

        sds obuf = replyToBuffer(c->rep);
        int towrite = sdslen(obuf) - c->bufpos;
        nwritten = write(fd, obuf+c->bufpos,towrite);
        /* Content */
        if (nwritten == -1) {
            if (errno == EAGAIN) {
                nwritten = 0;
            } else {
                errMsg("Write data to client: %s");
                freeClient(c);
                return;
            }
        }
        c->lastinteraction = el->lastSecond;
        list_move_tail(&c->elNode, &el->clients);
        if(nwritten < towrite) {
            c->bufpos += nwritten;
        }
        else {
#ifdef AE_MAX_CLIENT_IDLE_TIME
            resetClient(c);
#else
            freeClient(c);
#endif
            aeModifyFileEvent(el,c->fd,AE_READABLE);
            //printf("Send Reply: %.2lf\n", (double)(clock()));
        }
}

void freeClient(httpClient *c) {
    //printf("free client \n");
    aeDeleteFileEvent(c->el,c->fd);
    close(c->fd);
    /* Release memory */
    if(c->ip) free(c->ip);
    replyFree(c->rep);
    requestFree(c->req);
    __list_del_entry(&c->elNode);
    free(c);
}



/* resetClient prepare the client to process the next command */
void resetClient(httpClient *c) {
    c->bufpos = 0;
    replyReset(c->rep);
    requestReset(c->req);
}


#ifdef AE_MAX_CLIENT_IDLE_TIME
int closeTimedoutClients(aeEventLoop *el) {    
    if(el->myid != 0) {
        httpClient *c;
        int deletedNodes = 0;
        time_t now = time(NULL);
        list_for_each_entry_reverse(c, &el->clients, elNode) {
            if (now - c->lastinteraction > el->maxidletime)
            {
                freeClient(c);
                deletedNodes++;
            }
            else break;
        }
        return deletedNodes;
    }
    return 0;
}

#endif

/* Set the event loop to listen for write events on the client's socket.
 * Typically gets called every time a reply is built. */
int _installWriteEvent(aeEventLoop *el, httpClient *c) {
    if (aeModifyFileEvent(el,c->fd,AE_WRITABLE) == AE_ERR)
        return CLIENT_ERR;
    return CLIENT_OK;
}


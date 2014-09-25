#include <sys/uio.h>
#include "net/client.h"
#include "malloc.h"
#include "net/anet.h"   /* Networking the easy way */
#include "pthread.h"
#include "http/request_handler.h"
#include "ds/util.h"
#include "net/http_server.h"
#include "sys/tlpi_hdr.h"

static int _installWriteEvent(ae_ev_loop *el, http_client *c);


http_client *createClient(ae_ev_loop *el, int fd, const char* ip, int port) {
    http_client *c = malloc(sizeof(*c));
    if(!c) return NULL;
    /* Set the socket to nonblock as the default state set by the kernel is blocking or waiting */
    anetNonBlock(NULL,fd);
    anetTcpNoDelay(NULL,fd);
    c->fd = fd;
    c->rep = _reply_create();
    c->bufpos = 0;
    c->req = __request_create();
    c->lastinteraction = el->lastSecond;
    c->ip = strdup(ip);
    c->port = port;
    c->blocked = 0;    
    if(kfifo_in(el->acceptingClients,&c,HTTP_CLIENT_POINTER_SIZE) != HTTP_CLIENT_POINTER_SIZE) {
        free(c);
        return NULL;
    }
    write(el->socketpair_fds[0],"C", 1);
    return c;
}


void readQueryFromClient(ae_ev_loop *el, int fd, http_client *c) {
    request *req = c->req;
    char *buf = RBUF_LAST_IN(req);
    int buf_free = RBUF_FREE(req);
    int nread;
    nread = read(fd, buf, buf_free);
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
        RBUF_INCREASE(req,nread);
        //printf("Read Request: %.2lf \n", (double)(clock()));
        c->lastinteraction = el->lastSecond;
        list_move(&c->elNode, &el->clients);
        /* NOTICE: nread or nread-1 */
        switch(__request_parse(req)){
        case parse_not_completed:
            break;
        case parse_completed:
        {
            requestHandle(req,c->rep);
            el->processed++;
            if (_installWriteEvent(el, c) != CLIENT_OK) {
                freeClient(c);
                return;
            }
            /* For HANDLE_OK there is nothing to do */
            // if(handle_result == HANDLER_ERR) requestHandleError(r,c->rep);
                break;
        }
        case parse_error:
            el->processed++;
            if (_installWriteEvent(el, c) != CLIENT_OK) {
                freeClient(c);
                return;
            }
            requestHandleError(req,c->rep);
            break;
        default:
            break;
        };
    }
}

void sendReplyToClient(ae_ev_loop *el, int fd, http_client *c) {
    int nwritten = 0;

    CLIENT_NOTUSED(el);

        sds obuf = __reply_to_buffer(c->rep);
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

        list_move(&c->elNode, &el->clients);
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

void freeClient(http_client *c) {
    //printf("free client \n");
    aeDeleteFileEvent(c->el,c->fd);
    close(c->fd);
    /* Release memory */
    if(c->ip) free(c->ip);
    _reply_free(c->rep);
    __request_free(c->req);
    __list_del_entry(&c->elNode);
    free(c);
}



/* resetClient prepare the client to process the next command */
void resetClient(http_client *c) {
    c->bufpos = 0;
    _reply_reset(c->rep);
    __request_reset(c->req);
}


#ifdef AE_MAX_CLIENT_IDLE_TIME
int closeTimedoutClients(ae_ev_loop *el) {
    if(el->myid != 0) {
        http_client *c;
        http_client *tmp;
        int deletedNodes = 0;
        time_t now = time(NULL);
        list_for_each_entry_safe_reverse(c, tmp, &el->clients, elNode) {
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
int _installWriteEvent(ae_ev_loop *el, http_client *c) {
    if (aeModifyFileEvent(el,c->fd,AE_WRITABLE) == AE_ERR)
        return CLIENT_ERR;
    return CLIENT_OK;
}


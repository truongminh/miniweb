/* reply.c - http reply
 *
 * Copyright (c) 2013, Nguyen Truong Minh <nguyentrminh at gmail dot com>
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

#include "reply.h"
#include "malloc.h"


reply* _reply_create() {
    reply* r;
    if((r = malloc(sizeof(*r))) == NULL) return NULL;
    r->status = reply_ok;
    r->headers = table8cc_init();
    r->obuf = NULL;
    r->content = sdsempty();
    r->isCached = 0;
    return r;
}

void _reply_free(reply* r) {
    table8cc_free(r->headers);
    sdsfree(r->content);
    if(r->obuf&&r->isCached == 0) sdsfree(r->obuf);
    free(r);
}

void _reply_reset(reply *r) {
    table8cc_make_empty(r->headers);
    sdsclear(r->content);
    if(r->obuf && r->isCached == 0) sdsfree(r->obuf);
    r->obuf = NULL;
}


sds __reply_to_buffer(reply* r) {
    if(r->obuf == NULL) {
        sds obuf = sdsempty();
        obuf = sdscat(obuf,_reply_status_to_string(r->status));
        unsigned int i;
        struct table8cc_entry *h;
         __table8xx_for_each(r->headers, i, h, hlist) {
            obuf = sdscatprintf(obuf,"%s: %s\r\n",h->key,h->value);
        }
        if(r->content) {
            obuf = sdscatprintf(obuf,"Content-Length: %ld",sdslen(r->content));
            obuf = sdscat(obuf,"\r\n\r\n");
            obuf = sdscatsds(obuf,r->content);
        }
        r->obuf = obuf;
    }
    return r->obuf;
}

void reply_share_buffer(reply *src, reply *dst)
{
    dst->obuf = __reply_to_buffer(src);
    dst->isCached = 1;
}

void reply_add_header(reply *r, const char *name, const char *value) {
    return table8cc_add_fixed(r->headers,strdup(name),strdup(value));
}

void reply_set_content(reply *r , char* content){
    r->content = sdscat(r->content,content);
}

void reply_set_status(reply *r , reply_status_type status){

    r->status = status;
}

void reply_stock(reply *r, reply_status_type status, const char *content) {
    r->status = status;
    if (content) r->content = sdsnew(content);
}


char* _reply_status_to_string(reply_status_type status)
{
    switch(status) {
    case reply_ok: return "HTTP/1.1 200 OK\r\n"; break;
    case reply_created: return "HTTP/1.1 201 Created\r\n"; break;
    case reply_accepted: return "HTTP/1.1 202 Accepted\r\n"; break;
    case reply_no_content: return "HTTP/1.1 204 No Content\r\n"; break;
    case reply_multiple_choices: return "HTTP/1.1 300 Multiple Choices\r\n"; break;
    case reply_moved_permanently: return "HTTP/1.1 301 Moved Permanently\r\n"; break;
    case reply_moved_temporarily: return "HTTP/1.1 302 Moved Temporarily\r\n"; break;
    case reply_not_modified: return "HTTP/1.1 304 Not Modified\r\n"; break;
    case reply_bad_request: return "HTTP/1.1 400 Bad Request\r\n"; break;
    case reply_unauthorized: return "HTTP/1.1 401 Unauthorized\r\n"; break;
    case reply_forbidden: return "HTTP/1.1 403 Forbidden\r\n"; break;
    case reply_not_found: return "HTTP/1.1 404 Not Found\r\n"; break;
    case reply_internal_server_error: return "HTTP/1.1 500 Internal Server Error\r\n"; break;
    case reply_not_implemented: return "HTTP/1.1 501 Not Implemented\r\n"; break;
    case reply_bad_gateway: return "HTTP/1.1 502 Bad Gateway\r\n"; break;
    case reply_service_unavailable: return "HTTP/1.1 503 Service Unavailable\r\n"; break;
    default: return "HTTP/1.1 200 OK\r\n"; break;
    }
}

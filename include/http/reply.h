/* reply.h - http reply
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

#ifndef REPLY_H
#define REPLY_H
#include "lib/sds.h"
#include "lib/header.h"

#define REPLY_OK DICT_OK
#define REPLY_ERR DICT_ERR

/* Unused arguments generate annoying warnings... */
#define REPLY_NOTUSED(V) ((void) V)

/// The status of the reply.
typedef enum
{
    reply_ok = 200,
    reply_created = 201,
    reply_accepted = 202,
    reply_no_content = 204,
    reply_multiple_choices = 300,
    reply_moved_permanently = 301,
    reply_moved_temporarily = 302,
    reply_not_modified = 304,
    reply_bad_request = 400,
    reply_unauthorized = 401,
    reply_forbidden = 403,
    reply_not_found = 404,
    reply_internal_server_error = 500,
    reply_not_implemented = 501,
    reply_bad_gateway = 502,
    reply_service_unavailable = 503
} reply_status_type;


/// A reply to be sent to a client.
typedef struct reply_t
{
    reply_status_type status;
    /// The headers to be included in the reply.
    table8cc *headers;
    /// The content to be sent in the reply.
    sds content;
    /// The output buffer could be used in cause we want to cache the reply
    sds obuf;
    int isCached;
} reply;

#define replyToBeCached(r) do{r->isCached = 1;}while(0);

/**
 * _reply_create - create a new reply
 * @return: new reply
 */
reply* _reply_create();

/**
 * _reply_free - free a new reply
 * @r: the reply to be freed
 */
void _reply_free(reply *r);

/**
 * __reply_to_buffer - put headers and content into HTTP reply
 * @r: the reply contains headers and content
 * @return: the buffer, or the raw HTTP reply
 * NOTICE: don't call this function in handler
 */
sds __reply_to_buffer(reply* r);

/**
 * reply_add_header - add a header to the reply
 * @r: the reply
 * @name: header name
 * @value: header value
 */
void reply_add_header(reply* r, const char* name, const char* value);

/**
 * reply_set_content - set content of the reply
 * @r: the reply
 * @content: the content
 */
void reply_set_content(reply*r , char *content);

/**
 * reply_set_status - set status of the reply
 * @r: the reply
 * @vStatus: the status
 */
void reply_set_status(reply*r , reply_status_type vStatus);

/**
 * reply_share_buffer - share content pointer of the reply
 * @src: the reply that has content
 * @dst: the reply that needs the content
 * NOTICE: only the pointer is shared.
 *         The dst reply should not change the content
 */
void reply_share_buffer(reply *src, reply *dst);

/**
 * reply_stock - simple reply with status and content
 * @r: the reply
 * @status: the status
 * @content: the content
 */
void reply_stock(reply *r, reply_status_type status, const char *content);

/**
 * _reply_reset - reset the reply to the initial state
 * @r: the reply
 */
void _reply_reset(reply *r);

/**
 * _reply_status_to_string - get status string from status number
 * @status: the status number
 * @return: the status string
 * NOTICE: Don't change or free the returned status string
 */
char* _reply_status_to_string(reply_status_type status);

#endif // REPLY_H

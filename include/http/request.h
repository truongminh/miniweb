/* request.h - http request
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

#ifndef _REQUEST_H
#define _REQUEST_H

#include "lib/sds.h"
#include "lib/header.h"

#define MAX_REQUEST_SIZE 8096
#define HTTP_CONTENT_LENGTH_HEADER "Content-Length"
#define HTTP_POST_METHOD "POST"
#define REQUEST_BUFFER_ALIGNED

typedef enum {
    parse_not_completed = 0,
    parse_completed = 1,
    parse_error = 2
} request_parse_state;

typedef enum
{
  http_method_start,
  http_method,
  http_uri,
  http_version_h,
  http_version_t_1,
  http_version_t_2,
  http_version_p,
  http_version_slash,
  http_version_major_start,
  http_version_major,
  http_version_minor_start,
  http_version_minor,
  http_expecting_newline_1,
  http_header_line_start,
  http_header_lws,
  http_header_name,
  http_space_before_header_value,
  http_header_value,
  http_expecting_newline_2,
  http_expecting_newline_3,
  http_content
}  http_state;

/* efficient memory alignment */
#ifdef REQUEST_BUFFER_ALIGNED
#define RBUF_SIZE (MAX_REQUEST_SIZE \
    - 4 *sizeof(char*) - 8 * sizeof(int) - 2 * sizeof(table8cc*) \
    - sizeof(sds) - sizeof(http_state) - 16)
#else
#define RBUF_SIZE MAX_REQUEST_SIZE
#endif

#define RBUF_FREE(r) (RBUF_SIZE - r->buf_used)
#define RBUF_LAST_IN(r) (r->buf + r->buf_used)
#define RBUF_LAST_PARSE(r) (r->buf + r->buf_parsed)
#define RBUF_INCREASE(r, _used)  {r->buf_used += _used;}

/* A request received from a client. */
typedef struct
{
    char *method;
    char *uri;
    char *ptr;
    char *current_header_name;

    int first_header;
    int version_major;
    int version_minor;
    int conremain;
    int questionInURI;
    table8cc *queries;
    table8cc *headers;
    sds content;

    /* The current state of the parser. */
    http_state state;

    char buf[RBUF_SIZE];
    int buf_used;
    int buf_parsed;
} request;


/**
 * request_create - allocate and initialize a request
 * @return: the new request
 * NOTICE: don't call this function in handler
 */
request *__request_create();

/**
 * request_free - free a request
 * @r: request to be freed
 * NOTICE: don't call this function in handler
 */
void __request_free(request *r);

/**
 * request_get_header_value - find the value of a given header
 * @r: request to find
 * @key: key to find
 * @return: the pointer to the value of the given header of the request
 *          or NULL if not found
 * NOTICE: caller must not free the returned pointer
 */
char *request_get_header_value(request *r, const char *key);

/**
 * request_query_parse - parse query string
 * @r: request to parse
 */
void request_query_parse(request *r);


/**
 * request_query_find - find the value of a given key in query string
 * @r: request to find
 * @key: key to find
 * return: the value of the given key in the query table of the request
 *          or NULL if not found
 * NOTICE: this function uses an internal query table,
 *         so request_query_parse must be called first and
 *         the caller must not free the returned pointer
 * If you don't want to call request_query_parse first,
 * use the request_query_find_one function.
 */
char *request_query_find(request *r, const char *key);

/**
 * request_query_find_one - find the value of a given key in query string
 * @r: request to find
 * @key: key to find
 * @keylen: len of the given key
 * @return: the value of the given key or NULL if not found
 * NOTICE: caller must free the returned pointer explicitly
 */
char *request_query_find_one(request *r, const char* key, int keylen);

/**
 * request_reset - reset the request to its initial state
 * @r: request to be reset
 * NOTICE: don't call this function in handler
 */
void __request_reset(request *r);

/**
 * request_parse - parse http request into fields
 * @r: request to parse
 * @return: result of parsing,
 *          which should be incompleted, completed or error
 * NOTICE: don't call this function in handler
 */
request_parse_state __request_parse(request* r);

/**
 * request_print - print a request
 * @r: request to print
 */
void _request_print(request *r);

#endif

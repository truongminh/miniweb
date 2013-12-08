/* request.c - http request
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

#include <malloc.h>
#include <stdlib.h> /* for atoi */
#include <ctype.h>
#include <string.h>
#include "http/request.h"
#include "lib/dicttype.h"
#include "lib/compile.h"

/*
 * ********************************* Sample HTTP Request *******************************
 * GET /status HTTP/1.1\r\n                                                            *
 * Host: 192.168.1.1:8888 \r\n                                                         *
 * Connection: keep-alive\r\n                                                          *
 * Cache-Control: max-age=0\r\n                                                        *
 * User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/536.11 (KHTML, like Gecko) \  *
 *             Chrome/20.0.1132.57 Safari/536.11\r\n                                   *
 * Accept-Encoding: gzip,deflate,sdch\r\n                                              *
 * Accept-Language: en-US,en;q=0.8\r\n                                                 *
 * Accept-Charset: UTF-8,*;q=0.5\r\n                                                   *
 * Cookie: __uid=1342231131906312522; __create=1342231131; __C=4\r\n                   *
 * \r\n                                                                                *
 ***************************************************************************************
 * Reference: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */



static inline char *copy_string(const char *src, int len) {
    char *dst = malloc(len+1);
    memcpy(dst,src,len);
    dst[len] = '\0';
    return dst;
}

static inline int _isusual_header_key(char c)
{
    return isalnum(c) || c == '-';
}

static inline int _is_usual_value(char c)
{
    return c > 0 && !iscntrl(c);
}


request *request_create() {
    request* r;
    if((r = malloc(sizeof(*r))) == NULL) return NULL;
    r->method = r->uri = r->current_header_name = NULL;
    r->headers = table8cc_init();
    r->state = http_method_start;
    r->conremain = 0;
    r->content = NULL;
    r->questionInURI = 0;
    r->queries = NULL;
    r->buf_used = 0;
    r->buf_parsed = 0;
    return r;
}

void request_free(request *r) {
    if(r->method) free(r->method);
    if(r->uri) free(r->uri);
    table8cc_free(r->headers);
    if(r->queries) table8cc_free(r->queries);
    /* current_header and current_value was not added to headers */
    if(r->current_header_name) free(r->current_header_name);
    if(r->content) {
        sdsfree(r->content);
        r->content = NULL;
    }
    free(r);
}

char *request_get_header_value(request *r, const char* key){
    return table8cc_find(r->headers,key);
}

void request_query_parse(request *r) {
    if(r->questionInURI) {
        const char* query = r->uri + r->questionInURI + 1;
        // param_seperator
        char seperator = '=';
        register const char *ptr = query;
        char* _key = NULL;
        r->queries = table8cc_init();
        for(;;){
            while(*ptr && *ptr!=seperator) ptr++;
            if(unlikely(*ptr == '\0')) break; // finish
            if(_key) { // have key and value
                table8cc_add_fixed(r->queries,_key,copy_string(query,ptr-query));
                _key = NULL;
                seperator = '=';
            }
            else {
                _key = copy_string(query,ptr-query);
                seperator = '&';
            }
            query = ptr+1;
        };
        if(_key) { // last value
            table8cc_add_fixed(r->queries,_key,copy_string(query,ptr-query));
        }
        table8cc_print(r->queries);
    }
}

char *request_query_find(request *r, const char* key){
    if(r->queries) {
        return table8cc_find(r->queries,key);
    }
    return NULL;
}

/**
 * request_query_find_one - find the value of a given key in query string
 * @r: request to find
 * @key: key to find
 * @keylen: len of the given key
 * return: the value of the given key or NULL if not found
 */

char *request_query_find_one(request *r, const char* key, int keylen) {
    if(r->questionInURI) {
        const char* query = r->uri + r->questionInURI + 1;
        // param_seperator
        char seperator = '=';
        register const char *ptr = query;
        const char* _key = NULL;
        int _keylen;
        int found = 0;
        for(;;){
            while(*ptr && *ptr!=seperator) ptr++;
            if(unlikely(*ptr == '\0')) break; // finish
            if(_key) { // have key and value
                if(found) return copy_string(query,ptr-query);
                _key = NULL;
                seperator = '=';
            }
            else {
                _key = query; _keylen = ptr-query;
                found = (keylen == _keylen)? (memcmp(_key,key,keylen) == 0) : 0;
                seperator = '&';
            }
            query = ptr+1;
        };
        if(found) { // last value
            return copy_string(query,ptr-query);
        }
    }
    return NULL;
}

void request_reset(request *r){
    if(r->method) free(r->method);
    if(r->uri) free(r->uri);

    if(r->current_header_name) free(r->current_header_name);

    r->method = r->uri = r->current_header_name = NULL;

    table8cc_make_empty(r->headers);
    if(r->queries) table8cc_make_empty(r->queries);
#if 0
    /* Check for NULL character at the end of HTTP request */
    char *pcur = RBUF_LAST_PARSE(r);
    char *end = RBUF_LAST_IN(r);
    while(pcur < end || *pcur) {
        pcur++;
    }
    r->buf_parsed = pcur - r->buf;
    int remain = r->buf_used - r->buf_parsed;
    if(remain > 0 ) {
        printf("not parsed %d\t",remain);
        printf("next char [%d]\n",*end);
        /* Use memmove to guarantee correct behavior */
        memmove(r->buf,pcur,remain); // move remaining data to the begin of buffer
    }

    r->buf_used = remain;
    r->buf_parsed = 0;
#else
    r->buf_used = 0;
    r->buf_parsed = 0;
#endif

    r->state = http_method_start;
    r->conremain = 0;
    if(r->content) {
        sdsclear(r->content);
    }
    r->questionInURI = 0;
}


request_parse_state request_parse
(request* r)
{
    request_parse_state result = parse_not_completed;
    char current;
    char *pcur = RBUF_LAST_PARSE(r);
    char *end = RBUF_LAST_IN(r);
    /* requestParse is called when nread >= 1
     * Therefore, end is always greater than pcur
     */
    char *begin = pcur;
    char *header_name = r->current_header_name;
    char *header_value = NULL;
    http_state state = r->state;
    do
    {
        current = *pcur;
        switch (state)
        {
        case http_method_start:
            if (likely(isalpha(current)))
            {                
                state = http_method;
                begin = pcur;
                //*ptr++ = current;
            }
            else
            {                
                result = parse_error;
            }
            break;
        case http_method:
            if (current == ' ')
            {
                r->method = copy_string(begin,pcur-begin);
                state = http_uri;
                begin = pcur + 1;
            }
            else if (likely(isalpha(current)))
            {
                //*begin++ = current;
            }
            else
            {
                result = parse_error;
            }
            break;
        case http_uri:
            if (current == ' ')
            {
                r->uri = copy_string(begin,pcur-begin);
                state = http_version_h;
            }
            else if (unlikely(iscntrl(current)))
            {
                result =  parse_error;
            }
            else
            {
                if(current == '?') {
                    *pcur ='\0';
                    r->questionInURI = pcur - begin;
                }
                else {
                    // *begin++=current;
                }
            }
            break;

        case http_version_h:
            if (likely(current == 'H'))
            {
                state = http_version_t_1;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_version_t_1:
            if (likely(current == 'T'))
            {
                state = http_version_t_2;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_version_t_2:
            if (likely(current == 'T'))
            {
                state = http_version_p;
            }
            else
            {
                result =  parse_error;
            }
            break;
        case http_version_p:
            if (likely(current == 'P'))
            {
                state = http_version_slash;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_version_slash:
            if (likely(current == '/'))
            {
                r->version_major = 0;
                r->version_minor = 0;
                state = http_version_major_start;
            }
            else
            {
                result =  parse_error;
            }
            break;
        case http_version_major_start:
            if (likely(isdigit(current)))
            {
                r->version_major = r->version_major * 10 + current - '0';
                state = http_version_major;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_version_major:
            if (current == '.')
            {
                state = http_version_minor_start;
            }
            else if (likely(isdigit(current)))
            {
                r->version_major = r->version_major * 10 + current - '0';
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_version_minor_start:
            if (likely(isdigit(current)))
            {
                r->version_minor = r->version_minor * 10 + current - '0';
                state = http_version_minor;            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_version_minor:
            if (current == '\r')
            {
                state = http_expecting_newline_1;
            }
            else if (likely(isdigit(current)))
            {
                r->version_minor = r->version_minor * 10 + current - '0';
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_expecting_newline_1:
            if (likely(current == '\n'))
            {
                state = http_header_line_start;                
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_header_line_start:
            if (likely(_isusual_header_key(current)))
            {
                //*begin++=current;
                state = http_header_name;
                begin = pcur;
            }
            else if (current == '\r')
            {
                /* new line after new line, meaning all headers are parsed */
                state = http_expecting_newline_3; /* \r\n\r expect \n */
            }
            /*
            else if (current == ' ' || current == '\t')
            {
                //header_key = copy_string(begin, cur-begin);
                state = http_header_lws;
            }
            */
            else
            {
                result =  parse_error;
            }
            break;

        case http_header_lws:
            if (current == '\r')
            {
                header_value = copy_string(begin, pcur-begin);
                table8cc_add_fixed(r->headers,header_name,header_value);
                header_name = NULL;
                state = http_expecting_newline_2;
            }
            else if (isspace(current)) /* ignore space bfore header_name */
            {
                ;
            }
            else if (likely(_is_usual_value(current)))
            {
                state = http_header_value;
                begin = pcur + 1;
                //*begin++=current;
            }
            else {
                result =  parse_error;
            }
            break;

        case http_header_name:
            if (current == ':') // end of header_name
            {
                header_name = copy_string(begin, pcur-begin);
                state = http_space_before_header_value;                
            }
            else if (likely(_isusual_header_key(current)))
            {
                //*begin++ = current;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_space_before_header_value:
            if (likely(current == ' '))
            {
                state = http_header_value;
                begin = pcur + 1;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_header_value:
            if (likely(_is_usual_value(current)))
            {
                //*begin++=current;
            }
            else if (current == '\r')
            {
                /* <header_key><:><space><header_value><\r\n>*/
                header_value = copy_string(begin, pcur-begin);
                table8cc_add_fixed(r->headers,header_name,header_value);
                header_name = NULL;
                state = http_expecting_newline_2;
            }
            else
            {                
                result =  parse_error;
            }
            break;

        case http_expecting_newline_2:
            if (likely(current == '\n'))
            {
                state = http_header_line_start;                
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_expecting_newline_3:
            if (likely(current == '\n')) {
                /* All headers have been parsed */
                // Check "Content-Length" header                
                char *str = table8cc_find(r->headers,
                                              HTTP_CONTENT_LENGTH_HEADER);
                if(str) {
                    int content_length = atoi(str);
                    if(likely(content_length > 0 && content_length < MAX_REQUEST_SIZE)) {
                        state = http_content;
                        r->conremain = content_length;
                    }
                    else {
                        /* Request content is too long */
                        result = parse_error;
                    }
                }
                else {
                    result =  parse_completed;
                }
            }
            else result =  parse_error;
            break;
        case http_content:
            if(!r->content) r->content = sdsempty();            
            int len = min(r->conremain, end - pcur + 1);
            r->content = sdscatlen(r->content, pcur, len);
            r->conremain -= len;
            if(r->conremain == 0){
                result =  parse_completed;
            }
            else {
                /* All data was copied to r->content */
                pcur = end;
            }
            break;
        default:
            result = parse_error;
            break;
        }
        if (result) {
            break;
        }
    } while(++pcur < end);
    r->buf_parsed = pcur - r->buf;
    r->current_header_name = header_name;
    r->state = state;
    return  result;
}


void requestPrint(request *r){
    printf("%s %s\n",r->method,r->uri);
    table8cc_print(r->headers);

    if(r->content) {
        printf("Content: %s\n",r->content);
    }
    printf("Current Key: %s\n",r->current_header_name);
    printf("STATE: %d\n",r->state);
}

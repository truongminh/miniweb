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


static sds http_content_length_header;

static inline char *copy_string(char *src, int len) {
    char *dst = malloc(len+1);
    memcpy(dst,src,len);
    dst[len] = '\0';
    return dst;
}

static inline int isusualHeaderKey(char c)
{
    return isalnum(c) || c == '-';
}

static inline int isusualValue(char c)
{
    return c > 0 && !iscntrl(c);
}

void requestInit()
{
    http_content_length_header = sdsnew(HTTP_CONTENT_LENGTH_HEADER);
}

request *requestCreate() {
    request* r;
    if((r = malloc(sizeof(*r))) == NULL) return NULL;
    r->method = r->uri = r->current_header_key = NULL;
    r->headers = dictCreate(&charGeneralDictType,NULL);
    r->state = http_method_start;
    r->conremain = 0;
    r->content = NULL;
    r->questionInURI = 0;
    return r;
}

void requestFree(request *r) {
    if(r->method) free(r->method);
    if(r->uri) free(r->uri);
    dictRelease(r->headers);
    /* current_header and current_value was not added to headers */
    if(r->current_header_key) free(r->current_header_key);
    if(r->content) {
        sdsfree(r->content);
        r->content = NULL;
    }
    free(r);
}

sds requestGetHeaderValue(request *r, const sds key){
    return dictFetchValue(r->headers,key);
}

void requestReset(request *r){
    if(r->method) free(r->method);
    if(r->uri) free(r->uri);

    if(r->current_header_key) free(r->current_header_key);

    r->method = r->uri = r->current_header_key = NULL;
    dictRelease(r->headers);
    r->headers = dictCreate(&charGeneralDictType,NULL);

    r->state = http_method_start;
    r->conremain = 0;
    if(r->content) {
        sdsclear(r->content);
    }
    r->questionInURI = 0;
}

request_parse_state requestParse(request* r, char* begin, char* end)
{
    request_parse_state result = parse_not_completed;

    if(unlikely(end - begin > MAX_REQUEST_SIZE)) return parse_error;
    char current;
    char *buf = r->buf;
    char *ptr = r->ptr;
    char *header_key = r->current_header_key;
    char *header_value = NULL;
    http_state state = r->state;
    do
    {
        current = *begin;
        switch (state)
        {
        case http_method_start:
            if (likely(isalpha(current)))
            {
                state = http_method;
                ptr = buf;
                *ptr++ = current;
            }
            else
            {                
                result = parse_error;
            }
            break;
        case http_method:
            if (current == ' ')
            {
                r->method = copy_string(buf,ptr-buf);
                ptr = buf;
                state = http_uri;
            }
            else if (likely(isalpha(current)))
            {
                *ptr++ = current;
            }
            else
            {
                result = parse_error;
            }
            break;
        case http_uri:
            if (current == ' ')
            {
                r->uri = copy_string(buf,ptr-buf);
                ptr=buf;
                state = http_version_h;
            }
            else if (unlikely(iscntrl(current)))
            {
                result =  parse_error;
            }
            else
            {
                if(current == '?') {
                    *ptr++ ='\0';
                    r->questionInURI = ptr-buf;
                }
                else {
                    *ptr++=current;
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
            if (current == '\r')
            {
                /* new line after new line, meaning all headeres are parsed
                 * header_key and header_value are assigned to sdsempty
                 * to simplify the treatment of current_header fields in the request
                 * In particular, assigning the values to sdsempty(),
                 * there is no need to check NULL in requestReset and requestFree
                */
                state = http_expecting_newline_3;
            }
            else if (current == ' ' || current == '\t')
            {
                header_key = copy_string(buf,ptr-buf);
                ptr = buf;
                state = http_header_lws;
            }
            else if (likely(isusualHeaderKey(current)))
            {
                *ptr++=current;
                state = http_header_name;
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_header_lws:
            if (current == '\r')
            {
                header_value = copy_string(buf,ptr-buf);
                dictAdd(r->headers,header_key,header_value);
                header_key = NULL;
                ptr=buf;
                state = http_expecting_newline_2;
            }
            else if (isspace(current)) /* ignore space */
            {
                ;
            }
            else if (likely(isusualValue(current)))
            {
                state = http_header_value;
                *ptr++=current;
            }
            else {
                result =  parse_error;
            }
            break;

        case http_header_name:
            if (current == ':') // end of header_name
            {
                header_key = copy_string(buf,ptr-buf);
                ptr = buf;
                state = http_space_before_header_value;
            }
            else if (likely(isusualHeaderKey(current)))
            {
                *ptr++ = current;
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
            }
            else
            {
                result =  parse_error;
            }
            break;

        case http_header_value:
            if (current == '\r')
            {
                header_value = copy_string(buf,ptr-buf);
                dictAdd(r->headers,header_key,header_value);
                header_key = NULL;
                ptr=buf;
                state = http_expecting_newline_2;
            }
            else if (likely(isusualValue(current)))
            {
                *ptr++=current;
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
                char *str = dictFetchValue(r->headers,HTTP_CONTENT_LENGTH_HEADER);
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
            int len = min(r->conremain, end - begin + 1);
            r->content = sdscatlen(r->content, begin, len);
            r->conremain -= len;
            if(r->conremain == 0){
                result =  parse_completed;
            }
            else {
                /* All data was copied to r->content */
                begin = end;
            }
            break;
        default:
            result = parse_error;
            break;
        }
        if (result) {
            break;
        }
    } while(++begin < end);
    r->ptr = ptr;
    r->current_header_key = header_key;    
    r->state = state;
    return  result;
}


void requestPrint(request *r){
    printf("%s %s\n",r->method,r->uri);
    dictIterator *di = dictGetIterator(r->headers);
    dictEntry *de;
    while((de = dictNext(di)) != NULL) {
        printf("%s: %s\n",(char*)dictGetEntryKey(de),(char*)dictGetEntryVal(de));
    }
    dictReleaseIterator(di);
    if(r->content) {
        printf("Content: %s\n",r->content);
    }
    printf("Current Key: %s\n",r->current_header_key);    
    printf("STATE: %d\n",r->state);
}

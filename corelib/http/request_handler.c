/* request_handler.c
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

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>
#include <dirent.h>

#include "http/request_handler.h"
#include "lib/util.h"
#include "lib/dicttype.h"
#include "syslib/tlpi_hdr.h"

static __thread dict* handlers;

typedef int handlerProc(request *req, reply *rep);
static void loadLib(char *fn, char *initFunc);
static int loadLibDir(const char* dirname);
static reply *r_not_found;

void initRequestHandle()
{
    handlers = dictCreate(&charFunctionDictType, NULL);
    loadLibDir("mnwlib");
    r_not_found = replyCreate();
    replyStock(r_not_found,reply_not_found,"NOT FOUND");
}

int requestHandle(request *req, reply *rep)
{
    handlerProc *handler = dictFetchValue(handlers,req->uri);
    //requestPrint(req);
    if (handler) {
        handler(req,rep);
    } else {
        replyShareBuffer(r_not_found,rep);
    }
    return HANDLER_OK;
}

void requestHandleError(request *req, reply *rep) {
    (void)req;
    replySetStatus(rep,reply_not_found);
    replySetContent(rep,"ERROR");
}

void loadLib(char *fn, char *initFunc)
{
    void *libHandle; /* Handle for shared library */
    int (*funcp)(void *); /* Pointer to function with no arguments */
    const char *err;
    /* Load the shared library and get a handle for later use */
    libHandle = dlopen(fn, RTLD_LAZY);
    if (libHandle == NULL) {
        errMsg("dlopen: %s", dlerror());
        return;
    }
    /* Search library for symbol */
    (void) dlerror(); /* Clear dlerror() */
    funcp = dlsym(libHandle, initFunc);
    err = dlerror();

    if (err != NULL) {
        errMsg("dlsym: %s", err);
        return;
    }
    /* If the address returned by dlsym() is non-NULL, try calling it
    as a function that takes no arguments */
    if (funcp == NULL)
        printf("%s is NULL\n", initFunc);
    else {
        if((*funcp)(handlers)) {
            printf("[ERR] load [%s]\n", fn);
        }
        else {
            printf("[OK] [%s] loaded\n", fn);
        }
    }
    /* dlclose(libHandle); Do not close the library */
}

int loadLibDir(const char* dirname)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dirname)) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
          switch(ent->d_type) {
          case DT_REG:
          {
              sds fn = sdsnew(dirname);
              fn = sdscatprintf(fn, "/%s", ent->d_name);
              loadLib(fn,"init");
              sdsfree(fn);
              break;
          }
          case DT_DIR:
              /* ignore */
              break;
          case DT_LNK:
               /* ignore */
              break;
          default:
              break;
          }
      }
      closedir (dir);
      return EXIT_SUCCESS;
    } else {
      /* could not open directory */
      errMsg("cannot open [%s]\n",dirname);
      return EXIT_FAILURE;
    }
}

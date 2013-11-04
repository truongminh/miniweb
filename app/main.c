/* main.c
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
#include <string.h>
#include <dlfcn.h>
#include "ccache_config.h"
#include "signal_handler.h"
#include "usage.h"
#include "net/http_server.h"

struct mnw_options *options;

void loadLib(char *fn, char *initFunc)
{
    void *libHandle; /* Handle for shared library */
    int (*funcp)(char *, int); /* Pointer to function with no arguments */
    const char *err;
    /* Load the shared library and get a handle for later use */
    libHandle = dlopen(fn, RTLD_LAZY);
    if (libHandle == NULL) {
        printf("dlopen: %s", dlerror());
        return;
    }
    /* Search library for symbol */
    (void) dlerror(); /* Clear dlerror() */
    funcp = dlsym(libHandle, initFunc);
    err = dlerror();

    if (err != NULL) {
        printf("dlsym: %s", err);
        return;
    }
    /* If the address returned by dlsym() is non-NULL, try calling it
    as a function that takes no arguments */
    if (funcp == NULL)
        printf("%s is NULL\n", initFunc);
    else {
        printf("[OK] [%s] loaded\n", fn);
        (*funcp)(options->addr,options->port);
    }
    /* dlclose(libHandle); Do not close the library */
}

int main(int argc, char* argv[])
{
     char *search = strchr(argv[0],'/');
     program_name = (search == NULL)? argv[0]: (search+1);
     setupSignalHandlers();
     options = getOptions(argc,argv);
     loadLib("./libcore.so","initServer");
     return 0;
}


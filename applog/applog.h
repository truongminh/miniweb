#ifndef BMLOG_H
#define BMLOG_H

#include "http/request.h"
#include "http/reply.h"

int init(dict *handlers);
int handle_bmlog(request *req, reply *rep);

#endif // BMLOG_H

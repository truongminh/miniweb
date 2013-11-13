#ifndef BMLOG_H
#define BMLOG_H

#include "http/request.h"
#include "http/reply.h"
#include "lib/dict.h"
#include "lib/dicttype.h"

int init(dict *handlers);

#endif // BMLOG_H

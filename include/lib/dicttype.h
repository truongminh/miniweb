
#ifndef DICTTYPE_H
#define DICTTYPE_H

#include "lib/dict.h"

/* sds dict, keys and values are sds strings. */
dictType sdsDictType;
dictType keylistDictType;
dictType objSdsDictType;
dictType sdsGeneralDictType;
dictType charGeneralDictType;
dictType charFunctionDictType;

#endif // DICTTYPE_H

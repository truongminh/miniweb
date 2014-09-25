/* dictype.c - Frequently used dicttype
 */

#include <malloc.h> /* define NULL value */
#include <string.h> /* memcpy */
#include "dicttype.h"
#include "adlist.h"
#include "sds.h"
#include "objSds.h"

static int dictSdsKeyCompare(void *privdata, const void *key1,
        const void *key2)
{
    int l1,l2;
    DICT_NOTUSED(privdata);

    l1 = sdslen((sds)key1);
    l2 = sdslen((sds)key2);
    if (l1 != l2) return 0;
    return memcmp(key1, key2, l1) == 0;
}

static void dictSdsDestructor(void *privdata, void *val)
{
    DICT_NOTUSED(privdata);

    sdsfree(val);
}


static void dictGeneralDestructor(void *privdata, void *val)
{
    DICT_NOTUSED(privdata);
    free(val);
}

static void dictListDestructor(void *privdata, void *val)
{
    DICT_NOTUSED(privdata);

    listRelease(val);
}

void dictObjSdsDestructor(void *privdata, void *val)
{
    DICT_NOTUSED(privdata);
    objSdsSubRef((objSds*)val);
}

static unsigned int dictSdsHash(const void *key) {
    return dictGenHashFunction((unsigned char*)key, sdslen((char*)key));
}

dictType sdsDictType = {
    dictSdsHash,                /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictSdsKeyCompare,          /* key compare */
    dictSdsDestructor,          /* key destructor */
    dictSdsDestructor           /* val destructor */
};


dictType keylistDictType = {
    dictSdsHash,                /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictSdsKeyCompare,          /* key compare */
    dictSdsDestructor,  /* key destructor */
    dictListDestructor          /* val destructor */
};


dictType objSdsDictType = {
    dictSdsHash,                /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictSdsKeyCompare,          /* key compare */
    dictSdsDestructor,          /* key destructor */
    dictObjSdsDestructor/* val destructor */
};

static unsigned int dictCharKeyHash(const void *key) {
    const unsigned char *buf = key;
    unsigned int hash = 5381;
    while (*buf){
        hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
    }
    return hash;
}


static int dictCharKeyCompare(void *privdata, const void *key1,
        const void *key2)
{
    DICT_NOTUSED(privdata);    
    const char *c1 = key1;
    const char *c2 = key2;
    return strcmp(c1,c2) == 0;
}


dictType sdsGeneralDictType = {
    dictSdsHash,                /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictSdsKeyCompare,          /* key compare */
    dictSdsDestructor,          /* key destructor */
    dictGeneralDestructor           /* val destructor */
};

dictType charGeneralDictType = {
    dictCharKeyHash,           /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictCharKeyCompare,         /* key compare */
    dictGeneralDestructor,      /* key destructor */
    dictGeneralDestructor       /* val destructor */
};

dictType charFunctionDictType = {
    dictCharKeyHash,           /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    dictCharKeyCompare,         /* key compare */
    dictGeneralDestructor,      /* key destructor */
    NULL       /* val destructor */
};


#ifndef MODULE_H_INCLUDED
#define MODULE_H_INCLUDED

#define MINIWEB_INTERNAL __attribute__ ((error("This function is not available for module")))

#include "http/request.h"
#include "http/reply.h"
#include "lib/dict.h"

#define PLUGIN_INIT "init"

#include <malloc.h>
#include <string.h>
#include "lib/list.h"
#include "lib/hash.h"

const char *MODULE_DIR = "./handler";

typedef int handlerProc(request *req, reply *rep);

struct module_node {
    struct hlist_node hlist;
    char *key;
    handlerProc *value;
};

typedef struct hlist_head MODULE_TABLE;

typedef int miniweb_handler(MODULE_TABLE *);

/* 1024 clients, sizeof(void*) = 8 bytes;
   1 module_node table for a request; 1 module_node table for a reply;
   MODULE_TABLE_SIZE = 256
 ==> 1000 clients = 2 * 1024 * 256 * 8 = 4M */

#define MODULE_TABLE_SIZE_BITS 8
#define MODULE_TABLE_SIZE (1<<MODULE_TABLE_SIZE_BITS)
#define __hash_adjust(val,bits) (val >> (32 - bits))
#define MODULE_BUCKET(ht,key) (&ht[__hash_adjust(hash_nt_string(key), MODULE_TABLE_SIZE_BITS)])

/**
 * __MODULE_TABLE_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 */
#define __MODULE_TABLE_for_each(name, bkt, obj, member)				\
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < MODULE_TABLE_SIZE;\
            (bkt)++)\
        hlist_for_each_entry(obj, &name[bkt], member)

/**
 * hash_for_each_safe - iterate over a hashtable safe against removal of
 * hash entry
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @tmp: a &struct used for temporary storage
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 */
#define __MODULE_TABLE_for_each_safe(name, bkt, tmp, obj, member)			\
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < MODULE_TABLE_SIZE;\
            (bkt)++)\
        hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

#define __module_node_key_copy(dst, src) {dst = src;}
#define __module_node_value_copy(dst,src) __module_node_key_copy(dst,src)
#define __module_node_remove_from_table(n) hlist_del_init(n);

static inline int __module_node_key_compare(const char *s1 , const char *s2)
{
    while (*s1 == *s2++)
            if (*s1++ == '\0')
                    return 1;
    return 0;
}

static inline MODULE_TABLE *MODULE_TABLE_init()
{
    MODULE_TABLE *ht = malloc(sizeof(MODULE_TABLE) * MODULE_TABLE_SIZE);
    unsigned int i;
    for (i = 0; i < MODULE_TABLE_SIZE; i++)
        INIT_HLIST_HEAD(&ht[i]);
    return ht;
}

static inline void MODULE_TABLE_make_empty(MODULE_TABLE *ht){
    unsigned int i;
    struct module_node *h;
    struct hlist_node *tmp;
    __MODULE_TABLE_for_each_safe(ht, i, tmp, h, hlist) {
        free(h->key);
        free(h->value);
        __module_node_remove_from_table(&h->hlist);
        free(h);
    }
}

static inline void MODULE_TABLE_free(MODULE_TABLE *ht)
{
    MODULE_TABLE_make_empty(ht);
    free(ht);
}

static inline void MODULE_TABLE_add_fixed(MODULE_TABLE* ht, char *key, handlerProc *value) {
    struct module_node *h = malloc(sizeof(*h));
    __module_node_key_copy(h->key,key);
    __module_node_value_copy(h->value,value);
    INIT_HLIST_NODE(&h->hlist);
    hlist_add_head(&h->hlist, MODULE_BUCKET(ht,key));
}

static inline int MODULE_TABLE_del(MODULE_TABLE* ht, char *key) {
    struct module_node *h_possible;
    MODULE_TABLE *module_node_list = MODULE_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, module_node_list , hlist) {
        if(__module_node_key_compare(key,h_possible->key)) {
            __module_node_remove_from_table(&h_possible->hlist);
            return 1;
        }
    }
    return 0;
}

static inline handlerProc *MODULE_TABLE_find(MODULE_TABLE* ht, const char *key) {
    struct module_node *h_possible;
    MODULE_TABLE *module_node_list = MODULE_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, module_node_list , hlist) {
        if(__module_node_key_compare(key,h_possible->key)) {
            return h_possible->value;
        }
    }
    return NULL;
}

static inline void MODULE_TABLE_print(MODULE_TABLE *ht){
    printf("MODULEs:\n");
    unsigned int i;
    struct module_node *h;

    __MODULE_TABLE_for_each(ht, i, h, hlist) {
        printf("[%s: %p]\n",h->key,h->value);
    }
    char *hfind = "Accept";
    printf("Find: [%s:%p]\n", hfind,MODULE_TABLE_find(ht,hfind));
}

#define __BUF_SIZE 1024
static inline char *MODULE_TABLE_list(MODULE_TABLE *ht){
    unsigned int i;
    struct module_node *h;
    char *buffer = malloc(__BUF_SIZE);
    char *ptr = buffer;
    int len;
    __MODULE_TABLE_for_each(ht, i, h, hlist) {
        len = strlen(h->key);
        if (ptr - buffer + len > __BUF_SIZE - 2) {
            /* \0 and ? only two char is guaranteed */
            *ptr++ = '?';
            break;
        }
        memcpy(ptr,h->key,len);
        ptr += len;
        *ptr++ = '\n';
    }
    *ptr++ = '\0';
    return buffer;
}

#undef __module_node_key_copy
#undef __module_node_value_copy
#undef  __module_node_remove_from_table
#undef  __MODULE_TABLE_for_each
#undef __MODULE_TABLE_for_each_safe
#undef __hash_adjust

#endif

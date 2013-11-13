#ifndef _MINIWEB_HEADER_H
#define _MINIWEB_HEADER_H

#include <malloc.h>
#include <string.h>
#include "lib/list.h"
#include "lib/hash.h"


struct header {
    struct hlist_node hlist;
    char *key;
    char *value;
};

typedef struct hlist_head header_table;

/* 1000 requests + 1000 replies = 2000 * 256 = 512K */
#define HEADER_TABLE_SIZE_BITS 8
#define HEADER_TABLE_SIZE (1<<HEADER_TABLE_SIZE_BITS)
#define __hash_adjust(val,bits) (val >> (32 - bits))
#define HEADER_BUCKET(ht,key) (&ht[__hash_adjust(hash_nt_string(key), HEADER_TABLE_SIZE_BITS)])

/**
 * __header_table_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 */
#define __header_table_for_each(name, bkt, obj, member)				\
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HEADER_TABLE_SIZE;\
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
#define __header_table_for_each_safe(name, bkt, tmp, obj, member)			\
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HEADER_TABLE_SIZE;\
            (bkt)++)\
        hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

#define __header_key_copy(dst, src) {dst = src;}
#define __header_value_copy(dst,src) __header_key_copy(dst,src)
#define __header_remove_from_table(n) hlist_del_init(n);

static inline int __header_key_compare(const char *s1 , const char *s2)
{
    while (*s1 == *s2++)
            if (*s1++ == '\0')
                    return 1;
    return 0;
}

static inline header_table *header_table_init()
{
    header_table *ht = malloc(sizeof(header_table) * HEADER_TABLE_SIZE);
    unsigned int i;
    for (i = 0; i < HEADER_TABLE_SIZE; i++)
        INIT_HLIST_HEAD(&ht[i]);
    return ht;
}

static inline void header_table_make_empty(header_table *ht){
    unsigned int i;
    struct header *h;
    struct hlist_node *tmp;
    __header_table_for_each_safe(ht, i, tmp, h, hlist) {
        free(h->key);
        free(h->value);
        __header_remove_from_table(&h->hlist);
        free(h);
    }
}

static inline void header_table_free(header_table *ht)
{
    header_table_make_empty(ht);
    free(ht);
}

static inline void header_table_add_fixed(header_table* ht, char *key, char *value) {
    struct header *h = malloc(sizeof(*h));
    __header_key_copy(h->key,key);
    __header_value_copy(h->value,value);    
    INIT_HLIST_NODE(&h->hlist);
    hlist_add_head(&h->hlist, HEADER_BUCKET(ht,key));
}

static inline int header_table_del(header_table* ht, char *key) {
    struct header *h_possible;
    header_table *header_list = HEADER_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, header_list , hlist) {
        if(__header_key_compare(key,h_possible->key)) {
            __header_remove_from_table(&h_possible->hlist);
            return 1;
        }
    }
    return 0;
}

static inline char *header_table_find(header_table* ht, const char *key) {
    struct header *h_possible;
    header_table *header_list = HEADER_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, header_list , hlist) {
        if(__header_key_compare(key,h_possible->key)) {
            return h_possible->value;
        }
    }
    return NULL;
}

static inline void header_table_print(header_table *ht){
    printf("Headers:\n");
    unsigned int i;
    struct header *h;

    __header_table_for_each(ht, i, h, hlist) {
        printf("%s: %s\n",h->key,h->value);
    }
    char *hfind = "Accept";
    printf("Find: [%s:%s]\n", hfind,header_table_find(ht,hfind));
}
#endif

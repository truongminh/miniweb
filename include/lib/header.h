#ifndef _MINIWEB_HEADER_H
#define _MINIWEB_HEADER_H

#include <malloc.h>
#include <string.h>
#include "lib/list.h"
#include "lib/hash.h"


struct table8cc_entry {
    struct hlist_node hlist;
    char *key;
    char *value;
};

typedef struct hlist_head table8cc;

/* 1024 clients, sizeof(void*) = 8 bytes;
   1 header table for a request; 1 header table for a reply;
   HEADER_TABLE_SIZE = 256
 ==> 1000 clients = 2 * 1024 * 256 * 8 = 4M */

#define TABLE8XX_SIZE_BITS 8
#define TABLE8XX_SIZE (1<<TABLE8XX_SIZE_BITS)
#define __hash_adjust(val,bits) (val >> (32 - bits))
#define __TABLE8XX_BUCKET(ht,key) (&ht[__hash_adjust(hash_nt_string(key), TABLE8XX_SIZE_BITS)])

/**
 * __header_table_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 */
#define __table8xx_for_each(name, bkt, obj, member)				\
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < TABLE8XX_SIZE;\
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
#define __table8xx_for_each_safe(name, bkt, tmp, obj, member)			\
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < TABLE8XX_SIZE;\
            (bkt)++)\
        hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

#define __table_char_key_copy(dst, src) {dst = src;}
#define __table_char_value_copy(dst,src) __table_char_key_copy(dst,src)
#define __table_remove_entry(n) hlist_del_init(n);

static inline int __table_char_key_compare(const char *s1 , const char *s2)
{
    while (*s1 == *s2++)
            if (*s1++ == '\0')
                    return 1;
    return 0;
}

static inline table8cc *table8cc_init()
{
    table8cc *ht = malloc(sizeof(table8cc) * TABLE8XX_SIZE);
    unsigned int i;
    for (i = 0; i < TABLE8XX_SIZE; i++)
        INIT_HLIST_HEAD(&ht[i]);
    return ht;
}

static inline void table8cc_make_empty(table8cc *ht){
    unsigned int i;
    struct table8cc_entry *h;
    struct hlist_node *tmp;
    __table8xx_for_each_safe(ht, i, tmp, h, hlist) {
        free(h->key);
        free(h->value);
        __table_remove_entry(&h->hlist);
        free(h);
    }
}

static inline void table8cc_free(table8cc *ht)
{
    table8cc_make_empty(ht);
    free(ht);
}

static inline void table8cc_add_fixed(table8cc* ht, char *key, char *value) {
    struct table8cc_entry *h = malloc(sizeof(*h));
    __table_char_key_copy(h->key,key);
    __table_char_value_copy(h->value,value);
    INIT_HLIST_NODE(&h->hlist);
    hlist_add_head(&h->hlist, __TABLE8XX_BUCKET(ht,key));
}

static inline int table8cc_del(table8cc* ht, char *key) {
    struct table8cc_entry *h_possible;
    table8cc *header_list = __TABLE8XX_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, header_list , hlist) {
        if(__table_char_key_compare(key,h_possible->key)) {
            __table_remove_entry(&h_possible->hlist);
            return 1;
        }
    }
    return 0;
}

static inline char *table8cc_find(table8cc* ht, const char *key) {
    struct table8cc_entry *h_possible;
    table8cc *header_list = __TABLE8XX_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, header_list , hlist) {
        if(__table_char_key_compare(key,h_possible->key)) {
            return h_possible->value;
        }
    }
    return NULL;
}

static inline void table8cc_print(table8cc *ht){
    printf("Headers:\n");
    unsigned int i;
    struct table8cc_entry *h;

    __table8xx_for_each(ht, i, h, hlist) {
        printf("[%s: %s]\n",h->key,h->value);
    }
    char *hfind = "Accept";
    printf("Find: [%s:%s]\n", hfind,table8cc_find(ht,hfind));
}

#undef __hash_adjust
#undef __TABLE8XX_BUCKET
#undef __table_char_key_copy
#undef __table_char_value_copy
#undef __table_remove_entry

#endif

#ifndef _MINIWEB_HEADER_H
#define _MINIWEB_HEADER_H

#include <malloc.h>
#include <string.h>
#include "lib/list.h"
#include "lib/hash.h"


struct mnws{
    char *s; /* point to a position on an allocated buffer */
    u32 len; /* len <= 0 means the buffer is nolong usable */
};

#define MNWS_ASSIGN(m,_s,_len) {m.s = _s; m.len = _len;}
#define MNWS_USABLE(m) ((m)&&(m->len > 0))
#define MNWS_MAKE_UNUSABLE(m) m.len = 0;
#define MNWS_LEN(m) ((m).len)
#define MNWS_RESET(m) m.len = 0;

static inline int mnws_compare(struct mnws *m1 , struct mnws *m2)
{
    u32 l1, l2;
    l1 = MNWS_LEN(*m1);
    l2 = MNWS_LEN(*m2);
    return (l1 == l2) ? (memcmp(m1->s,m2->s,l1) == 0) : 0;
}

static inline void mnws_print(struct mnws *m)
{
    if(MNWS_USABLE(m)) {
        char *str = malloc(m->len + 1);
        memcpy(str,m->s,m->len);
        str[m->len] = '\0';
        printf("%s",str);
        free(str);
    }
    else printf("NULL\n");
}

static inline void mnws_print_fixed(char *ptr, int len)
{
    if(ptr) {
        char *str = malloc(len + 1);
        memcpy(str,ptr,len);
        str[len] = '\0';
        printf("[%s]\n",str);
        free(str);
    }
    else
        printf("NULL\n");
}

static inline char *mnws_to_str(struct mnws *m)
{
    if(MNWS_USABLE(m)) {
        char *str = malloc(m->len + 1);
        memcpy(str,m->s,m->len);
        str[m->len] = '\0';
        return str;
    }
    return NULL;
}

struct header {
    struct hlist_node hlist;
    struct mnws key[1];
    struct mnws value[1];
};

typedef struct hlist_head header_table;

/* 1000 requests + 1000 replies = 2000 * 256 = 512K */
#define HEADER_TABLE_SIZE_BITS 8
#define HEADER_TABLE_SIZE (1<<HEADER_TABLE_SIZE_BITS)
#define __hash_adjust(val,bits) (val >> (32 - bits))
#define HEADER_BUCKET(ht,key) (&ht[__hash_adjust(hash_string(key->s, key->len), HEADER_TABLE_SIZE_BITS)])

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

#define __header_key_copy(dst, src) {dst->s = src->s; dst->len = src->len; }
#define __header_value_copy(dst,src) __header_key_copy(dst,src)

#define __header_key_copy_fixed(dst, _s, _len) {dst->s = _s; dst->len = _len; }
#define __header_value_copy_fixed(dst, _s, _len) __header_key_copy_fixed(dst, _s, _len)

#define __header_remove_from_table(n) hlist_del_init(n);

static inline int __header_key_compare(struct mnws *key1 , struct mnws *key2)
{
    return mnws_compare(key1, key2);
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
        __header_remove_from_table(&h->hlist);
        free(h);
    }
}

static inline void header_table_free(header_table *ht)
{
    header_table_make_empty(ht);
    free(ht);
}

static inline void header_table_add(header_table* ht, struct mnws *key, struct mnws *value) {
    struct header *h = malloc(sizeof(*h));
    __header_key_copy(h->key,key);
    __header_value_copy(h->value,value);    
    INIT_HLIST_NODE(&h->hlist);
    hlist_add_head(&h->hlist, HEADER_BUCKET(ht,key));
}

static inline void header_table_add_fixed(header_table* ht, char *key, int key_len, char *value, int value_len) {
    struct header *h = malloc(sizeof(*h));
    __header_key_copy_fixed(h->key,key,key_len);
    __header_value_copy_fixed(h->value,value,value_len);
    INIT_HLIST_NODE(&h->hlist);
    hlist_add_head(&h->hlist, HEADER_BUCKET(ht,h->key));
}

static inline int header_table_del(header_table* ht, struct mnws *key) {
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

static inline struct mnws *header_table_find(header_table* ht, struct mnws *key) {
    struct header *h_possible;
    header_table *header_list = HEADER_BUCKET(ht,key);
    hlist_for_each_entry(h_possible, header_list , hlist) {
        if(__header_key_compare(key,h_possible->key)) {
            return h_possible->value;
        }
    }
    return NULL;
}

static inline struct mnws *header_table_find_str(header_table* ht, char *ckey) {
    struct mnws key;
    key.s = ckey;
    key.len = strlen(ckey);
    return header_table_find(ht,&key);
}

static inline char *header_table_find_str_str(header_table* ht, char *ckey) {
    struct mnws key;
    key.s = ckey;
    key.len = strlen(ckey);
    return mnws_to_str(header_table_find(ht,&key));
}

static inline void header_table_print(header_table *ht){
    printf("Headers:\n");
    unsigned int i;
    struct header *h;

    __header_table_for_each(ht, i, h, hlist) {
        mnws_print(h->key);
        printf(": ");
        mnws_print(h->value);
        printf("\n");
    }
    char *hfind = "Accept";
    printf("Find:\n%s: ",hfind);
    mnws_print(header_table_find_str(ht,hfind));
    printf("\n");
}
#endif

#pragma once

struct sllist;

int    sllist_delelm(struct sllist * *list, const void *del_elem, size_t size_elem);
int    sllist_delnem(struct sllist * *list, const void *del_elem, size_t size_elem, size_t num);
size_t sllist_serelm(struct sllist   *list, size_t is_multi_set, const void *ser_elem, size_t size_elem);
size_t sllist_sernem(struct sllist   *list, size_t is_multi_set, const void *ser_elem, size_t size_elem, size_t num);
void   sllist_addelm(struct sllist * *list, size_t is_multi_set, const void *new_elem, size_t size_elem);
void   sllist_addelp(struct sllist * *list, size_t is_multi_set, const void *new_elem, size_t size_elem, int pos);
void   sllist_dellst(struct sllist * *list);
void   sllist_prints(struct sllist   *list);
void   sllist_getelm(struct sllist   *list, void * *buf, size_t *size_elem, size_t numelm);
void   sllist_getdel(struct sllist * *list, void * *buf, size_t *size_elem, size_t numelm);
struct sllist *sllist_getnxt(struct sllist *list);

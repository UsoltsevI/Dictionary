#pragma once

struct sllist;

struct sllist *new_sllist();

int    sllist_delelm(struct sllist * *list, const void *del_elem, size_t size_elem);
size_t sllist_serelm(struct sllist   *list, const size_t is_multi_set, const void *ser_elem, size_t size_elem);
void   sllist_addelm(struct sllist * *list, const size_t is_multi_set, const void *new_elem, size_t size_elem);
void   sllist_dellst(struct sllist * *list);
void   sllist_prints(struct sllist   *list);
void   sllist_getelm(struct sllist   *list, void * *buf, size_t *size_elem, size_t numelm);
void   sllist_getdel(struct sllist * *list, void * *buf, size_t *size_elem, size_t numelm);


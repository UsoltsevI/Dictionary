#pragma once

struct schmap;

static const size_t SCUHMENF = -0xF;

struct schmap *new_schmap(const size_t initial_quantity);
size_t schmap_serelm(struct schmap *hmap, const void *ser_elem, const size_t size_elem);
void   schmap_delelm(struct schmap *hmap, const void *del_elem, const size_t size_elem);
void   schmap_addelm(struct schmap *hmap, const void *new_elem, const size_t size_elem);
void   schmap_prints(struct schmap *hmap);
void   schmap_delmap(struct schmap * *hmap);

size_t schmap_mpsize(struct schmap *hmap);
size_t schmap_numelm(struct schmap *hmap);

size_t set_num_bit_in_byte_sepchmap(size_t num_bit_in_byte);

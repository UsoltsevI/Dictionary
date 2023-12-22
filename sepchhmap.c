#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sepchhmap.h"
#include "slinklist.h"

static size_t NUM_BIT_IN_BYTE = 8;
static const size_t DELETED   = -0xD;

#define PERROR_PREFIX  fprintf(stderr, "In func <%s> in file <%s> (line %d): ",                        __func__, __FILE__, __LINE__)
#define PERROR_FALLOC  fprintf(stderr, "In func <%s> in file <%s> (line %d): failed allocation\n",     __func__, __FILE__, __LINE__)
#define PERROR_NULLPTR fprintf(stderr, "In func <%s> in file <%s> (line %d): NULL ptr at the input\n", __func__, __FILE__, __LINE__)

struct schmap {
    size_t map_size;
    struct sllist * *map;
    size_t num_elem;
    int    a;
};

static size_t schash(struct schmap *hmap, const void *elem, size_t size_elem);
static int check_input_data(struct schmap *hmap, const void *elem, size_t size_elem);

struct schmap *new_schmap(const size_t initial_quantity) {
    if (initial_quantity == 0) {
        PERROR_PREFIX; fprintf(stderr, "initial_quantity == 0\n");
        return NULL;
    }

    struct schmap *hmap = (struct schmap *) calloc(1, sizeof(struct schmap));

    if (hmap == NULL) {
        PERROR_FALLOC;
        return NULL;
    }

    hmap->map = (struct sllist * *) calloc(initial_quantity, sizeof(struct sllist *));

    if (hmap == NULL) {
        PERROR_FALLOC;
        free(hmap);
        return NULL;
    }

    hmap->map_size = initial_quantity;
    hmap->a        = random() % initial_quantity;

    return hmap;
}

size_t schmap_serelm(struct schmap *hmap, const void *ser_elem, const size_t size_elem) {
    if (check_input_data(hmap, ser_elem, size_elem))
        return 0;

    size_t hash = schash(hmap, ser_elem, size_elem);

    return sllist_serelm(hmap->map[hash], ser_elem, size_elem);
}

void schmap_delelm(struct schmap *hmap, const void *del_elem, const size_t size_elem) {
    if (check_input_data(hmap, del_elem, size_elem))
        return;

    size_t hash = schash(hmap, del_elem, size_elem);

    if (sllist_delelm(&hmap->map[hash], del_elem, size_elem))
        hmap->num_elem--;
}

void schmap_addelm(struct schmap *hmap, const void *new_elem, const size_t size_elem) {
    if (check_input_data(hmap, new_elem, size_elem))
        return;

    size_t hash = schash(hmap, new_elem, size_elem);

    sllist_addelm(&hmap->map[hash], new_elem, size_elem);

    hmap->num_elem++;
}

void schmap_delmap(struct schmap * *hmap) {
    if ((hmap == NULL) || (*hmap == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    for (size_t i = 0; i < (*hmap)->map_size; i++)
        sllist_dellst(&(*hmap)->map[i]);

    free((*hmap)->map);
    free( *hmap);
}

size_t schmap_mpsize(struct schmap *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    return hmap->map_size;
}

size_t schmap_numelm(struct schmap *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    return hmap->num_elem;
}

size_t set_num_bit_in_byte_sepchmap(size_t num_bit_in_byte) {
    if (num_bit_in_byte == 0) {
        PERROR_PREFIX; fprintf(stderr, "num_bit_in_byte == 0\n");
        return NUM_BIT_IN_BYTE;
    }

    size_t prev = NUM_BIT_IN_BYTE;
    NUM_BIT_IN_BYTE = num_bit_in_byte;
    return prev;
}

void schmap_prints(struct schmap *hmap) {
    printf("shhmap_prints {\n");

    if (hmap == NULL) {
        printf("hmap == NULL\n}\n");
        return;
    }

    for (size_t i = 0; i < hmap->map_size; i++) {
        printf("{");
        sllist_prints(hmap->map[i]);
        printf("}, ");
    }

    printf("}\n");
}

static size_t schash(struct schmap *hmap, const void *elem, size_t size_elem) {
    assert(hmap);
    assert(elem);
    assert(size_elem > 0);

    size_t hash = 0;

    for (size_t i = 0; i < size_elem; i++)
        hash = (hash * hmap->a + *((char *) (elem + i))) % hmap->map_size;

    assert(hash < hmap->map_size);

    return hash;

}

static int check_input_data(struct schmap *hmap, const void *elem, size_t size_elem) {
    if ((hmap == NULL) || (elem == NULL)) {
        PERROR_NULLPTR;
        return -1;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return -1;
    }

    return 0;
}

#undef PERROR_PREFIX
#undef PERROR_FALLOC
#undef PERROR_NULLPTR
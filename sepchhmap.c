#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "uhashmap.h"
#include "slinklist.h"

static size_t NUM_BIT_IN_BYTE = 8;
static const size_t DELETED   = -0xD;

#define PERROR_PREFIX  fprintf(stderr, "In func <%s> in file <%s> (line %d): ",                        __func__, __FILE__, __LINE__)
#define PERROR_FALLOC  fprintf(stderr, "In func <%s> in file <%s> (line %d): failed allocation\n",     __func__, __FILE__, __LINE__)
#define PERROR_NULLPTR fprintf(stderr, "In func <%s> in file <%s> (line %d): NULL ptr at the input\n", __func__, __FILE__, __LINE__)

struct uhmap {
    size_t map_size;
    size_t is_multi_set;
    struct sllist * *map;
    size_t num_elem;
    int    a;
};

static size_t schash(struct uhmap *hmap, const void *elem, size_t size_elem);
static int check_input_data(struct uhmap *hmap, const void *elem, size_t size_elem, int line, const char *func, const char *file);

#define CHECK_INPUT_DATA(map, elem, size) \
    check_input_data(map, elem, size, __LINE__, __func__, __FILE__)

struct uhmap *new_uhmap(size_t initial_quantity, size_t is_multi_set) {
    if (initial_quantity == 0) {
        PERROR_PREFIX; fprintf(stderr, "initial_quantity == 0\n");
        return NULL;
    }

    struct uhmap *hmap = (struct uhmap *) calloc(1, sizeof(struct uhmap));

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

    hmap->map_size     = initial_quantity;
    hmap->a            = random() % initial_quantity;
    hmap->is_multi_set = is_multi_set;

    return hmap;
}

size_t uhmap_ctnelm(struct uhmap *hmap, const void *ser_elem, size_t size_elem) {
    if (CHECK_INPUT_DATA(hmap, ser_elem, size_elem))
        return 0;

    size_t hash = schash(hmap, ser_elem, size_elem);

    return sllist_serelm(hmap->map[hash], hmap->is_multi_set, ser_elem, size_elem);
}

size_t uhmap_serelm(struct uhmap *hmap, const void *ser_elem, size_t size_elem) {
    if (CHECK_INPUT_DATA(hmap, ser_elem, size_elem))
        return 0;

    size_t hash = schash(hmap, ser_elem, size_elem);

    return sllist_serelm(hmap->map[hash], 0, ser_elem, size_elem);
}

void uhmap_delelm(struct uhmap *hmap, const void *del_elem, size_t size_elem) {
    if (CHECK_INPUT_DATA(hmap, del_elem, size_elem))
        return;

    size_t hash = schash(hmap, del_elem, size_elem);

    if (sllist_delelm(&hmap->map[hash], del_elem, size_elem))
        hmap->num_elem--;
}

void uhmap_addelm(struct uhmap * *hmap, const void *new_elem, size_t size_elem) {
    if (*hmap == NULL) {
        PERROR_NULLPTR;
        return;
    }

    if (CHECK_INPUT_DATA(*hmap, new_elem, size_elem))
        return;

    size_t hash = schash(*hmap, new_elem, size_elem);

    sllist_addelm(&((*hmap)->map[hash]), (*hmap)->is_multi_set, new_elem, size_elem);

    (*hmap)->num_elem++;
}

void uhmap_resize(struct uhmap * *hmap, size_t new_size) {
    if ((hmap == NULL) || (*hmap == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    if (new_size == 0) {
        PERROR_PREFIX; fprintf(stderr, "new_size == 0\n");
        return;
    }

    struct uhmap *new_hmap = new_uhmap(new_size, (*hmap)->is_multi_set);
    void *buf = NULL;

    for (size_t i = 0; i < (*hmap)->map_size; i++) {
        if ((*hmap)->map[i]) {
            size_t size_elem = 0;
            
            while ((*hmap)->map[i] != NULL) {
                sllist_getdel(&((*hmap)->map[i]), &buf, &size_elem, 0);
                uhmap_addelm(&new_hmap, buf, size_elem);
                free(buf);
                buf = NULL;
            }
        }
    }

    free((*hmap)->map);
    free( *hmap);

    *hmap = new_hmap;
}

void uhmap_delmap(struct uhmap * *hmap) {
    if ((hmap == NULL) || (*hmap == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    for (size_t i = 0; i < (*hmap)->map_size; i++)
        sllist_dellst(&(*hmap)->map[i]);

    free((*hmap)->map);
    free( *hmap);
}

size_t uhmap_mpsize(struct uhmap *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    return hmap->map_size;
}

size_t uhmap_numelm(struct uhmap *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    return hmap->num_elem;
}

size_t uhmap_ismult(struct uhmap *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    return hmap->is_multi_set;
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

void uhmap_prints(struct uhmap *hmap) {
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

static size_t schash(struct uhmap *hmap, const void *elem, size_t size_elem) {
    assert(hmap);
    assert(elem);
    assert(size_elem > 0);

    size_t hash = 0;

    for (size_t i = 0; i < size_elem; i++)
        hash = (hash * hmap->a + *((char *) (elem + i))) % hmap->map_size;

    assert(hash < hmap->map_size);

    return hash;

}

static int check_input_data(struct uhmap *hmap, const void *elem, size_t size_elem, int line, const char *func, const char *file) {
    if ((hmap == NULL) || (elem == NULL)) {
        fprintf(stderr, "In func <%s> in file <%s> (line %d): ", func, file, line);
        fprintf(stderr, "NULL ptr at the input\n");
        return -1;
    }

    if (size_elem == 0) {
        fprintf(stderr, "In func <%s> in file <%s> (line %d): ", func, file, line);
        fprintf(stderr, "invalid size_elem == 0\n");
        return -1;
    }

    return 0;
}

#undef PERROR_PREFIX
#undef PERROR_FALLOC
#undef PERROR_NULLPTR
#undef CHECK_INPUT_DATA

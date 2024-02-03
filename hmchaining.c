#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "uhashmap.h"
#include "slinklist.h"

#define PERROR_PREFIX  fprintf(stderr, "In func <%s> in file <%s> (line %d): ",                        __func__, __FILE__, __LINE__)
#define PERROR_FALLOC  fprintf(stderr, "In func <%s> in file <%s> (line %d): failed allocation\n",     __func__, __FILE__, __LINE__)
#define PERROR_NULLPTR fprintf(stderr, "In func <%s> in file <%s> (line %d): NULL ptr at the input\n", __func__, __FILE__, __LINE__)

struct list_num {
    struct sllist *list;
    size_t num;
};

struct uhmap {
    size_t size;
    size_t num_elem;
    size_t is_multi_set;
    struct sllist *top;
    struct sllist *last;
    struct list_num * *map;
    int a;
};

static size_t chash(struct uhmap *hmap, const void *elem, size_t size_elem);

struct uhmap *new_uhmap(size_t initial_quantity, size_t is_multi_set) {
    if (initial_quantity == 0) {
        PERROR_PREFIX; fprintf(stderr, "initial_quantity -= 0\n");
        return NULL;
    }

    struct uhmap *hmap = (struct uhmap *) calloc(1, sizeof(struct uhmap));

    if (hmap == NULL) {
        PERROR_FALLOC;
        return NULL;
    }

    hmap->map = (struct list_num * *) calloc(initial_quantity, sizeof(struct list_num *));
    hmap->size = initial_quantity;

    if (hmap->map == NULL) {
        PERROR_FALLOC;
        free(hmap);
        return NULL;
    }

    hmap->is_multi_set = is_multi_set;

    if (initial_quantity != 1) {
        hmap->a = random() % initial_quantity;
        
        if (hmap->a == 0)
            hmap->a++;
    }

    assert(hmap->a < hmap->size);

    return hmap;
}

void uhmap_addelm(struct uhmap * *hmap, const void *new_elem, size_t size_elem) {
    if (hmap == NULL || *hmap == NULL || new_elem == NULL) {
        PERROR_NULLPTR;
        return;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "size_elem == 0\n");
        return;
    }

    size_t hash = chash(*hmap, new_elem, size_elem);

    assert(hash < (*hmap)->size);

    if ((*hmap)->map[hash] == NULL) {
        (*hmap)->map[hash] = (struct list_num *) calloc(1, sizeof(struct list_num));

        if ((*hmap)->map[hash] == NULL) {
            PERROR_FALLOC;
            return;
        }
    }

    if ((*hmap)->top == NULL) {
        sllist_addelp(&((*hmap)->top), (*hmap)->is_multi_set, new_elem, size_elem, 0);

        assert((*hmap)->top != NULL);

        (*hmap)->last            = (*hmap)->top;
        (*hmap)->map[hash]->list = (*hmap)->top;
        
    } else if ((*hmap)->map[hash]->num) {
        sllist_addelp(&((*hmap)->map[hash]->list), (*hmap)->is_multi_set, new_elem, size_elem, 1);

        if ((*hmap)->last == (*hmap)->map[hash]->list)
            (*hmap)->last = sllist_getnxt((*hmap)->last);

    } else {
        sllist_addelp(&(*hmap)->last, (*hmap)->is_multi_set, new_elem, size_elem, 1);

        (*hmap)->last            = sllist_getnxt((*hmap)->last);
        (*hmap)->map[hash]->list = (*hmap)->last;

        assert((*hmap)->last != NULL);
    }

    (*hmap)->map[hash]->num += 1;
    (*hmap)->num_elem       += 1;
}

void uhmap_delelm(struct uhmap *hmap, const void *del_elem, size_t size_elem) {
    if (hmap == NULL || del_elem == NULL || size_elem == 0) {
        PERROR_NULLPTR;
        return;
    }

    size_t hash = chash(hmap, del_elem, size_elem);

    if (sllist_delnem(&hmap->map[hash]->list, del_elem, size_elem, hmap->map[hash]->num)) {
        hmap->map[hash]->num -= 1;
        hmap->num_elem       -= 1;
    }
}

void uhmap_resize(struct uhmap * *hmap, size_t new_size) {
    if (hmap == NULL || *hmap == NULL) {
        PERROR_NULLPTR;
        return;
    }

    if (new_size == 0) {
        PERROR_PREFIX; fprintf(stderr, "new_size == 0\n");
        return;
    }

    struct uhmap *new_hmap = new_uhmap(new_size, (*hmap)->is_multi_set);

    new_hmap->a = (*hmap)->a % new_size;

    if (new_hmap == NULL) {
        PERROR_FALLOC;
        return;
    }

    new_hmap->top = (*hmap)->top;

    if (new_hmap->top == NULL) {
        free((*hmap)->map);
        free( *hmap);

        *hmap = new_hmap;
        return;
    }

    struct sllist *cur  = (*hmap)->top;
    struct sllist *next = sllist_getnxt((*hmap)->top);

    void  *buf  = NULL;
    size_t size = 0;
    size_t hash1 = 0, hash2 = 0;

    assert(cur != NULL);

    sllist_getelm(cur, &buf, &size, 0);

    assert(buf  != NULL);
    assert(size != 0);

    hash1 = chash(new_hmap, buf, size);

    free(buf);
    buf = NULL;

    new_hmap->map[hash1] = (struct list_num *) calloc(1, sizeof(struct list_num));

    if (new_hmap->map[hash1] == NULL) {
        PERROR_FALLOC;
        uhmap_delmap(&new_hmap);
        return;
    }

    new_hmap->map[hash1]->list = cur;
    new_hmap->map[hash1]->num  = 1;

    if (next == NULL) {
        free((*hmap)->map);
        free( *hmap);

        *hmap = new_hmap;
        return;
    }

    while (next != NULL) {
        assert(buf == NULL);

        sllist_getelm(next, &buf, &size, 0);

        assert(buf  != NULL);
        assert(size != 0);

        hash2 = chash(new_hmap, buf, size);

        if (hash2 == hash1) {
            new_hmap->map[hash1]->num += 1;

            hash1 = hash2;

            cur  = next;
            next = sllist_getnxt(next);

        } else {
            if (new_hmap->map[hash2] == NULL) {
                new_hmap->map[hash2] = (struct list_num *) calloc(1, sizeof(struct list_num));

                if (new_hmap->map[hash2] == NULL) {
                    PERROR_FALLOC;
                    uhmap_delmap(&new_hmap);
                    return;
                }

                new_hmap->map[hash2]->list = next;
                new_hmap->map[hash2]->num  = 1;

                hash1 = hash2;

                cur  = next;
                next = sllist_getnxt(next);
                
            } else {
                free(buf);
                buf = NULL;
                
                sllist_getdel(&cur, &buf, &size, 1);

                assert(buf != NULL);

                sllist_addelp(&(new_hmap->map[hash2]->list), new_hmap->is_multi_set, buf, size, 1);

                new_hmap->map[hash2]->num += 1;

                next = sllist_getnxt(cur);                
            }
        }

        free(buf);
        buf = NULL;
    }

    free((*hmap)->map);
    free( *hmap);

    *hmap = new_hmap;
    return;
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

#define CHECK_INPUT_DATA(map, elem, size) \
    check_input_data(map, elem, size, __LINE__, __func__, __FILE__)

size_t uhmap_ctnelm(struct uhmap *hmap, const void *ser_elem, size_t size_elem) {
    if (CHECK_INPUT_DATA(hmap, ser_elem, size_elem))
        return 0;

    size_t hash = chash(hmap, ser_elem, size_elem);

    if (hmap->map[hash] == NULL)
        return 0;

    return sllist_sernem(hmap->map[hash]->list, hmap->is_multi_set, ser_elem, size_elem, hmap->map[hash]->num);
}

size_t uhmap_serelm(struct uhmap *hmap, const void *ser_elem, size_t size_elem) {
    if (CHECK_INPUT_DATA(hmap, ser_elem, size_elem))
        return 0;

    size_t hash = chash(hmap, ser_elem, size_elem);

    return sllist_sernem(hmap->map[hash]->list, 0, ser_elem, size_elem, hmap->map[hash]->num);
}

void uhmap_delmap(struct uhmap * *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return;
    }

    if (*hmap == NULL)
        return;
    
    sllist_dellst(&(*hmap)->top);
    free((*hmap)->map);
    free( *hmap);
}

static size_t chash(struct uhmap *hmap, const void *elem, size_t size_elem) {
    assert(hmap);
    assert(elem);
    assert(size_elem > 0);

    size_t hash = 0;

    for (size_t i = 0; i < size_elem; i++)
        hash = (hash * hmap->a + *((char *) (elem + i))) % hmap->size;

    assert(hash < hmap->size);

    return hash;
}


size_t uhmap_mpsize(struct uhmap *hmap) {
    if (hmap == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    return hmap->size;
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

void uhmap_savetb(struct uhmap *hmap, const char *binary_file) {
    if (hmap == NULL || binary_file == NULL) {
        PERROR_NULLPTR;
        return;
    }

#if 0
    FILE *file = fopen(binary_file, w);

    if (file == NULL) {
        PERROR_PREFIX("File opening error");
        perror(" ");
        return;
    }
#endif

    printf("The save function has not been implemented yet");
}

#undef PERROR_PREFIX
#undef PERROR_FALLOC
#undef PERROR_NULLPTR
#undef CHECK_INPUT_DATA
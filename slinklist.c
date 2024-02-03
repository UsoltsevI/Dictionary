#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "slinklist.h"

#define PERROR_PREFIX  fprintf(stderr, "In func <%s> in file <%s> (line %d): ",                        __func__, __FILE__, __LINE__)
#define PERROR_FALLOC  fprintf(stderr, "In func <%s> in file <%s> (line %d): failed allocation\n",     __func__, __FILE__, __LINE__)
#define PERROR_NULLPTR fprintf(stderr, "In func <%s> in file <%s> (line %d): NULL ptr at the input\n", __func__, __FILE__, __LINE__)

struct sllist {
    struct sllist *next;
    size_t size;
    void  *content;
};

void sllist_addelp(struct sllist * *list, size_t is_multi_set, const void *new_elem, size_t size_elem, int pos) {
    if ((list == NULL) || (new_elem == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return;
    }

    if (*list == NULL) {
        *list = (struct sllist *) calloc(1, sizeof(struct sllist));

        if (*list == NULL) {
            PERROR_FALLOC;
            return;
        }

        (*list)->content = malloc(size_elem);

        if ((*list)->content == NULL) {
            PERROR_FALLOC;
            free(*list);
            *list = NULL;
            return;
        }

        memcpy((*list)->content, new_elem, size_elem);
        (*list)->size = size_elem;

        return;
    }

    if (!is_multi_set && sllist_serelm(*list, is_multi_set, new_elem, size_elem))
        return;

    struct sllist *new_node = (struct sllist *) calloc(1, sizeof(struct sllist));

    if (new_node == NULL) {
        PERROR_FALLOC;
        return;
    }

    new_node->content = malloc(size_elem);

    if (new_node->content == NULL) {
        PERROR_FALLOC;
        free(new_node);
        return;
    }

    memcpy(new_node->content, new_elem, size_elem);
    new_node->size = size_elem;

    if (pos > 0) {
        new_node->next = (*list)->next;
        (*list)->next  = new_node;

    } else {
        new_node->next = *list;
        (*list) = new_node;
    }
}

struct sllist *sllist_getnxt(struct sllist *list) {
    if (list == NULL) {
        PERROR_NULLPTR;
        return NULL;
    }

    return list->next;
}

void sllist_addelm(struct sllist * *list, size_t is_multi_set, const void *new_elem, size_t size_elem) {
    if ((list == NULL) || (new_elem == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return;
    }

    struct sllist *last = *list;

    if (last != NULL) {
        while (last->next != NULL)
            last = last->next;

        sllist_addelp(&last, is_multi_set, new_elem, size_elem, 1);
        
    } else {
        sllist_addelp(&last, is_multi_set, new_elem, size_elem, 0);

        *list = last;
    }

#if 0
    struct sllist *last = *list;

    if (last == NULL) {
        *list = (struct sllist *) calloc(1, sizeof(struct sllist));
        last  = *list;

        if (last == NULL) {
            PERROR_FALLOC;
            return;
        }

    } else {
        if (is_multi_set) {
            while (last->next != NULL)
                last = last->next;

        } else {
            while (last->next != NULL) {
                if ((last->size == size_elem) && (!memcmp(last->content, new_elem, size_elem)))
                    return;

                last = last->next;
            }
        }

        last->next = (struct sllist *) calloc(1, sizeof(struct sllist));

        if (last->next == NULL) {
            PERROR_FALLOC;
            return;
        }
    
        last = last->next;
    }

    last->content = calloc(1, size_elem);

    if (last->content == NULL) {
        PERROR_FALLOC;
        return;
    }

    memcpy(last->content, new_elem, size_elem);

    last->size = size_elem;
#endif
}

int sllist_delelm(struct sllist * *list, const void *del_elem, size_t size_elem) {
    return sllist_delnem(list, del_elem, size_elem, -1);
}

int sllist_delnem(struct sllist * *list, const void *del_elem, size_t size_elem, size_t num) {
    if ((list == NULL) || (del_elem == NULL)) {
        PERROR_NULLPTR;
        return -1;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return -1;
    }

    if (num == 0)
        return 0;

    if (*list == NULL) 
        return -1;

    struct sllist *last = *list;

    if ((last->size == size_elem) && (!memcmp(last->content, del_elem, size_elem))) {
        free(last->content);
        *list = last->next;
        free(last);
        return 1;
    }

    size_t n = 0;

    while (last->next != NULL && n < num) {
        if ((last->next->size == size_elem) && (!memcmp(last->next->content, del_elem, size_elem))) {
            free(last->next->content);
            struct sllist *tofree = last->next;
            last->next = last->next->next;
            free(tofree->content);
            free(tofree);
            return 1;
        }

        last = last->next;
        n++;
    }

    return 0;
}

void sllist_getelm(struct sllist *list, void * *buf, size_t *size_elem, size_t numelm) {
    if ((list == NULL)|| (size_elem == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    if (*buf != NULL) {
        PERROR_PREFIX; fprintf(stderr, "*buf != NULL\n");
        return;
    }

    size_t counter = 0;

    while ((list != NULL) && (counter < numelm)) {
        list = list->next;
        counter++;
    }

    if (list == NULL)
        return;

    if (list->content == NULL)
        return;

    *buf = malloc(list->size);
    memcpy(*buf, list->content, list->size);
    *size_elem = list->size;

    assert((*size_elem) != 0);
}

void sllist_getdel(struct sllist * *list, void * *buf, size_t *size_elem, size_t numelm) {
    if ((list == NULL) || (buf == NULL) || (size_elem == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    if (*buf != NULL) {
        PERROR_PREFIX; fprintf(stderr, "*buf != NULL\n");
        return;
    }

    if (*list == NULL)
        return;

    if (numelm == 0) {
        *buf = malloc((*list)->size);

        if (*buf == NULL) {
            PERROR_FALLOC;
            return;
        }

        memcpy(*buf, (*list)->content, (*list)->size);
        struct sllist *tofree = *list;
        *list = (*list)->next;
        free(tofree);
        return;
    }

    size_t counter = 0;
    struct sllist *last = *list;

    while ((last->next != NULL) && (counter < numelm - 1)) {
        last = last->next;
        counter++;
    }

    if (last->next == NULL) {
        printf("last->next == NULL\n");
        return;
    }

    *buf = malloc(last->size);

    if (*buf == NULL) {
        PERROR_FALLOC;
        return;
    }

    memcpy(*buf, last->next->content, last->next->size);
    *size_elem = last->next->size;

    struct sllist *tofree = last->next;

    last->next = last->next->next;
    free(tofree->content);
    free(tofree);
}

size_t sllist_serelm(struct sllist *list, size_t is_multi_set, const void *ser_elem, size_t size_elem) {
    return sllist_sernem(list, is_multi_set, ser_elem, size_elem, -1);
}

size_t sllist_sernem(struct sllist *list, size_t is_multi_set, const void *ser_elem, size_t size_elem, size_t num) {
    if (ser_elem == NULL) {
        PERROR_NULLPTR;
        return 0;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return 0;
    }

    if (list == NULL)
        return 0;

    size_t res = 0;
    size_t n   = 0;

    if (is_multi_set) {
        while (list != NULL && n < num) {
            if ((list->size == size_elem) && (!memcmp(list->content, ser_elem, size_elem)))
                res++;

            list = list->next;
            n++;
        }

    } else {
        while (list != NULL && n < num) {
            if ((list->size == size_elem) && (!memcmp(list->content, ser_elem, size_elem)))
                return 1;

            list = list->next;
            n++;
        }
    }

    return res;
}

void sllist_dellst(struct sllist * *list) {
    if (list == NULL) {
        PERROR_NULLPTR;
        return;
    }

    if (*list == NULL)
        return;

    if ((*list)->next != NULL)
        sllist_dellst(&(*list)->next);

    free((*list)->content);
    free( *list);
}

void sllist_prints(struct sllist *list) {
    printf("sllist_printfs: ");

    if (list == NULL) {
        printf("list == NULL\n");
        return;
    }

    while (list != NULL) {
        for (size_t i = 0; i < list->size; i++)
            printf("%c", *((char *) (list->content + i)));

        printf("->");
        
        list = list->next;
    }

    printf("NULL\n");
}

#undef PERROR_PREFIX
#undef PERROR_FALLOC
#undef PERROR_NULLPTR

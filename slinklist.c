#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slinklist.h"

#define PERROR_PREFIX  fprintf(stderr, "In func <%s> in file <%s> (line %d): ",                        __func__, __FILE__, __LINE__)
#define PERROR_FALLOC  fprintf(stderr, "In func <%s> in file <%s> (line %d): failed allocation\n",     __func__, __FILE__, __LINE__)
#define PERROR_NULLPTR fprintf(stderr, "In func <%s> in file <%s> (line %d): NULL ptr at the input\n", __func__, __FILE__, __LINE__)

struct sllist {
    struct sllist *next;
    size_t size;
    void  *content;
};

void sllist_addelm(struct sllist * *list, const void *new_elem, size_t size_elem) {
    if ((list == NULL) || (new_elem == NULL)) {
        PERROR_NULLPTR;
        return;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return;
    }

    struct sllist *last = *list;

    if (last == NULL) {
        *list = (struct sllist *) calloc(1, sizeof(struct sllist));
        last  = *list;

        if (last == NULL) {
            PERROR_FALLOC;
            return;
        }

    } else {
        while (last->next != NULL)
            last = last->next;

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
}

int sllist_delelm(struct sllist * *list, const void *del_elem, size_t size_elem) {
    if ((list == NULL) || (del_elem == NULL)) {
        PERROR_NULLPTR;
        return -1;
    }

    if (size_elem == 0) {
        PERROR_PREFIX; fprintf(stderr, "invalid size_elem == 0\n");
        return -1;
    }

    if (*list == NULL) 
        return -1;

    struct sllist *last = *list;

    if ((last->size == size_elem) && (!memcmp(last->content, del_elem, size_elem))) {
        free(last->content);
        *list = last->next;
        free(last);
        return 1;
    }

    while (last->next != NULL) {
        if ((last->next->size == size_elem) && (!memcmp(last->next->content, del_elem, size_elem))) {
            free(last->next->content);
            struct sllist *tofree = last->next;
            last->next = last->next->next;
            free(tofree);
            return 1;
        }

        last = last->next;
    }

    return 0;
}

size_t sllist_serelm(struct sllist *list, const void *ser_elem, size_t size_elem) {
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

    while (list != NULL) {
        if ((list->size == size_elem) && (!memcmp(list->content, ser_elem, size_elem)))
            res++;

        list = list->next;
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


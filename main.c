#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "uhashmap.h"

void check_ans(size_t *ans_arr, size_t num_ans);

int main(int argc, char * *argv) {
    size_t num_ans = 0;
    size_t len_dic = 0;
    size_t len_ans = 0;
    size_t last_space = 0;
    size_t num_scanf = 0;

    num_scanf  = scanf("%lu",  &num_ans);
    num_scanf += scanf("%lu ", &len_dic);

    if (num_scanf != 2) {
        fprintf(stderr, "failed scanf (%d)\n", __LINE__);
        return -1;
    }

    size_t *ans_arr = (size_t *) calloc(num_ans + 1, sizeof(size_t));
    char   *buf_dic = (char   *) calloc(len_dic + 2, sizeof(char  ));
    struct uhash_map *hmap = new_uhmap(len_dic, 1);
    
    for (size_t i = 0; i < len_dic; i++) {
        num_scanf = scanf("%c", &buf_dic[i]);

        if (num_scanf != 1) {
            fprintf(stderr, "failed scanf (%d)\n", __LINE__);
            uhmap_delmap(&hmap);
            free(buf_dic); free(ans_arr);
            return -1;
        }

        if (buf_dic[i] == ' ') {
            uhmap_addelm(&hmap, &buf_dic[last_space], i - last_space);
            last_space = i + 1;
        }
    }

    uhmap_addelm(&hmap, &buf_dic[last_space], len_dic - last_space);

    num_scanf = scanf("%lu", &len_ans);

    if (num_scanf != 1) {
        fprintf(stderr, "failed scanf (%d)\n", __LINE__);
        uhmap_delmap(&hmap);
        free(buf_dic); free(ans_arr);
        return -1;
    }

    char *buf_ser = (char *) calloc(len_ans + 2, sizeof(char));

    for (size_t i = 0; i <= len_ans; i++)
        num_scanf += scanf("%c", &buf_ser[i]);

    if (num_scanf != len_ans + 2) {
        fprintf(stderr, "failed scanf (%d)\n", __LINE__);
        uhmap_delmap(&hmap);
        free(buf_dic); free(buf_ser); free(ans_arr);
        return -1;    
    }

    last_space = 1;
    size_t index_ans = 0;

    for (size_t i = 0; i < len_ans; i++) {
        if (buf_ser[i] == ' ') {
            ans_arr[index_ans] = uhmap_ctnelm(hmap, &buf_ser[last_space], i - last_space);
            last_space = i + 1;
            index_ans++;
        }
    }

    ans_arr[index_ans] = uhmap_ctnelm(hmap, &buf_ser[last_space], len_ans + 1 - last_space);

    for (size_t i = 0; i < num_ans; i++)
        printf("%lu ", ans_arr[i]);

    printf("\n");

    uhmap_delmap(&hmap);

    if ((argc == 2) && (!strcmp(argv[1], "check")))
        check_ans(ans_arr, num_ans);

    free(buf_dic);
    free(buf_ser);
    free(ans_arr);

    return 0;
}

void check_ans(size_t *ans_arr, size_t num_ans) {
    assert(ans_arr != NULL);
    size_t a = 0;
    int res_scanf = 0;

    for (size_t i = 0; i < num_ans; i++) {
        res_scanf = scanf("%lu", &a);

        if (res_scanf != 1) {
            printf("failure reading ans\n");
            return;
        }

        if (a != ans_arr[i]) {
            printf("FAILURE\n");
            return;
        }
    }

    printf("OK\n");
}

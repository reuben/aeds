#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct {
    char** vec;
    size_t length;
    size_t capacity;
} string_vec;

string_vec* vec_new(size_t n) {
    string_vec* v = (string_vec*)malloc(sizeof(string_vec));
    v->vec = (char**)malloc(sizeof(char*)*n);
    v->capacity = n;
    v->length = 0;
    return v;
}

void vec_new_inline(size_t n, void* where) {
    string_vec* v = (string_vec*)where;
    v->vec = (char**)malloc(sizeof(char*)*n);
    v->capacity = n;
    v->length = 0;
}

void vec_free(string_vec* v) {
    free(v->vec);
    free(v);
}

void vec_grow(string_vec* v, size_t new_n) {
    char** _new = (char**)malloc(sizeof(char*)*new_n);
    memcpy(_new, v->vec, v->length*sizeof(char*));
    free(v->vec);
    v->vec = _new;
    v->capacity = new_n;
}

void vec_append(string_vec* v, char* i) {
    if (v->length == v->capacity) {
        vec_grow(v, v->capacity*2);
    }
    v->vec[v->length] = i;
    v->length++;
}

char* vec_get(string_vec* v, int i) {
    return v->vec[i];
}

size_t vec_len(string_vec* v) {
    return v->length;
}

int compare_strings(const void* lhs, const void* rhs) {
    char* left = *(char**)lhs;
    char* right = *(char**)rhs;
    return strcmp(right, left);
}

void vec_sort(string_vec* v) {
    qsort(v->vec, v->length, sizeof(char*), compare_strings);
}

//
// in order to avoid copies, we count groups inline in the string data itself,
// as if it were an int. we can do this assuming sizeof(int) <= sizeof(char*),
// which is generally true.
//
int compare_ints(const void* lhs, const void* rhs) {
    int left = *(int*)lhs;
    int right = *(int*)rhs;
    if (left == right) {
        return 0;
    }
    return left < right ? 1 : -1;
}

void vec_sort_as_int(string_vec* v) {
    qsort(v->vec, v->length, sizeof(char*), compare_ints);
}

void vec_groups(string_vec* v) {
    char** vec = v->vec;

    int i = 1;
    char* last = vec[0];
    int group_count = 1;

    int j;
    for (j = 1; j < v->length; ++j) {
        if (strcmp(vec[j], last) != 0) {
            memcpy(&vec[i-1], &group_count, sizeof(int));

            last = vec[j];
            group_count = 1;
            i++;
        } else {
            group_count++;
        }
    }

    memcpy(&vec[i-1], &group_count, sizeof(int));
    v->length = i;
}

#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct {
    int* vec;
    size_t length;
    size_t capacity;
} int_vec;

int_vec* vec_new(size_t n) {
    int_vec* v = (int_vec*)malloc(sizeof(int_vec));
    v->vec = (int*)malloc(sizeof(int)*n);
    v->capacity = n;
    v->length = 0;
    return v;
}

void vec_new_inline(size_t n, void* where) {
    int_vec* v = (int_vec*)where;
    v->vec = (int*)malloc(sizeof(int)*n);
    v->capacity = n;
    v->length = 0;
}

void vec_free(int_vec* v) {
    free(v->vec);
    free(v);
}

void vec_grow(int_vec* v, size_t new_n) {
    int* _new = (int*)malloc(sizeof(int)*new_n);
    memcpy(_new, v->vec, v->length*sizeof(int));
    free(v->vec);
    v->vec = _new;
    v->capacity = new_n;
}

void vec_append(int_vec* v, int i) {
    if (v->length == v->capacity) {
        vec_grow(v, v->capacity*2);
    }
    v->vec[v->length] = i;
    v->length++;
}

int vec_get(int_vec* v, int i) {
    return v->vec[i];
}

size_t vec_len(int_vec* v) {
    return v->length;
}

void vec_uniq(int_vec* v) {
    int* vec = v->vec;

    int i = 1;
    int last = vec[0];
    for (int j = 1; j < v->length; ++j) {
        if (vec[j] != last) {
            last = vec[j];
            vec[i] = vec[j];
            i++;
        }
    }

    v->length = i;
}

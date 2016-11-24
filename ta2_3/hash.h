#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// hash a null terminated string (FNV-1a)
uint32_t hash(char* str) {
    uint32_t h = 2166136261;
    while (*str) {
        h = h ^ *str;
        h = h * 16777619;
        str++;
    }
    return h;
}

typedef struct {
    char* key; // weak
    int value;
} tagged_ptr;

typedef struct {
    tagged_ptr* vec;
    size_t load;
    size_t capacity;
} hash_t;

hash_t* hash_create(size_t capacity) {
    hash_t* h = (hash_t*)malloc(sizeof(hash_t));
    h->load = 0;
    h->vec = (tagged_ptr*)malloc(sizeof(tagged_ptr)*capacity);
    memset(h->vec, 0, sizeof(tagged_ptr)*capacity);
    h->capacity = capacity;
    return h;
}

void hash_free(hash_t* h) {
    free(h->vec);
    free(h);
}

size_t find(hash_t* h, char* key) {
    size_t idx = hash(key) % h->capacity;
    size_t jump = (idx % 97) + 1;
    size_t count = 0;
    while (h->vec[idx].key != NULL &&
           strcmp(h->vec[idx].key, key) != 0 &&
           count < h->capacity) {
        idx = (idx + jump) % h->capacity;
    }
    return idx;
}

int* hash_get(hash_t* h, char* key) {
    size_t idx = find(h, key);
    if (h->vec[idx].key == NULL || strcmp(h->vec[idx].key, key) != 0) {
        return NULL;
    }
    return &(h->vec[idx].value);
}

void hash_grow(hash_t*, size_t);

bool hash_insert(hash_t* h, char* key, int value) {
    size_t idx = find(h, key);
    if (h->vec[idx].key != NULL) {
        return false;
    }

    if (((float)h->load/(float)h->capacity) > 0.75f) {
        hash_grow(h, h->capacity*2);
        idx = find(h, key);
    }
    // don't bother shrinking, we never remove elements from the hash

    h->vec[idx].key = key;
    h->vec[idx].value = value;
    h->load++;
    return true;
}

void hash_grow(hash_t* h, size_t new_n) {
    hash_t* new_h = hash_create(new_n);
    for (int i = 0; i < h->capacity; ++i) {
        if (h->vec[i].key != NULL) {
            hash_insert(new_h, h->vec[i].key, h->vec[i].value);
            free(h->vec[i].key);
        }
    }
    free(h->vec);
    assert(h->load == new_h->load);
    h->vec = new_h->vec;
    h->capacity = new_n;
}

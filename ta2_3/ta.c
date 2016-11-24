#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int i;
    for (i = 0; i < h->capacity; ++i) {
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

void hash_insert_or_add(hash_t* h, char* key, int value) {
    int* v = hash_get(h, key);
    if (!v) {
        hash_insert(h, key, value);
    } else {
        *v += value;
    }
}

int ascii_to_index(char c) {
    return c - 'a';
}

int main(void) {
    FILE* fin = stdin; //fopen("input", "r");

    // https://www.urionlinejudge.com.br/judge/en/problems/view/1519

    char line[10005];

    while (1) {
        fgets(line, 10001, fin);
        if (line[0] == '.' && line[1] == '\0') {
            break;
        }

        // add words to hash table (value is character count)
        //     update if existing

        hash_t* h = hash_create(700); // ~10^4/15

        char* start = line;
        char* end = line+1;
        while (*end) {
            if (*end == ' ' || *end == '\n') {
                *end = 0;
                if (end - start > 2) {
                    hash_insert_or_add(h, start, end-start-2);
                }
                start = end+1;
                end = start;
                if (*start == '\n') {
                    *start = 0;
                    break;
                }
            }
            end++;
        }
        // hack: mark end of input
        *(end+1) = 1;

        // iterate over all keys, find largest character count for all 26 abbreviations
        char* largest_key[26] = {0};
        int largest_count[26] = {0};

        tagged_ptr* vec = h->vec;
        int i;
        for (i = 0; i < h->capacity; ++i) {
            if (vec[i].key != NULL) {
                int idx = ascii_to_index(vec[i].key[0]);
                if (vec[i].value > largest_count[idx]) {
                    largest_count[idx] = vec[i].value;
                    largest_key[idx] = vec[i].key;
                }
            }
        }

        // iterate over input, compare with abbreviation at end of every word, print
        start = line;
        end = line+1;
        while (*start != 1 && *end != 1) {
            if (*end == 0) {
                int idx = ascii_to_index(*start);
                if (largest_key[idx] && !strcmp(start, largest_key[idx])) {
                    printf("%c.%s", *start, *(end+1) == 1 ? "" : " ");
                } else {
                    printf("%s%s", start, *(end+1) == 1 ? "" : " ");
                }
                start = end+1;
                end = start;
            }
            end++;
        }
        // fuck the police
        fseek(stdout, -1, SEEK_CUR);
        printf("\n");

        int abbrevs = 0;
        for (i = 0; i < 26; ++i) {
            if (largest_key[i]) {
                abbrevs++;
            }
        }
        printf("%d\n", abbrevs);

        for (i = 0; i < 26; ++i) {
            if (largest_key[i]) {
                printf("%c. = %s\n", 'a' + i, largest_key[i]);
            }
        }
    }

    return 0;
}

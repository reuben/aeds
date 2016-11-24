#include <stdio.h>
#include <string.h>

#include "string_vec.h"

static const int MAX_WORD_SIZE = 50;
static const int WORDS_PER_LINE = 1000000;
const size_t MAX_LINE_SIZE = MAX_WORD_SIZE*WORDS_PER_LINE;

static char* shared_line_buffer = NULL;

char* read_line(FILE* fin) {
    fgets(shared_line_buffer, MAX_LINE_SIZE, fin);
    return shared_line_buffer;
}

int compare_chars(const void* lhs, const void* rhs) {
    char* left = (char*)lhs;
    char* right = (char*)rhs;

    if (*left == *right) {
        return 0;
    }
    return *left < *right ? -1 : 1;
}

void process_line(FILE* fin) {
    char* line = read_line(fin);
    string_vec* words = vec_new(WORDS_PER_LINE/100); // conservative estimate

    char* start = line;
    char* token = line;
    while (*token) {
        if (*token == ' ' || *token == '\n') {
            *token = 0;
            qsort(start, strlen(start), sizeof(char), compare_chars);
            vec_append(words, start);
            start = token+1;
        }
        token++;
    }

    vec_sort(words);
    vec_groups(words);
    vec_sort_as_int(words);

    int i;
    for (i = 0; i < vec_len(words)-1; ++i) {
        printf("%d ", (int)vec_get(words, i));
    }
    printf("%d\n", (int)vec_get(words, i));

    vec_free(words);
}

int main(void) {
    FILE* fin = stdin; // fopen("input", "r");
    int lines;
    fscanf(fin, "%d\n", &lines);

    shared_line_buffer = (char*)malloc(sizeof(char)*MAX_LINE_SIZE+1);

    int i;
    for (i = 0; i < lines; ++i) {
        process_line(fin);
    }

    free(shared_line_buffer);

    return 0;
}

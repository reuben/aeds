#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int to;
    int weight;
} edge_t;

typedef struct {
    edge_t* vec;
    size_t length;
    size_t capacity;
} edge_vec;

edge_vec* vec_new(size_t n) {
    edge_vec* v = (edge_vec*)malloc(sizeof(edge_vec));
    v->vec = (int*)malloc(sizeof(int)*n);
    v->capacity = n;
    v->length = 0;
    return v;
}

void vec_new_inline(size_t n, void* where) {
    edge_vec* v = (edge_vec*)where;
    v->vec = (int*)malloc(sizeof(int)*n);
    v->capacity = n;
    v->length = 0;
}

void vec_free(edge_vec* v) {
    free(v->vec);
    free(v);
}

void vec_grow(edge_vec* v, size_t new_n) {
    int* _new = (int*)malloc(sizeof(int)*new_n);
    memcpy(_new, v->vec, v->length*sizeof(int));
    free(v->vec);
    v->vec = _new;
    v->capacity = new_n;
}

void vec_append(edge_vec* v, int i) {
    if (v->length == v->capacity) {
        vec_grow(v, v->capacity*2);
    }
    v->vec[v->length] = i;
    v->length++;
}

int vec_get(edge_vec* v, int i) {
    return v->vec[i];
}

size_t vec_len(edge_vec* v) {
    return v->length;
}

typedef struct {
    int vertices;
    edge_vec* edges;
} graph_t;

graph_t* graph_new(int vertices) {
    graph_t* g = (graph_t*)malloc(sizeof(graph_t));
    g->vertices = vertices;
    g->edges = (edge_vec*)malloc(sizeof(edge_vec)*vertices);
    int i;
    for (i = 0; i < vertices; ++i) {
        // edges only at square angles -> max degree = 4, *2 for undirectedness
        vec_new_inline(8, &g->edges[i]);
    }
    return g;
}

void graph_append_edge(graph_t* g, int from, int to) {
    assert(from < g->vertices);
    assert(to < g->vertices);

    vec_append(&g->edges[from], to);
}

graph_t* from_input(FILE* fin) {
    int vertices, edges;
    fscanf(fin, "%d %d", &vertices, &edges);
    graph_t* g = graph_new(vertices);

    int i;
    for (i = 0; i < edges; ++i) {
        int from, to;
        fscanf(fin, "%d %d\n", &from, &to);
        graph_append_edge(g, from, to);
        graph_append_edge(g, to, from);
    }

    return g;
}

bool dfs_visit(graph_t* g, int start, int* parents) {
    assert(start < g->vertices);
    int i;
    for (i = 0; i < vec_len(&g->edges[start]); ++i) {
        int vertex = vec_get(&g->edges[start], i);
        if (!vec_linear_search(ugly_global_of_doom, vertex)) {
            vec_append(ugly_global_of_doom, vertex);
            length += counting_dfs(g, vertex, 1);
        }
    }
    return length;
}

bool has_cycle(graph_t* g) {
    ugly_global_of_doom = vec_new(g->vertices);
    vec_append(ugly_global_of_doom, start);
    bool res = dfs_visit(g, start, length) * 2;
    vec_free(ugly_global_of_doom);
    return res;
}

int main(void) {
    FILE* fin = fopen("input", "r");

    int tests;
    fscanf(fin, "%d\n", &tests);

    int i;
    for (i = 0; i < tests; ++i) {
        graph_t* graph = from_input(fin);
        if (has_cycle(graph)) {
            printf("SIM\n");
        } else {
            printf("NAO\n")
        }
    }

    return 0;
}

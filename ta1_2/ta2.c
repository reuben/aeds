#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int from;
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
    v->vec = (edge_t*)malloc(sizeof(edge_t)*n);
    v->capacity = n;
    v->length = 0;
    return v;
}

void vec_new_inline(size_t n, void* where) {
    edge_vec* v = (edge_vec*)where;
    v->vec = (edge_t*)malloc(sizeof(edge_t)*n);
    v->capacity = n;
    v->length = 0;
}

void vec_free(edge_vec* v) {
    free(v->vec);
    free(v);
}

void vec_grow(edge_vec* v, size_t new_n) {
    edge_t* _new = (edge_t*)malloc(sizeof(edge_t)*new_n);
    memcpy(_new, v->vec, v->length*sizeof(edge_t));
    free(v->vec);
    v->vec = _new;
    v->capacity = new_n;
}

void vec_append(edge_vec* v, edge_t i) {
    if (v->length == v->capacity) {
        vec_grow(v, v->capacity*2);
    }
    v->vec[v->length] = i;
    v->length++;
}

edge_t vec_get(edge_vec* v, int i) {
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

void graph_append_edge(graph_t* g, int from, int to, int weight) {
    assert(from < g->vertices);
    assert(to < g->vertices);

    edge_t edge;
    edge.from = from;
    edge.to = to;
    edge.weight = weight;

    vec_append(&g->edges[from], edge);
}

typedef struct {
    edge_t* edges;
    int vertex_count;
    int edge_count;
    int total_cost;
} edges_and_total_cost_t;

bool from_input(FILE* fin, edges_and_total_cost_t* out) {
    int vertices, edges;
    fscanf(fin, "%d %d", &vertices, &edges);
    if (!vertices || !edges) {
        return false;
    }

    out->vertex_count = vertices;
    out->edge_count = edges;

    edge_t* edgelist = (edge_t*)malloc(sizeof(edge_t)*edges);
    edge_t* edge = edgelist;

    int i, total_cost = 0;
    for (i = 0; i < edges; ++i) {
        fscanf(fin, "%d %d %d\n", &(edge->from), &(edge->to), &(edge->weight));       
        total_cost += edge->weight;
        edge++;
    }

    out->edges = edgelist;
    out->total_cost = total_cost;
    return true;
}

int compare_edges(const void* lhs, const void* rhs) {
    edge_t* left = (edge_t*)lhs;
    edge_t* right = (edge_t*)rhs;

    if (left->weight == right->weight) {
        return 0;
    }
    return left->weight < right->weight ? -1 : 1;
}

typedef struct {
  int* set;
  int size;
} dset_t;

dset_t* dset_new(int size) {
  dset_t* set = (dset_t*)malloc(sizeof(dset_t));
  int i;
  set->set = (int*)malloc(sizeof(int)*size);
  for (i = 0; i < size; ++i) {
    set->set[i] = -1;
  }
  set->size = size;
  return set;
}

int dset_find(dset_t* set, int x) {
  assert(x >= 0);
  assert(x < set->size);
  if (set->set[x] < 0) {
    return x;
  }
  return dset_find(set, set->set[x]);
}

void dset_union(dset_t* set, int x, int y) {
  int xr, yr;
  assert(x < set->size);
  assert(y < set->size);
  xr = dset_find(set, x);
  yr = dset_find(set, y);
  set->set[xr] = yr;
}

int dset_same_set(dset_t* set, int x, int y) {
  return dset_find(set, x) == dset_find(set, y);
}

// returns sum of weights in MST
int kruskal_mst(edge_t* edgelist, int num_of_edges, int num_of_vertices) {
    qsort(edgelist, num_of_edges, sizeof(edge_t), compare_edges);

    edge_t* edge = edgelist;
    dset_t* set = dset_new(num_of_vertices);
    int sum_of_weights = 0;
    int i;
    for (i = 0; i < num_of_edges; ++i) {
        if (!dset_same_set(set, edge->from, edge->to)) {
            dset_union(set, edge->from, edge->to);
            sum_of_weights += edge->weight;
        }
        edge++;
    }

    return sum_of_weights;
}

int main(void) {
    FILE* fin = stdin; //fopen("input", "r");

    edges_and_total_cost_t in;
    while (from_input(fin, &in)) {
        int minimal_cost = kruskal_mst(in.edges, in.edge_count, in.vertex_count);
        printf("%d\n", in.total_cost - minimal_cost);
    }

    return 0;
}

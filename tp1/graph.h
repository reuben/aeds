#pragma once

typedef struct {
  unsigned id;
  unsigned index;
  float distance;
} vertex_t;

vertex_t* vertex_new(unsigned id);
unsigned vertex_id(vertex_t* v);
float vertex_distance(vertex_t* v);
void vertex_set_distance(vertex_t* v, float distance);

typedef struct edge_t {
  float weight;
  vertex_t* target;
  struct edge_t* next;
} edge_t;

edge_t* edge_new(float weight, vertex_t* target);
void edge_free(edge_t* e);
void edgelist_free(edge_t* e);
vertex_t* edge_target(edge_t* e);
float edge_weight(edge_t* e);
edge_t* edge_next(edge_t* e);

typedef struct {
  unsigned vertex_count;
  vertex_t** vertices;
  edge_t** edges;
} graph_t;

graph_t* graph_new(unsigned vertex_count);
void graph_free(graph_t* g);
void graph_append_edge(graph_t* g,
                       unsigned from,
                       float weight,
                       vertex_t* target);
edge_t* graph_get_edges(graph_t* g, unsigned from);
edge_t* graph_get_edge(graph_t* g, unsigned from, unsigned to);
vertex_t* graph_get_vertex(graph_t* g, unsigned vertex);
unsigned graph_vertex_count(graph_t* g);

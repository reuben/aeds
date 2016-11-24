#include "graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* vertex impl */

vertex_t* vertex_new(unsigned id)
{
  vertex_t* v = (vertex_t*)malloc(sizeof(vertex_t));
  v->id = id;
  v->index = 0;
  v->distance = 1.f;
  return v;
}

unsigned vertex_id(vertex_t* v)
{
  return v->id;
}

float vertex_distance(vertex_t* v)
{
  return v->distance;
}

void vertex_set_distance(vertex_t* v, float distance)
{
  v->distance = distance;
}

/* edge impl */

edge_t* edge_new(float weight, vertex_t* target)
{
  edge_t* e = (edge_t*)malloc(sizeof(edge_t));
  e->weight = weight;
  e->target = target;
  e->next = NULL;
  return e;
}

void edge_free(edge_t* e)
{
  free(e);
}

void edgelist_free(edge_t* e)
{
  while (e) {
    edge_t* tmp = e->next;
    edge_free(e);
    e = tmp;
  }
}

vertex_t* edge_target(edge_t* e)
{
  return e->target;
}

float edge_weight(edge_t* e)
{
  return e->weight;
}

edge_t* edge_next(edge_t* e)
{
  return e->next;
}

/* graph impl */

graph_t* graph_new(unsigned vertex_count)
{
  graph_t* g = (graph_t*)malloc(sizeof(graph_t));
  g->vertex_count = vertex_count;
  g->vertices = (vertex_t**)malloc(vertex_count * sizeof(vertex_t*));
  g->edges = (edge_t**)malloc(vertex_count * sizeof(edge_t*));
  unsigned i;
  for (i = 0; i < vertex_count; ++i) {
    g->vertices[i] = vertex_new(i);
    g->edges[i] = NULL;
  }
  return g;
}

void graph_free(graph_t* g)
{
  unsigned i;
  for (i = 0; i< g->vertex_count; ++i) {
    edgelist_free(g->edges[i]);
    free(g->vertices[i]);
  }
  free(g->edges);
  free(g->vertices);
  free(g);
}

void graph_append_edge(graph_t* g,
                       unsigned from,
                       float weight,
                       vertex_t* target)
{
  assert(from < g->vertex_count);
  assert(target->id < g->vertex_count);

  edge_t* edge = edge_new(weight, target);

  edge_t* e = g->edges[from];
  if (!e) {
    g->edges[from] = edge;
  } else {
    while (e->next) {
      e = e->next;
    }
    e->next = edge;
  }
}

edge_t* graph_get_edges(graph_t* g, unsigned from)
{
  assert(from < g->vertex_count);

  return g->edges[from];
}

edge_t* graph_get_edge(graph_t* g, unsigned from, unsigned to)
{
  assert(from < g->vertex_count);
  assert(to < g->vertex_count);

  edge_t* e = graph_get_edges(g, from);
  while (e) {
    if (e->target->id == to) {
      return e;
    }
    e = e->next;
  }
  return NULL;
}

vertex_t* graph_get_vertex(graph_t* g, unsigned vertex)
{
  assert(vertex < g->vertex_count);

  return g->vertices[vertex];
}

unsigned graph_vertex_count(graph_t* g)
{
  return g->vertex_count;
}

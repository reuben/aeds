#pragma once

#include "graph.h"

typedef struct {
  vertex_t** heap;
  unsigned size;
} pqueue_t;

pqueue_t* pq_new(unsigned size);

void pq_free(pqueue_t* q);

void pq_push(pqueue_t* q, vertex_t* node);

vertex_t* pq_pop(pqueue_t* q);

unsigned pq_size(pqueue_t* q);

void pq_decrease_key(pqueue_t* q, vertex_t* node);

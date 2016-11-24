#include "pqueue.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

static unsigned parent(unsigned i)
{
  return i / 2;
}

static unsigned left(unsigned i)
{
  return 2 * i;
}

static unsigned right(unsigned i)
{
  return 2 * i + 1;
}

static float pri(pqueue_t* q, unsigned i)
{
  return q->heap[i]->distance;
}

static bool compare(pqueue_t* q, unsigned l, unsigned r)
{
  return pri(q, l) < pri(q, r);
}

static bool compare2(pqueue_t* q, unsigned l, unsigned r)
{
  return pri(q, l) > pri(q, r);
}

static void min_heapify(pqueue_t* q, unsigned i)
{
  unsigned l = left(i);
  unsigned r = right(i);
  unsigned smallest;
  vertex_t* tmp;
  if (l <= q->size && compare(q, l, i)) {
    smallest = l;
  } else {
    smallest = i;
  }
  if (r <= q->size && compare(q, r, smallest)) {
    smallest = r;
  }
  if (smallest != i) {
    tmp = q->heap[i];
    q->heap[i] = q->heap[smallest];
    q->heap[i]->index = i;
    q->heap[smallest] = tmp;
    q->heap[smallest]->index = smallest;
    min_heapify(q, smallest);
  }
}

static void build_min_heap(pqueue_t* q)
{
  unsigned i;
  for (i = q->size/2; i > 0; --i) {
    min_heapify(q, i);
  }
}

pqueue_t* pq_new(unsigned mem)
{
  pqueue_t* q = (pqueue_t*)malloc(sizeof(pqueue_t));
  q->heap = (vertex_t**)malloc(mem * sizeof(vertex_t*));
  q->size = 0;
  build_min_heap(q);
  return q;
}

void pq_free(pqueue_t* q)
{
  free(q->heap);
  free(q);
}

void pq_push(pqueue_t* q, vertex_t* node)
{
  vertex_t* tmp;
  unsigned i;
  q->size++;
  q->heap[q->size] = node;
  q->heap[q->size]->index = q->size;
  i = q->size;
  while (i > 1 && compare2(q, parent(i), i)) {
    tmp = q->heap[i];
    q->heap[i] = q->heap[parent(i)];
    q->heap[i]->index = i;
    q->heap[parent(i)] = tmp;
    q->heap[parent(i)]->index = parent(i);
    i = parent(i);
  }
}

vertex_t* pq_pop(pqueue_t* q)
{
  vertex_t* tmp = q->heap[1];
  q->heap[1] = q->heap[q->size];
  q->heap[1]->index = q->size;
  q->size--;
  min_heapify(q, 1);
  return tmp;
}

unsigned pq_size(pqueue_t* q)
{
  return q->size;
}

void pq_decrease_key(pqueue_t* q, vertex_t* node)
{
  unsigned i = node->index;
  vertex_t* tmp;
  while (i > 1 && compare2(q, parent(i), i)) {
    tmp = q->heap[i];
    q->heap[i] = q->heap[parent(i)];
    q->heap[i]->index = i;
    q->heap[parent(i)] = tmp;
    q->heap[parent(i)]->index = parent(i);
    i = parent(i);
  }
}

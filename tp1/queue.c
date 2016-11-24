#include "queue.h"

#include <stdlib.h>

queue_t* queue_new(unsigned capacity)
{
  queue_t* q = (queue_t*)malloc(sizeof(queue_t));
  q->vec = (unsigned*)malloc(capacity * sizeof(unsigned));
  q->capacity = capacity;
  q->start = q->length = 0;
  return q;
}

void queue_free(queue_t* q)
{
  free(q->vec);
  free(q);
}

unsigned queue_idx(queue_t* q, unsigned i)
{
  return (q->start + i) % q->capacity;
}

void queue_grow(queue_t* q, unsigned new_capacity)
{
  unsigned* vec = (unsigned*)malloc(new_capacity * sizeof(unsigned));
  unsigned i;
  for (i = 0; i < q->length; ++i) {
    vec[i] = q->vec[queue_idx(q, i)];
  }
  free(q->vec);
  q->start = 0;
  q->vec = vec;
  q->capacity = new_capacity;
}

void queue_enqueue(queue_t* q, unsigned v)
{
  if (q->length == q->capacity) {
    queue_grow(q, q->capacity * 2);
  }

  q->vec[queue_idx(q, q->length)] = v;
  q->length++;
}

unsigned queue_dequeue(queue_t* q)
{
  unsigned first = q->vec[q->start];
  q->start = queue_idx(q, 1);
  q->length--;
  return first;
}

unsigned queue_length(queue_t* q)
{
  return q->length;
}

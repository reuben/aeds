#pragma once

typedef struct {
  unsigned* vec;
  unsigned start;
  unsigned length;
  unsigned capacity;
} queue_t;

queue_t* queue_new(unsigned capacity);
void queue_free(queue_t* q);
unsigned queue_idx(queue_t* q, unsigned i);
void queue_grow(queue_t* q, unsigned new_capacity);
void queue_enqueue(queue_t* q, unsigned v);
unsigned queue_dequeue(queue_t* q);
unsigned queue_length(queue_t* q);

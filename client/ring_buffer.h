#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>

typedef struct {
  int *buffer;
  int head, tail, len, capacity;
} ring;

void print_ring(ring *ring);
void init_ring(ring *ring, int capacity);
bool ring_empty(ring *ring);
bool ring_full(ring *ring);
void Consume(ring *ring);
void resize_ring(ring *ring);
void add_ring(ring *ring, int data);
void print_ring(ring *ring);

#endif

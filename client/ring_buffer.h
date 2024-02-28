#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
typedef struct rng ring;

void print_ring(ring *r);
void info_ring(ring *r);
ring *init_ring(int capacity, int max_capacity);
bool ring_empty(ring *r);
bool ring_full(ring *r);
void Consume(ring *r);
void ring_destroy(ring *r);
bool resize_ring(ring *r);
bool add_ring(ring *r, int data);
void print_ring(ring *r);

#endif

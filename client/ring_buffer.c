#include "ring_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 5

typedef struct rng {
  int *buffer;
  int head, tail, len, capacity;
} ring;

ring *init_ring(int capacity) {
  ring *r = (ring *)malloc(sizeof(ring));
  r->head = 0;
  r->tail = 0;
  r->len = 0;
  r->capacity = capacity;
  r->buffer = (int *)malloc(r->capacity * sizeof(int));
  return r;
}

bool ring_empty(ring *ring) { return ring->len == 0; }
bool ring_full(ring *ring) { return ring->len == ring->capacity; }

void ring_destroy(ring *ring) { free(ring->buffer); }

void Consume(ring *ring) {
  ring->buffer[ring->head] = 2;
  ring->head = (ring->head + 1) % ring->capacity;
}

bool resize_ring(ring *ring) {
  if (ring_full(ring)) {
    return false;
  }
  int newSize = ring->capacity * 2;
  int *newBuffer = (int *)malloc(newSize * sizeof(int));
  if (newBuffer == NULL) {
    perror("Error allocating memory \n");
  }
  int i = 0;

  for (i = 0; i < ring->head; i++) {
    newBuffer[i] = ring->buffer[i];
  }

  for (i = ring->head; i <= (ring->capacity - ring->head); i++) {
    newBuffer[newSize - (ring->capacity - ring->head) + i - ring->head] =
        ring->buffer[i - ring->head + 1];
  }

  ring->capacity = newSize;
  ring->head = (ring->head + (ring->capacity / 2)) % ring->capacity;
  for (i = 0; i < ring->capacity; i++) {
    ring->buffer[i] = newBuffer[i];
  }
  free(newBuffer);
  return true;
}

bool add_ring(ring *ring, int data) {
  if (ring_full(ring)) {
    return false;
  }

  ring->buffer[ring->tail] = data;
  ring->len = ring->len + 1;
  ring->tail = (ring->tail + 1) % ring->capacity;

  if ((ring->tail) % ring->capacity == ring->head &&
      ring->len != ring->capacity) {

    resize_ring(ring);
  }

  return true;
}

void print_ring(ring *ring) {
  for (int i = 0; i < ring->tail * 4; i++) {
    printf(" ");
  }
  printf(">T\n");
  for (int i = 0; i < ring->head * 4; i++) {
    printf(" ");
  }
  printf(">H\n");

  for (int i = 0; i < ring->capacity; i++) {
    printf("[%d] ", ring->buffer[i]);
  }
  printf("\n");
  printf("\n");
}

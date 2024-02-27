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

void resize_ring(ring *ring) {
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
}

void add_ring(ring *ring, int data) {
  if ((ring->tail + 1) % ring->capacity == ring->head) {
    resize_ring(ring);
  }
  ring->buffer[ring->tail] = data;
  ring->tail = (ring->tail + 1) % ring->capacity;
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

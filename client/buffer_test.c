#include "ring_buffer.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int step = 0;

  ring *ringBuffer = init_ring(5);
  while (1) {
    add_ring(ringBuffer, 1);
    if (step % 3 == 0 && step != 0)
      Consume(ringBuffer);

    printf("\n");
    print_ring(ringBuffer);
    step++;
    getchar();
    system("clear");
  }

  return 0;
}

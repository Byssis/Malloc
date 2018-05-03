#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define ROUNDS 100
#define LOOP 100000


int main(){
  void *init = sbrk(0);
  void *current;

  printf("The initial top of the hesp %p\n", init);
  int i,j;
  for (i = 0; i < ROUNDS; i++) {
    for (j = 0; j < LOOP; j++) {
      size_t size = (rand() % 4000) + sizeof(int);
      int *memory;
      memory = malloc(size);
      if(memory == NULL){
        fprintf(stderr, "malloc failed\n");
        return (1);
      }
      //*memory = 123;
      free(memory);
    }
    current = sbrk(0);
    int allocated = (int)((current-init) / 1024);
    printf("%d\n", j);
    printf("The current top of the heap is %p\n", current);
    printf("    increased by %d Kbyte\n", allocated);
  }
  return 0;
}

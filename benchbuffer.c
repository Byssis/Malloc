#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define ROUNDS 1000
#define LOOP 10000
#define BUFFER 50

int main(){
  int i,j;
  void *buffer[BUFFER];
  void *init = sbrk(0);
  void *current;

  printf("The initial top of the heap %p\n", init);

  for(i = 0; i < BUFFER; i++){
    buffer[i] = NULL;
  }

  for (i = 0; i < ROUNDS; i++) {
    for (j = 0; j < LOOP; j++) {
      int index = rand() % BUFFER;
      if(buffer[index] != NULL){
        free(buffer[index]);
      }
      size_t size = (size_t) request();
      int * memory;
      //printf("%d %d\n", j, size);
      memory = malloc(size);

      if(memory == NULL){
        fprintf(stderr, "memory allocation failed\n");
        return (1);
      }
      buffer[index] = memory;
      *memory = 123;
    }
    current = sbrk(0);
    int allocated = (int)((current-init) / 1024);
    printf("%d\n", i);
    printf("The current top of the heap is %p\n", current);
    printf("    increased by %d Kbyte\n", allocated);

  }
  return 0;
}

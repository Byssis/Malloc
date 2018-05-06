#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define ROUNDS 1
#define LOOP 100000
#define BUFFER 100

int main(){
  int i,j;
  void *buffer[BUFFER];
  void *init = sbrk(0);
  void *current;
  struct timeval t0, t1;

  printf("The initial top of the heap %p\n", init);

  for(i = 0; i < BUFFER; i++){
    buffer[i] = NULL;
  }

  for (i = 0; i < ROUNDS; i++) {
    gettimeofday(&t0, 0);
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
    gettimeofday(&t1, 0);
    int elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
    current = sbrk(0);
    int allocated = (int)((current-init) / 1024);
    //printf("%d\n", i);
    printf("The current top of the heap is %p\n", current);
    printf("    increased by %d Kbyte\n", allocated);
    printf("    Time per operations %f\n", (double)elapsed/(2*LOOP));
  }
  return 0;
}

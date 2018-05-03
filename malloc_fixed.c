#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

struct chunk {
  int size;
  struct chunk *next;
};

struct chunk * flist [9];

void *malloc(size_t size){
  printf("Malloc %d\n", size);
  int index = 0;
  int start = 32;
  while(size > start - sizeof(struct chunk)){
    index++;
    start *= 2;
  }
  printf("Malloc %d, index %d\n", size, index);
  if(flist[index] == NULL){
    printf("New block of size 2^%d\n", index);
    int s = pow(2, index);
    void * memory = sbrk(s);
    if(memory == (void *) -1){
      return NULL;
    }else{
      struct chunk *cnk = (struct chunk*) memory;
      cnk->size = s;
      return (void *) (cnk + 1);
    }
  }
  //printf("List %d\n");
  struct chunk *m = (struct chunk*)flist[index];
  printf("Exisiting block %d, index %d\n", m->size, index);
  flist[index] = m->next;
  return (void *) (m + 1);
}


void free (void *memory){
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk *) memory - 1);
    int size = cnk->size; //+ sizeof(struct chunk);
    int index = 0;
    printf("Free size %d\n", size);
    int start = 32;
    while(size > start){
      index++;
      start *= 2;
    }
    cnk->next = flist[index];
    flist[index] = cnk;
  }
  return;
}

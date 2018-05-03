#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct chunk {
  int size;
  struct chunk *next;
};

struct chunk * flist = NULL;
int frag = 0;
void *malloc(size_t size){
  if(size == 0){
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL) {
    if(next->size >= size){
      if(prev != NULL){
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      frag += next->size - size;
      //printf("Internal fragmentation %d\n", frag/1024);
      return (void *)(next + 1);
    } else {
      prev = next;
      next = next->next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if(memory == (void *) - 1){
    return NULL;
  }else{
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    return (void*)(cnk + 1);
  }
}


void free (void *memory){
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk *) memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

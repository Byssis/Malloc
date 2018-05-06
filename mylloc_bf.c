#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct chunk {
  int size;
  struct chunk *next;
  struct chunk *prev;
};

struct chunk * flist = NULL;

int list_size = 0;

void *malloc(size_t size){
  if(size == 0){
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;
  int diff = -1;
  struct chunk *bf = NULL;

  while (next != NULL) {
    if(next->size >= size){
      if(next->size >= size && (next->size - size < diff || diff < 0)){
        diff = next->size - size;
        bf = next;
      }
      if(diff == 0)
       break;
    }
    prev = next;
    next = next->next;
  }
  if(diff >= 0){
    list_size--;

    if(bf == flist)
      flist = flist->next;

    if(bf-> next != NULL)
      bf->next->prev = bf->prev;

    if(bf->prev != NULL)
      bf->prev->next = bf->next;

    return (void *)(bf + 1);
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
    cnk->prev = NULL;
    if(flist != NULL)
      flist->prev = cnk;
    flist = cnk;
    list_size++;
  }
  return;
}

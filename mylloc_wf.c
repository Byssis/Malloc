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
  struct chunk *wf = NULL;

  while (next != NULL) {
    if(next->size >= size){
      if(next->size - size > diff || diff < 0){
        diff = next->size - size;
        wf = next;
      }
    }
    prev = next;
    next = next->next;
  }
  if(diff >= 0){
    list_size--;
  
    //printf("Internal fragmentation %d\n", frag);
    if(wf == flist)
      flist = flist->next;

    if(wf-> next != NULL)
      wf->next->prev = wf->prev;

    if(wf->prev != NULL)
      wf->prev->next = wf->next;

    return (void *)(wf + 1);
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
    //printf("%d\n", list_size);
  }
  return;
}

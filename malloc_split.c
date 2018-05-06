#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

struct chunk {
  int size;
  struct chunk *next;
};

struct chunk * flist [7];
int size_list [7];

void *get_block(int index){
  if(flist[index] == NULL){
    if(index > 6){
      struct chunk *cnk1 = (struct chunk*)sbrk(4096);
      cnk1->size = 4096 - sizeof(struct chunk);
      cnk1->next = NULL;
      return (void*)(cnk1);
    }
    void * memory = (void *)get_block(index + 1);
    if(memory == NULL)
      return NULL;
    int size = pow(2, index + 5);
    struct chunk *cnk1 = (struct chunk*)memory;
    struct chunk *cnk2 = (struct chunk*)(memory + size);
    cnk1->size = size - sizeof(struct chunk);
    cnk2->size = size - sizeof(struct chunk);
    flist[index] = cnk2;
    size_list[index] = 1;
    cnk1->next = NULL;
    cnk2->next = NULL;
    return (void*)(cnk1);
  }else{
    struct chunk *cnk = flist[index];
    flist[index] = cnk->next;
    size_list[index]--;
    return (void *)cnk;
  }
}
void *malloc(size_t size){
  int index = 0;
  int start = 32;
  while(size + sizeof(struct chunk) > start ){
    index++;
    start *= 2;
  }
  void * memory = (void *) get_block(index);
  if (memory == NULL)
    return NULL;
  struct chunk *cnk = (struct chunk*)memory;
  return  (void *)(cnk + 1);
}

void free (void *memory){
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk *) memory - 1);
    int size = cnk->size + sizeof(struct chunk);
    int index = 0;
    int start = 32;
    while(size > start){
      index++;
      start *= 2;
    }
    cnk->next = flist[index];
    flist[index] = cnk;
    size_list[index]++;
  }
  return;
}

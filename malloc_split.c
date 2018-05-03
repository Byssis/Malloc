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

void print_list(){
  int i;
  for (i = 0; i < 7; i++) {
    printf("index %d %d, ", i, size_list[i]);
  }
  printf("\n");
}

void *get_block(int index){
  printf("Get block index %d\n", index);
  if(index > 6)
    return sbrk(4096);

  if(flist[index] == NULL){
    printf("Index %d empty\n", index);
    void * memory = (void *)get_block(index + 1);
    if(memory == NULL)
      return NULL;
    int size = pow(2, index + 5);
    struct chunk *cnk1 = (struct chunk*)memory;
    printf("size*sizeof(void*) %d\n", size);
    struct chunk *cnk2 = (struct chunk*)(memory + size);
    cnk1->size = size - sizeof(struct chunk);
    cnk2->size = size - sizeof(struct chunk);
    //printf("Add one to index %d\n", index);
    flist[index] = cnk2;
    size_list[index] = 1;
    cnk1->next = NULL;
    cnk2->next = NULL;
    printf("Index: %d, cnk 1:%p, cnk2: %p\n", index, cnk1, cnk2);
    //printf("Next Index: %d, cnk 1:%p, cnk2: %p\n", index, cnk1->next, cnk2->next);
    return (void*)(cnk1);
  }else{
    printf("Get from free list index %d\n", index);
    struct chunk *cnk = flist[index];
    //printf("Get from free list index %p\n", cnk);
    flist[index] = cnk->next;
    size_list[index]--;
    //printf("Next %p\n", cnk->next);
    //printf("Remove one from %d\n", index);
    return (void *)cnk;
  }
}

void *malloc(size_t size){
  printf("Malloc %d, size of chunk: %d\n", size, sizeof(struct chunk));
  int index = 0;
  int start = 32;
  while(size > start - sizeof(struct chunk)){
    index++;
    start *= 2;
  }
  void * memory = (void *) get_block(index);
  print_list();
  if (memory == NULL)
    return NULL;
  struct chunk *cnk = (struct chunk*)memory;
  return  (void *)(cnk + 1);
}

void free (void *memory){
  //printf("Free\n");
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk *) memory - 1);
    int size = cnk->size + sizeof(struct chunk);
    int index = 0;
    printf("  Free size %d\n", size);
    int start = 32;
    while(size > start){
      index++;
      start *= 2;
    }
    printf("  Free index size %d\n", index);
    cnk->next = flist[index];
    flist[index] = cnk;
    size_list[index]++;
  }

  print_list();
  return;
}

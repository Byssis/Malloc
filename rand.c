#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 4000
#define MIN 8

int request() {
  /* k is log(MAX/MIN) */
  double k = log(((double)MAX) / MIN);

  /* r is [0 .. k] */
  double r = ((double)(rand() % (int)(k*10000))) / 10000;

  /* size is [0 .. MAX]  */
  int size = (int)((double)MAX/exp(r));

  return size;
}

/*int main(int argc, char const *argv[]) {
  if(argc < 2 ){
    printf("usage: rand <loop>\n");
    exit(1);
  }
  int loop = atoi(argv[1]);
  int i;
  for (i = 0; i < loop; i++) {
    int size = request();
    printf("%d\n", size);
  }
}*/

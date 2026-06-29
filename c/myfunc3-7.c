#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void set_array_rand(int n, int* a, int max)
{
  int i;
  srand((unsigned int)time(NULL));
  for(i=0; i<n; i++){
    a[i] = rand() % (max+1);
  }
}

void print_array(int n, int* a)
{
  int i;
  for(i=0; i<n; i++){
    printf("%d ", a[i]);
  }
  printf("\n");
}

int comp(const void *a, const void *b)
{
return *(int*)a - *(int*)b;
}
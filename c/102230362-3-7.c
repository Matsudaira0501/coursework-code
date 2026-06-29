#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myfunc3-7.h"

#define N  5

int main(void)
{
  int i, j;
  int score[N];
  float ave, dev;
  double m, u;

  set_array_rand(N, score, 10);
  qsort(score, N, sizeof(int), comp);

  /* calc average and deviation */

  for(i=0; i<N; i++){
    ave += score[i];
  }
  
  for(i=0; i<N; i++){
    m += pow(score[i] - ave/N, 2.0);
  }


  printf("max %d, ave %f\n", score[N-1], ave/N);
  printf("score, standard score\n");
  for(i=0; i<N; i++){
	 for(j=0; j<score[i]; j++){
    printf("#");
   }
   for(j=10; j>score[i]; j--){
    printf(" ");
   }
   u = sqrt(m/N);
   dev = (score[i] - ave/N)*10/u +50;
  
  printf("  (%d)  (%4.2f)\n", score[i], dev);
  }


  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
int N = 256;

int main(void){
int i, j, n, k;
double x0, y0, h, diff, EPS_ITER;
double b[N], x[N], x_old[N], c[N];
double (*A)[N];
struct timeval time1;
struct timeval time2;
double diff_time;
unsigned long int flops, m;
A = (double (*)[N])malloc(N*N*sizeof(double));
h = 0.10;


FILE *fp;

fp = fopen("kaiseki9-1.csv", "w");
    if(fp == NULL){
        printf("file not open");
        return -1;
}

n = sqrt(N);

//Aの生成
for (i=0; i<N; i++) {
  for (j=0; j<N; j++) {
    
    if(i+1 == j+1){A[i][j] = 4;}
    else if((i+1-n == j+1 && i+1 > n)
          ||(i+1-1 == j+1 && (i+1) % n != 1)
          ||(i+1+1 == j+1 && (i+1) % n != 0)
          ||(i+1+n == j+1 && (i+1) <= N - n)){A[i][j] = -1;}
    else A[i][j] = 0;
    
    //printf("%f ", A[i][j]);
  }
//printf("\n");
}

//Bの生成
for (i=0; i<N; i++) {
    
    if(i+1 == N/2 + n/2){b[i] = 1;}
    
    else b[i] = 0;
    //確認用
    //printf("%f\n", b[i]);
}

//初期ベクトル
for (i=0; i<N; i++){
    x_old[i] = 0.0;
}

//オイラー法のアルゴリズム
for(k=0; k<1000; k++){

  //Axの積
  for (i=0; i<N; i++){
    c[i] = 0;
    for (j=0; j<N; j++){
      c[i] += A[i][j] * x[j];
    }
  }
  
  //オイラー法の計算
  for (i=0; i<N; i++){
    x[i] = x_old[i] + h * (b[i] - c[i]);
    x_old[i] = x[i]; 
  } 

}

for (i=0; i<N; i++) {
//確認用
//N=4 1, -1, -1, 1
fprintf(fp, ", %f", x[i]);
  if(i % n == 15 && i != 0 && i != N){
    fprintf(fp, "\n");
  }
}

return 0;

//第5回課題1.2の結果と比較すると、概形はほぼ同じ形となったが
//最大値は第5回課題のほうが大きくなった。

}


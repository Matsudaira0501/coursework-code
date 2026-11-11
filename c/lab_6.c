#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
int N = 4096;

int main(void){
int k, i, j, n, l, iter, MAX_ITER;
double dtemp, dtemp2, dtemp3, dtemp4, diff, omega, EPS_ITER;
double b[N], c[N], x[N], x_old[N], y[N];
double (*A)[N];
struct timeval time1;
struct timeval time2;
double diff_time;
unsigned long int flops, m;
A = (double (*)[N])malloc(N*N*sizeof(double));
omega = 1.80;
EPS_ITER = 0.00000001;
MAX_ITER = 10000;

printf("1-1, 2\n");

FILE *fp;

fp = fopen("kaiseki7-1.csv", "w");
    if(fp == NULL){
        printf("file not open");
        return -1;
}

for (N=4; N <= 1024;){
N = N*4;
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

flops = 0;
// Jacobi 反復
gettimeofday(&time1, NULL);
for (iter=1; iter<MAX_ITER; iter++){
  diff = 0.0;
    for (i=0; i<N; i++){
      dtemp = 0.0;
      for (j=0; j<=(i-1); j++){
        dtemp += A[i][j]*x_old[j];
        flops += 3;
      }
      
      dtemp2 = 0.0;
      for (j=i+1; j<N; j++){
        dtemp2 += A[i][j]*x_old[j];
        flops += 3;
      }

      x[i]= (b[i]-dtemp-dtemp2)/A[i][i];
      diff += (x[i]-x_old[i]) * (x[i]-x_old[i]);
      flops += 18;
      }
   
  dtemp = sqrt(diff);

//収束判定
  if (dtemp < EPS_ITER){
    break;
  }
//ベクトル更新
  for (i=0; i<N; i++){
    x_old[i] = x[i];
    flops++;
  }
}
gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("N=%d, 実行時間:%f[s]\n", N, diff_time);
printf("flops=%lu\n", flops);

m = N*N*N;
fprintf(fp, "%d, %lu, %lu, %f\n", N, m, flops, diff_time);

/*
//printf("x\n");
for (i=0; i<N; i++) {
//確認用
//N=4 1/12, 1/24, 7/24, 1/12
printf("%f ", x[i]);
  if(i % n == 15 && i != 0 && i != N){
    printf("\n");
  }
}
*/

//初期ベクトル
for (i=0; i<N; i++){
  x[i] = 0.0;
  }

flops = 0;
// Gauss Seidel 反復
gettimeofday(&time1, NULL);
for (iter=1; iter<MAX_ITER; iter++){
  diff = 0.0;
  for (i=0; i<N; i++) {
    dtemp = 0.0;
    for (j=0; j<=(i-1); j++){
      dtemp += A[i][j] * x[j];
      flops += 3;
    }
    dtemp2 = 0.0;
    for(j=i+1; j<N; j++){
      dtemp2 += A[i][j] * x[j];
      flops += 3;
    }
    dtemp3 = (b[i]-dtemp-dtemp2)/A[i][i];
    diff += (x[i]-dtemp3)*(x[i]-dtemp3);
    x[i] = dtemp3;
    flops += 17;
  }
  dtemp = sqrt(diff);
  flops++;
//収束判定
  if (dtemp < EPS_ITER){
  break;
  }
}

gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("N=%d, 実行時間:%f[s]\n", N, diff_time);
printf("flops=%lu\n", flops);

fprintf(fp, "%d, %lu, %lu, %f\n", N, m, flops, diff_time);

for (i=0; i<N; i++){
  x[i] = 0.0 ;
}
// SOR 反復
flops = 0;
gettimeofday(&time1, NULL);
for (iter=1; iter<MAX_ITER; iter++) {
  diff = 0.0;
  for (i=0; i<N; i++){
    dtemp = 0.0;
    for (j=0; j<=(i-1); j++){
      dtemp += A[i][j]*x[j];
      flops += 3;
    }
    dtemp2 = 0.0;
    for(j=i+1; j<N; j++){
      dtemp2 += A[i][j]*x[j];
      flops += 3;
    }
    dtemp3 = x[i] + omega*( (b[i]-dtemp-dtemp2)/A[i][i] - x[i] );
    diff += (x[i]-dtemp3)*(x[i]-dtemp3);
    x[i] = dtemp3;
    flops += 20;
  }
  dtemp = sqrt(diff);
  flops++;
//収束判定
  if (dtemp < EPS_ITER){
    break;
  }
}

gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("N=%d, 実行時間:%f[s]\n", N, diff_time);
printf("flops=%lu\n", flops);

fprintf(fp, "%d, %lu, %lu, %f\n", N, m, flops, diff_time);

}
printf("1-3\n");
N = 256;
EPS_ITER = 0.0000000001;
n = sqrt(N);

FILE *fp1;

fp1 = fopen("kaiseki7-2.csv", "w");
    if(fp1 == NULL){
        printf("file not open");
        return -1;
}

//M^-1Nの生成
for (i=0; i<N; i++) {
  for (j=0; j<N; j++) {
    
    if(i+1 == j+1){A[i][j] = 0;}
    else if((i+1-n == j+1 && i+1 > n)
          ||(i+1-1 == j+1 && (i+1) % n != 1)
          ||(i+1+1 == j+1 && (i+1) % n != 0)
          ||(i+1+n == j+1 && (i+1) <= N - n)){A[i][j] = 0.25;}
    else A[i][j] = 0;
    
    //printf("%f ", A[i][j]);
  }
//printf("\n");
}

srand(2);
//ベクトルの生成
for (i=0; i<N; i++){
  x[i] = rand()/(1.0+RAND_MAX);
}

//正規化
dtemp = 0.0;
for(i=0; i<N; i++) {
  dtemp += x[i] * x[i];
}
dtemp = 1.0 / sqrt(dtemp);
for(i=0; i<N; i++) {
  x[i] = x[i] * dtemp;
}

for(k=0; k < MAX_ITER; k++) {

  //行列とベクトルの積
  for(i=0; i < N; i++){
    y[i] = 0;
    for(j = 0; j < N; j++) {
      y[i] += A[i][j] * x[j];
    }
  }

  dtemp = 0.0;
  dtemp2 = 0.0;
  for (i=0; i<N; i++) {
    dtemp += y[i] * y[i];
    dtemp2 += y[i] * x[i];
  }


  dtemp3 = dtemp / dtemp2;
  if(fabs(dtemp3 - dtemp4) < EPS_ITER && k != 0){
    printf("反復回数:%d\n", k);
    break;
  }
  
  dtemp4 = dtemp3;

  dtemp = 1.0 / sqrt(dtemp);
  for(i=0; i<N; i++){
    x[i] = y[i] * dtemp;
  }

}

printf("最大固有値:%f\n", dtemp3);
printf("固有ベクトルは添付ファイル\n");
for (i=0; i<N; i++) {
//確認用
//N=4 1, -1, -1, 1
fprintf(fp1, ", %f", x[i]);
  if(i % n == 15 && i != 0 && i != N){
    fprintf(fp1, "\n");
  }
}

printf("1-4\n実際の反復回数から、Nに近いと予想される。\n");
printf("1-5\nAが疎な行列であるから計算量はO(N^2)で済んだと考えられる。\n");

return 0;

}
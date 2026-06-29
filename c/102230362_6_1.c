#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
int N = 64 * 64;

int main(void){
int k, i, j, n, l;
double dtmp, dakj, temp;
double B[N], c[N], x[N];
double (*A)[N];
struct timeval time1;
struct timeval time2;
double diff_time;
unsigned long int flops, m;
A = (double (*)[N])malloc(N*N*sizeof(double));
//課題1-2のためにN=1024 とする。
N = 32 * 32;
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
    /*確認用
    printf("%f ", A[i][j]);*/
  }
//printf("\n");
}

gettimeofday(&time1, NULL);

printf("1-1\n(略)\n");
//1-1 LU分解(外積形式ガウス法)
for (k=0; k<N; k++) {
  dtmp = 1.0 / A[k][k];
  for (i=k+1; i<N; i++) {
    A[i][k] = A[i][k]*dtmp;
  }

  for (j=k+1; j<N; j++) {
    dakj = A[k][j];
    for (i=k+1; i<N; i++) {
      A[i][j] = A[i][j] - A[i][k] *dakj;
    }
  }
}

/*確認用
for (i=0; i<N; i++) {
  for (j=0; j<N; j++) {
    printf("%f ", A[i][j]);
  }
printf("\n");
}
*/

printf("1-2\n添付ファイル参照\n");
//1-2 前進後退代入

for (i=0; i<N; i++) {
    
    if(i+1 == N/2 + n/2){B[i] = 1;}
    
    else B[i] = 0;
    //確認用
    //printf("%f\n", B[i]);
}

//printf("c\n");
for (k=0; k<N; k++) {
  c[k] = B[k];
  
  for (j=0; j<k; j++) {
    c[k] -= A[k][j] * c[j];
  }
  //printf("%f\n", c[k]);
}

x[N-1] = c[N-1] / A[N-1][N-1];

for(k=N-2; k>=0; k--) {
  x[k] = c[k];
  
  for(j=k+1; j<N; j++) {
    x[k] -= A[k][j] * x[j];
  }
  x[k] = x[k] / A[k][k];
}

gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("N=%d, 実行時間:%f[s]\n", N, diff_time);

FILE *fp;

fp = fopen("kaiseki6-1.csv", "w");
    if(fp == NULL){
        printf("file not open");
        return -1;
}

//printf("x\n");
for (i=0; i<N; i++) {
//確認用
//N=4 1/12, 1/24, 7/24, 1/12
fprintf(fp, ", %f", x[i]);
  if(i % n == 31 && i != 0 && i != N){
    fprintf(fp, "\n");
  }
}

printf("1-3\n概算:O(N^3)\nグラフは添付ファイル参照\n");

FILE *fp1;

fp1 = fopen("kaiseki6-2.csv", "w");
    if(fp1 == NULL){
        printf("file not open");
        return -1;
}

for(l=4;l <= 64;){
N = l * l;
n = sqrt(N);
flops = 0;

gettimeofday(&time1, NULL);
//Aの生成
for (i=0; i<N; i++) {
  for (j=0; j<N; j++) {
    
    if(i+1 == j+1){A[i][j] = 4;}
    else if((i+1-n == j+1 && i+1 > n)
          ||(i+1-1 == j+1 && (i+1) % n != 1)
          ||(i+1+1 == j+1 && (i+1) % n != 0)
          ||(i+1+n == j+1 && (i+1) <= N - n)){A[i][j] = -1;}
    else A[i][j] = 0;
  
  flops++;
  }
flops++;
}

//高速化したLU分解(外積形式ガウス法)
for (k=0; k<N; k++) {
  dtmp = 1.0 / A[k][k];
  /*
  for (i=k+1; i<N; i++) {
    A[i][k] = A[i][k]*dtmp;
    flops += 2;
  }
  */
  
/*(1-4)
ループアンローリングを行い、複数の処理を同時に行うことで、
処理時間の削減を試みた。(1-2のN=1024のときと比較した。)
その結果約3%ほどの処理時間の削減が出来たと思われれる。
*/


  for (i=k+1; i<N-8; i+=8) {
    double d1, d2, d3, d4, d5, d6, d7, d8;
    
    d1 = A[i][k];
    d2 = A[i+1][k];
    d3 = A[i+2][k];
    d4 = A[i+3][k];
    d5 = A[i+4][k];
    d6 = A[i+5][k];
    d7 = A[i+6][k];
    d8 = A[i+7][k];
    
    
    d1 *= dtmp;
    d2 *= dtmp;
    d3 *= dtmp;
    d4 *= dtmp;
    d5 *= dtmp;
    d6 *= dtmp;
    d7 *= dtmp;
    d8 *= dtmp;

    A[i][k] = d1;
    A[i+1][k] = d2;
    A[i+2][k] = d3;
    A[i+3][k] = d4;
    A[i+4][k] = d5;
    A[i+5][k] = d6;
    A[i+6][k] = d7;
    A[i+7][k] = d8;

    flops += 10;
  }


  for (j=k+1; j<N; j++) {
    dakj = A[k][j];
    
    /*
    for (i=k+1; i<N; i++) {
      A[i][j] = A[i][j] - A[i][k] *dakj;
      flops += 3;
    }*/

    for (i=k+1; i<N-4; i+=4) {
      double d1, d2, d3, d4;
      d1 = A[i][k];
      d2 = A[i+1][k];
      d3 = A[i+2][k];
      d4 = A[i+3][k];

      d1 *= dakj;
      d2 *= dakj;
      d3 *= dakj;
      d4 *= dakj;

      A[i][j] -= d1;
      A[i+1][j] -= d2;
      A[i+2][j] -= d3;
      A[i+3][j] -= d4;
      
      flops += 10;
    }
    
    flops ++;
  }
  flops += 2;
}

// 前進後退代入

for (i=0; i<N; i++) {
    
    if(i+1 == N/2 + n/2){B[i] = 1;}
    
    else B[i] = 0;
    
    flops++;
}

/*(1-4)
前進代入および後退代入では、配列の1要素に繰り返しアクセスを行っていた。
そこで、スカラ変数を用いて配列の要素を格納しその変数に対してアクセスを行うことで、
処理時間の削減を試みた。(1-2のN=1024のときと比較した。)
しかし、効果はあまり見られなかった。
*/

for (k=0; k<N; k++) {
  //c[k] = B[k];
  temp = B[k];

  for (j=0; j<k; j++) {
    //c[k] -= A[k][j] * c[j];
    temp -= A[k][j] * c[j];
    flops += 3;
  }

  c[k] = temp;
  flops++;
}

x[N-1] = c[N-1] / A[N-1][N-1];
flops += 10;

for(k=N-2; k>=0; k--) {
  //x[k] = c[k];
  temp = c[k];

  for(j=k+1; j<N; j++) {
    //x[k] -= A[k][j] * x[j];
    temp -= A[k][j] * x[j];
    flops += 4;
  }
  //x[k] = x[k] / A[k][k];
  temp = temp / A[k][k];

  x[k] = temp;
  flops += 13;
}

gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("N=%d, 実行時間:%f[s]\n", N, diff_time);
printf("flops=%lu, \n", flops);

m = N*N*N;
fprintf(fp1, "%d, %lu, %lu, %f\n", N, m, flops, diff_time);
l = l * 2;
}

return 0;

}
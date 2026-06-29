#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

int N = 1024; 

int main(void){

int i, j, i_eig, k, i_loop, MAX_ITER;
double dtemp, d, s, c, diff, dik, djk, dki, dkj, dlambda_bef, dlambda, EPS;
double x[N], x_old[N], y[N], Lambda[N];
double (*A)[N], (*Q)[N], (*T)[N], (*R)[N];
struct timeval time1;
struct timeval time2;
double diff_time;
unsigned long int flops, m;
A = (double (*)[N])malloc(N*N*sizeof(double));
Q = (double (*)[N])malloc(N*N*sizeof(double));
T = (double (*)[N])malloc(N*N*sizeof(double));
R = (double (*)[N])malloc(N*N*sizeof(double));
N = 5;
MAX_ITER = 100000;
EPS = 0.00000001;
i_eig = N-1;

printf("A\n");
//Aの生成
for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    
    if((i == 0 || j == 0) && (i <= 0 && j <= 0)){
        A[i][j] = 5;
    }
    else if((i == 1 || j == 1) && (i <= 1 && j <= 1)){
        A[i][j] = 4;
    }
    else if((i == 2 || j == 2) && (i <= 2 && j <= 2)){
        A[i][j] = 3;
    }
    else if((i == 3 || j == 3) && (i <= 3 && j <= 3)){
        A[i][j] = 2;
    }
    else A[i][j] = 1;
    printf("%f ", A[i][j]);
  }
printf("\n");
}

printf("\n");
//課題1-i

//初期R = A
for(i=0; i<=i_eig; i++ ){
  for(j=0; j<=i_eig; j++){
    R[i][j] = A[i][j];
  } 
}

//初期Q = I
    for (i=0; i<=i_eig; i++ ) {
      for(j=0; j<=i_eig; j++) {
        Q[i][j] = 0.0; 
      } 
    }
    
    for (i=0; i<=i_eig ; i++){
        Q[i][i] = 1.0;
    }

/*
for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    printf("%f ", Q[i][j]);
  }
printf("\n");
}
*/

// ギブンズ法によるQR分解
for(i=0; i<=i_eig ; i++) {
  for(j=i+1; j<=i_eig ; j++) {
// Rを作る
    d = sqrt (R[i][i] * R[i][i] + R[j][i] * R[j][i]);
    s = R[j][i] / d; 
    c = R[i][i] / d;
  
    for(k=0; k<=i_eig; k++){
      dik = R[i][k]; 
      djk = R[j][k];
      R[i][k] = dik*c + djk*s;
      R[j][k] = -dik*s + djk*c; 
    }
// Qを作る
    for(k=0; k<=i_eig; k++){
      dki = Q[k][i]; 
      dkj = Q[k][j];
      Q[k][i] = dki*c + dkj*s;
      Q[k][j] = -dki*s + dkj*c; 
    }
    
  }
}

/*
printf("Q\n");
for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    printf("%f ", Q[i][j]);
  }
printf("\n");
}

printf("\n");


for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    printf("%f ", R[i][j]);
  }
printf("\n");
}
*/

printf("QR = A\n");
//QRの積
for (i=0; i<=i_eig; i++) {
  for (j=0; j<=i_eig; j++) {
    A[i][j] = 0.0;

    for (k=0; k<=i_eig; k++) {
      A[i][j] += Q[i][k] * R[k][j];
    }
    printf("%f ", A[i][j]);
  }
printf("\n");  
}

printf("\n");
//Qの転置
for(i=0; i<=i_eig; i++) {
  for (j=0; j<=i_eig; j++) {
    T[j][i] = Q[i][j];
    }
}


printf("QQ = I\n");
//QQの積
for(i=0; i<=i_eig; i++) {
  for (j=0; j<=i_eig; j++) {
    A[i][j] = 0.0;
    
    for (k=0; k<=i_eig; k++) {
      A[i][j] += Q[i][k] * T[k][j];
    }
    printf("%f ", A[i][j]);
  }
printf("\n");  
}

printf("\n");
/*

N = 100;

//Aの生成
for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    
    if(i+1 == j+1){
        A[i][j] = 4;
    }
    
    else A[i][j] = 1;
  }

}

*/

//Aの生成
for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    
    if((i == 0 || j == 0) && (i <= 0 && j <= 0)){
        A[i][j] = 5;
    }
    else if((i == 1 || j == 1) && (i <= 1 && j <= 1)){
        A[i][j] = 4;
    }
    else if((i == 2 || j == 2) && (i <= 2 && j <= 2)){
        A[i][j] = 3;
    }
    else if((i == 3 || j == 3) && (i <= 3 && j <= 3)){
        A[i][j] = 2;
    }
    else A[i][j] = 1;
  }
}

//固有値を求めるループ
for(i_eig = N-1; i_eig >= 0; i_eig--){
  dlambda_bef = A[i_eig][i_eig];

//QR法の反復ループ
  for(i_loop=1; i_loop <= MAX_ITER; i_loop++){

//初期R = A
    for(i=0; i<=i_eig; i++ ){
      for(j=0; j<=i_eig; j++){
        R[i][j] = A[i][j];
      } 
    }

//初期Q = I
    for (i=0; i<=i_eig; i++ ) {
      for(j=0; j<=i_eig; j++) {
        Q[i][j] = 0.0; 
      } 
    }
    
    for (i=0; i<=i_eig ; i++){
        Q[i][i] = 1.0;
    }

// ギブンズ法によるQR分解
for(i=0; i<=i_eig ; i++) {
  for(j=i+1; j<=i_eig ; j++) {
// Rを作る
    d = sqrt (R[i][i] * R[i][i] + R[j][i] * R[j][i]);
    s = R[j][i] / d; 
    c = R[i][i] / d;
  
    for(k=0; k<=i_eig; k++){
      dik = R[i][k]; 
      djk = R[j][k];
      R[i][k] = dik*c + djk*s;
      R[j][k] = -dik*s + djk*c; 
    }
// Qを作る
    for(k=0; k<=i_eig; k++){
      dki = Q[k][i];
      dkj = Q[k][j];
      Q[k][i] = dki*c + dkj*s;
      Q[k][j] = -dki*s + dkj*c;
    }
    
  }
}

// RQの積
for (i=0; i<=i_eig; i++) {
  for (j=0; j<=i_eig; j++) {
    A[i][j] = 0.0;
    for (k=0; k<=i_eig; k++) {
      A[i][j] += R[i][k] * Q[k][j];
    }
  }
}

//収束判定
  dlambda = A[i_eig][i_eig];
  if(fabs(dlambda_bef-dlambda) / fabs(dlambda) < EPS){
    goto converge;
  }

//現在の固有値近似値
dlambda_bef = dlambda;
}
//固有値反復の終わり

//収束しない
//*N = -i_eig;
return;

//収束した
converge:
Lambda[i_eig] = A[i_eig][i_eig];
}

//対角要素に固有値が並んだAから全固有値を取り出す。
printf("Aの全固有値\n");
for (i=0; i<N; i++){
  for (j=0; j<N; j++) {
    if(i == j){
      printf("%f ", A[i][j]);
    }
  }
printf("\n");
}



return 0;

}
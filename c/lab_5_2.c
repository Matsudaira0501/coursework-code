#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

double f(double x);
double df(double x);

int main(void){

//積分範囲
double a=0;
double b=1;
//保証精度
double EPS = 0.000000001;
//分割数
int N=2;
//高さ
double h = (b-a)/2;
double T = h * (f(a)+2.0*f((a+b)/2)+f(b))/2;
double S = h * (f(a)+4.0*f((a+b)/2)+f(b))/3; 
int i, k;
double T_new, S_new;

printf("2-1\n");
//シンプソン則の計算
//printf("N=%d:%.10f\n", N, S);

while(1){
  N = 2*N;
  h = h/2;
  double s = 0.0;


  for(i = 1; i<= N-1; i+=2){
    s = s + f(a + i*h);
  }

  T_new = T/2 + h*s;
  S_new = (4.0*T_new - T)/3.0;

  //printf("N=2^%d:%.11f\n", ct, S_new); //積分値を返す
  
  if (fabs(S_new - S) < (EPS * fabs(S_new))){
    break;
  }
  
  T = T_new;
  S = S_new;
}

printf("シンプソン則の答え:%.15f\n", S_new);
//保証精度10桁を満たす。

//ロンバーグ積分の計算
N=1;
int n = 0;
h = b-a;
double TT[10][10];
TT[0][0] = h * (f(a) + f(b)) /2.0;



while(1){
  N = 2*N;
  h=h/2.0; 
  double s = 0.0;

  for(i = 1; i<= N - 1; i+=2){
    s = s + f(a + i * h);
  }

  TT[n+1][0] = TT[n][0] / 2.0 + h * s;
 
  if (N == 512){
    break;
  }
  n++;
}
//ロンバーク積分法
for(k=1; k<=9; k++){ 
  for(n=k; n<=9; n++) {
    TT[n][k] = (pow(4.0, (double) k) * TT[n][k-1] - TT[n-1][k-1]) / (pow (4.0, (double)k) - 1.0);
    //printf("T[%d][%d]=%.20f\n", n, k, TT[n][k]);
  }
}


printf("ロンバーグ積分の答え:%.15f\n", TT[5][5]);

double ans = df(1) - df(0);
printf("(積分の答え:%.15f)\n", ans);

/*
ロンバーグ積分の答えと本来の積分の答えは全く同じものとなった。
本来この結果はありえないので、桁落ちや情報落ちを起こしていると考えられる。
*/

printf("2-2\n添付ファイル参照\n");

//各グラフについて(x, y)をプロットし、csvファイルで出力しExcelでグラフにした。
//以下のコードからわかるように
//kaiseki5-1.csvが台形則
//kaiseki5-2.csvがシンプソン則
//kaiseki5-3.csvがT[3][3]
//kaiseki5-4.csvがT[5][5] である。

FILE *fp;
//台形則
fp = fopen("kaiseki5-1.csv", "w");
    if(fp == NULL){
        printf("file not open");
        return -1;
}

N=1;
h = b-a;
T = h * (f(a) + f(b)) /2.0;

fprintf(fp, "%d,%.10f\n", N, ans-T);

while(1){
  N = 2*N;
  h=h/2.0; 
  double s = 0.0;

  for(i = 1; i<= N - 1; i+=2){
    s = s + f(a + i * h);
  }

  T_new = T / 2.0 + h * s;
  fprintf(fp, "%d,%.10f\n", N, ans-T_new); //積分値を返す
  if (fabs(T_new - T) < (EPS * fabs(T_new))){
    break;
  }
  
  T = T_new;
}

FILE *fp1;
//シンプソン則
fp1 = fopen("kaiseki5-2.csv", "w");
    if(fp1 == NULL){
        printf("file not open");
        return -1;
}

N = 2;
h = (b-a)/2; 
T = h * (f(a)+2.0*f((a+b)/2)+f(b))/2;
S = h * (f(a)+4.0*f((a+b)/2)+f(b))/3; 

fprintf(fp1, "%d,%.10f\n", N, ans-S);

while(1){
  N = 2*N;
  h = h/2;
  double s = 0.0;


  for(i = 1; i<= N-1; i+=2){
    s = s + f(a + i*h);
  }

  T_new = T/2 + h*s;
  S_new = (4.0*T_new - T)/3.0;

  fprintf(fp1, "%d,%.11f\n", N, ans-S_new); //積分値を返す
  
  if (fabs(S_new - S) < (EPS * fabs(S_new))){
    break;
  }
  
  T = T_new;
  S = S_new;
}


FILE *fp2;
//T[n][n]
fp2 = fopen("kaiseki5-3.csv", "w");
    if(fp2 == NULL){
        printf("file not open");
        return -1;
}

N=1;
n = 0;
h = b-a;
TT[0][0] = h * (f(a) + f(b)) /2.0;

fprintf(fp2, "%d,%.10f\n", N-1, ans-TT[0][0]);


while(1){
  N = 2*N;
  h=h/2.0; 
  double s = 0.0;

  for(i = 1; i<= N - 1; i+=2){
    s = s + f(a + i * h);
  }

  TT[n+1][0] = TT[n][0] / 2.0 + h * s;
 
  if (N == 32){
    break;
  }
  n++;
}
//ロンバーク積分法
for(k=1; k<=5; k++){ 
  for(n=k; n<=5; n++) {
    TT[n][k] = (pow(4.0, (double) k) * 
    TT[n][k-1] - TT[n-1][k-1])/ (pow (4.0, (double)k) - 1.0);
    if(n == k){
      fprintf(fp2, "%d,%.20f\n", n, ans-TT[n][k]);
    }
  }

}

return 0;
}

/*(3)考察
T[5][5] と ans が完全に一致してしまったため誤差は0となった。
これは、正しい結果とは言えず情報落ちや桁落ちを起こしている可能性があると考えられる。
高精度かつ高速に積分結果を得るためには、シンプソン則かロンバーグ積分を用いるとよいだろう。
*/

double f(double x){
return cos(x)*exp(x);
}

double df(double x){
return 0.5 * exp(x) * (sin(x) + cos(x));
}
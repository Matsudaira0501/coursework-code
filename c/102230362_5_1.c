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
int N=1;
//高さ
double h = b-a; 
//分割数1の時の台形則
double T = h * (f(a) + f(b)) /2.0;
int i, ct;
double T_new;

printf("1-1\n");
//演算量削減前の計算
//printf("演算量削減前\n");
//printf("N=%d:%.10f\n", N, T);

ct = 0;
while(1){
  ct+=1;
  N = 2*N;
  h = (b-a)/N;
  T_new=0.0;


  for(i = 0; i< N; i++){
    double t = ((f(a+h*i) + f(a+h*(i+1))) / 2.0) * h;
    T_new = T_new + t;
  }

  //printf("N=2^%d:%.10f\n", ct, T_new); //積分値を返す
  
  if (fabs(T_new - T) < (EPS * fabs(T_new))){
    break;
  }
  
  T = T_new;
}

printf("削減前の答え:%.10f\n", T_new);

//演算量削減後の計算
N=1;
h = b-a;
T = h * (f(a) + f(b)) /2.0;

//printf("演算量削減後\n");
//printf("N=%d:%.10f\n", N, T);

ct = 0;
while(1){
  ct+=1;
  N = 2*N;
  h=h/2.0; 
  double s = 0.0;

  for(i = 1; i<= N - 1; i+=2){
    s = s + f(a + i * h);
  }

  T_new = T / 2.0 + h * s;
  //printf("N=2^%d:%.10f\n", ct, T_new); //積分値を返す
  if (fabs(T_new - T) < (EPS * fabs(T_new))){
    break;
  }
  
  T = T_new;
}

printf("削減後の答え:%.10f\n", T_new);

double ans = df(1) - df(0);
printf("(積分の答え:%.10f)\n", ans);
//このように保証精度10桁を満たす。

printf("1-2\n");
struct timeval time1;
struct timeval time2;
double diff_time;


printf("演算量削減前\n");
//演算量削減前の計算時間の計測
N=1;
h = b-a;
T = h * (f(a) + f(b)) /2.0;
ct = 0;

gettimeofday(&time1, NULL);
while(ct <= 24){
  ct+=1;
  N = 2*N;
  h = (b-a)/N;
  T_new=0.0;


  for(i = 0; i< N; i++){
    double t = ((f(a+h*i) + f(a+h*(i+1))) / 2.0) * h; 
    T_new = T_new + t; //411flops * N回ループ
  }

  //printf("N=2^%d:%.10f\n", ct, T_new); //積分値を返す
  
  /*
  if (fabs(T_new - T) < (EPS * fabs(T_new))){
    break;
  }
  */
  
  T = T_new;
}
//Σ[1...25](411*2^k+12) flops

gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("実行時間:%f[s]\n", diff_time);
printf("演算量:約 2.76×10^10 flops\n");


gettimeofday(&time1, NULL);
printf("演算量削減後\n");
//演算量削減後の計算時間の計測
N=1;
h = b-a;
T = h * (f(a) + f(b)) /2.0;
ct = 0;

while(ct <= 24){
  ct+=1;
  N = 2*N;
  h=h/2.0; 
  double s = 0.0;

  for(i = 1; i<= N - 1; i+=2){
    s = s + f(a + i * h); //205flops * N/2回ループ
  }

  T_new = T / 2.0 + h * s;
  
  //printf("N=2^%d:%.10f\n", ct, T_new); //積分値を返す
  
  /*
  if (fabs(T_new - T) < (EPS * fabs(T_new))){
    break;
  }
  */

  T = T_new;
}
//Σ[1...25](205*2^(k-1)+6) flops
gettimeofday(&time2, NULL);
diff_time = time2.tv_sec - time1.tv_sec +  (double)(time2.tv_usec - time1.tv_usec) / 1000000;
printf("実行時間:%f[s]\n", diff_time);
printf("演算量:約 6.88×10^9 flops\n");

/*(3)考察
実行時間、演算量ともに計算量削減前と後で4:1となった。
削減前の式でf()に値を代入するのを1回に減らすことができれば計算量は半減するだろうと予想できる。

改善前
for(i = 0; i< N; i++){
    double t = ((f(a+h*i) + f(a+h*(i+1))) / 2.0) * h;
    T_new = T_new + t;
  }

改善後
T_new += f(a) * h/2;
for(i = 1; i< N-1; i++){
    double t = f(a+h*i) * h;
    T_new = T_new + t;
  }
T_new += f(b) * h/2;

*/


return 0;
}

double f(double x){
return cos(x)*exp(x);
}

double df(double x){
return 0.5 * exp(x) * (sin(x) + cos(x));
}
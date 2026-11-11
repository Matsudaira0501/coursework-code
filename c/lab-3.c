#include <stdio.h>
#include <math.h>


/*f(0)=0は自明であり、f(x)は偶関数であるから0<x<5の範囲のみを考える。
f(1)<0, f(2)>0、f(4)>0, f(5)<0であることから2文法、
初期捜索点として、x1 = 1.5, x2　=　4.5 を利用しニュートン法を用いた。


2文法(0<x<5の範囲)

1. 2つ
2. 1.44741821289062500000, 4.71238708496093750000
3. 15回, 16回
4. 3075flops, 3280flops(多少の誤差あり)

ニュートン法

1. 2つ
2. 1.44741427129623678738, 4.71238898061156774588
3. 4回, 4回
4. 1680flops, 1680flops(多少の誤差あり)

範囲が-5<x<5の場合は2にマイナスを付けたものと0を追加し、それ以外を2倍にする

まとめ

5. 初期値によると思われるが、今回は二分法よりもニュートン法の方が
反復回数も計算量も少なくなった。ニュートン法は二分法に比べ1ループの計算量
が約2倍となったため、ニュートン法のループ回数の2倍が二分法のループ回数よりも
少なければニュートン法の方が時間的効率が良いと考えられる。

*/
double f(double x);
double df(double x);

int main(void){
  double x_start1 = 1.0;
  double x_start2 = 2.0;
  double err = 100.0;
  double a = x_start1;
  double b = x_start2;
  double c;
  int ct = 0;

  while(err>0.0000000001){
      ct += 1;
      c = (a+b)/2.0;
      double fc = f(c);
      err = fc*fc;
      if(fc > 0){
        b = c;
      }
      else{
        a = c;
      }
      printf("%d:%.20f\n",ct, c);
  }
  printf("result:%.20f\n",c);
  
  double x_start3 = 5.0;
  double x_start4 = 4.0;
  double err1 = 100.0;
  double a1 = x_start3;
  double b1 = x_start4;
  double c1;
  int ct1 = 0;

  while(err1>0.0000000001){
      ct1 += 1;
      c1 = (a1+b1)/2.0;
      double fc1 = f(c1);
      err1 = fc1*fc1;
      if(fc1 > 0){
        b1 = c1;
      }
      else{
        a1 = c1;
      }
      printf("%d:%.20f\n",ct1, c1);
  }
  printf("result:%.20f\n",c1);

  double ERR = 0.0000000001;
  int ct2 = 0;
  int ct3 = 0;
  double x1 = 1.5;
  double x2 = 4.5;

  double xn; // x_new x_old
    while(1){
        ct2++;
        xn = -1 *f(x1) / df(x1) + x1;
        printf("%d:%.20f\n",ct2, xn);
      if(fabs(xn - x1) < ERR * fabs(xn)){break;}
        x1 = xn;
    }
    printf("result:%.20f\n", xn);

  double xn1; // x_new x_old
    while(1){
        ct3++;
        xn1 = -1 *f(x2) / df(x2) + x2;
        printf("%d:%.20f\n",ct3, xn1);
      if(fabs(xn1 - x2) < ERR * fabs(xn1)){break;}
        x2 = xn1;
    }
    printf("result:%.20f\n", xn1);

}
double f(double x){
  return exp(-(x*x))-cos(x);  //50+1+1+150=202flops?
}

double df(double x){
  return (-2)*x*exp(-(x*x))+sin(x);  //1+1+50+1+1+150=204flops?
}
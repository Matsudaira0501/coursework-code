#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){

    //変数型宣言
    double a = 1;
    double b = -300000000.00000002;
    double c = 6;

    //解の公式にそのまま代入
    double x1 = (-b + sqrt(b*b - 4*a*c)) / 2*a;
    double x2 = (-b - sqrt(b*b - 4*a*c)) / 2*a;

    printf("誤った答えは、x=%e, %e \n", x1, x2);

    //講義で紹介された方法を使用
    //b>>4acより、√b^2-4ac ≒ b となり、
    //-b - sqrt(b*b - 4*a*c)で桁落ちが発生するのを防ぐため、
    //x2において、分子の有理化を行った。
    double x3 = (-b + sqrt(b*b - 4*a*c)) / 2*a;
    double x4 = -2*c /(b - sqrt(b*b - 4*a*c));

    printf("正しい答えは、x=%e, %e \n", x3, x4);


    return 0;
}
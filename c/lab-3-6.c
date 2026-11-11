#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);

int main(void)
{
    int a, b, c;
    
    printf("Input a: ");
    scanf("%d", &a);
    printf("Input b: ");
    scanf("%d", &b);

    if ((a < 0) || (b < 0)){
        printf("Error! Invalid value: a= %d, b= %d\n", a, b);
        exit(EXIT_FAILURE);
    }

    if (a < b){ 
        c = a; a = b; b = c;
    }

    printf("gcd(%d,%d)= %d\n", a, b, gcd(a, b));
    return 0;
}

int gcd(int a, int b)
{
    int m,n,r;
    m = a; n = b;

    while (1){
        r = m % n; m = n; n = r;
        if (n == 0){
            return m;
            break;
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#define MAX 20

void msort(int data[], int first, int last);
void display( int data[], int n );

int main(){
  int i, a[MAX], b[MAX];

  srand(0);

  for( i = 0 ; i < MAX ; i++ ){
    a[i] = MAX - i;
    a[i] = rand() % MAX;
  }

  display( a, MAX );

  msort( a, 0, MAX-1 ); 

  display( a, MAX );
  
  return 0;
}

void msort(int data[], int first, int last) {
  int x, tmp[MAX], i, j, k;
  if( first >= last ) return;
  x = (first+last)/2;
  msort( data, first, x );
  msort( data, x+1, last );
  for( i = first ; i <= x ; i++ ) tmp[i] = data[i];
  for( i = j = first, k = x+1 ; i <= last ; i++ ){
    if( j <= x && (k > last || tmp[j] <= data[k] ) ) {
      data[i] = tmp[j];
      j ++;
    } else {
      data[i] = data[k];
      k ++;
    }
  }
  return;
}

void display( int data[], int n ){
  int i;
  for( i = 0 ; i < n ; i++ ){
    printf("%2d ", data[i] );
  }
  printf("\n");
  return;
}

//35行目"<"の後に"="を追加した。
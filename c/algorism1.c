#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAX 100000

void bubblesort(int data[], int n);
void insertsort( int data[], int n );
void quicksort(int data[], int first, int last);
void selectionsort(int data[], int n);
void combsort(int data[], int n);

int main(void){
  int i, a[MAX];
  clock_t start_clock, end_clock;

  //srand
  srand(3);

  for( i = 0 ; i < MAX ; i++ )
    a[i] = rand() % MAX;
  //最良
  //  a[i] = i; 
  //最大
  //  a[i] = MAX - i;
  start_clock = clock();

//for( i = 0 ; i < MAX ; i++ )
  //printf("%d \n", a[i]);
   
  //ソート種類
  //  bubblesort( a, MAX ); 
  //  insertsort( a, MAX); 
  //  quicksort( a, 0, MAX-1 );
  //  selectionsort( a, MAX);
    combsort( a, MAX);

//for( i = 0 ; i < MAX ; i++ )
  //printf("%d \n", a[i]);
  
  end_clock = clock();

  printf("clock: %f \n", (double)(end_clock - start_clock) / CLOCKS_PER_SEC);

  return 0;
}

void bubblesort(int data[], int n)
{
  int i, j, t, sw = 1;
  for( i = 1 ; sw && i < n ; i++ ){
    sw = 0;
    for( j = n - 1 ; j >= i ; j-- ){
      if( data[j-1] > data[j] ){
	t = data[j-1];
	data[j-1] = data[j];
	data[j] = t;
	sw = 1;
      }
    }
  }
  return;
}

void insertsort( int data[], int n )
{
  int i, j, k, t;

  for( i=1 ; i < n ; i++ ){
    for( j=i-1 ; j >= 0 ; j-- ){
      if( data[j+1] < data[j] ){
	t = data[j+1];
	data[j+1] = data[j];
	data[j] = t;
      }else{
	break;
      }
    }
  }
  return;
}

void quicksort(int data[], int first, int last)
{
  int i, j, x, t;
  x = data[(first+last)/2];
  i = first;
  j = last;
  while(1){
    while( data[i] < x ) i++;
    while( x < data[j] ) j--;
    if( i >= j ) break;
    t = data[i]; 
    data[i] = data[j]; 
    data[j] = t;
    i++;
    j--;
  }
  if( first < i-1 )
    quicksort(data,first,i-1); 
  if( j+1 < last )
    quicksort(data,j+1,last);
}

void selectionsort( int data[], int n )
{
  int i, j, min, tmp;

  for( i=0; i <= n; i++){
    min = i;
  
    for( j=i; j <= n; j++){
      if(data[j] < data[min]){
        min =j;
      }
    }
  tmp = data[i];
  data[i] = data[min];
  data[min] = tmp;
  } 
  return;
}

void combsort( int data[], int n)
{
  int i, gap = n;
  bool done = false;

  while (gap > 1 || !done) {
    gap = (gap * 10) / 13;
    if (!gap) {
      gap = 1;}
    
    
    done = true;
    for (i = 0; i < n - gap; i++) {
      if (data[i] > data[i + gap]) {
        int tmp = data[i];
        data[i] = data[i + gap];
        data[i + gap] = tmp;
        done = false;
      }
    }
  }
  return;
}
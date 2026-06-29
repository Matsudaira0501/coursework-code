#include <stdio.h>
#include <stdlib.h>

#define MAX 15 /* 2分木の要素数の最大値 */

void preorder(char heap[], int i);
void inorder(char heap[], int i);
void postorder(char heap[], int i);

int main(){

  /* 文字を要素とするヒープによる完全2分木 */
  char heap[MAX] = { 'a', 'b', 'c', 'd', 'e',
		     'f', 'g', 'h', 'i', 'j',
		     '\0', '\0', '\0', '\0', '\0' };

  preorder(heap, 0);
  putchar('\n');
  inorder(heap, 0);
  putchar('\n');
  postorder(heap, 0);
  putchar('\n');

  return 0;
}

/* 先行順の評価でノードのラベルを印字する関数 */
void preorder(char heap[], int i){
  if( i >= MAX || heap[i] == '\0' ) return;
  printf("%c ", heap[i]);
  preorder(heap, 2*i+1);
  preorder(heap, 2*i+2);
  return;
}

/* 中間順の評価でノードのラベルを印字する関数 */
void inorder(char heap[], int i){
  if( i >= MAX || heap[i] == '\0' ) return;
  inorder(heap, 2*i+1);
  printf("%c ", heap[i]);
  inorder(heap, 2*i+2);
  return;
}

/* 後行順で評価でノードのラベルを印字する関数 */
void postorder(char heap[], int i){
  if( i >= MAX || heap[i] == '\0' ) return;
  postorder(heap, 2*i+1);
  postorder(heap, 2*i+2);
  printf("%c ", heap[i]);
  return;
}

/*実行後、結果がスライドと一致した。*/
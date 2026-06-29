#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int price;
    int quantity;
} Product;

// 製品を追加する関数
void add_product(Product **list, int *count, const char *name, int price, int quantity) {

}

// 製品リストを表示する関数
void display_products(Product *list, int count) {
    int i;
    printf("Product List:\n");
    for (i = 0; i < count; i++) {
        printf("Name: %s, Price: %d, Quantity: %d\n", list[i].name, list[i].price, list[i].quantity);
    }
}

// 製品を削除する関数
void remove_product(Product **list, int *count, int index) {

}

// メモリを解放する関数
void free_list(Product *list) {
    free(list);
}

int main() {
    Product *product_list = NULL;
    int product_count = 0;

    // 製品を追加
    add_product(&product_list, &product_count, "Laptop", 200000, 10);
    add_product(&product_list, &product_count, "Smartphone", 150000, 25);
    add_product(&product_list, &product_count, "Tablet", 100000, 15);

    // 製品リストの表示
    display_products(product_list, product_count);

    // 製品の削除
    remove_product(&product_list, &product_count, 1); // Smartphoneを削除

    // 製品リストの再表示
    printf("\nAfter removal:\n");
    display_products(product_list, product_count);

    // メモリを解放
    free_list(product_list);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Struktur Node untuk BST
struct Node {
    int key;
    struct Node* left;
    struct Node* right;
};

// Fungsi untuk membuat node baru
struct Node* createNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Fungsi untuk menyisipkan sebuah key pada BST
struct Node* insert(struct Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

// Fungsi untuk mencari sebuah key dalam BST
struct Node* search(struct Node* root, int key) {
    // Jika root adalah NULL atau key ditemukan pada root
    if (root == NULL || root->key == key)
        return root;

    // Jika key lebih besar dari root, maka pencarian dilakukan pada subtree kanan
    if (root->key < key)
        return search(root->right, key);

    // Jika key lebih kecil dari root, maka pencarian dilakukan pada subtree kiri
    return search(root->left, key);
}

// Fungsi utama
int main() {
    struct Node* root = NULL;
    int n = 10000; // Jumlah elemen
    clock_t start, end;

    // Menyisipkan data secara sekuensial
    start = clock();
    for (int i = 1; i <= n; i++) {
        root = insert(root, i);
    }
    end = clock();
    printf("Waktu penyisipan BST: %f detik\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Mencari elemen terakhir
    start = clock();
    search(root, n);
    end = clock();
    printf("Waktu pencarian BST: %f detik\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}

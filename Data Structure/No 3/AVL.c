#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Struktur Node untuk AVL Tree
struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
};

// Fungsi untuk mendapatkan ketinggian sebuah node
int height(struct Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Fungsi untuk mendapatkan nilai maksimum dari dua bilangan
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fungsi untuk membuat node baru
struct Node* createNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Fungsi untuk melakukan rotasi kanan
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Fungsi untuk melakukan rotasi kiri
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Fungsi untuk mendapatkan faktor keseimbangan node
int getBalance(struct Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Fungsi untuk menyisipkan sebuah key pada AVL tree
struct Node* insert(struct Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Fungsi untuk mencari sebuah key dalam AVL tree
struct Node* search(struct Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (root->key < key)
        return search(root->right, key);

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
    printf("Waktu penyisipan AVL Tree: %f detik\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Mencari elemen terakhir
    start = clock();
    search(root, n);
    end = clock();
    printf("Waktu pencarian AVL Tree: %f detik\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}

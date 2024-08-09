#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur Metadata untuk menyimpan informasi tentang file
typedef struct Metadata {
    char filename[100];
    int size;
    char created[20];
    char modified[20];
    char permissions[20];
} Metadata;

// Struktur Node AVL Tree
typedef struct Node {
    Metadata data;
    struct Node *left;
    struct Node *right;
    int height;
} Node;


// Fungsi untuk mendapatkan nilai maksimum dari dua integer
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fungsi untuk mendapatkan tinggi sebuah node
int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Fungsi untuk membuat node baru dengan metadata yang diberikan
Node *createNode(Metadata data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // Node baru memiliki tinggi 1
    return node;
}

// Fungsi untuk melakukan rotasi kanan
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    // Perbarui tinggi
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Kembalikan root baru
    return x;
}

// Fungsi untuk melakukan rotasi kiri
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    // Perbarui tinggi
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Kembalikan root baru
    return y;
}

// Fungsi untuk mendapatkan faktor keseimbangan dari node
int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}


// Fungsi untuk menyisipkan node baru ke dalam AVL Tree
Node *insert(Node *node, Metadata data) {
    if (node == NULL)
        return createNode(data);

    // Penyisipan berdasarkan nama file
    if (strcmp(data.filename, node->data.filename) < 0)
        node->left = insert(node->left, data);
    else if (strcmp(data.filename, node->data.filename) > 0)
        node->right = insert(node->right, data);
    else
        return node; // Duplikat tidak diizinkan

    // Perbarui tinggi node ini
    node->height = 1 + max(height(node->left), height(node->right));

    // Dapatkan faktor keseimbangan node ini
    int balance = getBalance(node);

    // Jika node ini menjadi tidak seimbang, ada 4 kasus

    // Kasus Kiri-Kiri
    if (balance > 1 && strcmp(data.filename, node->left->data.filename) < 0)
        return rightRotate(node);

    // Kasus Kanan-Kanan
    if (balance < -1 && strcmp(data.filename, node->right->data.filename) > 0)
        return leftRotate(node);

    // Kasus Kiri-Kanan
    if (balance > 1 && strcmp(data.filename, node->left->data.filename) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Kasus Kanan-Kiri
    if (balance < -1 && strcmp(data.filename, node->right->data.filename) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Kembalikan pointer node (tidak berubah jika tidak seimbang)
    return node;
}


// Fungsi untuk mencari node berdasarkan nama file
Node *search(Node *root, const char *filename) {
    if (root == NULL || strcmp(filename, root->data.filename) == 0)
        return root;

    if (strcmp(filename, root->data.filename) < 0)
        return search(root->left, filename);

    return search(root->right, filename);
}


// Fungsi untuk mendapatkan node dengan nilai terkecil (untuk penghapusan)
Node *getMinValueNode(Node *node) {
    Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Fungsi untuk menghapus node dari AVL Tree
Node *delete(Node *root, const char *filename) {
    if (root == NULL)
        return root;

    // Penghapusan berdasarkan nama file
    if (strcmp(filename, root->data.filename) < 0)
        root->left = delete(root->left, filename);
    else if (strcmp(filename, root->data.filename) > 0)
        root->right = delete(root->right, filename);
    else {
        // Node dengan satu anak atau tanpa anak
        if (root->left == NULL || root->right == NULL) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            // Node dengan dua anak: Dapatkan pengganti terkecil
            Node *temp = getMinValueNode(root->right);
            root->data = temp->data;
            root->right = delete(root->right, temp->data.filename);
        }
    }

    if (root == NULL)
        return root;

    // Perbarui tinggi node ini
    root->height = 1 + max(height(root->left), height(root->right));

    // Dapatkan faktor keseimbangan node ini
    int balance = getBalance(root);

    // Jika node ini menjadi tidak seimbang, ada 4 kasus

    // Kasus Kiri-Kiri
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Kasus Kiri-Kanan
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Kasus Kanan-Kanan
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Kasus Kanan-Kiri
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


int main() {
    Node *root = NULL;

    Metadata file1 = {"file1.txt", 1234, "2024-01-01", "2024-01-02", "rw-r--r--"};
    Metadata file2 = {"file2.txt", 5678, "2024-01-02", "2024-01-03", "rw-r--r--"};
    Metadata file3 = {"file3.txt", 910, "2024-01-03", "2024-01-04", "rw-r--r--"};

    // Menyisipkan file ke dalam AVL Tree
    root = insert(root, file1);
    root = insert(root, file2);
    root = insert(root, file3);

    // Menampilkan metadata secara in-order
    printf("In-order traversal of the AVL Tree:\n");
    printInOrder(root);

    // Mencari file
    Node *searchResult = search(root, "file2.txt");
    if (searchResult != NULL) {
        printf("File found:\n");
        printMetadata(searchResult->data);
    } else {
        printf("File not found\n");
    }

    // Menghapus file
    printf("Deleting file1.txt...\n");
    root = delete(root, "file1.txt");

    // Menampilkan metadata secara in-order setelah penghapusan
    printf("In-order traversal after deletion:\n");
    printInOrder(root);

    // Mencari file yang dihapus
    searchResult = search(root, "file1.txt");
    if (searchResult != NULL) {
        printf("File found:\n");
        printMetadata(searchResult->data);
    } else {
        printf("File successfully deleted\n");
    }

    return 0;
}

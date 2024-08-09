#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur Metadata
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

// Fungsi untuk membuat node baru 
Node *createNode(Metadata data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    return node;
}

// Fungsi untuk melakukan rotasi kanan
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Fungsi untuk melakukan rotasi kiri
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

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

    if (strcmp(data.filename, node->data.filename) < 0)
        node->left = insert(node->left, data);
    else if (strcmp(data.filename, node->data.filename) > 0)
        node->right = insert(node->right, data);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // LL
    if (balance > 1 && strcmp(data.filename, node->left->data.filename) < 0)
        return rightRotate(node);

    // RR
    if (balance < -1 && strcmp(data.filename, node->right->data.filename) > 0)
        return leftRotate(node);

    // LR
    if (balance > 1 && strcmp(data.filename, node->left->data.filename) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (balance < -1 && strcmp(data.filename, node->right->data.filename) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

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

// Fungsi untuk mendapatkan node dengan nilai terkecil 
Node *getMinValueNode(Node *node) {
    Node *current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Fungsi untuk menghapus node dari AVL Tree
Node *deleteNode(Node *root, const char *filename) {
    if (root == NULL)
        return root;

    if (strcmp(filename, root->data.filename) < 0)
        root->left = deleteNode(root->left, filename);
    else if (strcmp(filename, root->data.filename) > 0)
        root->right = deleteNode(root->right, filename);
    else {
        if (root->left == NULL || root->right == NULL) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            Node *temp = getMinValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.filename);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // LL
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LR
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RL
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Fungsi untuk mencetak metadata 
void printMetadata(Metadata data) {
    printf("Filename: %s\n", data.filename);
    printf("Size: %d bytes\n", data.size);
    printf("Created: %s\n", data.created);
    printf("Modified: %s\n", data.modified);
    printf("Permissions: %s\n", data.permissions);
    printf("\n");
}

// Fungsi untuk mencetak AVL Tree secara in-order
void printInOrder(Node *root) {
    if (root != NULL) {
        printInOrder(root->left);
        printMetadata(root->data);
        printInOrder(root->right);
    }
}

int main() {
    Node *root = NULL;

    Metadata file1 = {"file1.txt", 128, "2021-01-01", "2021-12-01", "rw-r--r--"};
    Metadata file2 = {"file2.txt", 256, "2022-01-01", "2022-12-01", "rw-r--r--"};
    Metadata file3 = {"file3.txt", 512, "2023-01-01", "2023-12-01", "rw-r--r--"};
    Metadata file4 = {"file4.txt", 1024, "2024-01-01", "2024-12-01", "rw-r--r--"};

    // Menyisipkan file ke dalam AVL Tree
    root = insert(root, file1);
    root = insert(root, file2);
    root = insert(root, file3);
    root = insert(root, file4);

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
    printf("Deleting file3.txt...\n");
    root = deleteNode(root, "file3.txt");

    // Menampilkan metadata secara in-order setelah penghapusan
    printf("In-order traversal after deletion:\n");
    printInOrder(root);

    // Mencari file yang dihapus
    searchResult = search(root, "file2.txt");
    if (searchResult != NULL) {
        printf("File found:\n");
        printMetadata(searchResult->data);
    } else {
        printf("File successfully deleted\n");
    }

    return 0;
}

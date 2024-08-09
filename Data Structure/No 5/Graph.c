#include <stdio.h>
#include <stdlib.h>

// Struktur untuk menyimpan sisi graf
typedef struct {
    int u, v, weight;
} Edge;

// Struktur untuk Union-Find
typedef struct {
    int *parent;
    int *rank;
    int size;
} UnionFind;

// Fungsi untuk inisialisasi Union-Find
UnionFind* createUnionFind(int size) {
    UnionFind *uf = (UnionFind *)malloc(sizeof(UnionFind));
    uf->parent = (int *)malloc(size * sizeof(int));
    uf->rank = (int *)malloc(size * sizeof(int));
    uf->size = size;
    for (int i = 0; i < size; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    return uf;
}

// Fungsi untuk menemukan root dari sebuah elemen
int find(UnionFind *uf, int u) {
    if (uf->parent[u] != u) {
        uf->parent[u] = find(uf, uf->parent[u]);
    }
    return uf->parent[u];
}

// Fungsi untuk menggabungkan dua set
void unionSets(UnionFind *uf, int u, int v) {
    int rootU = find(uf, u);
    int rootV = find(uf, v);
    if (rootU != rootV) {
        if (uf->rank[rootU] > uf->rank[rootV]) {
            uf->parent[rootV] = rootU;
        } else if (uf->rank[rootU] < uf->rank[rootV]) {
            uf->parent[rootU] = rootV;
        } else {
            uf->parent[rootV] = rootU;
            uf->rank[rootU]++;
        }
    }
}

// Fungsi untuk membandingkan dua sisi 
int compareEdges(const void *a, const void *b) {
    return ((Edge *)a)->weight - ((Edge *)b)->weight;
}

// Fungsi untuk menjalankan algoritma Kruskal
void kruskal(int n, Edge edges[], int numEdges) {
    UnionFind *uf = createUnionFind(n);
    qsort(edges, numEdges, sizeof(Edge), compareEdges); 

    int totalWeight = 0;
    printf("Edges in MST:\n");
    for (int i = 0; i < numEdges; i++) {
        Edge edge = edges[i];
        if (find(uf, edge.u) != find(uf, edge.v)) {
            unionSets(uf, edge.u, edge.v);
            printf("%d - %d: %d km\n", edge.u, edge.v, edge.weight);
            totalWeight += edge.weight;
        }
    }
    printf("Total weight: %d km\n", totalWeight);

    free(uf->parent);
    free(uf->rank);
    free(uf);
}

int main() {
    // Definisikan sisi-sisi graf
    Edge edges[] = {
        {0, 1, 30},
        {0, 2, 20},
        {0, 3, 10},
        {1, 3, 5},
        {1, 4, 25},
        {2, 3, 13},
        {2, 4, 40}
    };
    int numEdges = sizeof(edges) / sizeof(edges[0]);

    int n = 5; 
    kruskal(n, edges, numEdges);

    return 0;
}

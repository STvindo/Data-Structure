#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definisikan ukuran maksimal teks dan stack
#define MAX 100

// Struktur untuk stack
typedef struct {
    char data[MAX][MAX];
    int top;
} Stack;

// Inisialisasi stack
void initStack(Stack *s) {
    s->top = -1;
}

// Memeriksa apakah stack kosong
int isEmpty(Stack *s) {
    return s->top == -1;
}

// Memeriksa apakah stack penuh
int isFull(Stack *s) {
    return s->top == MAX - 1;
}

// Push data ke stack
void push(Stack *s, char *str) {
    if (!isFull(s)) {
        strcpy(s->data[++(s->top)], str);
    }
}

// Pop data dari stack
void pop(Stack *s, char *str) {
    if (!isEmpty(s)) {
        strcpy(str, s->data[(s->top)--]);
    }
}

// Struktur untuk editor teks
typedef struct {
    char content[MAX];
    Stack undo_stack;
    Stack redo_stack;
} TextEditor;

// Inisialisasi editor teks
void initEditor(TextEditor *editor) {
    editor->content[0] = '\0';
    initStack(&editor->undo_stack);
    initStack(&editor->redo_stack);
}

// Mengetikkan teks ke editor
void typeText(TextEditor *editor, char *text) {
    // Simpan status saat ini ke undo stack
    push(&editor->undo_stack, editor->content);
    // Gabungkan teks baru dengan konten yang ada
    strcat(editor->content, text);
    // Kosongkan redo stack
    initStack(&editor->redo_stack);
}

// Undo operasi terakhir
void undo(TextEditor *editor) {
    if (!isEmpty(&editor->undo_stack)) {
        // Simpan status saat ini ke redo stack
        push(&editor->redo_stack, editor->content);
        // Kembalikan status sebelumnya dari undo stack
        pop(&editor->undo_stack, editor->content);
    }
}

// Redo operasi terakhir yang di-undo
void redo(TextEditor *editor) {
    if (!isEmpty(&editor->redo_stack)) {
        // Simpan status saat ini ke undo stack
        push(&editor->undo_stack, editor->content);
        // Kembalikan status dari redo stack
        pop(&editor->redo_stack, editor->content);
    }
}

// Menampilkan konten editor
void showContent(TextEditor *editor) {
    printf("%s\n", editor->content);
}

// Fungsi utama
int main() {

    // Membersihkan Hasil Program Sebelumnnya
    system("cls");

    TextEditor editor;
    initEditor(&editor);

    typeText(&editor, "Halo");
    typeText(&editor, " Saya");
    typeText(&editor, " Murid");
    typeText(&editor, " Binus");
    typeText(&editor, " Online");
    showContent(&editor); 

    undo(&editor);
    showContent(&editor);

    redo(&editor);
    showContent(&editor);

    undo(&editor);
    undo(&editor);
    showContent(&editor);

    undo(&editor);
    showContent(&editor);

    undo(&editor);
    showContent(&editor);

    return 0;
}

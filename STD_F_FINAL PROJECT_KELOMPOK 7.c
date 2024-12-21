#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define MAX 1000
#define MAX_STR 1000

// Struktur untuk Tree (Guru dan Siswa)
struct TreeNode {
    char nama[MAX_STR];
    char role[MAX_STR];   // Guru atau Siswa
    int nomorInduk;       // Nomor induk
    struct TreeNode* left;   // Guru
    struct TreeNode* right;  // Siswa
};
typedef struct TreeNode TreeNode;

// Queue untuk urutan absen
char queue[MAX][MAX_STR];
int depan = -1;
int belakang = -1;
int hitung = 1;

// Fungsi untuk membuat node Tree
TreeNode* buatTreeNode(char* nama, char* role, int nomorInduk) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->nama, nama);
    strcpy(newNode->role, role);
    newNode->nomorInduk = nomorInduk;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Fungsi untuk menambahkan data ke Tree berdasarkan peran
void addToTree(TreeNode** root, char* nama, char* role, int nomorInduk) {
    TreeNode* newNode = buatTreeNode(nama, role, nomorInduk);

    if (*root == NULL) {
        *root = newNode;
        return;
    }

    // Jika nomor induk lebih kecil, masukkan ke kiri
    if (nomorInduk < (*root)->nomorInduk) {
        if ((*root)->left == NULL) {
            (*root)->left = newNode;
        } else {
            addToTree(&(*root)->left, nama, role, nomorInduk);
        }
    }
    // Jika nomor induk lebih besar, masukkan ke kanan
    else if (nomorInduk > (*root)->nomorInduk) {
        if ((*root)->right == NULL) {
            (*root)->right = newNode;
        } else {
            addToTree(&(*root)->right, nama, role, nomorInduk);
        }
    }
}

// Fungsi untuk menampilkan Tree
void displayTree(TreeNode* root, int level) {
    if (root == NULL) return;
    for (int i = 0; i < level; i++) printf("    ");
    printf("- %s (%s) [Nomor Induk: %d]\n", root->nama, root->role, root->nomorInduk);
    displayTree(root->left, level + 1);
    displayTree(root->right, level + 1);
}

// Queue Functions
void createQueue() {
    depan = belakang = -1;
    hitung = 1;
    printf("Pembuatan urutan absen berhasil\n");
}

// Menambahkan data ke queue
void enqueue(char* data) {
    if ((belakang + 1) % MAX == depan) {
        printf("Queue penuh\n");
    } else {
        if (depan == -1) depan = 0;
        belakang = (belakang + 1) % MAX;

        char temp[MAX_STR];
        snprintf(temp, sizeof(temp), "%d. %s", hitung++, data);
        strcpy(queue[belakang], temp);
    }
}

// Menampilkan isi Queue
void tampilkanQueue() {
    if (depan == -1) {
        printf("Queue kosong\n");
        return;
    }
    printf("Urutan absen: \n");
    for (int i = depan; i != belakang; i = (i + 1) % MAX) {
        printf("%s\n", queue[i]);
    }
    printf("%s\n", queue[belakang]);
}

// Fungsi untuk menambahkan data mahasiswa
void tambahData(TreeNode** root) {
    char nama[MAX_STR];
    char peran[MAX_STR];
    int nomorInduk;

    fflush(stdin);
    printf("Nama: ");
    scanf(" %[^\n]", nama);

    printf("Peran (Guru/Siswa): ");
    scanf(" %[^\n]", peran);

    printf("Nomor Induk: ");
    scanf("%d", &nomorInduk);

    // Masukkan nama dan peran ke dalam queue untuk absen
    enqueue(nama);

    // Masukkan data ke dalam tree berdasarkan peran dan nomor induk
    addToTree(root, nama, peran, nomorInduk);
}

int main() {
    TreeNode* root = NULL;  // Root untuk Tree (Guru/Siswa)
    int pilihan;

    createQueue();

    do {
        printf("\nMenu\n");
        printf("1. Tambah Data Siswa/Guru\n");
        printf("2. Tampilkan Urutan Absensi\n");
        printf("3. Tampilkan Struktur Kehadiran (Tree)\n");
        printf("4. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tambahData(&root);
                break;

            case 2:
                tampilkanQueue();
                break;

            case 3:
                printf("\nStruktur Kehadiran (Tree):\n");
                displayTree(root, 0);
                break;

            case 4:
                break;

            default:
                printf("Opsi tidak valid.\n");
        }
    } while (pilihan != 4);

    return 0;
}

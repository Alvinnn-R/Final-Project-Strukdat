#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define MAX_STR 100

// Struktur untuk Tree (Guru dan Siswa)
struct TreeNode {
    char nama[MAX_STR];
    char peran[MAX_STR];
    long long id;
    struct TreeNode* left;
    struct TreeNode* right;
};
typedef struct TreeNode TreeNode;

// Struktur untuk Linked List (Daftar Absen)
struct LinkedListNode {
    char nama[MAX_STR];
    char peran[MAX_STR];
    long long id;
    struct LinkedListNode* next;
};
typedef struct LinkedListNode LinkedListNode;

// Struktur untuk Queue (Antrian)
struct QueueMasuk {
    char nama[MAX_STR];
    char peran[MAX_STR];
    long long id;
};
typedef struct QueueMasuk QueueMasuk;

QueueMasuk queue[MAX];
int depan = -1;
int belakang = -1;

// Head untuk Linked List
LinkedListNode* head = NULL;

// Prototipe fungsi
void tampilkanSubTree(TreeNode* root, int level);

// Fungsi untuk membuat node Tree
TreeNode* buatTreeNode(char* nama, char* peran, long long id) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newNode->nama, nama);
    strcpy(newNode->peran, peran);
    newNode->id = id;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void tambahTree(TreeNode** root, char* nama, char* peran, long long id) {
    if (*root == NULL) {
        *root = buatTreeNode(nama, peran, id);
        return;
    }
    if (strcmp(peran, "Guru") == 0) {
        if ((*root)->left == NULL) {
            (*root)->left = buatTreeNode(nama, peran, id);
        } else {
            tambahTree(&(*root)->left, nama, peran, id);
        }
    } else if (strcmp(peran, "Siswa") == 0) {
        if ((*root)->right == NULL) {
            (*root)->right = buatTreeNode(nama, peran, id);
        } else {
            tambahTree(&(*root)->right, nama, peran, id);
        }
    }
}

// Fungsi untuk menampilkan Tree terpisah menjadi Guru dan Siswa
void tampilkanTree(TreeNode* root) {
    if (root == NULL) {
        printf("Tree kosong.\n");
        return;
    }

    printf("\nStruktur Kehadiran:\n");

    if (root->left != NULL || strcmp(root->peran, "Guru") == 0) {
        printf("Guru:\n");
        if (root->left != NULL) {
            tampilkanSubTree(root->left, 1);
        } else if (strcmp(root->peran, "Guru") == 0) {
            printf("- %s [ID: %lld]\n", root->nama, root->id);
        }
    }

    if (root->right != NULL || strcmp(root->peran, "Siswa") == 0) {
        printf("\nSiswa:\n");
        if (root->right != NULL) {
            tampilkanSubTree(root->right, 1);
        } else if (strcmp(root->peran, "Siswa") == 0) {
            printf("- %s [ID: %lld]\n", root->nama, root->id);
        }
    }
}

// Fungsi untuk menampilkan subtree (rekursif)
void tampilkanSubTree(TreeNode* root, int level) {
    if (root == NULL) return;

    for (int i = 0; i < level; i++) printf("    ");
    printf("- %s [ID: %lld]\n", root->nama, root->id);

    tampilkanSubTree(root->left, level + 1);
    tampilkanSubTree(root->right, level + 1);
}

// Fungsi untuk memperbarui data di Tree
void editTree(TreeNode* root, long long id, char* namaBaru, char* peranBaru) {
    if (root == NULL) return;

    if (root->id == id) {
        strcpy(root->nama, namaBaru);
        strcpy(root->peran, peranBaru);
    }

    editTree(root->left, id, namaBaru, peranBaru);
    editTree(root->right, id, namaBaru, peranBaru);
}

// Fungsi untuk menghapus data dari Tree
TreeNode* hapusTree(TreeNode* root, long long id) {
    if (root == NULL) return NULL;

    if (id < root->id) {
        root->left = hapusTree(root->left, id);
    } else if (id > root->id) {
        root->right = hapusTree(root->right, id);
    } else {
        // Node ditemukan
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Node memiliki dua anak
        TreeNode* temp = root->right;
        while (temp->left != NULL) temp = temp->left;
        root->id = temp->id;
        strcpy(root->nama, temp->nama);
        strcpy(root->peran, temp->peran);
        root->right = hapusTree(root->right, temp->id);
    }
    return root;
}

// Queue Functions
void enqueue(char* nama, char* peran, long long id) {
    if ((belakang + 1) % MAX == depan) {
        printf("Queue penuh\n");
        return;
    }
    if (depan == -1) depan = 0;
    belakang = (belakang + 1) % MAX;

    QueueMasuk newEntry;
    strcpy(newEntry.nama, nama);
    strcpy(newEntry.peran, peran);
    newEntry.id = id;

    queue[belakang] = newEntry;
    printf("Data berhasil dimasukkan ke dalam queue.\n");
}

void tampilkanQueue() {
    if (depan == -1) {
        printf("Queue kosong.\n");
        return;
    }
    printf("Urutan absensi:\n");
    for (int i = depan; i != belakang; i = (i + 1) % MAX) {
        printf("- %s (%s) [ID: %lld]\n", queue[i].nama, queue[i].peran, queue[i].id);
    }
    printf("- %s (%s) [ID: %lld]\n", queue[belakang].nama, queue[belakang].peran, queue[belakang].id);
}

// Fungsi untuk memperbarui data di Queue
void updateQueue(long long id, char* namaBaru, char* peranBaru) {
    for (int i = depan; i != belakang; i = (i + 1) % MAX) {
        if (queue[i].id == id) {
            strcpy(queue[i].nama, namaBaru);
            strcpy(queue[i].peran, peranBaru);
            return;
        }
    }
    if (queue[belakang].id == id) {
        strcpy(queue[belakang].nama, namaBaru);
        strcpy(queue[belakang].peran, peranBaru);
    }
}

// Fungsi untuk menghapus data dari Queue
void deleteFromQueue(long long id) {
    if (depan == -1) return;

    for (int i = depan; i != belakang; i = (i + 1) % MAX) {
        if (queue[i].id == id) {
            for (int j = i; j != belakang; j = (j + 1) % MAX) {
                queue[j] = queue[(j + 1) % MAX];
            }
            belakang = (belakang - 1 + MAX) % MAX;
            if (depan > belakang) depan = -1;
            return;
        }
    }
    if (queue[belakang].id == id) {
        belakang = (belakang - 1 + MAX) % MAX;
        if (depan > belakang) depan = -1;
    }
}

// Linked List Functions
void tambahLinkedList(char* nama, char* peran, long long id) {
    LinkedListNode* newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    strcpy(newNode->nama, nama);
    strcpy(newNode->peran, peran);
    newNode->id = id;
    newNode->next = head;
    head = newNode;
    printf("Data berhasil ditambahkan ke daftar absen.\n");
}

void tampilkanLinkedList() {
    LinkedListNode* temp = head;
    if (temp == NULL) {
        printf("Daftar absen kosong.\n");
        return;
    }
    printf("Daftar Absen:\n");
    while (temp != NULL) {
        printf("- %s (%s) [ID: %lld]\n", temp->nama, temp->peran, temp->id);
        temp = temp->next;
    }
}

void editLinkedList(TreeNode** root, long long id, char* namaBaru, char* peranBaru) {
    LinkedListNode* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            strcpy(temp->nama, namaBaru);
            strcpy(temp->peran, peranBaru);
            printf("Data berhasil diubah di Linked List.\n");

            // Perbarui di Tree dan Queue
            editTree(*root, id, namaBaru, peranBaru);
            updateQueue(id, namaBaru, peranBaru);
            return;
        }
        temp = temp->next;
    }
    printf("Data tidak ditemukan.\n");
}

void hapusLinkedList(TreeNode** root, long long id) {
    LinkedListNode* temp = head;
    LinkedListNode* prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Data tidak ditemukan.\n");
        return;
    }

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    printf("Data berhasil dihapus dari Linked List.\n");

    // Hapus dari Tree dan Queue
    *root = hapusTree(*root, id);
    deleteFromQueue(id);
}

// Submenu untuk pengelolaan data
void submenuPengelolaan(TreeNode** root) {
    int subPilihan;
    char nama[MAX_STR], peran[MAX_STR];
    long long id;

    system("cls");

    do {
        printf("\nSubmenu Data Siswa/Guru\n");
        printf("1. Tambah Data\n");
        printf("2. Tampilkan Data\n");
        printf("3. Edit Data\n");
        printf("4. Hapus Data\n");
        printf("5. Kembali ke Menu Utama\n");
        printf("Pilih opsi: ");
        scanf("%d", &subPilihan);

        switch (subPilihan) {
            case 1:
                printf("Nama: ");
                scanf(" %[^\n]", nama);
                printf("Peran (Guru/Siswa): ");
                scanf(" %[^\n]", peran);
                printf("Nomor Induk: ");
                scanf("%lld", &id);

                tambahLinkedList(nama, peran, id);
                enqueue(nama, peran, id);
                tambahTree(root, nama, peran, id);
                break;

            case 2:
                tampilkanLinkedList();
                break;

            case 3:
                printf("Masukkan ID untuk diedit: ");
                scanf("%lld", &id);
                printf("Nama baru: ");
                scanf(" %[^\n]", nama);
                printf("Peran baru: ");
                scanf(" %[^\n]", peran);
                editLinkedList(root, id, nama, peran);
                break;

            case 4:
                printf("Masukkan ID untuk dihapus: ");
                scanf("%lld", &id);
                hapusLinkedList(root, id);
                break;

            case 5:
                printf("Kembali ke menu utama.\n");
                system("cls");
                break;

            default:
                printf("Opsi tidak valid.\n");
        }
    } while (subPilihan != 5);
}

// Program utama
int main() {
    TreeNode* root = NULL;
    int pilihan;

    do {
        printf("\nMenu Utama\n");
        printf("1. Data Siswa/Guru\n");
        printf("2. Tampilkan Urutan Absensi (Queue)\n");
        printf("3. Tampilkan Struktur Kehadiran (Tree)\n");
        printf("4. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                submenuPengelolaan(&root);
                break;

            case 2:
                tampilkanQueue();
                break;

            case 3:
                tampilkanTree(root);
                break;

            case 4:
                printf("Keluar dari program.\n");
                break;

            default:
                printf("Opsi tidak valid.\n");
        }
    } while (pilihan != 4);

    return 0;
}
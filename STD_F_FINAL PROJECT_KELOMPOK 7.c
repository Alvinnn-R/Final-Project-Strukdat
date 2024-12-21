#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define CLEAR "clear"
#endif

// Struktur untuk Linked List (Data Siswa)
typedef struct Siswa {
    int id;
    char nama[50];
    struct Siswa *next;
} Siswa;

Siswa *headSiswa = NULL;

// Struktur untuk Linked List (Materi Absensi)
typedef struct Materi {
    char namaMateri[50];
    char batasWaktu[50];
    struct Materi *next;
} Materi;

Materi *headMateri = NULL;

// Struktur untuk Queue (Absensi Kehadiran)
#define MAX 100
typedef struct Queue {
    int front, rear;
    int id[MAX];
    char nama[MAX][50];
    char status[MAX][50]; // Status kehadiran (hadir/tidak hadir/sakit)
} Queue;

Queue queue = {-1, -1};

// Fungsi untuk membersihkan layar
void clearScreen() {
    system(CLEAR);
}

// Fungsi untuk memeriksa apakah ID sudah ada
int cekIdSiswa(int id) {
    Siswa *current = headSiswa;
    while (current != NULL) {
        if (current->id == id) {
            return 1; // ID ditemukan
        }
        current = current->next;
    }
    return 0; // ID tidak ditemukan
}

// Fungsi untuk menambahkan siswa secara manual
void tambahSiswaManual(int id, char *nama) {
    if (cekIdSiswa(id)) {
        printf("ID %d sudah ada, data tidak ditambahkan.\n", id);
        return;
    }

    Siswa *newSiswa = (Siswa *)malloc(sizeof(Siswa));
    newSiswa->id = id;
    strcpy(newSiswa->nama, nama);

    if (headSiswa == NULL || headSiswa->id > id) {
        newSiswa->next = headSiswa;
        headSiswa = newSiswa;
    } else {
        Siswa *current = headSiswa;
        while (current->next != NULL && current->next->id < id) {
            current = current->next;
        }
        newSiswa->next = current->next;
        current->next = newSiswa;
    }
}

// Fungsi untuk inisialisasi data siswa
void inisialisasiDataSiswa() {
    tambahSiswaManual(10, "Rina Aulia");
    tambahSiswaManual(9, "Dedi Sugianto");
    tambahSiswaManual(8, "Siti Nurhaliza");
    tambahSiswaManual(7, "Andi Wijaya");
    tambahSiswaManual(6, "Lina Permata");
    tambahSiswaManual(5, "Budi Prasetyo");
    tambahSiswaManual(4, "Ayu Kartika");
    tambahSiswaManual(3, "Eko Susilo");
    tambahSiswaManual(2, "Rini Lestari");
    tambahSiswaManual(1, "Fajar Rahmat");
}

// Fungsi Linked List untuk Data Siswa
void tambahSiswa() {
    Siswa *newSiswa = (Siswa *)malloc(sizeof(Siswa));
    printf("Masukkan ID Siswa: ");
    scanf("%d", &newSiswa->id);
    getchar();

    if (cekIdSiswa(newSiswa->id)) {
        printf("ID %d sudah ada, data tidak ditambahkan.\n", newSiswa->id);
        free(newSiswa);
        return;
    }

    printf("Masukkan Nama Siswa: ");
    fgets(newSiswa->nama, sizeof(newSiswa->nama), stdin);
    newSiswa->nama[strcspn(newSiswa->nama, "\n")] = '\0'; // Menghapus newline

    if (headSiswa == NULL || headSiswa->id > newSiswa->id) {
        newSiswa->next = headSiswa;
        headSiswa = newSiswa;
    } else {
        Siswa *current = headSiswa;
        while (current->next != NULL && current->next->id < newSiswa->id) {
            current = current->next;
        }
        newSiswa->next = current->next;
        current->next = newSiswa;
    }

    printf("Data siswa berhasil ditambahkan!\n");
}

// Fungsi untuk menampilkan data siswa yang terurut berdasarkan ID
void tampilkanSiswa() {
    if (headSiswa == NULL) {
        printf("Data siswa kosong!\n");
        return;
    }

    printf("ID\tNama\n");
    Siswa *current = headSiswa;
    while (current != NULL) {
        printf("%d\t%s\n", current->id, current->nama);
        current = current->next;
    }
}

void editSiswa() {
    int id;
    printf("Masukkan ID Siswa yang ingin diubah: ");
    scanf("%d", &id);
    getchar();

    Siswa *current = headSiswa;
    while (current != NULL) {
        if (current->id == id) {
            printf("Masukkan Nama Baru: ");
            fgets(current->nama, sizeof(current->nama), stdin);
            current->nama[strcspn(current->nama, "\n")] = '\0';
            printf("Data siswa berhasil diubah!\n");
            return;
        }
        current = current->next;
    }
    printf("Siswa dengan ID %d tidak ditemukan!\n", id);
}

void hapusSiswa() {
    int id;
    printf("Masukkan ID Siswa yang ingin dihapus: ");
    scanf("%d", &id);

    Siswa *current = headSiswa, *prev = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                headSiswa = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Data siswa berhasil dihapus!\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Siswa dengan ID %d tidak ditemukan!\n", id);
}

// Struktur untuk Tree Node (Absensi Berdasarkan Materi)
typedef struct TreeNode {
    char namaMateri[50];
    struct TreeNode *left, *right;
    Queue absensi; // Absensi kehadiran untuk materi ini
} TreeNode;

TreeNode *root = NULL;

// Fungsi untuk membuat node baru pada tree
TreeNode *buatNodeTree(char *namaMateri) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    strcpy(newNode->namaMateri, namaMateri);
    newNode->left = newNode->right = NULL;
    newNode->absensi.front = -1;
    newNode->absensi.rear = -1;
    return newNode;
}

// Fungsi untuk menambahkan data ke tree
TreeNode *tambahKeTree(TreeNode *root, char *namaMateri, Queue *absensi) {
    if (root == NULL) {
        TreeNode *newNode = buatNodeTree(namaMateri);
        newNode->absensi = *absensi; // Salin absensi ke node
        return newNode;
    }

    if (strcmp(namaMateri, root->namaMateri) < 0) {
        root->left = tambahKeTree(root->left, namaMateri, absensi);
    } else if (strcmp(namaMateri, root->namaMateri) > 0) {
        root->right = tambahKeTree(root->right, namaMateri, absensi);
    }

    return root;
}

// Fungsi untuk menampilkan absensi dari tree secara in-order traversal
void tampilkanTree(TreeNode *root) {
    int i;
    if (root == NULL) return;

    tampilkanTree(root->left);

    printf("\nMateri: %s\n", root->namaMateri);
    printf("ID\tNama\t\t\t\tKehadiran\n");
    printf("===============================================\n");
    for (i = root->absensi.front; i <= root->absensi.rear; i++) {
        printf("%d\t%-20s\t%s\n", root->absensi.id[i], root->absensi.nama[i], root->absensi.status[i]);
    }

    tampilkanTree(root->right);
}

// Fungsi untuk proses absensi dan menambahkan ke tree
void tambahMateriDanKelolaAbsensiTree() {
    Materi *newMateri = (Materi *)malloc(sizeof(Materi));
    int pilihan;

    printf("\nPilih Materi Struktur Data:\n");
    printf("1. Struct\n");
    printf("2. Pointer\n");
    printf("3. Linked List\n");
    printf("4. Stack\n");
    printf("5. Queue\n");
    printf("6. Tree\n");
    printf("Masukkan pilihan (1-6): ");
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1: strcpy(newMateri->namaMateri, "Struct"); break;
        case 2: strcpy(newMateri->namaMateri, "Pointer"); break;
        case 3: strcpy(newMateri->namaMateri, "Linked List"); break;
        case 4: strcpy(newMateri->namaMateri, "Stack"); break;
        case 5: strcpy(newMateri->namaMateri, "Queue"); break;
        case 6: strcpy(newMateri->namaMateri, "Tree"); break;
        default: printf("Pilihan tidak valid!\n"); free(newMateri); return;
    }

    printf("Materi: %s\n", newMateri->namaMateri);

    // Masukkan ke linked list materi
    newMateri->next = headMateri;
    headMateri = newMateri;

    // Proses absensi siswa
    if (headSiswa == NULL) {
        printf("Tidak ada data siswa untuk melakukan absensi!\n");
        return;
    }

    // Reset queue jika absensi baru dimulai
    queue.front = -1;
    queue.rear = -1;

    Siswa *current = headSiswa;
    while (current != NULL) {
        if (queue.rear == MAX - 1) {
            printf("Antrian penuh, tidak dapat menambahkan lebih banyak data.\n");
            return;
        }

        if (queue.front == -1) {
            queue.front = 0;
        }

        queue.rear++;
        queue.id[queue.rear] = current->id;
        strcpy(queue.nama[queue.rear], current->nama);

        printf("ID: %d, Nama: %s\n", current->id, current->nama);
        printf("Masukkan status kehadiran (hadir/tidak hadir/sakit): ");
        scanf(" %49s", queue.status[queue.rear]);

        current = current->next;
    }

    printf("Absensi selesai!\n");

    // Tambahkan ke tree berdasarkan materi
    root = tambahKeTree(root, newMateri->namaMateri, &queue);
}

// Fungsi Queue untuk Absensi Kehadiran
void tampilkanKehadiran() {
    int i;
    if (queue.front == -1) {
        printf("Antrian kehadiran kosong!\n");
        return;
    }

    printf("\nData Absensi Berdasarkan Urutan:\n");
    printf("ID\tNama\t\t\t\tKehadiran\n");
    printf("===============================================\n");
    for (i = queue.front; i <= queue.rear; i++) {
        printf("%d\t%-20s\t%s\n", queue.id[i], queue.nama[i], queue.status[i]);
    }
}

// Menu baru untuk menampilkan absensi berdasarkan materi
void tampilkanKehadiranBerdasarkanMateri() {
    if (root == NULL) {
        printf("Tidak ada data absensi berdasarkan materi!\n");
        return;
    }

    printf("\n=== Data Kehadiran Berdasarkan Materi ===\n");
    tampilkanTree(root);
}

int main() {
    // Inisialisasi data siswa
    inisialisasiDataSiswa();

    // Menu Program
    int pilihan;
    do {
        clearScreen();
        printf("=== Program Absensi Sekolah ===\n");
        printf("1. Kelola Data Siswa (Linked List)\n");
        printf("2. Kelola Absensi Sekolah (Linked List)\n");
        printf("3. Menampilkan Absensi Kehadiran (Queue)\n");
        printf("4. Menampilkan Kehadiran Berdasarkan Materi (Tree)\n");
        printf("5. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: {
                int subMenu;
                do {
                    clearScreen();
                    printf("=== Kelola Data Siswa ===\n");
                    printf("1. Tambah Data Siswa\n");
                    printf("2. Tampilkan Data\n");
                    printf("3. Edit Data\n");
                    printf("4. Hapus Data\n");
                    printf("5. Kembali ke Menu Utama\n");
                    printf("Pilih: ");
                    scanf("%d", &subMenu);
                    getchar();

                    switch (subMenu) {
                        case 1: tambahSiswa(); break;
                        case 2: tampilkanSiswa(); break;
                        case 3: editSiswa(); break;
                        case 4: hapusSiswa(); break;
                        case 5: break;
                        default: printf("Pilihan tidak valid!\n");
                    }
                    printf("\nPress Enter to continue...");
                    getchar();
                } while (subMenu != 5);

                break;
            }
            case 2:
                tambahMateriDanKelolaAbsensiTree();
                break;
            case 3:
                tampilkanKehadiran();
                getchar();
                break;
            case 4:
                tampilkanKehadiranBerdasarkanMateri();
                getchar();
                break;
            case 5:
                printf("Terima kasih telah menggunakan program absensi sekolah.\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
    } while (pilihan != 5);

    return 0;
}

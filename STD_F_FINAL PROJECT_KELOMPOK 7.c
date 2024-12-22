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
    tambahSiswaManual(10, "Siti Nurhaliza");
    tambahSiswaManual(9, "Rini Lestari");
    tambahSiswaManual(8, "Rina Aulia");
    tambahSiswaManual(7, "Lina Permata");
    tambahSiswaManual(6, "Fajar Rahmat");
    tambahSiswaManual(5, "Eko Susilo");
    tambahSiswaManual(4, "Dedi Sugianto");
    tambahSiswaManual(3, "Budi Prasetyo");
    tambahSiswaManual(2, "Ayu Kartika");
    tambahSiswaManual(1, "Andi Wijaya");
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
    getchar();

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
    printf("ID\tNama\t\t    Kehadiran\n");
    printf("======================================\n");
    for (i = root->absensi.front; i <= root->absensi.rear; i++) {
        printf("%d\t%-20s\t%s\n", root->absensi.id[i], root->absensi.nama[i], root->absensi.status[i]);
    }

    tampilkanTree(root->right);
}

// Fungsi untuk proses absensi dan menambahkan ke tree
void tambahMateriDanKelolaAbsensiTree() {
    Materi *newMateri = (Materi *)malloc(sizeof(Materi));
    int pilihan;

    printf("\nPilih Mata Pelajaran:\n");
    printf("1. Sistem Operasi\n");
    printf("2. Struktur Data\n");
    printf("3. Metode Numerik\n");
    printf("4. Basis Data Lanjut\n");
    printf("5. Statistik Komputasi\n");
    printf("6. Pemrograman Lanjut\n");
    printf("Masukkan pilihan (1-6): ");
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1: strcpy(newMateri->namaMateri, "Sistem Operasi"); break;
        case 2: strcpy(newMateri->namaMateri, "Struktur Data"); break;
        case 3: strcpy(newMateri->namaMateri, "Metode Numerik"); break;
        case 4: strcpy(newMateri->namaMateri, "Basis Data Lanjut"); break;
        case 5: strcpy(newMateri->namaMateri, "Statistik Komputasi"); break;
        case 6: strcpy(newMateri->namaMateri, "Pemrograman Lanjut"); break;
        default: printf("Pilihan tidak valid!\n"); free(newMateri); return;
    }

    printf("Pelajaran: %s\n", newMateri->namaMateri);

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
        printf("Masukkan status kehadiran [Hadir (H) / Tidak hadir (A) / Sakit (S)]: ");
        scanf(" %49s", queue.status[queue.rear]);
        while (strcmp(queue.status[queue.rear], "H") != 0 && strcmp(queue.status[queue.rear], "A") != 0 && strcmp(queue.status[queue.rear], "S") != 0) {
        printf("Input tidak valid! Masukkan hanya [H, A, S]: ");
        scanf(" %49s", queue.status[queue.rear]);
    }


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
    printf("ID\tNama\t\t    Kehadiran\n");
    printf("======================================\n");
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
        printf("1. Kelola Data Siswa\n");
        printf("2. Kelola Absensi Sekolah\n");
        printf("3. Menampilkan Absensi Kehadiran\n");
        printf("4. Menampilkan Kehadiran Berdasarkan Materi\n");
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
    } while (pilihan != 5);

    return 0;
}

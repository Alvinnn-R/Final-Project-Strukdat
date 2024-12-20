/**Sistem Absensi Sekolah
"1. Linked List: menyimpan daftar siswa di setiap kelas (nama, npm, status kehadiran)
2. Queue: memastikan siswa diabsen berdasarkan urutan waktu mereka hadir (FIFO)
3. Stack: menyimpan histori absensi harian."**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define MAX 1000
#define MAX_STR 1000

struct node {
    char histori [1000];
    int data;
    struct node* next;
};
typedef struct node node;

struct node* buatNodeHistori (char* histori) {
    struct node* nodeBaru = (struct node*)malloc(sizeof(struct node));
    strcpy(nodeBaru->histori, histori);
    nodeBaru->next = NULL;
    return nodeBaru;
}

struct node* buatNodeData (int data) {
    struct node* nodeBaru = (struct node*)malloc(sizeof(struct node));
    nodeBaru->data = data;
    nodeBaru->next = NULL;
    return nodeBaru;
}

char queue [MAX][MAX_STR];
int depan = -1;
int belakang = -1;
int hitung = 1;

void createQueue () {
    depan = belakang = -1;
    hitung = 1;
    printf("Pembuatan urutan absen berhasil\n");
}

void tambahData (node **head) {
    int bil;
    node *pNew;
    node *pCur;

    fflush(stdin);
    printf("Kode Mahasiswa : ");
    fflush(stdin);
    scanf("%d", &bil);
    pNew = buatNodeData(bil);

    if (pNew != NULL) {
        pNew->next = *head;
        if (*head == NULL) {
            pNew->next = pNew;
        } else {
            pCur = *head;
            while (pCur->next != *head) {
                pCur = pCur->next;
            }
            pCur->next = pNew;
        }
        *head = pNew;
    } else {
        printf("Kode tidak terdeteksi");
        getch();
    }
}

void enqueue (char *data) {
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

void push (struct node** top, char* histori) {
    struct node* nodeBaru = buatNodeHistori(histori);
    nodeBaru->next = *top;
    *top = nodeBaru;
    printf("Histori '%s' telah ditambahkan ke stack.\n", histori);
}

void pop (struct node** top) {
    if (*top == NULL) {
        printf("Stack kosong, tidak ada histori untuk dihapus.\n");
        return;
    }
    struct node* temp = *top;
    *top = (*top)->next;
    printf("Histori '%s' telah dihapus dari stack.\n", temp->histori);
    free(temp);
}

void tampilkanHistori (struct node* top) {
    if (top == NULL) {
        printf("Stack kosong, tidak ada histori. \n");
        return;
    }
    struct node* temp = top;
    printf("Histori dalam stack: \n");
    while (temp != NULL) {
        printf("%s\n", temp->histori);
        temp = temp->next;
    }
}

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

int main() {
    node* head = NULL;
    node* top = NULL;
    int pilihan;
    char histori [1000];
    char dataQueue[MAX_STR];
    createQueue();

    do {
        printf("\nMenu\n");
        printf("1. Tambah Data Siswa\n");
        printf("2. Tampilkan Urutan Absensi\n");
        printf("3. Tambah Histori\n");
        printf("4. Tampilkan Histori\n");
        printf("5. Hapus Histori\n");
        printf("6. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
            tambahData(&head);
            printf("Nama Siswa untuk Queue: ");
            scanf(" %[^\n]", dataQueue);
            break;

            case 2:
            tampilkanQueue();
            break;

            case 3:
            printf("Masukkan histori: ");
            scanf(" %[^\n]", histori);
            push(&top, histori);
            break;

            case 4:
            tampilkanHistori(top);
            break;

            case 5:
            pop(&top);
            break;

            case 6:
            break;

            default:
            printf("Opsi tidak valid.\n");
        }
    } while (pilihan != 6);
    return 0;
}
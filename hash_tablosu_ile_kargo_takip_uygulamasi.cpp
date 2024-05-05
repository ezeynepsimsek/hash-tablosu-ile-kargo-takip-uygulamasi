#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLO_BOYUTU 10

typedef struct {
    int takipNo;
    char gonderici[50];
    char alici[50];
    char kargoDurum[50];
} Kargo;

Kargo* hash_tablo[TABLO_BOYUTU];

void initHashTablo() {
    int i = 0;
    while (i < TABLO_BOYUTU) {
        hash_tablo[i] = NULL;
        i++;
    }
}

int hash1(int takipNo) {
    return takipNo % TABLO_BOYUTU;
}

int hash2(int takipNo) {
    return 7 - (takipNo % 7);
}

int doubleHashing(int hash1Result, int deneme, int hash2Result) {
    return (hash1Result + deneme * hash2Result) % TABLO_BOYUTU;
}

void ekle(Kargo* yeniKargo) {
    int takipNo = yeniKargo->takipNo;
    int hashIndex1 = hash1(takipNo);
    int hashIndex2 = hash2(takipNo);

    int indeks = hashIndex1;
    int deneme = 0;

    while (deneme < TABLO_BOYUTU) {
        if (hash_tablo[indeks] == NULL) {
            hash_tablo[indeks] = (Kargo*)malloc(sizeof(Kargo));
            *hash_tablo[indeks] = *yeniKargo;
            printf("EKLE -> %d: Hash tablosunun %d. indeksine eklendi.\n", takipNo, indeks);
            return;
        }

        indeks = doubleHashing(hashIndex1, deneme + 1, hashIndex2);
        deneme++;
    }

    printf("EKLE -> %d: Hash tablosu dolu, ekleme yapilamadi.\n", takipNo);
}

void kargolariDosyadanOku(char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    Kargo kargo;

    while (fscanf(dosya, "%d %s %s %s", &kargo.takipNo, kargo.gonderici, kargo.alici, kargo.kargoDurum) != EOF) {
        ekle(&kargo);
    }

    fclose(dosya);
}

void kargoGirisi() {
    Kargo yeniKargo;

    printf("Takip Numarasi: ");
    scanf("%d", &yeniKargo.takipNo);

    printf("Gonderici: ");
    scanf("%s", yeniKargo.gonderici);

    printf("Alici: ");
    scanf("%s", yeniKargo.alici);

    printf("Kargo Durumu: ");
    scanf("%s", yeniKargo.kargoDurum);

    ekle(&yeniKargo);
}

void kargoTakip(int takipNo) {
    int hashIndex1 = hash1(takipNo);
    int hashIndex2 = hash2(takipNo);

    int indeks = hashIndex1;
    int deneme = 0;

    while (deneme < TABLO_BOYUTU) {
        if (hash_tablo[indeks] == NULL) {
            printf("Kargo bulunamadi.\n");
            return;
        }

        if (hash_tablo[indeks]->takipNo == takipNo) {
            printf("Kargo %d adimda bulundu.\n", deneme + 1);
            printf("Takip No: %d\nGonderici: %s\nAlici: %s\nKargo Durumu: %s\n",
                   hash_tablo[indeks]->takipNo, hash_tablo[indeks]->gonderici,
                   hash_tablo[indeks]->alici, hash_tablo[indeks]->kargoDurum);
            return;
        }

        indeks = doubleHashing(hashIndex1, deneme + 1, hashIndex2);
        deneme++;
    }

    printf("Kargo bulunamadi.\n");
}

void kargoTeslimEt(int takipNo) {
    int hashIndex1 = hash1(takipNo);
    int hashIndex2 = hash2(takipNo);

    int indeks = hashIndex1;
    int deneme = 0;

    while (deneme < TABLO_BOYUTU) {
        if (hash_tablo[indeks] == NULL) {
            printf("Kargo bulunamadi.\n");
            return;
        }

        if (hash_tablo[indeks]->takipNo == takipNo) {
            printf("Kargo teslim edildi. Takip No: %d\n", takipNo);
            free(hash_tablo[indeks]); // Bellekten kargo bilgilerini sil
            hash_tablo[indeks] = NULL; // Tablodan referansi kaldir
            return;
        }

        indeks = doubleHashing(hashIndex1, deneme + 1, hashIndex2);
        deneme++;
    }

    printf("Kargo bulunamadi.\n");
}

void kargolariListele() {
    int i = 0;
    while (i < TABLO_BOYUTU) {
        printf("Indeks %d: ", i);

        if (hash_tablo[i] == NULL) {
            printf("Bos\n");
        } else {
            printf("Takip No: %d, Gonderici: %s, Alici: %s, Kargo Durumu: %s\n",
                   hash_tablo[i]->takipNo, hash_tablo[i]->gonderici,
                   hash_tablo[i]->alici, hash_tablo[i]->kargoDurum);
        }
        i++;
    }
}

int main() {
    initHashTablo();
    kargolariDosyadanOku("veri.txt");

    int secim, takipNo;

    do {
        printf("\n1. Kargo Girisi\n2. Kargo Takip\n3. Kargo Teslim Et\n4. Kargolari Listele\n5. Cikis\n");
        printf("Secim yapin: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                kargoGirisi();
                break;
            case 2:
                printf("Takip Numarasi: ");
                scanf("%d", &takipNo);
                kargoTakip(takipNo);
                break;
            case 3:
                printf("Takip Numarasi: ");
                scanf("%d", &takipNo);
                kargoTeslimEt(takipNo);
                break;
            case 4:
                kargolariListele();
                break;
            case 5:
                printf("Programdan cikiliyor.\n");
                break;
            default:
                printf("Geçersiz secim, tekrar deneyin.\n");
                break;
        }
    } while (secim != 5);

    int j = 0;
    while (j < TABLO_BOYUTU) {
        if (hash_tablo[j] != NULL) {
            free(hash_tablo[j]);
        }
        j++;
    }

    return 0;
}

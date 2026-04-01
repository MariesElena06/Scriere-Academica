#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generareRandom(int array[], int n);
void generareSortat(int array[], int n);
void generareReverse(int array[], int n);
void generareJumatateSortat(int array[], int n);
void generareAproapeSortat(int array[], int n);

void BubbleSort(int array[], int n);
void SelectionSort(int array[], int n);
void InsertionSort(int array[], int n);
void MergeSort(int array[], int st, int dr);
void Interclasare(int array[], int st, int mij, int dr);
void QuickSort(int array[], int st, int dr);
int elem_maxim(int array[], int n);
void CountSort(int array[], int n);
void counting(int array[], int n, int c);
void RadixSort(int array[], int n);
void heapify(int array[], int n, int i);
void HeapSort(int array[], int n);

// functia pentru determinarea timpului de executie:

#ifdef _WIN32
#include <windows.h>
double masurareTimp(void (*algoritmSortare)(int [], int), int array[], int n){
    LARGE_INTEGER freq, start, end;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    algoritmSortare(array, n);

    QueryPerformanceCounter(&end);

    return (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
}

#else
#define _POSIX_C_SOURCE 199309L
#include <time.h>

double masurareTimp(void (*algoritmSortare)(int [], int), int array[], int n){
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    algoritmSortare(array, n);

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}
#endif

// algoritmii de generare a sirurilor de elemente:

void generareRandom(int array[], int n){
    for (int i = 0; i < n; i++)
        array[i] = rand() % 1000000;
}

void generareSortat(int array[], int n){
    for (int i = 0; i < n; i++)
        array[i] = i;
}

void generareReverse(int array[], int n){
    for (int i = 0; i < n; i++)
        array[i] = n - i;
}

void generareJumatateSortat(int array[], int n){
    for (int i = 0; i < n / 2; i++)
        array[i] = i;
    
    for (int i = n / 2; i < n; i++)
        array[i] = rand() % 1000000;
}

void generareAproapeSortat(int array[], int n){
    generareSortat(array, n);

    for (int i = 0; i < n / 10; i++){
        int x = rand() % n;
        int y = rand() % n;

        int aux = array[x];
        array[x] = array[y];
        array[y] = aux;
    }
}

int elem_maxim(int array[], int n){
    int maxim = array[0];

    for (int i = 1; i < n; i++)
        if (array[i] > maxim)
            maxim = array[i];

    return maxim;
}

// algoritmii de sortare:

//Bubble sort
void BubbleSort(int array[], int n){
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (array[j] > array[j + 1]){
                int aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
}

//Selection sort
void SelectionSort(int array[], int n){
    for (int i = 0; i < n - 1; i++){
        int k = i;

        for (int j = i + 1; j < n; j++)
            if (array[k] > array[j])
                k = j;

        if (k != i){
            int aux = array[k];
            array[k] = array[i];
            array[i] = aux;
        }
    }
}

//Insertion sort
void InsertionSort(int array[], int n){
    for (int i = 1; i < n; i++){
        int aux = array[i], j = i - 1;

        while ((j >= 0) && (aux < array[j])){
            array[j + 1] = array[j];
            j--;
        }
        
        array[j + 1] = aux;
    }
}

//Merge Sort
void MergeSort(int array[], int st, int dr){
    if (st < dr){
        int mij = (st + dr) / 2;

        MergeSort(array, st, mij);
        MergeSort(array, mij + 1, dr);

        Interclasare(array, st, mij, dr);
    }
}
void Interclasare(int array[], int st, int mij, int dr){
    int i = st, j = mij + 1, k = 0;
    int *I = (int*) malloc ((dr - st + 1) * sizeof(int));

    while ((i <= mij) && (j <= dr)){
        if (array[i] <= array[j])
            I[k++] = array[i++];
        else
            I[k++] = array[j++];
    }
    while (i <= mij)
        I[k++] = array[i++];
    
    while (j <= dr)
        I[k++] = array[j++];  

    for (int i = 0; i < k; i++)
        array[st + i] = I[i];
}

void MergeSortWrap(int array[], int n){
    MergeSort(array, 0, n-1);
}

//Quick Sort
void QuickSort(int array[], int st, int dr){
    int i = st, j = dr, pivot = array[(st + dr) / 2];

    while (i <= j){
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;

        if (i <= j){
            int aux = array[i];
            array[i] = array[j];
            array[j] = aux;

            i++; j--;
        }
    }

    if (st < j) QuickSort(array, st, j);

    if (i < dr) QuickSort(array, i, dr);
}

void QuickSortWrap(int array[], int n){
    QuickSort(array, 0, n-1);
}

//Count Sort
void CountSort(int array[], int n){
    int maxim = elem_maxim(array, n);
    int *freq = (int*) malloc ((maxim + 1) * sizeof(int));
    int *aux = (int*) malloc (n * sizeof(int));

    for (int i = 0; i <= maxim; i++)
        freq[i] = 0;

    for (int i = 0; i < n; i++)
        freq[array[i]]++;

    for (int i = 1; i <= maxim; i++) 
        freq[i] += freq[i - 1];

    for (int i = n - 1; i >= 0; i--){
        aux[freq[array[i]] - 1] = array[i];
        freq[array[i]]--;;
    }
    
    for (int i = 0; i < n; i++)
        array[i] = aux[i];

    free(aux);
    free(freq);
}

//RadixSort
void counting(int array[], int n, int exp){
    int freq[10] = {0};
    int *aux = (int*) malloc (n * sizeof(int));

    for (int i = 0; i < n; i++)
        freq[(array[i] / exp) % 10]++;

    for (int i = 1; i <= 9; i++)
        freq[i] += freq[i - 1];

    for (int i = n - 1; i >= 0; i--){
        int cif = (array[i] / exp) % 10;
        aux[freq[cif] - 1] = array[i];
        freq[cif]--;
    }

    for (int i = 0; i < n; i++)
        array[i] = aux[i];

    free(aux);
}
void RadixSort(int array[], int n){
    int maxim = elem_maxim(array, n);
    for (int exp = 1; maxim / exp > 0; exp *= 10)
        counting(array, n, exp);
}

//HeapSort
void heapify(int array[], int n, int i){
    int maxim = i, st = 2 * i + 1, dr = 2 * i + 2;
    
    if (st < n && array[st] > array[maxim])
        maxim = st;

    if (dr < n && array[dr] > array[maxim])
        maxim = dr;

    if (maxim != i){
        int aux = array[i];
        array[i] = array[maxim];
        array[maxim] = aux;

        heapify(array, n, maxim);
    }
}
void HeapSort(int array[], int n){
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(array, n, i);
    
    for (int i = n - 1; i >= 0; i--){
        int aux = array[0];
        array[0] = array[i];
        array[i] = aux;

        heapify(array, i, 0);
    }
}

int main(){
    int dimensiuni[] = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};
    int nr_dim = sizeof(dimensiuni) / sizeof(dimensiuni[0]);

    FILE *out = fopen("Rezultate_Sortari.csv", "w");
    fprintf(out, "Dimensiune, Algoritm, Random, Sortat, Reverse, Jumatate Sortat, Aproape Sortat\n");

    struct {
        const char *nume;
        void (*functie)(int [], int);
    } algo[] = {
        {"Bubble Sort", BubbleSort},
        {"Selection Sort", SelectionSort},
        {"Insertion Sort", InsertionSort},
        {"Merge Sort", MergeSortWrap},
        {"Quick Sort", QuickSortWrap},
        {"Count Sort", CountSort},
        {"Radix Sort", RadixSort},
        {"Heap Sort", HeapSort}
    };

    int nr_algos = sizeof(algo) / sizeof(algo[0]);

    for (int s = 0; s < nr_dim; s++){
        int n = dimensiuni[s];

        int *base = (int*) malloc (n * sizeof(int));
        int *arr = (int*) malloc (n * sizeof(int));

        for (int a = 0; a < nr_algos; a++){
            double tip[5];

            if (dimensiuni[s] >= 200000 && 
                (strcmp(algo[a].nume, "Bubble Sort") == 0 ||
                strcmp(algo[a].nume, "Selection Sort") == 0 ||
                strcmp(algo[a].nume, "Insertion Sort") == 0)){
                printf("<Dimensiune: %d> - Trecem peste %s deoarece dimensiunea este prea mare.\n\n", dimensiuni[s], algo[a].nume);
                continue;
            }

            void (*gen[5])(int [], int) = {
                generareRandom,
                generareSortat,
                generareReverse,
                generareJumatateSortat,
                generareAproapeSortat
            };

            const char *tip_gen[5] = {"Random", "Sortat", "Reverse", "Jumatate Sortat", "Aproape Sortat"};
        
            for (int g = 0; g < 5; g++){
                printf("<Dimensiune: %d> - Testam %s pe un vector %s\n", n, algo[a].nume, tip_gen[g]);

                gen[g](base, n);
                memcpy(arr, base, n * sizeof(int));

                tip[g] = masurareTimp(algo[a].functie, arr, n);

                printf("<Dimensiune: %d> - %s testat pe vector %s completat in %.6f s\n\n", n, algo[a].nume, tip_gen[g], tip[g]);
            }

            fprintf(out, "%d,%s,%.6f,%.6f,%.6f,%.6f,%.6f\n", n, algo[a].nume, tip[0], tip[1], tip[2], tip[3], tip[4]);
        }

        free(base);
        free(arr);
    }
    fclose(out);
    return 0;
}
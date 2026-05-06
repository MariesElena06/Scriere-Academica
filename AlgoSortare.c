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
void CountingSort(int array[], int n);
void counting(int array[], int n, int c, int *aux);
void RadixSort(int array[], int n);
void heapify(int array[], int n, int i);
void HeapSort(int array[], int n);

// functia pentru determinarea timpului de executie (cu timeout):
#include <windows.h>
#include <time.h>

typedef struct {
    void (*alg)(int [], int);
    int *array;
    int n;
    double time;
} Task;

DWORD WINAPI run(LPVOID param){
    Task *t = (Task*)param;

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    t->alg(t->array, t->n);

    QueryPerformanceCounter(&end);

    t->time = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    return 0;
}

double masurareTimp(void (*alg)(int [], int), int array[], int n, double limita, int *timeout){
    *timeout = 0;

    Task *task = malloc(sizeof(Task));
    task->alg = alg;
    task->array = array;
    task->n = n;
    task->time = 0;

    HANDLE hThread = CreateThread(NULL, 0, run, task, 0, NULL);

    DWORD wait = WaitForSingleObject(hThread, (DWORD)(limita * 1000));

    if (wait == WAIT_TIMEOUT){
        *timeout = 1;
        TerminateThread(hThread, 0);
        CloseHandle(hThread);
        free(task);
        return limita;
    }

    CloseHandle(hThread);

    double t = task->time;
    free(task);
    return t;
}

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
int elem_minim(int array[], int n){
    int minim = array[0];

    for (int i = 1; i < n; i++)
        if (array[i] < minim)
            minim = array[i];

    return minim;
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

    for (int a = 0; a < k; a++)
        array[st + a] = I[a];

    free(I);
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
void CountingSort(int array[], int n){
    int maxim = elem_maxim(array, n), minim = elem_minim(array, n);
    int range = maxim - minim + 1;

    int *freq = calloc (range, sizeof(int));
    int *aux = malloc (n * sizeof(int));

    for (int i = 0; i < n; i++)
        freq[array[i] - minim]++;

    for (int i = 1; i < range; i++) 
        freq[i] += freq[i - 1];

    for (int i = n - 1; i >= 0; i--){
        aux[freq[array[i] - minim] - 1] = array[i];
        freq[array[i] - minim]--;
    }
    
    memcpy(array, aux, n * sizeof(int));
    
    free(freq);
    free(aux);
}

//RadixSort
void counting(int array[], int n, int exp, int *aux){
    int freq[10] = {0};

    for (int i = 0; i < n; i++)
        freq[(array[i] / exp) % 10]++;

    for (int i = 1; i <= 9; i++)
        freq[i] += freq[i - 1];

    for (int i = n - 1; i >= 0; i--){
        int cif = (array[i] / exp) % 10;
        aux[freq[cif] - 1] = array[i];
        freq[cif]--;
    }

    memcpy(array, aux, n * sizeof(int));
}
void RadixSort(int array[], int n){
    int *neg = malloc(n * sizeof(int));
    int *pos = malloc(n * sizeof(int));
    int *aux = malloc(n * sizeof(int));

    int nneg = 0, npos = 0;

    for (int i = 0; i < n; i++){
        if (array[i] < 0)
            neg[nneg++] = -array[i];
        else
            pos[npos++] = array[i];
    }

    if (npos > 0){
        int maxp = elem_maxim(pos, npos);
        for (int exp = 1; maxp / exp > 0; exp *= 10)
            counting(pos, npos, exp, aux);
    }

    if (nneg > 0){
        int maxn = elem_maxim(neg, nneg);

        for (int exp = 1; maxn / exp > 0; exp *= 10)
            counting(neg, nneg, exp, aux);

        for (int i = 0; i < nneg / 2; i++){
            int temp = neg[i];
            neg[i] = neg[nneg - i - 1];
            neg[nneg - i - 1] = temp;
        }

        for (int i = 0; i < nneg; i++)
            neg[i] = -neg[i];
    }

    int k = 0;
    for (int i = 0; i < nneg; i++)
        array[k++] = neg[i];
    for (int i = 0; i < npos; i++)
        array[k++] = pos[i];

    free(pos);
    free(neg);
    free(aux);
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
    int dimensiuni[] = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000, 10000000};
    int nr_dim = sizeof(dimensiuni) / sizeof(dimensiuni[0]);

    FILE *out = fopen("Rezultate_Sortari.csv", "w");
    fprintf(out, "Dimensiune,Algoritm,Random,Sortat,Reverse,Jumatate Sortat,Aproape Sortat\n");

    struct {
        const char *nume;
        void (*functie)(int [], int);
    } algo[] = {
        {"Bubble Sort", BubbleSort},
        {"Selection Sort", SelectionSort},
        {"Insertion Sort", InsertionSort},
        {"Merge Sort", MergeSortWrap},
        {"Quick Sort", QuickSortWrap},
        {"Counting Sort", CountingSort},
        {"Radix Sort", RadixSort},
        {"Heap Sort", HeapSort}
    };

    int nr_algos = sizeof(algo) / sizeof(algo[0]);

    void (*gen[5])(int [], int) = {
                generareRandom,
                generareSortat,
                generareReverse,
                generareJumatateSortat,
                generareAproapeSortat
    };

    const char *tip_gen[5] = {"Random", "Sortat", "Reverse", "Jumatate Sortat", "Aproape Sortat"};

    for (int s = 0; s < nr_dim; s++){
        int n = dimensiuni[s];

        int *base = (int*) malloc (n * sizeof(int));
        int *arr = (int*) malloc (n * sizeof(int));

        for (int a = 0; a < nr_algos; a++){
            char tip[5][20] = {{0}};
        
            for (int g = 0; g < 5; g++){
                printf("<Dimensiune: %d> - Testam %s pe un vector %s\n", n, algo[a].nume, tip_gen[g]);

                gen[g](base, n);
                memcpy(arr, base, n * sizeof(int));

                int timeout = 0;

                double t = masurareTimp(algo[a].functie, arr, n, 15.0, &timeout);

                if (timeout){
                    printf("<Dimensiune: %d> - TIMEOUT (>15 sec)\n", n);
                    strcpy(tip[g], "15+");
                }
                else
                    sprintf(tip[g], "%.8f", t);

                printf("<Dimensiune: %d> - %s testat pe vector %s completat in %s s\n\n", n, algo[a].nume, tip_gen[g], tip[g]);
            }

            fprintf(out, "%d,%s,%s,%s,%s,%s,%s\n", n, algo[a].nume, tip[0], tip[1], tip[2], tip[3], tip[4]);
        }

        free(base);
        free(arr);
    }
    fclose(out);
    return 0;
}
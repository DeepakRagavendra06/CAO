#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 1000
int arr[MAX], n;

// Sorting prototypes
void bubbleSort();
void selectionSort();
void insertionSort();
void printArray();

// Timing data
void record_time(const char* algo, int threads, double time);

int main() {
    int choice, threads;

    printf("Enter number of elements: ");
    scanf("%d", &n);
    printf("Enter elements:\n");
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    do {
        printf("\nMenu:\n");
        printf("1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n4. Show Graph\n5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 3) {
            printf("Enter number of threads: ");
            scanf("%d", &threads);
            omp_set_num_threads(threads);

            double start = omp_get_wtime();

            switch (choice) {
                case 1: bubbleSort(); record_time("Bubble", threads, omp_get_wtime() - start); break;
                case 2: selectionSort(); record_time("Selection", threads, omp_get_wtime() - start); break;
                case 3: insertionSort(); record_time("Insertion", threads, omp_get_wtime() - start); break;
            }

            double end = omp_get_wtime();
            printf("Sorted Array: ");
            printArray();
            printf("Time taken: %f seconds\n", end - start);
        } else if (choice == 4) {
            system("python3 plot_graph.py");
        }

    } while (choice != 5);

    return 0;
}


void bubbleSort() {
    int i, temp, phase;
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            #pragma omp parallel for private(i, temp)
            for (i = 0; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        } else {
            #pragma omp parallel for private(i, temp)
            for (i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        }
    }
}

// --- Selection Sort ---
void selectionSort() {
    int i, j, min, temp;
    for (i = 0; i < n - 1; i++) {
        min = i;
        #pragma omp parallel for
        for (j = i + 1; j < n; j++) {
            #pragma omp critical
            {
                if (arr[j] < arr[min]) {
                    min = j;
                }
            }
        }
        if (min != i) {
            temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }
}

// --- Insertion Sort ---
void insertionSort() {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            #pragma omp single
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}


void printArray() {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

void record_time(const char* algo, int threads, double time) {
    FILE *fp = fopen("timings.csv", "a");
    fprintf(fp, "%s,%d,%f\n", algo, threads, time);
    fclose(fp);
}


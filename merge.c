#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int arr[], int left[], int leftSize, int right[], int rightSize) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = left[i++];
    }

    while (j < rightSize) {
        arr[k++] = right[j++];
    }
}

void mergeSort(int arr[], int size) {
    if (size <= 1) {
        return;
    }

    int mid = size / 2;
    int* left = (int*)malloc(mid * sizeof(int));
    int* right = (int*)malloc((size - mid) * sizeof(int));

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < mid; i++) {
                left[i] = arr[i];
            }
            mergeSort(left, mid);
        }

        #pragma omp section
        {
            for (int i = mid; i < size; i++) {
                right[i - mid] = arr[i];
            }
            mergeSort(right, size - mid);
        }
    }

    merge(arr, left, mid, right, size - mid);

    free(left);
    free(right);
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int size = 10;
    int arr[] = { 41, 67, 34, 0, 69, 24, 78, 58, 62, 64 };

    printf("Original array: ");
    printArray(arr, size);

    printf("Parallel Merge Sort:\n");
    mergeSort(arr, size);
    printf("Sorted array: ");
    printArray(arr, size);

    return 0;
}

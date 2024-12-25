#include <stdlib.h>
#include <string.h>

void quickSort(int* array, int low, int high) {
    if (low >= high) {
        return;
    }

    int pivot = array[low + (high - low) / 2];
    int left = low;
    int right = high;

    while (left <= right) {
        while (array[left] < pivot) {
            left++;
        }
        while (array[right] > pivot) {
            right--;
        }
        if (left <= right) {
            int temp = array[left];
            array[left] = array[right];
            array[right] = temp;
            left++;
            right--;
        }
    }

    quickSort(array, low, right);
    quickSort(array, left, high);
}

int* Sort(int* array, int size) {
    if (array == NULL || size < 2) {
        return array;
    }
    quickSort(array, 0, size - 1);
    return array;
}
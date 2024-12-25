#include <stdio.h>
#include <stdlib.h>
#include "all/bubble_sort.c"
#include "all/integral_rectangle.c"

int main() {
    printf("First file. 0 - integral, else - sort\n");
    int number_of_function;
    scanf("%d", &number_of_function);

   if (number_of_function == 0) {
        float A, B, e;
        printf("Enter A, B and e: ");
        scanf("%f %f %f", &A, &B, &e);

        float result = SinIntegral(A, B, e);
        printf("int_A^B sin(x)dx = %f\n", result);
    } else {
        int size;
        printf("Enter the size of the array: ");
        scanf("%d", &size);

        if (size <= 0) {
            printf("Invalid size.\n");
            return 1;
        }

        int* arr = (int*)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        printf("Enter %d elements: ", size);
        for (int i = 0; i < size; ++i) {
            scanf("%d", &arr[i]);
        }

        Sort(arr, size);

        printf("Sorted array: ");
        for (int i = 0; i < size; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        free(arr);
        return 0;
    }
}
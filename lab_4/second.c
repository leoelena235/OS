#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

float (*integral)(float, float, float);
int* (*sort)(int*, int);

void* handle_integral;
void* handle_sort;

void load_libraries() {
    printf("Second file. 0 - bubble_sort and rectangle_integral, else - hoare_sort and trapezoid_integral: \n");
    int realization;
    scanf("%d", &realization);

    if (realization == 0) {
        handle_sort = dlopen("./libraries/libbubble_sort.so", RTLD_LAZY);
        handle_integral = dlopen("./libraries/libintegral_rectangle.so", RTLD_LAZY);
    } else {
        handle_sort = dlopen("./libraries/libhoare_sort.so", RTLD_LAZY);
        handle_integral = dlopen("./libraries/libintegral_trapezoid.so", RTLD_LAZY);
    }

    if (!handle_sort) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    if (!handle_integral) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    dlerror();

    integral = dlsym(handle_integral, "SinIntegral");
    sort = dlsym(handle_sort, "Sort");

    char* error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
}

int main() {
    load_libraries();

    printf("number of func: 0 - integral, else - sort\n");
    int number_of_function;
    scanf("%d", &number_of_function);

    if (number_of_function == 0) {
        printf("Enter A, B and e: ");
        float A, B, e;
        scanf("%f %f %f", &A, &B, &e);

        float result = (*integral)(A, B, e);
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

        extern void Sort(int* array, int size);
        arr = (*sort)(arr, size);

        printf("Sorted array: ");
        for (int i = 0; i < size; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        free(arr);
        return 0;
    }

    dlclose(handle_sort);
    dlclose(handle_integral);
}
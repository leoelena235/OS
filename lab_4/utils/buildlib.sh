gcc -shared -fPIC include/bubble_sort.c -o libraries/libbubble_sort.so
gcc -shared -fPIC include/hoare_sort.c -o libraries/libhoare_sort.so
gcc -shared -fPIC include/integral_trapezoid.c -o libraries/libintegral_trapezoid.so -lm
gcc -shared -fPIC include/integral_rectangle.c -o libraries/libintegral_rectangle.so -lm
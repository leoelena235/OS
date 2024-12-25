#include <math.h>

float SinIntegral(float A, float B, float e) {
    float result = 0;
    for (float cur = A; cur < B; cur += e) {
        float next = cur + e;
        float height = sinf((cur + next) / 2.0);
        result += height * e;
    }

    return result;
}
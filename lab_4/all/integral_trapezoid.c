#include <math.h>

float SinIntegral(float A, float B, float e) {
    float result = 0;
    for (float cur = A; cur < B; cur += e) {
        float next = cur + e;
        float side1 = sinf(cur);
        float side2 = sinf(next);
        result += 0.5 * (side1 + side2) * e;
    }

    return result;
}
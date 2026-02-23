#include "matrix.c"
#include <stdio.h>
#include <math.h>

int main()
{
    float A[3][3] = {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
        {7.0f, 8.0f, 9.0f}
    };

    float B[3][3] = {
        {9.0f, 8.0f, 7.0f},
        {6.0f, 5.0f, 4.0f},
        {3.0f, 2.0f, 1.0f}
    };

    float C[3][3];

    multiply_matrices(A, B, C);

    printf("A * B:\n");
    print_matrix(C);


    float Tscale[3][3];
    init_identity_matrix(Tscale);

    scale(Tscale, 2.0f, 3.0f);   /* x2, y3 */

    printf("\nScale matrix (sx=2, sy=3):\n");
    print_matrix(Tscale);

    float x = 1.0f, y = 1.0f;
    float tx, ty;

    transform_point(Tscale, x, y, &tx, &ty);

    printf("Point (%.2f, %.2f) after scale -> (%.2f, %.2f)\n",
           x, y, tx, ty);

    float Tshift[3][3];
    init_identity_matrix(Tshift);

    shift(Tshift, 5.0f, -2.0f);  /* dx=5, dy=-2 */

    printf("\nShift matrix (dx=5, dy=-2):\n");
    print_matrix(Tshift);

    transform_point(Tshift, x, y, &tx, &ty);

    printf("Point (%.2f, %.2f) after shift -> (%.2f, %.2f)\n",
           x, y, tx, ty);


    float Trotate[3][3];
    init_identity_matrix(Trotate);

    rotate(Trotate, 3.14159265359 / 2.0f);   /* 90 fok */

    printf("\nRotate matrix (90 degrees):\n");
    print_matrix(Trotate);

    transform_point(Trotate, 1.0f, 0.0f, &tx, &ty);

    printf("Point (1, 0) after 90° rotation -> (%.2f, %.2f)\n",
           tx, ty);

    return 0;
}
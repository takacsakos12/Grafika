#include "matrix.h"

#include <stdio.h>
#include <math.h>


void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0f;  /* 0.0 -> 0.0f */
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

static void copy_matrix(const float src[3][3], float dst[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            dst[i][j] = src[i][j];
        }
    }
}


void init_identity_matrix(float matrix[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}


void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3])
{
    int i, j, k;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            float sum = 0.0f;
            for (k = 0; k < 3; ++k) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
}

void transform_point(const float matrix[3][3], float x, float y, float* tx, float* ty)
{
    float px = x;
    float py = y;
    float pw = 1.0f;

    float rx = matrix[0][0] * px + matrix[0][1] * py + matrix[0][2] * pw;
    float ry = matrix[1][0] * px + matrix[1][1] * py + matrix[1][2] * pw;
    float rw = matrix[2][0] * px + matrix[2][1] * py + matrix[2][2] * pw;

    if (rw != 0.0f && rw != 1.0f) {
        rx /= rw;
        ry /= rw;
    }

    *tx = rx;
    *ty = ry;
}


void scale(float transform[3][3], float sx, float sy)
{
    float s[3][3];
    float tmp[3][3];

    init_identity_matrix(s);
    s[0][0] = sx;
    s[1][1] = sy;

    multiply_matrices(s, transform, tmp);
    copy_matrix(tmp, transform);
}

void shift(float transform[3][3], float dx, float dy)
{
    float t[3][3];
    float tmp[3][3];

    init_identity_matrix(t);
    t[0][2] = dx;
    t[1][2] = dy;

    multiply_matrices(t, transform, tmp);
    copy_matrix(tmp, transform);
}

void rotate(float transform[3][3], float angle_rad)
{
    float r[3][3];
    float tmp[3][3];

    float c = cosf(angle_rad);
    float s = sinf(angle_rad);

    init_identity_matrix(r);
    r[0][0] = c;   r[0][1] = -s;
    r[1][0] = s;   r[1][1] =  c;

    multiply_matrices(r, transform, tmp);
    copy_matrix(tmp, transform);
}
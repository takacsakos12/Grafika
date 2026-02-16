#include "matrix.h"

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
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

void init_identity_matrix(float matrix[3][3])
{
    int i;
    int j;
     for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if(i == j)  {
                matrix[i][j] = 1.0;
            }
            else {
                matrix[i][j] = 0.0;
            }
        }
    }
}


void multiply_matrices(float matrix[3][3], float matrix2[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i)
for (int j = 0; j < 3; ++j)
result[i][j] = 0;

for (int i = 0; i < 3; ++i)
for (int j = 0; j < 3; ++j)
for (int k = 0; k < 3; ++k)
result[i][j] += matrix[i][k] * matrix2[k][j];
}
void transform_point(float coordinate[2] ,float transform_matrix[2][2], float result_coordinate[2])  {
    float temp[2];
    for(int i=0; i < 2; i++)    {
        for(int j=0; i < 2; j++)    {
            result[] =
        }  
    }

}




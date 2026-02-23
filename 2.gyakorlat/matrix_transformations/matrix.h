#ifndef MATRIX_H
#define MATRIX_H

void init_zero_matrix(float matrix[3][3]);
void print_matrix(const float matrix[3][3]);
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

void init_identity_matrix(float matrix[3][3]);
void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3]);

/* Homogén 2D pont transzformáció: (x,y,1) -> (tx,ty) */
void transform_point(const float matrix[3][3], float x, float y, float* tx, float* ty);


void scale(float transform[3][3], float sx, float sy);     /* origó középpontú */
void shift(float transform[3][3], float dx, float dy);
void rotate(float transform[3][3], float angle_rad);       /* origó középpontú */

#endif
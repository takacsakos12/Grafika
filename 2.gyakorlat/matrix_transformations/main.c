#include "matrix.h"
#include "matrix.c"

int main(int argc, char* argv[])
{
	float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
	float b[3][3];
	float c[3][3];
    

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    
    
    add_matrices(a, b, c);

    init_identity_matrix(c);
    float d[3][3];
    multiply_matrices(a,b,d);

    //print_matrix(c);
    print_matrix(d);
    

	return 0;
}


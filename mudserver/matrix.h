/* matrix.h                                         */
/* Matrix routines should we ever want to           */
/* implement scaling and rotation in server         */
/*                                                  */
/* Iain Ollerenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */
/*                                                  */
#ifndef MATRIX_H
#define MATRIX_H

struct point {
    float p[3];
};

typedef struct point point;

struct matrix {
    float mat[3][3];
};

typedef struct matrix matrix;

point translation(point p, int a, int b);

point scaling(point p, int a, int b);

point rotation(point p, int x);

point multiply_point(point p, matrix a);

void print_point(point p);

void print_matrix(matrix m);

point make_point(float a, float b);

void easy_point(point p);
#endif // MATRIX_H

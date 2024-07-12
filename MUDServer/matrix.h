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

point multpoint(point p, matrix a);

void printpoint(point p);

void printMatrix(matrix m);

point mkpoint(float a, float b);

void easypoint(point p);
#endif // MATRIX_H

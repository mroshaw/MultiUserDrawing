/* matrix.c                                         */
/* Matrix routines should we ever want to           */
/* implement scaling and rotation in server         */
/*                                                  */
/* Iain Ollerenshaw 13/10/97                        */
/*                                                  */
/* Version History:                                 */
/* Date     Who     Comments                        */
/* 13/10/97 IO      Created                         */
/*                                                  */

#include "matrix.h"
#include <stdio.h>
#include <math.h>

point mkpoint(float a, float b)
{
    point temp;
    temp.p[0]=a;
    temp.p[1]=b;
    temp.p[2]=1.0;
    return temp;
}

/*  Translation matrix procedure.
Takes a point and two translation integers a and b
Returns a new point */
point translation (point p, int a, int b)
{
    matrix translation;
    point result;
    /* Set up translation matrix: */
    translation.mat[0][0] = 1.0;
    translation.mat[0][1] = 0;
    translation.mat[0][2] = 0;  /* = 1 0 0 */
    translation.mat[1][0] = 0;  /*  0 1 0 */
    translation.mat[1][1] = 1.0; /* a b 1 */
    translation.mat[1][2] = 0;
    translation.mat[2][0] = a;
    translation.mat[2][1] = b;
    translation.mat[2][2] = 1.0;
    /* Multiply together to get the result! */ result = multpoint (p, translation);
    return result;
}

/*  Scaling matrix
Takes a matrix and two scaling integers a and b.
Returns a new point
*/
point scaling (point p, int a, int b)
{
    matrix scaling;
    point result;
    /* Set up scaling matrix:   */
    scaling.mat[0][0] = a;
    scaling.mat[0][1] = 0;
    scaling.mat[0][2] = 0; /* = a 0 0 */
    scaling.mat[1][0] = 0; /*   0 b 0 */
    scaling.mat[1][1] = b; /*   0 0 1 */
    scaling.mat[1][2] = 0;
    scaling.mat[2][0] = 0;
    scaling.mat[2][1] = 0;
    scaling.mat[2][2] = 1.0;
    /* Multiply together to get the result! */ result = multpoint (p, scaling);
    return result;
}

/*  Rotation matrix
Takes a point and an integer angle x in radians.
Returns a new point
*/
point rotation (point p, int x)
{
    matrix rotation;
    point result;
    float xCos,xSin;
    xCos = cos (x);
    xSin = sin (x);
    /* Set up rotation matrix:  */
    rotation.mat[0][0] = xCos;
    rotation.mat[0][1] = xSin;
    rotation.mat[0][2] = 0; /* = cosx sinx 0    */
    rotation.mat[1][0] = -xSin; /*  -sinx cosx 0 */
    rotation.mat[1][1] = xCos;  /*  0   0  1   */
    rotation.mat[1][2] = 0;
    rotation.mat[2][0] = 0;
    rotation.mat[2][1] = 0;
    rotation.mat[2][2] = 1.0;

    /* Display resulting matrix */
    fprintf(stdout, "Rotation matrix is :\n");
    printMatrix(rotation);
    fprintf(stdout, "\n");

    /* Multiply together to get the result! */
    result = multpoint (p, rotation);
    return result;
}
/* Displays a matrix on screen  */
void printMatrix (matrix m)
{
    int i,j;
    fprintf(stdout, "\n");
    for (i = 0; i<3; i++)
        for (j=0; j<3; j++)
            fprintf(stdout, "%f\t",m.mat[i][j]);
    fprintf(stdout, "\n");
    fprintf(stdout, "\n");
}
/* Multiplies together a point p and a matrix a. The result is a point */
point multpoint (point p, matrix a)
{
    point result;
    int i,j;
    float res;
    for (i=0; i< 3; i++)
    {
        res=0;
        for (j=0; j<3; j++)
        {
            res+= p.p[j]*a.mat[j][i];
        }
        result.p[i]=res;
    }
    return result;
}

/* Displays a points co-ordinates on the screen */
void printpoint (point p)
{
    fprintf(stdout, "(%f,%f)\n",p.p[0], p.p[1]);
}

/* Just prints a points x and y co-ords one after the other on new
lines: */
void easypoint (point p)
{
    fprintf(stdout, "%f\n",p.p[0]);
    fprintf(stdout, "%f\n",p.p[1]);
}

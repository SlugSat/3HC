#ifndef MATRIX_MATH_3_H
#define MATRIX_MATH_3_H

/**
 * matrixmath3.h
 *
 * This file implements a basic 3x3 matrix math library. Basic matrix
 * operations are provided along with the matrix inverse function (though that
 * function cannot handle singular matrices).
 *
 * Matrices are defined in row-major order, so that the matrix:
 *   0 1 2
 *   3 4 5
 *   6 7 8
 * is represented by the array:
 *
 * `float mat[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};`
 */

/**
 * FP_DELTA defines the tolerance for testing equality for floating-point
 * numbers. Used within MatrixEquals() and is useful for comparison of return
 * values of MatrixDeterminant() and MatrixTrace().
 */
#define FP_DELTA 0.0001

/******************************************************************************
 * Matrix - Matrix Operations
 *****************************************************************************/

/**
 * Matrix3Multiply performs a matrix-matrix multiplication operation on two 3x3
 * matrices and returns the result in the third argument.
 */
void Matrix3Multiply(float mat1[3][3], float mat2[3][3], float result[3][3]);

/**
 * Matrix3Add performs a matrix addition operation on two 3x3 matrices and
 * returns the result in the third argument.
 */
void Matrix3Add(float mat1[3][3], float mat2[3][3], float result[3][3]);

/**
 * Matrix3Equals checks if the two matrix arguments are equal. The result is
 * 0 if FALSE and 1 if TRUE to follow the standard C conventions of TRUE and
 * FALSE.
 */
int Matrix3Equals(float mat1[3][3], float mat2[3][3]);

/******************************************************************************
 * Matrix - Scalar Operations
 *****************************************************************************/

/**
 * Matrix3ScalarMultiply performs the multiplication of a matrix by a scalar.
 * The result is returned in the third argument.
 */
void Matrix3ScalarMultiply(float x, float mat[3][3], float result[3][3]);

/**
 * Matrix3ScalarAdd performs the addition of a matrix by a scalar. The result
 * is returned in the third argument.
 */
void Matrix3ScalarAdd(float x, float mat[3][3], float result[3][3]);

/******************************************************************************
 * Matrix - Vector Operations
 *****************************************************************************/

 /**
  * Matrix3VectorMultiply performs the multiplication of a matrix by a vector.
  * The result is returned in the third argument.
  */
void Matrix3VectorMultiply(float vec[3], float mat[3][3], float result[3]);

/******************************************************************************
 * Unary Matrix Operations
 *****************************************************************************/

/**
 * Matrix3Determinant calculates the determinant of a matrix and returns the
 * value as a float.
 */
float Matrix3Determinant(float mat[3][3]);

/**
 * MatrixTrace calculates the trace of a matrix. The result is returned as a
 * float.
 */
float Matrix3Trace(float mat[3][3]);

/**
 * MatrixTranspose calculates the transpose of a matrix and returns the
 * result through the second argument
 */
void Matrix3Transpose(float mat[3][3], float result[3][3]);

/**
 * MatrixInverse calculates the inverse of a matrix and returns the
 * result through the second argument.
 */
void Matrix3Inverse(float mat[3][3], float result[3][3]);

/**
 * MatrixPrint sends a 3x3 array to standard output with clean formatting.
 */
void Matrix3Print(float mat[3][3]);

#endif // MATRIX_MATH_3_H

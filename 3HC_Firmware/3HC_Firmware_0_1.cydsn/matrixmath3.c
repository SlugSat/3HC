/**
 * matrixmath3.c
 *
 * Written by Caleb Cotter
 * This file contains the implementations of a 3x3 matrix math library.
 */

 //#define TEST_MATRIX_MATH_3

 // Header Files
 #include "matrixmath3.h"

// Standard libraries
#include <math.h>
#include <stdio.h>

/******************************************************************************
 * Matrix - Matrix Operations
 *****************************************************************************/

void Matrix3Multiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int i = 0, j = 0, k = 0;
    float sum;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          sum = 0;
          for (k = 0; k <3; k++) {
             sum += mat1[i][k] * mat2[k][j];
          }
          result[i][j] = sum;
          sum = 0;
        }
    }
}

void Matrix3Add(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    int i = 0, j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

int Matrix3Equals(float mat1[3][3], float mat2[3][3]) {
    int i = 0, j = 0;
    int result = 1;
    float d;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            d = fabs(mat1[i][j] - mat2[i][j]);
            if (d < FP_DELTA) {
                continue;
            } else {
                result = 0;
                break;
            }
        }
    }
    return result;
}

/******************************************************************************
 * Matrix - Scalar Operations
 *****************************************************************************/

void Matrix3ScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    int i = 0, j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = (mat[i][j] * x);
        }
    }
}

void Matrix3ScalarAdd(float x, float mat[3][3], float result[3][3]) {
    int i = 0, j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = (mat[i][j] + x);
        }
    }
}

/******************************************************************************
 * Matrix - Vector Operations
 *****************************************************************************/

void Matrix3VectorMultiply(float vec[3], float mat[3][3], float result[3]) {
  int i = 0, j = 0;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      result[i] += vec[j] * mat[i][j];
    }
  }
}

/******************************************************************************
 * Unary Matrix Operations
 *****************************************************************************/

float Matrix3Determinant(float mat[3][3]) {
    float determTotal = 0;
    float leftRightDiag = 0;
    float rightLeftDiag = 0;
    leftRightDiag += (mat[0][0] * mat[1][1] * mat[2][2]);
    leftRightDiag += (mat[0][1] * mat[1][2] * mat[2][0]);
    leftRightDiag += (mat[0][2] * mat[1][0] * mat[2][1]);

    rightLeftDiag += (mat[0][2] * mat[1][1] * mat[2][0]);
    rightLeftDiag += (mat[0][0] * mat[1][2] * mat[2][1]);
    rightLeftDiag += (mat[0][1] * mat[1][0] * mat[2][2]);

    determTotal = leftRightDiag - rightLeftDiag;

    return determTotal;
}

float Matrix3Trace(float mat[3][3]) {
    float traceTotal = 0;
    int i = 0, j = 0;
    for (i = 0; i < 3; i++) {
        traceTotal += mat[i][j];
        j++;
    }
    return traceTotal;
}

void Matrix3Transpose(float mat[3][3], float result[3][3]) {
    int i = 0, j = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[j][i] = mat[i][j];
        }
    }
}

void Matrix3Inverse(float mat[3][3], float result[3][3]) {
    float determinateInv = 0;
    float cofactorMat[3][3] = {{},{},{}};
    float adjugateMat[3][3] = {{},{},{}};

    cofactorMat[0][0] = (mat[1][1] * mat[2][2]) - (mat[1][2] * mat[2][1]);
    cofactorMat[0][1] = (mat[1][2] * mat[2][0]) - (mat[1][0] * mat[2][2]);
    cofactorMat[0][2] = (mat[1][0] * mat[2][1]) - (mat[1][1] * mat[2][0]);
    cofactorMat[1][0] = (mat[0][2] * mat[2][1]) - (mat[0][1] * mat[2][2]);
    cofactorMat[1][1] = (mat[0][0] * mat[2][2]) - (mat[0][2] * mat[2][0]);
    cofactorMat[1][2] = (mat[0][1] * mat[2][0]) - (mat[0][0] * mat[2][1]);
    cofactorMat[2][0] = (mat[0][1] * mat[1][2]) - (mat[0][2] * mat[1][1]);
    cofactorMat[2][1] = (mat[0][2] * mat[1][0]) - (mat[0][0] * mat[1][2]);
    cofactorMat[2][2] = (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
    Matrix3Transpose(cofactorMat, adjugateMat);
    determinateInv = (1 / Matrix3Determinant(mat));
    Matrix3ScalarMultiply(determinateInv, adjugateMat, result);
}

void Matrix3Print(float mat[3][3]) {
    int i = 0, j = 0;
    printf(" -----------------------------------\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("| % .6f ", (double) mat[i][j]);
        }
        printf("|\n -----------------------------------\n");
    }
    printf("\n");
}

/******************************************************************************
 * Test Harness
 *****************************************************************************/
 #ifdef TEST_MATRIX_MATH_3
 #include <stdio.h>

 // **** Set macros and preprocessor directives ****
 #define NUM_TESTS 10.0

  float testResult[3][3] = {
     {},
     {},
     {}
 };
 int testVal = 0;
 int totalTestsPassed = 0;
 int functionTestsPassed = 0;

 int main(void){
   printf("\n----- matrixmath3 -----\n");

   // *********** Matrix Equals Test ***********
   float eqArray1[3][3] = {
       {-40, 22.2, 3.14},
       {4.2, 67.23, 74.23},
       {-68.32, 8.1, 1}
   };
   float eqArray2[3][3] = {
       {-40, 22.2, 3.14},
       {4.2, 67.23, 74.23},
       {-68.32, 8.1, 1}
   };
   testVal = Matrix3Equals(eqArray1, eqArray2);
   functionTestsPassed += testVal;

   float eqArray3[3][3] = {
       {-40, 12.32, 3.14},
       {15, 67.23, 74.23},
       {68.32, 8.1, 1}
   };
   float eqArray4[3][3] = {
       {40, 0, 3.14},
       {4.2, 0, 72.2},
       {-68.32, 8.12, 0}
   };
   testVal = Matrix3Equals(eqArray3, eqArray4);
   if (testVal == 0) {
       functionTestsPassed++;
   }

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Equals()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Equals()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Multiply Test ***********
   float multaddArray1[3][3] = {
       {1, 2, 3},
       {4, 5, 6},
       {7, 8, 9}
   };
   float multaddArray2[3][3] = {
       {9, 8, 7},
       {6, 5, 4},
       {3, 2, 1}
   };
   float multExpResult1[3][3] = {
       {30, 24, 18},
       {84, 69, 54},
       {138, 114, 90}
   };

   Matrix3Multiply(multaddArray1, multaddArray2, testResult);
   testVal = Matrix3Equals(testResult, multExpResult1);
   functionTestsPassed += testVal;

   float multaddArray3[3][3] = {
       {3, 12, 33},
       {-4.43, 5.26, -2.23},
       {4.2, -8.8, 9.1}
   };
   float multaddArray4[3][3] = {
       {2, 3, 0},
       {4, 2, 0},
       {1, -2, -1}
   };
   float multExpResult2[3][3] = {
       {87, -33, -33},
       {9.95, 1.69, 2.23},
       {-17.7, -23.2, -9.1}
   };

   Matrix3Multiply(multaddArray3, multaddArray4, testResult);
   testVal = Matrix3Equals(testResult, multExpResult2);
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Multiply()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Multiply()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Add Test ***********
   float addExpResult1[3][3] = {
       {10, 10, 10},
       {10, 10, 10},
       {10, 10, 10}
   };
   Matrix3Add(multaddArray1, multaddArray2, testResult);
   testVal = Matrix3Equals(testResult, addExpResult1);
   functionTestsPassed += testVal;

   float addExpResult2[3][3] = {
       {5, 15, 33},
       {-0.43, 7.26, -2.23},
       {5.2, -10.8, 8.1}
   };
   Matrix3Add(multaddArray3, multaddArray4, testResult);
   testVal = Matrix3Equals(testResult, addExpResult2);
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Add()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Add()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Scalar Multiply Test ***********
   float sFactor1 = -45.22;
   float sArray1[3][3] = {
       {1, 2, 3},
       {4, 5, 6},
       {7, 8, 9}
   };
   float sMultResult1[3][3] = {
       {-45.22, -90.44, -135.66},
       {-180.88, -226.1, -271.32},
       {-316.54, -361.76, -406.98}
   };
   Matrix3ScalarMultiply(sFactor1, sArray1, testResult);
   testVal = Matrix3Equals(testResult, sMultResult1);
   functionTestsPassed += testVal;

   float sFactor2 = 2.0;
   float sArray2[3][3] = {
       {1, 2, 3},
       {4, 5, 6},
       {7, 8, 9}
   };
   float sMultResult2[3][3] = {
       {2, 4, 6},
       {8, 10, 12},
       {14, 16, 18}
   };
   Matrix3ScalarMultiply(sFactor2, sArray2, testResult);
   testVal = Matrix3Equals(testResult, sMultResult2);
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3ScalarMultiply()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3ScalarMultiply()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Scalar Add Test ***********
   float sAddResult1[3][3] = {
       {-44.22, -43.22, -42.22},
       {-41.22, -40.22, -39.22},
       {-38.22, -37.22, -36.22}
   };
   Matrix3ScalarAdd(sFactor1, sArray1, testResult);
   testVal = Matrix3Equals(testResult, sAddResult1);
   functionTestsPassed += testVal;

   float sAddResult2[3][3] = {
       {3, 4, 5},
       {6, 7, 8},
       {9, 10, 11}
   };
   Matrix3ScalarAdd(sFactor2, sArray2, testResult);
   testVal = Matrix3Equals(testResult, sAddResult2);
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3ScalarAdd()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3ScalarAdd()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Vector Multiply Test ***********
   float vec1[3] = {1, 2, 3};
   float vecArray1[3][3] = {
       {1, 2, 3},
       {4, 5, 6},
       {7, 8, 9}
   };
   float vecResult1[3] = {};
   Matrix3VectorMultiply(vec1, vecArray1, vecResult1);
   if ((fabs(vecResult1[0] - 14) < FP_DELTA) &&
       (fabs(vecResult1[1] - 32) < FP_DELTA) &&
       (fabs(vecResult1[2] - 50) < FP_DELTA)){
     testVal = 1;
   } else {
     testVal = 0;
   }
   functionTestsPassed += testVal;

   float vec2[3] = {0, 1, 1};
   float vecArray2[3][3] = {
       {0.213, 21.3, -12.3},
       {78.2, 143.4, -143.4},
       {20.0, 12.134, 9.69}
   };
   float vecResult2[3] = {};
   Matrix3VectorMultiply(vec2, vecArray2, vecResult2);
   if ((fabs(vecResult2[0] - 9) < FP_DELTA) &&
       (fabs(vecResult2[1] - 0) < FP_DELTA) &&
       (fabs(vecResult2[2] - 21.824) < FP_DELTA)){
     testVal = 1;
   } else {
     testVal = 0;
   }
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3VectorMultiply()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3VectorMultiply()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Determinant ***********
   float determResult = 0;
   float determTest1[3][3] = {
       {2, 1, 1},
       {1, -2.1, 1},
       {1, 1, 2}
   };
   float expectedDeterm1 = -8.3;
   determResult = Matrix3Determinant(determTest1);
   determResult -= expectedDeterm1;
   determResult = fabs(determResult);
   if (determResult < FP_DELTA) {
       functionTestsPassed++;
   }

   float determTest2[3][3] = {
       {12.1, 21.1, 2},
       {4.2, 0, 2.1},
       {23.1, 1.3, 4.3}
   };
   float expectedDeterm2 = 620.382;
   determResult = Matrix3Determinant(determTest2);
   determResult -= expectedDeterm2;
   determResult = fabs(determResult);
   if (determResult < FP_DELTA) {
       functionTestsPassed++;
   }

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Determinant()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Determinant()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Trace ***********
   float traceResult = 0;
   float traceTest1[3][3] = {
       {10, 18.2, 13.2},
       {3.134, 20, 74.2},
       {-34.2, 12.3, 30}
   };
   float expectedTrace1 = 60.0;
   traceResult = Matrix3Trace(traceTest1);
   traceResult -= expectedTrace1;
   traceResult = fabs(traceResult);
   if (traceResult < FP_DELTA) {
       functionTestsPassed++;
   }

   float traceTest2[3][3] = {
       {-3.123, 0.0, 0.0},
       {0.0, 12.112, 0},
       {0.0, 0.0, 11.32}
   };
   float expectedTrace2 = 20.309;
   traceResult = Matrix3Trace(traceTest2);
   traceResult -= expectedTrace2;
   traceResult = fabs(traceResult);
   if (traceResult < FP_DELTA) {
       functionTestsPassed++;
   }

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Trace()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Trace()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Matrix Transpose ***********
   float transArray1[3][3] = {
       {0, 1, 1},
       {2, 0, 1},
       {2, 2, 0}
   };
   float transResult1[3][3] = {
       {0, 2, 2},
       {1, 0, 2},
       {1, 1, 0}
   };
   Matrix3Transpose(transArray1, testResult);
   testVal = Matrix3Equals(transResult1, testResult);
   functionTestsPassed += testVal;

   float transArray2[3][3] = {
       {-12.32, 34.21, -693.4},
       {27.43, 573.2, 89.2134},
       {23.24, 45.21, 23.121}
   };
   float transResult2[3][3] = {
       {-12.32, 27.43, 23.24},
       {34.21, 573.2, 45.21},
       {-693.4, 89.2134, 23.121}
   };
   Matrix3Transpose(transArray2, testResult);
   testVal = Matrix3Equals(transResult2, testResult);
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Transpose()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Transpose()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;


   // *********** Matrix Inverse ***********
   float inverseArray1[3][3] = {
       {3, 0, 2},
       {2, 0, -2},
       {0, 1, 1}
   };
   float inverseResult1[3][3] = {
       {0.2, 0.2, 0},
       {-0.2, 0.3, 1},
       {0.2, -0.3, 0}
   };
   Matrix3Inverse(inverseArray1, testResult);
   testVal = Matrix3Equals(inverseResult1, testResult);
   functionTestsPassed += testVal;

   float inverseArray2[3][3] = {
       {3, 0, 8},
       {4, 7, 1},
       {7, 0, 1}
   };
   float inverseResult2[3][3] = {
       {-0.01886, 0, 0.15094},
       {-0.00808, 0.142857, -0.078167},
       {0.132075, 0, -0.056603}
   };
   Matrix3Inverse(inverseArray2, testResult);
   testVal = Matrix3Equals(inverseResult2, testResult);
   functionTestsPassed += testVal;

   if (functionTestsPassed == 2) {
       printf("PASSED (%d/2): Matrix3Inverse()\n", functionTestsPassed);
       totalTestsPassed++;
   } else {
       printf("FAILED (%d/2): Matrix3Inverse()\n", functionTestsPassed);
   }
   functionTestsPassed = 0;
   testVal = 0;

   // *********** Testing Summary ***********
   float percentPassed = ((totalTestsPassed / NUM_TESTS) * 100.0);
   printf("----------------------\n");
   printf("%d out of 10 functions passed (%3.1f%%).\n\n", totalTestsPassed, (double) percentPassed);

   // *********** Matrix Print Test ***********
   float arrayToPrint[3][3] = {
       {1.10, 2.20, 3.30},
       {4.40, 5.50, 6.60},
       {7.70, 8.80, 9.90}
   };
   printf("Output of Matrix3Print():\n");
   Matrix3Print(arrayToPrint);
   printf("Expected output of Matrix3Print():\n");
   printf(" ____________________ \n");
   printf("| 1.10 | 2.20 | 3.30 |\n");
   printf(" -------------------- \n");
   printf("| 4.40 | 5.50 | 6.60 |\n");
   printf(" -------------------- \n");
   printf("| 7.70 | 8.80 | 9.90 |\n");
   printf(" -------------------- \n");
 }
 #endif

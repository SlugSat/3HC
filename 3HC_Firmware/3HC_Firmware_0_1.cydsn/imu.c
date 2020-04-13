/**
 * imu.c
 *
 * Caleb Cotter - 2019
 *
 */

 //#define TEST_IMU

 // Header Files
 #include "imu.h"
 #include "matrixmath3.h"

 // Standard Libraries
 #include <math.h>

/* ------------------------- Compile Time Options ------------------------- */

/* Closed Loop Parameters */
#define ACC_K_P 0.200000
#define ACC_K_I (ACC_K_P/10)
#define MAG_K_P 0.200000
#define MAG_K_I (MAG_K_P/10)

/* Open Loop PARAMETERS */
#define USE_MATRIX_EXPONENTIAL_FORM

 /* Accelerometer Calibration Values */
 static float accScaleMatrix[3][3] = {{ 0.6061,  0.0018,  0.0058},
                                      { 0.0017,  0.6010, -0.0047},
                                      { 0.0045, -0.0008,  0.5935}};

 static float accOffsetVector[3]   =  {-0.0119, -0.0123,  0.0483};

 /* Magnetometer Calibration Values */
 static float magScaleMatrix[3][3] = {{ 0.0037, -0.0000, -0.0000},
                                      {-0.0001,  0.0039,  0.0000},
                                      {-0.0001,  0.0000,  0.0036}};

 static float magOffsetVector[3]   =  {-0.9859, -0.0477,  0.8850};

 /* Gyroscope Calibration Values */
 static float gyrBias[3] = {162, 172, 94};
 static float gyrSF[3] = {135.608468,131.851116,132.948363};


 /* Magnetometer to Accelerometer Mislignment Matrix */
 static float mag2accMatrix[3][3] = {{ 0.100, -0.100,  0.500},
                                     { 0.250,  0.125,  0.000},
                                     { 0.875,  1.000, -0.375}};

/******************************************************************************
 * Public Functions
 *****************************************************************************/
 void IMU_integrateClosedLoop(float gyr_corr[3], float acc_corr[3], float mag_corr[3], float deltaT, float accelInertial[3], float magInertial[3], float dcm_minus[3][3], float bias_minus[3], float dcm_plus[3][3], float bias_plus[3]) {
   // Inertial to Body Tranformation Matrix
   float dcm_minus_inverse[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
   Matrix3Transpose(dcm_minus, dcm_minus_inverse);

   // Gyroscope Bias Removal
   int i = 0;
   float gyr[3] = {0,0,0};
   for (i = 0; i < 3; i++) gyr[i] = gyr_corr[i] - bias_minus[i];

   // Accelerometer
   float acc[3] = {0,0,0};
   float accelInertial_unit[3] = {0,0,0};
   float acc_rx[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
   float acc_ref[3] = {0,0,0};
   float wmeas_a[3] = {0,0,0};
   norm(acc_corr, acc);
   norm(accelInertial, accelInertial_unit);
   rcross(acc, acc_rx);
   Matrix3VectorMultiply(accelInertial_unit, dcm_minus_inverse, acc_ref);
   Matrix3VectorMultiply(acc_ref, acc_rx, wmeas_a);

   // Magnetometer
   float mag[3] = {0,0,0};
   float magInertial_unit[3] = {0,0,0};
   float mag_rx[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
   float mag_ref[3] = {0,0,0};
   float wmeas_m[3] = {0,0,0};
   norm(mag_corr, mag);
   norm(magInertial, magInertial_unit);
   rcross(mag, mag_rx);
   Matrix3VectorMultiply(magInertial_unit, dcm_minus_inverse, mag_ref);
   Matrix3VectorMultiply(mag_ref, mag_rx, wmeas_m);

   // Update Attitude Estimate
   float estimatedRates[3] = {0,0,0};
   for (i = 0; i < 3; i++) estimatedRates[i] = gyr[i] + (ACC_K_P * wmeas_a[i]) + (MAG_K_P * wmeas_m[i]);
   float r[3][3];
   rexp(estimatedRates, r);
   Matrix3Multiply(dcm_minus, r, dcm_plus);

   // Update Bias Estimate
   for (i = 0; i < 3; i++) bias_plus[i] = bias_minus[i] - (ACC_K_I * wmeas_a[i]) - (MAG_K_I * wmeas_m[i]);
 }
 void IMU_integrateOpenLoop(float rate[3], float dt, float dcm_minus[3][3], float dcm_plus[3][3]) {
   // Matrix Exponential Integration
   #ifdef USE_MATRIX_EXPONENTIAL_FORM
   int i = 0;
   float ang[3];
   float r[3][3];
   for(i = 0; i < 3; i++) ang[i] = rate[i]*dt;
   rexp(ang, r);
   Matrix3Multiply(dcm_minus, r, dcm_plus);
   // Forward Integration
   #else
   float rx[3][3], rxdt[3][3], dcm_delta[3][3];
   rcross(rate, rx);
   Matrix3ScalarMultiply(dt, rx, rxdt);
   Matrix3Multiply(dcm_minus, rxdt, dcm_delta);
   Matrix3Add(dcm_minus, dcm_delta, dcm_plus);
   #endif
 }

 void IMU_dcm2euler(float dcm[3][3], float e[3]) {
   e[0] = atan2(dcm[1][0], dcm[0][0]);
   e[1] = asin(-dcm[2][0]);
   e[2] = atan2(dcm[2][1], dcm[2][2]);
 }

 void IMU_euler2dcm(float e[3], float dcm[3][3]) {
   dcm[0][0] = cos(e[1])*cos(e[0]);
   dcm[1][0] = cos(e[1])*sin(e[0]);
   dcm[2][0] = -sin(e[1]);

   dcm[0][1] = sin(e[2])*sin(e[1])*cos(e[0])-cos(e[2])*sin(e[0]);
   dcm[1][1] = sin(e[2])*sin(e[1])*sin(e[0])+cos(e[2])*cos(e[0]);
   dcm[2][1] = sin(e[2])*cos(e[1]);

   dcm[0][2] = cos(e[2])*sin(e[1])*cos(e[0])+sin(e[2])*sin(e[0]);
   dcm[1][2] = cos(e[2])*sin(e[1])*sin(e[0])-sin(e[2])*cos(e[0]);
   dcm[2][2] = cos(e[2])*cos(e[1]);
 }

 void IMU_corrAcc(float acc[3], float acc_corr[3]) {
    int i = 0;
    float acc_scaled[3] ={};
    Matrix3VectorMultiply(acc, accScaleMatrix, acc_scaled);
    for(i = 0; i < 3; i++) {
      acc_corr[i] = acc_scaled[i] - accOffsetVector[i];
    }
  }

  void IMU_corrMag(float mag[3], float mag_corr[3]) {
    int i = 0;
    float mag_scaled[3] = {};
    Matrix3VectorMultiply(mag, magScaleMatrix, mag_scaled);
    for(i = 0; i < 3; i++) {
      mag_corr[i] = mag_scaled[i] - magOffsetVector[i];
    }
  }

  void IMU_corrGyr(float gyro[3], float gyr_corr[3]) {
    int i = 0;
    for(i = 0; i < 3; i++) {
      gyr_corr[i] = IMU_deg2rad((gyro[i] - gyrBias[i])/gyrSF[i]);
    }
  }

  void IMU_alignMag2Acc(float mag_corr[3], float mag_aligned[3]) {
    Matrix3VectorMultiply(mag_corr, mag2accMatrix, mag_aligned);
  }

 float IMU_rad2deg(float x) {
   return (x * 180 / M_PI);
 }

 float IMU_deg2rad(float x) {
   return (x * M_PI / 180);
 }

/******************************************************************************
 * Private Functions
 *****************************************************************************/
 static void rexp(float w[3], float wexp[3][3]) {
   static float wnorm = 0, s = 0, c = 0;
   static float scrx[3][3],ssrxrx[3][3],temp[3][3],rx[3][3];
   static float i3[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
   wnorm = magnitude(w);
   rcross(w,rx);
   s = sinc(wnorm/2);
   c = cos(wnorm/2);
   Matrix3ScalarMultiply(s*c,rx,scrx);
   Matrix3Multiply(rx,rx,temp);
   Matrix3ScalarMultiply(s*s/2,temp,ssrxrx);
   Matrix3Add(scrx,ssrxrx,temp);
   Matrix3Add(i3,temp, wexp);
 }

 static void rcross(float r[3], float rx[3][3]) {
   rx[0][0] = 0;
   rx[0][1] = -r[2];
   rx[0][2] = r[1];
   rx[1][0] = r[2];
   rx[1][1] = 0;
   rx[1][2] = -r[0];
   rx[2][0] = -r[1];
   rx[2][1] = r[0];
   rx[2][2] = 0;
 }

 static void norm(float v[3], float vnorm[3]) {
   static float m = 0;
   m = magnitude(v);
   vnorm[0] = v[0]/m;
   vnorm[1] = v[1]/m;
   vnorm[2] = v[2]/m;
 }

 static float magnitude(float v[3]) {
   return sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
 }

 static float sinc(float x) {
   if ((x < 0.05) && (x > -0.05)) {
     return (1 - pow(x,2)/6 + pow(x,4)/120 - pow(x,6)/5040 + pow(x,8)/362880);
   } else {
     return (sin(x)/(x));
   }
 }

/******************************************************************************
 * Test Harness
 *****************************************************************************/
#ifdef TEST_IMU
#include <stdio.h>

int main(void){
  printf("\n---------- Unit Testing: IMU ----------\n");
  printf("\n  -------- Public Functions --------   \n");

  // --------------- IMU_integrateClosedLoop ---------------
  printf("\n--- IMU_integrateClosedLoop() ---\n");
  float gyro[3] = {0.5,1,-0.375};
  float acc[3]  = {0,0,-1};
  float mag[3]  = {1,0,0};
  float delta_t = 1;
  float accelInertial[3] = {0,0,-1};
  float magInertial[3] = {1,0,0};
  float bias_minus[3] = {0.1,0.1,0.1};
  float dcm_minus[3][3] = {{ 0.000000, -0.866025,  0.500000},{ 0.500000,  0.433012,  0.750000},{-0.866025,  0.250000,  0.433012}};
  float dcm_plus[3][3];
  float bias_plus[3];
  IMU_integrateClosedLoop(gyro, acc, mag, delta_t, accelInertial, magInertial, dcm_minus, bias_minus, dcm_plus, bias_plus);
  printf("Gyro:\tX:%f\tY:%f\tZ:%f\nAcc:\tX:%f\tY:%f\tZ:%f\nMag\tX:%f\tY:%f\tZ:%f\nDelta T: %f\n", gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2],mag[0],mag[1],mag[2],delta_t);
  printf("Acc Intertial:\tX:%f\tY:%f\tZ:%f\nMag Inerital:\tX:%f\tY:%f\tZ:%f\n", accelInertial[0],accelInertial[1],accelInertial[2],magInertial[0],magInertial[1],magInertial[2]);
  printf("DCM_minus: \n");
  Matrix3Print(dcm_minus);
  printf("DCM_plus: \n");
  Matrix3Print(dcm_plus);

  // --------------- IMU_integrateOpenLoop ---------------
  printf("\n--- IMU_integrateOpenLoop() ---\n");
  float gyro1[3] = {0.5,1,-0.375};
  float delta_t1 = 1;
  float dcm_old[3][3] = {{ 0.000000, -0.866025,  0.500000},{ 0.500000,  0.433012,  0.750000},{-0.866025,  0.250000,  0.433012}};
  float dcm_new[3][3];
  IMU_integrateOpenLoop(gyro1, delta_t1, dcm_old, dcm_new);
  printf("Gyro:\tX:%f Y:%f Z:%f\nDelta T: %f\n", gyro1[0],gyro1[1],gyro1[2], delta_t1);
  printf("DCM_minus: \n");
  Matrix3Print(dcm_old);
  printf("DCM_plus: \n");
  Matrix3Print(dcm_new);

  // --------------- IMU_dcm2euler() ---------------
  printf("\n--- IMU_dcm2euler() ---\n");
  float dcmIn[3][3] = {{ 0.000000, -0.866025,  0.500000},{ 0.500000,  0.433012,  0.750000},{-0.866025,  0.250000,  0.433012}};
  float eulerOut[3];
  IMU_dcm2euler(dcmIn,eulerOut);
  printf("DCM Input: \n");
  Matrix3Print(dcmIn);
  printf("Euler Output: \n");
  printf("Yaw: % .6f rad Pitch: % .6f rad Roll: % .6f rad\n", eulerOut[0],eulerOut[1],eulerOut[2]);

  // --------------- IMU_euler2dcm() ---------------
  printf("\n--- IMU_euler2dcm() ---\n");
  float eulerIn[3] = {0.9599311, 0.2617994, -0.7853982};
  float dcmOut[3][3];
  printf("Euler Input: \n");
  printf("Yaw: % .6f rad Pitch: % .6f rad Roll: % .6f rad\n", eulerIn[0],eulerIn[1],eulerIn[2]);
  IMU_euler2dcm(eulerIn, dcmOut);
  printf("\nDCM Output: \n");
  Matrix3Print(dcmOut);

  // --------------- IMU_corrAcc() ---------------
  printf("\n--- IMU_corrAcc() ---\n");
  float accVec[3] = {1,1,1};
  float accVecCorr[3];
  IMU_corrAcc(accVec, accVecCorr);
  printf("Input:\tX:%f Y:%f Z:%f\n", accVec[0],accVec[1],accVec[2]);
  printf("Output:\tX:%f Y:%f Z:%f\n", accVecCorr[0],accVecCorr[1],accVecCorr[2]);

  // --------------- IMU_corrMag() ---------------
  printf("\n--- IMU_corrMag() ---\n");
  float magVec[3] = {1,1,1};
  float magVecCorr[3];
  IMU_corrMag(magVec, magVecCorr);
  printf("Input:\tX:%f Y:%f Z:%f\n", magVec[0],magVec[1],magVec[2]);
  printf("Output:\tX:%f Y:%f Z:%f\n", magVecCorr[0],magVecCorr[1],magVecCorr[2]);

  // --------------- IMU_corrGyr() ---------------
  printf("\n--- IMU_corrGyr() ---\n");
  float gyroRates[3] = {100, 100, 100};
  float gyroRatesCorr[3];
  IMU_corrGyr(gyroRates, gyroRatesCorr);
  printf("Input:\tX:%f Y:%f Z:%f\n", gyroRates[0],gyroRates[1],gyroRates[2]);
  printf("Output:\tX:%f Y:%f Z:%f\n", gyroRatesCorr[0],gyroRatesCorr[1],gyroRatesCorr[2]);

  // --------------- IMU_alignMag2Acc() ---------------
  printf("\n--- IMU_alignMag2Acc() ---\n");
  float magVecMisaligned[3] = {1,1,1};
  float magVecAligned[3];
  IMU_alignMag2Acc(magVecMisaligned, magVecAligned);
  printf("Input:\tX:%f Y:%f Z:%f\n", magVecMisaligned[0],magVecMisaligned[1],magVecMisaligned[2]);
  printf("Output:\tX:%f Y:%f Z:%f\n", magVecAligned[0],magVecAligned[1],magVecAligned[2]);

  // --------------- IMU_rad2deg() ---------------
  printf("\n--- IMU_rad2deg() ---\n");
  printf("Input: 6.28318 rad\nExpected:\t360.000000 deg\nAcutal:\t\t%f deg\n",IMU_rad2deg(6.2831853));

  // --------------- IMU_deg2rad() ---------------
  printf("\n--- IMU_deg2rad() ---\n");
  printf("Input: 360 deg\nExpected:\t6.283185 rad\nAcutal:\t\t%f rad\n",IMU_deg2rad(360));

  printf("\n  -------- Private Functions -------- \n");

  // -------------- rexp() --------------
  printf("\n--- rexp() ---\n");
  float vec0[3] = {0.5,1,-0.375};
  float rexp_res[3][3];
  rexp(vec0, rexp_res);
  printf("Input:\tX:%f Y:%f Z:%f\n", vec0[0],vec0[1],vec0[2]);
  printf("\nOutput: \n");
  Matrix3Print(rexp_res);

  // -------------- rcross() --------------
  printf("\n--- rcross() ---\n");
  float vec1[3] = {0.5,-1.0,2.2};
  float rcross_res[3][3];
  rcross(vec1, rcross_res);
  printf("Input:\tX:%f Y:%f Z:%f\n", vec1[0],vec1[1],vec1[2]);
  printf("\nOutput: \n");
  Matrix3Print(rcross_res);

  // -------------- norm() ---------------
  printf("\n--- norm() ---\n");
  float vec2[3] = {0.5,-1.0,2.2};
  float vecNorm[3];
  norm(vec2, vecNorm);
  printf("Input:\tX:%f Y:%f Z:%f\n", vec2[0],vec2[1],vec2[2]);
  printf("Output:\tX:%f Y:%f Z:%f\n", vecNorm[0],vecNorm[1],vecNorm[2]);

  // -------------- mag() --------------
  printf("\n--- magnitude() ---\n");
  float vec3[3] = {1,-1.0,1};
  printf("Input:\tX:%f Y:%f Z:%f\n", vec3[0],vec3[1],vec3[2]);
  printf("Output: %f\n", magnitude(vec3));


  // -------------- sinc() ---------------
  printf("\n--- sinc() ---\n");
  int i = 0;
  float theta[21] = {-9.42477796076938, -8.48230016469244, -7.53982236861550,
                     -6.59734457253857, -5.65486677646163, -4.71238898038469,
                     -3.76991118430775, -2.82743338823081, -1.88495559215388,
                     -0.942477796076938, 0, 0.942477796076938, 1.88495559215388,
                      2.82743338823081, 3.76991118430775, 4.71238898038469,
                      5.65486677646163, 6.59734457253856, 7.53982236861550,
                      8.48230016469244, 9.42477796076938};

  float result[21] = {0.000000, 0.095377, 0.126137, 0.046839, -0.103943,
                     -0.212206, -0.155914, 0.109292, 0.504551, 0.858393,
                      1, 0.858393, 0.504, 0.109292, -0.155914, -0.212206,
                     -0.103943, 0.046839, 0.126137, 0.095377, 0.00000};

  printf("Expected:\tAcutal:\n");
  for(i = 0; i < 21; i++) {
    printf("% .6f\t% .6f\n", result[i], sinc(theta[i]));
  }
  printf("\n---------------------------------------\n");
}
#endif

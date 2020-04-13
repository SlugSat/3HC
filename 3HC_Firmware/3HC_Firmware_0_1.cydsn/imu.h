#ifndef IMU_H
#define IMU_H
/**
 * imu.h
 *
 * Caleb Cotter - 2019
 *
 * Library of functions for computing and storing 3D data from the IMU.
 *
 * Euler = [Yaw, Pitch, Roll] (ZYX)
 * DCM (ZYX)
 *
 */

/******************************************************************************
 * Datatypes
 *****************************************************************************/

/******************************************************************************
 * Public Functions
 *****************************************************************************/
 void IMU_integrateClosedLoop(float gyr[0], float mag[3], float acc[3], float deltaT, float magInertial[3], float accelInertial[3],
                              float dcm_minus[3][3], float bias_minus[3], float dcm_plus[3][3], float bias_plus[3]);

 void IMU_integrateOpenLoop(float rate[3], float dt, float dcm_minus[3][3], float dcm_plus[3][3]);

 void IMU_dcm2euler(float dcm[3][3], float euler[3]);

 void IMU_euler2dcm(float euler[3], float dcm[3][3]);

 void IMU_corrAcc(float acc[3], float acc_corr[3]);

 void IMU_corrMag(float mag[3], float mag_corr[3]);

 void IMU_corrGyr(float gyro[3], float gyr_corr[3]);

 void IMU_alignMag2Acc(float mag_corr[3], float mag_aligned[3]);

 float IMU_rad2deg(float x);

 float IMU_deg2rad(float x);

/******************************************************************************
 * Private Functions
 *****************************************************************************/
 static void rexp(float w[3], float wexp[3][3]);

 static void rcross(float r[3], float rx[3][3]);

 static void norm(float v[3], float vnorm[3]);

 static float magnitude(float v[3]);

 static float sinc(float x);

#endif

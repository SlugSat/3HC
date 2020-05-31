/* ========================================
 * FILE:   config.h
 * AUTHOR: Caleb Cotter
 *
 * CREATED ON MARCH 4, 2020, 12:10 AM
 *
 * ========================================
 *
 * Copyright SLUGSAT, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF SLUGSAT.
 *
 * ========================================
*/

/* Conversion Factors */
/*
static const float conversion[3] = {1,1,1};
*/

/* Electrical Parameters */
#define SYS_VOLTAGE 36.0
#define CURRENT_LIMIT_MAX  40.0
#define CURRENT_LIMIT_MIN -40.0

/* PID Loop Parameters */
#define X_KP 20.0
#define X_KI 1.0
#define X_KD 0.25

#define Y_KP 50.0
#define Y_KI 2.0
#define Y_KD 0.25

#define Z_KP 10.0
#define Z_KI 0.5
#define Z_KD 0.25

/* Current Sensor Calibration Values */
static const float currScaleMatrix[3][3] = {{-0.00116348067,  0.00000000000,  0.00000000000},
                                            { 0.00000000000, -0.00116348067,  0.00000000000},
                                            { 0.00000000000,  0.00000000000, -0.00116348067}};

static const float currZeroThreshold = 0.005;

/* Magnetometer Calibration Values */
static const float magScaleMatrix[3][3] =  {{ 0.0037, -0.0000, -0.0000},
                                            {-0.0001,  0.0039,  0.0000},
                                            {-0.0001,  0.0000,  0.0036}};
static const float magOffsetVector[3]   =   {-0.9859, -0.0477,  0.8850};

/* Magnetometer to Coil Frame Mislignment Matrix */
/*static float mag2coilMatrix[3][3] =  {{ 1, 0, 0},
                                      { 0, 1, 0},
                                      { 0, 0, 1}};
*/

/* [] END OF FILE */

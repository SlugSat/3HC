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

/* Current Sensor Calibration Values */
static const float currScaleMatrix[3][3] = {{-0.00116348067,  0.00000000000,  0.00000000000},
                                            { 0.00000000000, -0.00116348067,  0.00000000000},
                                            { 0.00000000000,  0.00000000000, -0.00116348067}};
static const float currOffsetVector[3]   =  {-33572, -33572, -33572};

/* Magnetometer Calibration Values */
static const float magScaleMatrix[3][3] =  {{ 0.0037, -0.0000, -0.0000},
                                            {-0.0001,  0.0039,  0.0000},
                                            {-0.0001,  0.0000,  0.0036}};
static const float magOffsetVector[3]   =   {-0.9859, -0.0477,  0.8850};

/* Magnetometer to Coil Frame Mislignment Matrix */
static float mag2coilMatrix[3][3] =  {{ 1, 0, 0},
                                      { 0, 1, 0},
                                      { 0, 0, 1}};

/* [] END OF FILE */

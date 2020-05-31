/* ========================================
 * FILE:   PID.h
 * AUTHOR: Caleb Cotter
 *
 * CREATED ON APRIL 14, 2020, 2:26 PM
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

#ifndef PID_H
#define PID_H

#include "project.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct PID_Obj* PID_Controller;
    
PID_Controller PID_Init(float kp, float ki, float kd);

float PID_Update(PID_Controller controller, float pv, float sp);

void PID_End(PID_Controller* controller);
    
#endif
/* [] END OF FILE */

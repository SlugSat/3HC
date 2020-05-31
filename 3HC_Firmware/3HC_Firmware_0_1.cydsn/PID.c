/* ========================================
 * FILE:   PID.c
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

#include "PID.h"

typedef struct PID_Obj{
    float kp,ki,kd;
    float prev_error;
    float i_error;
}PID_Obj;

PID_Controller PID_Init(float kp, float ki, float kd){
    PID_Controller c = malloc(sizeof(PID_Obj));
    
    c->kp = kp;
    c->ki = ki;
    c->kd = kd;
    c->prev_error = 0;
    c->i_error = 0;
    
    return c;
}

float PID_Update(PID_Controller c, float pv, float sp){
    float u, e;
    
    // Calculate Error
    e = sp-pv;
    
    // Integral Anti-Windup
    if(c->prev_error > 0 && e <= 0){
        c->i_error = 0;
    }
    
    // Compute Control
    c->i_error += e;
    u = c->kp*e + c->ki*c->i_error + c->kd*(e-c->prev_error);
    
    // Store Error
    c->prev_error = e;
    
    return u;
}

void PID_End(PID_Controller* controller){
    if(*controller != NULL && controller != NULL){
        free(*controller);
        *controller = 0;
    }
}

/* [] END OF FILE */

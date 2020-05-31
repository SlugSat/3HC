/* ========================================
 * FILE:   NMEA0183.h
 * AUTHOR: Caleb Cotter
 *
 * CREATED ON FEBRUARY 24, 2020, 1:25 PM
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

#ifndef NEMA0183_H
#define NEMA0183_H
    
#include "project.h"
#include <stdio.h>
#include <stdint.h>
    
#define MAX_MSG_LENGTH 64
#define MAX_ID_LENGTH 8
#define MAX_PAYLOAD_LENGTH 53
#define CHECKSUM_LENGTH 2
    
int NMEA0183_DecodeMsg(uint8 * msg, char * id, char * payload);

int NMEA0183_EncodeMsg(uint8 * msg, char * id, char * payload);

#endif
/* [] END OF FILE */

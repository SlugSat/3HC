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

void NMEA0183_Init();

int NMEA0183_DecodeMsg(char * message);

int NMEA0183_EncodeMsg(char * message);

#endif
/* [] END OF FILE */

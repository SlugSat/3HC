/* ========================================
 * FILE:   NMEA0183.c
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

#include "NMEA0183.h"

/******************************************************************************
 * Private Functions                                                          *  
 ******************************************************************************/

uint8_t StringChecksum(char * string) {

    //Local Variables
    int stringLength = strlen(string);
    int i;
    uint8_t checksum = 0x00;

    //Calculate One-Byte XOR Checksum
    for (i = 0; i < stringLength; i++) {
        checksum ^= string[i];
    }
    return checksum;
}

uint8_t AsciiToInt(char letter) {
    switch (letter) {
        case '0'...'9':
            return (letter - 48);
            break;
        case 'a'...'f':
            return (letter - 87);
            break;
        case 'A'...'F':
            return (letter - 55);
            break;
    }
    return -1;
}

/******************************************************************************
 * Public Functions                                                           *  
 ******************************************************************************/

void NMEA0183_Init();

int NMEA0183_DecodeMsg(char * message);

int NMEA0183_EncodeMsg(uint8 * msg, char * id, char * payload){
    char temp[MAX_MSG_LENGTH];
    uint8_t checksum;
    
    // Build Message and Calc Checksum
    sprintf(temp,"%s,%s", id, payload);
    checksum = StringChecksum(temp);
    
    // Add Checksum to Message
    sprintf(msg,"$%s*%2x\n",temp,checksum);
    
    return 0;
}

/* [] END OF FILE */

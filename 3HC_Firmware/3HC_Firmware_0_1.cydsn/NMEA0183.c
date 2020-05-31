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

int NMEA0183_DecodeMsg(uint8 * msg, char * id, char * payload){
    // Input Checking
    if(msg == NULL || id == NULL || payload == NULL){
        return -1;
    }
    
    // Local Variables
    uint8_t temp[MAX_MSG_LENGTH];
    uint8_t checksum;

    // Get Message Components
    if(sscanf((char *) msg,"$%[^,],%[^*]*%02X", id, payload, (unsigned int *) &checksum) != 3){
        return -1;
    }
    
    // Validate Checksum
    sprintf((char *) temp,"%s,%s",id,payload);
    if(checksum != StringChecksum((char *)temp)){
        sprintf(id,"ERR");
        return -1;
    }
    
    return 0;
}

int NMEA0183_EncodeMsg(uint8 * msg, char * id, char * payload){
    // Input Checking
    if(msg == NULL || id == NULL || payload == NULL){
        return -1;
    }
    
    // Local Variables
    uint8_t temp[MAX_MSG_LENGTH];
    uint8_t checksum;
    
    // Build Message and Calc Checksum
    sprintf((char *) temp,"%s,%s", id, payload);
    checksum = StringChecksum((char *)temp);
    
    // Add Checksum to Message
    sprintf((char *)msg,"$%s*%02X\n",temp,checksum);
    
    return 0;
}

/* [] END OF FILE */

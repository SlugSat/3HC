/* 
 * File:   I2C.h
 * Author: Caleb Cotter
 * Ported for PSoC5LP from Uno32 Platform Driver written by Max Dunne
 *
 * Created on February 12, 2020, 8:53 PM
 */

#include "project.h"
#include "I2C.h"

void I2C_Init(){
    I2C_Module_Start();
}

unsigned char I2C_ReadRegister(unsigned char I2CAddress, unsigned char deviceRegisterAddress){
    unsigned char status = 0;
    unsigned char byte = 0;

    /* Send start condition and slave address, followed by write bit */ 
    status = I2C_Module_MasterSendStart(I2CAddress, I2C_Module_WRITE_XFER_MODE);     
    
    /* Check if the master status is error free */
    if(status == I2C_Module_MSTR_NO_ERROR)
    {
        status = I2C_Module_MasterWriteByte(deviceRegisterAddress);
        
        /* Send start condition and slave address, followed by read bit */
        status = I2C_Module_MasterSendRestart(I2CAddress, I2C_Module_READ_XFER_MODE);

        /* Check if the master status is error free */
        if(status == I2C_Module_MSTR_NO_ERROR)
        {
            /* Load the read buffer with the data read from slave, followed by ACK */
            byte = I2C_Module_MasterReadByte(I2C_Module_ACK_DATA);
        }
    }

    /* Send stop condition after the transaction is completed */
    I2C_Module_MasterSendStop();
    
    return byte;
}

unsigned char I2C_WriteReg(unsigned char I2CAddress, unsigned char deviceRegisterAddress, char data){
    unsigned char status = 0;

    /* Send start condition and slave address, followed by write bit */
    status = I2C_Module_MasterSendStart(I2CAddress, I2C_Module_WRITE_XFER_MODE);     
    
    /* Check if the master status is error free */
    if(status == I2C_Module_MSTR_NO_ERROR)
    {
        status = I2C_Module_MasterWriteByte(deviceRegisterAddress);
        status = I2C_Module_MasterWriteByte(data);
    }
    /* Send stop condition after the transaction is completed */
    I2C_Module_MasterSendStop();
    
    return status;
}

int I2C_ReadInt(char I2CAddress, char deviceRegisterAddress, char isBigEndian){
    unsigned char status = 0;
    unsigned char upperByte = 0;
    unsigned char lowerByte = 0;
    int returnVal;
    
    /* Send start condition and slave address, followed by write bit */ 
    status = I2C_Module_MasterSendStart(I2CAddress, I2C_Module_WRITE_XFER_MODE);     

    /* Check if the master status is error free */
    if(status == I2C_Module_MSTR_NO_ERROR)
    {
        status = I2C_Module_MasterWriteByte(deviceRegisterAddress);
        
        /* Send start condition and slave address, followed by read bit */
        status = I2C_Module_MasterSendRestart(I2CAddress, I2C_Module_READ_XFER_MODE);

        /* Check if the master status is error free */
        if(status == I2C_Module_MSTR_NO_ERROR)
        {
            /* Load the read buffer with the data read from slave, followed by ACK */
            upperByte = I2C_Module_MasterReadByte(I2C_Module_ACK_DATA);
            lowerByte = I2C_Module_MasterReadByte(I2C_Module_ACK_DATA);
        }
    }

    /* Send stop condition after the transaction is completed */
    I2C_Module_MasterSendStop();
    
    if(isBigEndian){
        returnVal = ((upperByte << 8) | lowerByte);
    } else {
        returnVal = ((lowerByte << 8) | upperByte);
    }
    return returnVal;
}
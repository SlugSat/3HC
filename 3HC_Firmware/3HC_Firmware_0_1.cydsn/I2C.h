/* 
 * File:   I2C.h
 * Author: Max
 *
 * Created on February 22, 2018, 1:17 PM
 */

#ifndef I2C_H
#define	I2C_H

/**
 * @Function I2C_Init(Rate)
 * @param void
 * @return none
 * @brief  Initializes the I2C System for use with the IMU
 * @author Max Dunne */
void I2C_Init();

/**
 * @Function I2C_ReadRegister(unsigned char I2CAddress,unsigned char deviceRegisterAddress)
 * @param I2CAddresss - 7-bit address of I2C device wished to interact with
 * @param deviceRegisterAddress - 8-bit address of register on device
 * @return Value at Register or 0
 * @brief  Reads one device register on chosen I2C device
 * @author Max Dunne*/
unsigned char I2C_ReadRegister(unsigned char I2CAddress,unsigned char deviceRegisterAddress);

/**
 * @Function I2C_WriteReg(unsigned char I2CAddress, unsigned char deviceRegisterAddress, char data)
 * @param I2CAddresss - 7-bit address of I2C device wished to interact with
 * @param deviceRegisterAddress - 8-bit address of register on device
 * @param data - data wished to be written to register
 * @return 0 if error and 1 if success
 * @brief  Writes one device register on chosen I2C device
 * @author Max Dunne*/
unsigned char I2C_WriteReg(unsigned char I2CAddress, unsigned char deviceRegisterAddress, char data);


/**
 * @Function I2C_ReadInt(char I2CAddress, char deviceRegisterAddress, char isBigEndian)
 * @param I2CAddresss - 7-bit address of I2C device wished to interact with
 * @param deviceRegisterAddress - 8-bit lower address of register on device
 * @param isBigEndian - Boolean determining if device is big or little endian
 * @return 0 if error and 1 if success
 * @brief  Reads two sequential registers to build a 16-bit value. isBigEndian
 * whether the first bits are either the high or low bits of the value
 * @author Max Dunne*/
int I2C_ReadInt(char I2CAddress, char deviceRegisterAddress, char isBigEndian);

#endif	/* I2C_H */


/* 
 * File:   MPU9250.h
 * Author: Max
 * Modified by: Caleb Cotter
 * Edited to work on PSoC5 with I2C Driver Wrapper
 *
 * Created on February 27, 2018, 8:14 AM
 */

#ifndef MPU9250_H
#define	MPU9250_H

/**
 * @Function MPU9250_Init(Rate)

 * @return 0 if error, 1 if succeeded
 * @brief  Initializes the MPU 9250 for usage. This will set all sensors to approximately 100HZ
 * and Accel: 2g,Gyro:  250dps and Mag: 16-bit for the sensors
 * @author Max Dunne */
char MPU9250_Init(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadAccelX(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadAccelY(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadAccelZ(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadGyroX(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadGyroY(void);


/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadGyroZ(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadMagX(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadMagY(void);

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadMagZ(void);



#endif	/* MPU9250_H */
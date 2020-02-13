#include "MPU9250.h"
#include "I2C.h"

#ifdef MPU9250_TEST
#include <stdio.h>
#endif

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define TRUE 1
#define FALSE 0

//#define DEBUG_MPU_CHANNEL

#ifdef DEBUG_MPU_CHANNEL
#define dbprintf(...) printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

#define MAG_I2C_ADDRESS 0x0C
#define ACCGYR_I2C_ADDRESS 0x68

#define MAG_WHOAMI_VALUE 0x48
#define MPU_WHOAMI_VALUE 0x71

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

static enum {
    AK_WHOAMI_REGISTER,
    AK_HXL = 0x03,
    AK_HXH,
    AK_HYL,
    AK_HYH,
    AK_HZL,
    AK_HZH,
    AK_ST2 = 0x9,
    AK_CNTL1 = 0xA,
    AK_CNTL2
} AK8963_REGISTERS;

static enum {
    MPU_SMPLRT_DIV = 25,
    MPU_CONFIG,
    MPU_GYRO_CONFIG,
    MPU_ACCEL_CONFIG,
    MPU_ACCEL_CONFIG2,
    MPU_INT_BYPASS_CONFIG = 55,
    MPU_INT_CONFIG,
    ACCEL_XOUT_H = 59,
    ACCEL_XOUT_L,
    ACCEL_YOUT_H,
    ACCEL_YOUT_L,
    ACCEL_ZOUT_H,
    ACCEL_ZOUT_L,
    GYRO_XOUT_H = 67,
    GYRO_XOUT_L,
    GYRO_YOUT_H,
    GYRO_YOUT_L,
    GYRO_ZOUT_H,
    GYRO_ZOUT_L,
    MPU_POWER_CTRL1 = 107,
    MPU_WHOAMI_REGISTER = 117
} MPU9250_REGISTERS;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function MPU9250_Init(Rate)

 * @return 0 if error, 1 if succeeded
 * @brief  Initializes the MPU 9250 for usage. This will set all sensors to approximately 100HZ
 * and Accel: 2g,Gyro:  250dps and Mag: 16-bit for the sensors
 * @author Max Dunne */
char MPU9250_Init(void) {
    unsigned int intReturn;
    unsigned char byteReturn;

    I2C_Init();

    // we have to configure the MPU first due to idiocy with the I2C setup internally
    byteReturn = I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_WHOAMI_REGISTER);
    //    printf("\r\n%X", byteReturn);
    if (byteReturn != MPU_WHOAMI_VALUE) {
        return FALSE; // make sure we are talking to the right device first
    }

    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_POWER_CTRL1, 0x01); // turn it on and hopefully use pll

    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_CONFIG, 0x03); // don't really want a dlp but turning it off is non trivial
    // setting it to fastest bandwidth for the gyro
    //byteReturn = I2C_ReadRegister(ACCGYR_I2C_ADDRESS,MPU_GYRO_CONFIG);
    //printf("\r\ngyro config: %d",byteReturn);
    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_GYRO_CONFIG, 0x00); //set scale on gyro to 250 degrees /s
    dbprintf("\r\nGYRO CONFIG Set To: 0X%X", I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_GYRO_CONFIG));
    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_SMPLRT_DIV, 5); // set the output data rate to 100Hz
    dbprintf("\r\nSMART RATE Set To: 0X%X", I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_SMPLRT_DIV));

    // we now can configure the accelerometer

    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_ACCEL_CONFIG2, 0b1001);
    //sets the bandwidth to 184 and internal to 1K, actually output controlled by smart divider
    dbprintf("\r\nACCEL CONFIG2 Set To: 0X%X", I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_ACCEL_CONFIG2));

    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_ACCEL_CONFIG, 0);
    dbprintf("\r\nACCEL CONFIG Set To: 0X%X", I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_ACCEL_CONFIG));

    // finally we configure the I2C to allow us to talk to the mag
    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_INT_BYPASS_CONFIG, 0x22);
    dbprintf("\r\nBypass CONFIG Set To: 0X%X", I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_INT_BYPASS_CONFIG));


    byteReturn = I2C_ReadRegister(MAG_I2C_ADDRESS, AK_WHOAMI_REGISTER);
    //    printf("\r\n%X", byteReturn);
    if (byteReturn != MAG_WHOAMI_VALUE) {
        return FALSE; // make sure we are talking to the right device first
    }

    //configure output rate and scale
    //    I2C_WriteReg(MAG_I2C_ADDRESS,AK_CNTL1,0b0);
    //    I2C_WriteReg(MAG_I2C_ADDRESS,AK_CNTL1,0b1111);
    //    I2C_WriteReg(MAG_I2C_ADDRESS,AK_CNTL1,0b0);
    I2C_WriteReg(MAG_I2C_ADDRESS, AK_CNTL1, 0b10110);
    dbprintf("\r\nMAG CNTL1 CONFIG Set To: 0X%X", I2C_ReadRegister(MAG_I2C_ADDRESS, AK_CNTL1));
    return TRUE;
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadAccelX(void) {
    return I2C_ReadInt(ACCGYR_I2C_ADDRESS, ACCEL_XOUT_H, TRUE);
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadAccelY(void) {
    return I2C_ReadInt(ACCGYR_I2C_ADDRESS, ACCEL_YOUT_H, TRUE);
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadAccelZ(void) {
    return I2C_ReadInt(ACCGYR_I2C_ADDRESS, ACCEL_ZOUT_H, TRUE);
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadGyroX(void) {
    return I2C_ReadInt(ACCGYR_I2C_ADDRESS, GYRO_XOUT_H, TRUE);
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadGyroY(void) {
    return I2C_ReadInt(ACCGYR_I2C_ADDRESS, GYRO_YOUT_H, TRUE);
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadGyroZ(void) {
    return I2C_ReadInt(ACCGYR_I2C_ADDRESS, GYRO_ZOUT_H, TRUE);
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadMagX(void) {
    short SensorValue = I2C_ReadInt(MAG_I2C_ADDRESS, AK_HXL, FALSE);
    I2C_ReadRegister(MAG_I2C_ADDRESS, AK_ST2);
    return SensorValue;
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadMagY(void) {
    short SensorValue = I2C_ReadInt(MAG_I2C_ADDRESS, AK_HYL, FALSE);
    I2C_ReadRegister(MAG_I2C_ADDRESS, AK_ST2);
    return SensorValue;
}

/**
 * @Function MPU9250_Read*()
 * @param None
 * @return Returns raw sensor reading
 * @brief reads sensor axis as given by name
 * @author Max Dunne*/
int MPU9250_ReadMagZ(void) {
    short SensorValue = I2C_ReadInt(MAG_I2C_ADDRESS, AK_HZL, FALSE);
    I2C_ReadRegister(MAG_I2C_ADDRESS, AK_ST2);
    return SensorValue;
}

#ifdef MPU9250_TEST

#include <stdio.h>

#include "serial.h"
#include <math.h>

int main(void) {
    BOARD_Init();

    int aX, aY, aZ, gX, gY, gZ, mX, mY, mZ;
    printf("\r\nWelcome to mpu9250 test, system will be inited and configured and then data printed");
    if (!MPU9250_Init()) {
        printf("\r\nSensor failed to come up");
        while (1);
    }
    printf("\r\nSensor is up");
    //    while(1);
    while (1) {
        if (IsTransmitEmpty()) {
            aX = MPU9250_ReadAccelX();
            aY = MPU9250_ReadAccelY();
            aZ = MPU9250_ReadAccelZ();
            gX = MPU9250_ReadGyroX();
            gY = MPU9250_ReadGyroY();
            gZ = MPU9250_ReadGyroZ();
            mX = MPU9250_ReadMagX();
            mY = MPU9250_ReadMagY();
            mZ = MPU9250_ReadMagZ();
            printf("\r\n(X,Y,Z)Accel : (%d,%d,%d)Gyro : (%d,%d,%d)Mag: (%d,%d,%d)", aX, aY, aZ, gX, gY, gZ, mX, mY, mZ);
            mX = 0;
        }
    }

    while (1);
    return 1;
}

#endif
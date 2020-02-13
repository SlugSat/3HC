#include "project.h"
#include "I2C.h"
#include <stdio.h>

#define HIGH_PRIORITY       (1u)
#define DEFAULT_PRIORITY    (3u)
#define LOW_PRIORITY        (7u)

// Function Prototypes
CY_ISR_PROTO(UPDATE_UI);

// Global Variables
char LCD_MSG[2][16];
unsigned char update = 1;
unsigned char readVal = 0;

// IMU Definitions
#define MAG_I2C_ADDRESS 0x0C
#define ACCGYR_I2C_ADDRESS 0x68

#define MAG_WHOAMI_VALUE 0x48
#define MPU_WHOAMI_VALUE 0x71

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


int main(void)
{   
    I2C_Init();
    
    I2C_WriteReg(ACCGYR_I2C_ADDRESS, MPU_POWER_CTRL1, 0x01);
    readVal = I2C_ReadRegister(ACCGYR_I2C_ADDRESS, MPU_POWER_CTRL1);
    
    sprintf(LCD_MSG[0],"Wrote %x", 0x01);
    sprintf(LCD_MSG[1],"Read  %x", readVal);
    
    LCD_Start();
    // Update UI if Flag is Set
    if(update == 1){
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString(LCD_MSG[0]);
        LCD_Position(1,0);
        LCD_PrintString(LCD_MSG[1]);
        update = 0;
    }
}
    /*
    // Variables
    int count;
    
    // Initialize Hardware
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    
    // Configure Magnetometer
    I2C_Start();
    
    
    
    // Configure Interrupts
    UPDATE_UI_StartEx(UPDATE_UI);
    UPDATE_UI_SetPriority(LOW_PRIORITY);
    CyGlobalIntEnable;
    
    

    // Main Program Loop
    for(;;)
    {   
        I2C_MasterSendStart(ACCGYR_I2C_ADDRESS, 1);
        I2C_MasterReadBuf(ACCGYR_I2C_ADDRESS, &byteReturn, 1, I2C_MODE_COMPLETE_XFER);
        
        count = QuadDec_GetCounter();
        //sprintf(LCD_MSG,"%i %i", count, Encoder_Button_Read());
        sprintf(LCD_MSG,"I2C Return %x", byteReturn);
        // Update UI if Flag is Set
        if(update == 1){
            LCD_ClearDisplay();
            LCD_PrintString(LCD_MSG);
            update = 0;
        }
        
    }
}
*/

CY_ISR(UPDATE_UI){
    update = 1;
}

/* [] END OF FILE */

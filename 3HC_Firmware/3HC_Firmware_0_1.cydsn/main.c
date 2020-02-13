/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

#define HIGH_PRIORITY       (1u)
#define DEFAULT_PRIORITY    (3u)
#define LOW_PRIORITY        (7u)

// Function Prototypes
CY_ISR_PROTO(UPDATE_UI);

// Global Variables
char LCD_MSG[32];
unsigned char update = 1;
uint8_t byteReturn;


// Magnetometer Defines
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


int main(void)
{
    
    #define SLAVE_ADDRESS 0x68u
    #define SUB_ADDRESS_LOCATION 117
    
    uint8 i = 0;
    uint8 status = 0;

    I2C_Start();
    /* Initialize the read buffer */
    uint8 read_data_value = 0;

    /* Send start condition and slave address, followed by write bit */ 
    status = I2C_MasterSendStart(SLAVE_ADDRESS, I2C_WRITE_XFER_MODE);     
    
    /* Check if the master status is error free */
    if(status == I2C_MSTR_NO_ERROR)
    {
        status = I2C_MasterWriteByte(SUB_ADDRESS_LOCATION);
        
        /* Send start condition and slave address, followed by read bit */
        status = I2C_MasterSendRestart(SLAVE_ADDRESS, I2C_READ_XFER_MODE);

        /* Check if the master status is error free */
        if(status == I2C_MSTR_NO_ERROR)
        {
            /* Load the read buffer with the data read from slave, followed by ACK */
            read_data_value = I2C_MasterReadByte(I2C_ACK_DATA);
        }
        CyDelay(1);
    }

    /* Send stop condition after the transaction is completed */
    I2C_MasterSendStop();
    
    LCD_Start();
    sprintf(LCD_MSG,"%x", read_data_value);
    // Update UI if Flag is Set
    if(update == 1){
        LCD_ClearDisplay();
        LCD_PrintString(LCD_MSG);
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

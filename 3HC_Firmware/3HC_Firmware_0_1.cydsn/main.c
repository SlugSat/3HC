#include "project.h"
#include "MPU9250.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define HIGH_PRIORITY       (1u)
#define DEFAULT_PRIORITY    (3u)
#define LOW_PRIORITY        (7u)

// Function Prototypes
CY_ISR_PROTO(UPDATE_UI);

// Global Variables
char LCD_MSG[2][16];
unsigned char update = 1;
int count;


int main(void)
{   
    // Initialize Hardware
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    MPU9250_Init();
    
    // Configure Interrupts
    UPDATE_UI_StartEx(UPDATE_UI);
    UPDATE_UI_SetPriority(LOW_PRIORITY);
    CyGlobalIntEnable;

    // Main Program Loop
    for(;;)
    {   
        // --- Read in I/O and Sensor Data to Program Variables ---
        // IMU
        
        // Current Sensors
        
        // UI Controls       
        count = QuadDec_GetCounter();
        
        
        // --- Processing ---
        
        
        
        
        
        
        // --- Communications to Host ---

        
        
        
        // --- Update UI ---
        if(update == 1){
            
            sprintf(LCD_MSG[0],"X: %d uT", MPU9250_ReadMagX());
            sprintf(LCD_MSG[1],"Y: %d uT", MPU9250_ReadMagY());
            
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString(LCD_MSG[0]);
            LCD_Position(1,0);
            LCD_PrintString(LCD_MSG[1]);
            update = 0;
        }
    }
}

CY_ISR(UPDATE_UI){
    update = 1;
     
}

/* [] END OF FILE */

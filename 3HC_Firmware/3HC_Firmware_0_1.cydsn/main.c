#include "project.h"
#include "MPU9250.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define USBUART_BUFFER_SIZE (64u)

#define HIGH_PRIORITY       (1u)
#define DEFAULT_PRIORITY    (3u)
#define LOW_PRIORITY        (7u)

// Function Prototypes
CY_ISR_PROTO(UPDATE_UI);

// Global Variables
char LCD_MSG[2][16];
uint8 buffer[USBUART_BUFFER_SIZE];
unsigned char update = 1;
int count;
int currentRaw;
float xCurr;


int main(void)
{   
    // Initialize Hardware
    USBUART_Start(USBUART_device, USBUART_5V_OPERATION);
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    MPU9250_Init();
    
    X_PWM_Start();
    ADC_Start();
    
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
        
        ADC_StartConvert();
        /* Wait until the ADC conversion is complete */
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        /* Since the ADC conversion is complete, stop the conversion before
         * switching the mux */
        ADC_StopConvert();
        currentRaw = ADC_GetResult32();
        xCurr = (currentRaw - 32768)/32768; 
        
        // --- Processing ---
        X_PWM_WriteCompare(count % 4096);
        
        
        
        
        // --- Communications to Host ---
        
        
        
        
        // --- Update UI ---
        if(update == 1){
            
            //sprintf(LCD_MSG[0],"X: %d uT", MPU9250_ReadMagX());
            //sprintf(LCD_MSG[1],"Y: %d uT", MPU9250_ReadMagY());
            sprintf(LCD_MSG[0],"I: %i", currentRaw);
            sprintf(LCD_MSG[1],"Duty: %i", (count % 4095));
            
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

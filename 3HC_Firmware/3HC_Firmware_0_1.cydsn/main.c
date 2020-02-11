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

int main(void)
{
    // Variables
    int count;
    
    // Initialize Hardware
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    
    // Configure Interrupts
    UPDATE_UI_StartEx(UPDATE_UI);
    UPDATE_UI_SetPriority(LOW_PRIORITY);
    CyGlobalIntEnable;
    
    // Main Program Loop
    for(;;)
    {   
        count = QuadDec_GetCounter();
        //sprintf(LCD_MSG,"%i %i", count, Encoder_Button_Read());
        sprintf(LCD_MSG,"Dammit, that was a missed opportunity");
        
        // Update UI if Flag is Set
        if(update == 1){
            LCD_ClearDisplay();
            LCD_PrintString(LCD_MSG);
            update = 0;
        }
        
    }
}

CY_ISR(UPDATE_UI){
    update = 1;
}

/* [] END OF FILE */

/* ========================================
 * FILE:   main.c
 * AUTHOR: Caleb Cotter
 * 
 * DESCRIPTION: MAIN FILE FOR 3-AXIS
 * HELMHOLTZ COIL CONTROLLER FIRMWARE
 *
 * CREATED ON FEBRUARY 24, 2020, 2:01 PM
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

#include "project.h"
#include "MPU9250.h"
#include "config.h"
#include "NMEA0183.h"
#include <stdio.h>

#define versionStr "V0.1"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

// Function Prototypes
#define HIGH_PRIORITY       (1u)
#define DEFAULT_PRIORITY    (3u)
#define LOW_PRIORITY        (7u)

CY_ISR_PROTO(UPDATE_UI);
CY_ISR_PROTO(BUTTON_RISING_EDGE);
CY_ISR_PROTO(UPDATE_PID);
CY_ISR_PROTO(SAMPLE_CURRSENSOR);

// Datatypes
typedef enum bool{
    FALSE = 0,
    TRUE = 1
}bool;

typedef enum axis{
    X_STATE = 0,
    Y_STATE,
    Z_STATE
}axis;

typedef enum ui_mode{
    SPLASH_SCREEN = 0,
    X_DISPLAY,
    Y_DISPLAY,
    Z_DISPLAY,
    MAG_DISPLAY_XY,
    MAG_DISPLAY_Z,
    SYS_DEBUG
}ui_mode;

//--- Global Variables ---
// UI
char LCD_MSG[2][16];
unsigned char update = 1;
int count;
bool buttonClicked;
char strbuf[64];

// State Variables
ui_mode ui_state = SPLASH_SCREEN;
axis currReadState = X_STATE;
float currCoil[3];
float magField[3];

// Comms Variables
#define USBFS_DEVICE  (0u)
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)
#define USBUART_BUFFER_SIZE (64u)
uint16 length;
uint8 USB_Buff_IN[USBUART_BUFFER_SIZE];
uint8 USB_Buff_OUT[USBUART_BUFFER_SIZE];
char i = 0;

// --- Main Program ---
int main(void)
{   
    // --- Initialize Hardware ---
    // Comms
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    
    // UI
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    
    // Sensors
    ///MPU9250_Init();
    ADC_Start();
    AMux_Start();
    AMux_Select(X_STATE);
    ADC_StartConvert();
    
    // Outputs
    X_PWM_Start();
    Y_PWM_Start();
    Z_PWM_Start();
    XYZ_Direction_Write(0b010);
    
    // Configure Interrupts
    UPDATE_UI_StartEx(UPDATE_UI);
    UPDATE_UI_SetPriority(LOW_PRIORITY);
    BUTTON_RISING_EDGE_StartEx(BUTTON_RISING_EDGE);
    BUTTON_RISING_EDGE_SetPriority(LOW_PRIORITY);
    PID_ISR_StartEx(UPDATE_PID);
    PID_ISR_SetPriority(HIGH_PRIORITY);
    ADC_SAMPLECURRSENSOR_StartEx(SAMPLE_CURRSENSOR);
    ADC_SAMPLECURRSENSOR_SetPriority(HIGH_PRIORITY);
    CyGlobalIntEnable;

    // --- Main Loop ---
    for(;;)
    {   
        // --- Read in I/O and Sensor Data to Program Variables ---        
        // Read UI Knob      
        count = QuadDec_GetCounter();
        
        // Read Magnetic Field
        // magField[X_AXIS] = MPU9250_ReadMagX()*1.66666;
        // magField[Y_AXIS] = MPU9250_ReadMagY()*1.66666;
        // magField[Z_AXIS] = MPU9250_ReadMagZ()*1.66666;
        
        // Read Current on Each
        // Done in ISR

        // --- Processing ---
        X_PWM_WriteCompare(count % 4096);
        Y_PWM_WriteCompare(count % 4096);
        Z_PWM_WriteCompare(count % 4096);
        
        // --- Communications to Host ---
        if (0u != USBFS_IsConfigurationChanged())
        {
            if (0u != USBFS_GetConfiguration())
            {
                USBFS_EnableOutEP(OUT_EP_NUM);
            }
        }
        if (USBFS_OUT_BUFFER_FULL == USBFS_GetEPState(OUT_EP_NUM))
        {
            length = USBFS_GetEPCount(OUT_EP_NUM);
            USBFS_ReadOutEP(OUT_EP_NUM, USB_Buff_OUT, length);
            while (USBFS_OUT_BUFFER_FULL == USBFS_GetEPState(OUT_EP_NUM))
            {
            }
            USBFS_EnableOutEP(OUT_EP_NUM);
        }
        while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN_EP_NUM));
        sprintf(strbuf,"%f",(float) count);
        NMEA0183_EncodeMsg(USB_Buff_IN, "XCUR", strbuf); 
        //sprintf((char *) USB_Buff_IN,"$XCUR,%f*2f\n", (float) count);
        USBFS_LoadInEP(IN_EP_NUM, USB_Buff_IN, USBUART_BUFFER_SIZE);

        
        // --- Update UI ---
        if(update == 1){
            
            switch(ui_state){
                case SPLASH_SCREEN:
                    sprintf(LCD_MSG[0],"3-AXIS HHC");
                    sprintf(LCD_MSG[1],"%s",versionStr);
                    if(buttonClicked == TRUE){
                        ui_state = SYS_DEBUG;
                        buttonClicked = FALSE;
                    }
                    break;
                case X_DISPLAY:
                    sprintf(LCD_MSG[0],"X I: %.3f", currCoil[X_AXIS]);
                    sprintf(LCD_MSG[1],"  Duty: %3.3f%%", (float)(X_PWM_ReadCompare() % X_PWM_ReadPeriod())/X_PWM_ReadPeriod()*100);
                    if(buttonClicked == TRUE){
                        ui_state = Y_DISPLAY;
                        buttonClicked = FALSE;
                    }
                    break;
                case Y_DISPLAY:
                    sprintf(LCD_MSG[0],"Y I: %.3f", currCoil[Y_AXIS]);
                    sprintf(LCD_MSG[1],"  Duty: %3.3f%%", (float)(Y_PWM_ReadCompare() % Y_PWM_ReadPeriod())/Y_PWM_ReadPeriod()*100);
                    if(buttonClicked == TRUE){
                        ui_state = Z_DISPLAY;
                        buttonClicked = FALSE;
                    }
                    break;
                case Z_DISPLAY:
                    sprintf(LCD_MSG[0],"Z I: %.3f", currCoil[Z_AXIS]);
                    sprintf(LCD_MSG[1],"  Duty: %3.3f%%", (float)(Z_PWM_ReadCompare() % Z_PWM_ReadPeriod())/Z_PWM_ReadPeriod()*100);
                    if(buttonClicked == TRUE){
                        ui_state = MAG_DISPLAY_XY;
                        buttonClicked = FALSE;
                    }
                    break;
                case MAG_DISPLAY_XY:
                    sprintf(LCD_MSG[0],"X: %.3f", magField[X_AXIS]);
                    sprintf(LCD_MSG[1],"Y: %.3f", magField[Y_AXIS]);
                    if(buttonClicked == TRUE){
                        ui_state = MAG_DISPLAY_Z;
                        buttonClicked = FALSE;
                    }
                    break;
                case MAG_DISPLAY_Z:
                    sprintf(LCD_MSG[0],"Z: %.3f", magField[Z_AXIS]);
                    sprintf(LCD_MSG[1],"");
                    if(buttonClicked == TRUE){
                        ui_state = SYS_DEBUG;
                        buttonClicked = FALSE;
                    }
                    break;
                case SYS_DEBUG:
                    sprintf(LCD_MSG[0],"T %s",USB_Buff_IN);
                    sprintf(LCD_MSG[1],"R %s",USB_Buff_OUT);
                    if(buttonClicked == TRUE){
                        ui_state = X_DISPLAY;
                        buttonClicked = FALSE;
                    }
                    break;
                default:
                    break;
            }
            
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString(LCD_MSG[0]);
            LCD_Position(1,0);
            LCD_PrintString(LCD_MSG[1]);
            update = 0;
        }
    }
    // --- End Main Loop ---
}
// --- End Main Program ---

// --- Interrupt Service Routines ---
CY_ISR(UPDATE_UI){
    update = 1;
}

CY_ISR(BUTTON_RISING_EDGE){
    buttonClicked = TRUE;
    BUTTON_RISING_EDGE_ClearPending();
}

CY_ISR(SAMPLE_CURRSENSOR){
     switch(currReadState){
        case X_STATE:
            currCoil[X_AXIS] = (ADC_GetResult32() + (currOffsetVector[0]))*currScaleMatrix[0][0];
            AMux_Select(Y_STATE);
            currReadState = Y_STATE;
            break;
        case Y_STATE:
            currCoil[Y_AXIS] = (ADC_GetResult32() + (currOffsetVector[1]))*currScaleMatrix[1][1];
            AMux_Select(Z_STATE);
            currReadState = Z_STATE;
            break;
        case Z_STATE:
            currCoil[Z_AXIS] = (ADC_GetResult32() + (currOffsetVector[2]))*currScaleMatrix[2][2];
            AMux_Select(X_STATE);
            currReadState = X_STATE;
            break;
        default:
            break;
    }
    ADC_StartConvert();
}

CY_ISR(UPDATE_PID){
    
}

/* [] END OF FILE */

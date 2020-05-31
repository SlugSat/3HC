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
#include "PID.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define versionStr "V0.2"

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
    SYS_MODE,
    X_DISPLAY,
    Y_DISPLAY,
    Z_DISPLAY,
    MAG_DISPLAY_XY,
    MAG_DISPLAY_Z,
    SYS_DEBUG
}ui_mode;

typedef enum sys_mode{
    IDLE,
    RUN
}sys_mode;

typedef enum fault_code{
    OVERCURRENT,
    IMU_NOT_DETECTED
}fault_code;
//--- Global Variables ---
// UI
char LCD_MSG[2][16];
unsigned char update = 1;
int count;
bool buttonClicked;
char strbuf[64];

// State Variables
ui_mode ui_state = SPLASH_SCREEN;
sys_mode sys_state = IDLE;
axis currReadState = X_STATE;
float sysPower = 0;
float fieldSet[3] = {0};
float currCoil[3] = {0};
int currCoilRaw[3] = {0};
float magField[3] = {0};
PID_Controller axisController[3];
int retVal = 0;

// Calibration Variables
float currOffsetVector[3] = {0};

// Comms Variables
#define USBFS_DEVICE  (0u)
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)
#define USBUART_BUFFER_SIZE (64u)
uint16 length;
uint8 USB_Buff_IN[USBUART_BUFFER_SIZE];
uint8 USB_Buff_OUT[USBUART_BUFFER_SIZE];
char id[MAX_ID_LENGTH];
char payload[MAX_PAYLOAD_LENGTH];
char i = 0;

// --- Main Program ---
int main(void)
{   
    // --- Initialize Hardware ---
    // Comms
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    sprintf(strbuf,"%f",fieldSet[X_AXIS]);
    NMEA0183_EncodeMsg(USB_Buff_IN, "XSET", strbuf);
    
    // UI
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    LCD_Backlight_Write(0b100);
    ENC_Backlight_Write(0b100);
    
    // Sensors
    MPU9250_Init();
    ADC_Start();
    AMux_Start();
    AMux_Select(X_STATE);
    ADC_StartConvert();
    
    // Outputs
    X_PWM_Start();
    Y_PWM_Start();
    Z_PWM_Start();

    // PID Controllers
    axisController[X_AXIS] = PID_Init(X_KP,X_KI,X_KD);
    axisController[Y_AXIS] = PID_Init(Y_KP,Y_KI,Y_KD);
    axisController[Z_AXIS] = PID_Init(Z_KP,Z_KI,Z_KD);
    
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
    
    // --- Startup Calibration ---
    // Current Sensors
    // Shutoff Outputs
    X_PWM_WriteCompare(0);
    Y_PWM_WriteCompare(0);
    Z_PWM_WriteCompare(0);
    XYZ_Direction_Write(0b000);
    CyDelay(500);
    currOffsetVector[X_AXIS] = (float) -currCoilRaw[X_AXIS];
    currOffsetVector[Y_AXIS] = (float) -currCoilRaw[Y_AXIS];
    currOffsetVector[Z_AXIS] = (float) -currCoilRaw[Z_AXIS];
    
    PID_Timer_Start();
    
    // --- Main Loop ---
    for(;;)
    {   
        // --- Read in I/O and Sensor Data to Program Variables ---        
        // Read UI Knob      
        count = QuadDec_GetCounter();
        
        // Read Magnetic Field
        magField[X_AXIS] = MPU9250_ReadMagX()*1.66666;
        magField[Y_AXIS] = MPU9250_ReadMagY()*1.66666;
        magField[Z_AXIS] = MPU9250_ReadMagZ()*1.66666;
        
        // Compute System Power
        sysPower = SYS_VOLTAGE*(fabs(currCoil[X_AXIS])+fabs(currCoil[Y_AXIS])+fabs(currCoil[Z_AXIS]));
        
        //-------------------------------------------------------------------
        // --- Communications to Host ---
        if (0u != USBFS_IsConfigurationChanged())
        {
            if (0u != USBFS_GetConfiguration())
            {
                USBFS_EnableOutEP(OUT_EP_NUM);
            }
        }
        
        // Read from Host
        if (USBFS_OUT_BUFFER_FULL == USBFS_GetEPState(OUT_EP_NUM))
        {
            length = USBFS_GetEPCount(OUT_EP_NUM);
            USBFS_ReadOutEP(OUT_EP_NUM, USB_Buff_OUT, length);
            while (USBFS_OUT_BUFFER_FULL == USBFS_GetEPState(OUT_EP_NUM))
            {
            }
            USBFS_EnableOutEP(OUT_EP_NUM);
            retVal = NMEA0183_DecodeMsg(USB_Buff_OUT, id, payload);
        }
        
        // --- Processing ---
        if(strcmp(id,"SETX") == 0){
            fieldSet[X_AXIS] = atof(payload);
        }
        else if(strcmp(id,"SETY") == 0){
            fieldSet[Y_AXIS] = atof(payload);
        }
        else if(strcmp(id,"SETZ") == 0){
            fieldSet[Z_AXIS] = atof(payload);
        }
        else if(strcmp(id,"READ") == 0){
            if(strcmp(payload,"XCUR") == 0){
                sprintf(strbuf,"%f",currCoil[X_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "XCUR", strbuf);
            }
            else if(strcmp(payload,"YCUR") == 0){
                sprintf(strbuf,"%f",currCoil[Y_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "YCUR", strbuf);
            }
            else if(strcmp(payload,"ZCUR") == 0){
                sprintf(strbuf,"%f",currCoil[Z_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "ZCUR", strbuf);
            }
            else if(strcmp(payload,"XMAG") == 0){
                sprintf(strbuf,"%f",magField[X_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "XMAG", strbuf);
            }
            else if(strcmp(payload,"YMAG") == 0){
                sprintf(strbuf,"%f",magField[Y_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "YMAG", strbuf);
            }
            else if(strcmp(payload,"ZMAG") == 0){
                sprintf(strbuf,"%f",magField[Z_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "ZMAG", strbuf);
            }
            else if(strcmp(payload,"XSET") == 0){
                sprintf(strbuf,"%f",fieldSet[X_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "XSET", strbuf);
            }
            else if(strcmp(payload,"YSET") == 0){
                sprintf(strbuf,"%f",fieldSet[Y_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "YSET", strbuf);
            }
            else if(strcmp(payload,"ZSET") == 0){
                sprintf(strbuf,"%f",fieldSet[Z_AXIS]);
                NMEA0183_EncodeMsg(USB_Buff_IN, "ZSET", strbuf);
            }
            else if(strcmp(payload,"FAULT") == 0){
                sprintf(strbuf,"%s","CURRENT");
                NMEA0183_EncodeMsg(USB_Buff_IN, "FAULT", strbuf);
            }
        }
        else if(strcmp(id,"WRTE") == 0){
            if(strcmp(payload,"IDLE") == 0){
                sys_state = IDLE;
            }
            else if(strcmp(payload,"RUN") == 0){
                sys_state = RUN;
            }
        }
        
        // Write to Host
        if (USBFS_IN_BUFFER_EMPTY == USBFS_GetEPState(IN_EP_NUM)){
            USBFS_LoadInEP(IN_EP_NUM, USB_Buff_IN, USBUART_BUFFER_SIZE);
        }
        //-------------------------------------------------------------------
        
        XYZ_Direction_Write((((fieldSet[Z_AXIS] >= 0) ? 1 : 0) << 2) & (((fieldSet[Y_AXIS] >= 0) ? 1 : 0) << 1) & ((fieldSet[X_AXIS] >= 0) ? 1 : 0));
        
        if(update == 1){
            
            switch(ui_state){
                case SPLASH_SCREEN:
                    sprintf(LCD_MSG[0],"3-AXIS HHC");
                    sprintf(LCD_MSG[1],"%s, %i",versionStr,count);
                    if(buttonClicked == TRUE){
                        ui_state = SYS_MODE;
                        buttonClicked = FALSE;
                    }
                    break;
                case SYS_MODE:
                    sprintf(LCD_MSG[0],"Status");
                    sprintf(LCD_MSG[1],"%.0fW",sysPower);
                    if(buttonClicked == TRUE){
                        ui_state = X_DISPLAY;
                        buttonClicked = FALSE;
                    }
                    break;
                case X_DISPLAY:
                    sprintf(LCD_MSG[0],"X I: %.3f", currCoil[X_AXIS]);
                    //sprintf(LCD_MSG[1],"  Duty: %3.3f%%", (float)(X_PWM_ReadCompare() % X_PWM_ReadPeriod())/X_PWM_ReadPeriod()*100);
                    sprintf(LCD_MSG[1],"Set I: %.3f", fieldSet[X_AXIS]);
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
                    sprintf(LCD_MSG[1]," ");
                    if(buttonClicked == TRUE){
                        ui_state = SYS_DEBUG;
                        buttonClicked = FALSE;
                    }
                    break;
                case SYS_DEBUG:
                    sprintf(LCD_MSG[0],"ID %s R%i",id,retVal);
                    sprintf(LCD_MSG[1],"P %s",payload);
                    //sprintf(LCD_MSG[0],"T %s",USB_Buff_IN);
                    //sprintf(LCD_MSG[1],"R %s",USB_Buff_OUT);
                    if(buttonClicked == TRUE){
                        ui_state = SYS_MODE;
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
            currCoilRaw[X_AXIS] = ADC_GetResult32();
            currCoil[X_AXIS] = (currCoilRaw[X_AXIS] + (currOffsetVector[0]))*currScaleMatrix[0][0];
            if(fabs(currCoil[X_AXIS]) < currZeroThreshold) currCoil[X_AXIS] = 0;
            AMux_Select(Y_STATE);
            currReadState = Y_STATE;
            break;
        case Y_STATE:
            currCoilRaw[Y_AXIS] = ADC_GetResult32();
            currCoil[Y_AXIS] = (currCoilRaw[Y_AXIS] + (currOffsetVector[1]))*currScaleMatrix[1][1];
            if(fabs(currCoil[Y_AXIS]) < currZeroThreshold) currCoil[Y_AXIS] = 0;
            AMux_Select(Z_STATE);
            currReadState = Z_STATE;
            break;
        case Z_STATE:
            currCoilRaw[Z_AXIS] = ADC_GetResult32();
            currCoil[Z_AXIS] = (currCoilRaw[Z_AXIS] + (currOffsetVector[2]))*currScaleMatrix[2][2];
            if(fabs(currCoil[Z_AXIS]) < currZeroThreshold) currCoil[Z_AXIS] = 0;
            AMux_Select(X_STATE);
            currReadState = X_STATE;
            break;
        default:
            break;
    }
    ADC_StartConvert();
}

CY_ISR(UPDATE_PID){
    Z_PWM_WriteCompare(fieldSet[Z_AXIS]*20);
    Y_PWM_WriteCompare(fieldSet[Y_AXIS]*20);
    X_PWM_WriteCompare(fieldSet[X_AXIS]*20);
    //abs((int) PID_Update(axisController[X_AXIS],currCoil[X_AXIS],fieldSet[X_AXIS])));
    //PID_Update(axisController[Y_AXIS],currCoil[Y_AXIS],fieldSet[Y_AXIS]);
    //PID_Update(axisController[Z_AXIS],currCoil[Z_AXIS],fieldSet[Z_AXIS]);
}

/* [] END OF FILE */

#include "project.h"
#include "MPU9250.h"
#include "config.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define USBUART_BUFFER_SIZE (64u)

#define HIGH_PRIORITY       (1u)
#define DEFAULT_PRIORITY    (3u)
#define LOW_PRIORITY        (7u)

// Function Prototypes
CY_ISR_PROTO(UPDATE_UI);
CY_ISR_PROTO(SAMPLE_CURRSENSOR);

// Datatypes
typedef struct vec3_f{
    float x;
    float y;
    float z;
}vec3_f;

typedef struct vec3_i{
    int x;
    int y;
    int z;
}vec3_i;

typedef enum axis{
    X_STATE = 0,
    Y_STATE,
    Z_STATE
}axis;

//--- Global Variables ---
// UI
char LCD_MSG[2][16];
unsigned char update = 1;
int count;

// State Variables
axis currReadState = X_STATE;
vec3_f currCoil;
vec3_f magField;

// Comms Variables
uint16 length;
uint8 USB_Buff_IN[USBUART_BUFFER_SIZE];
uint8 USB_Buff_OUT[USBUART_BUFFER_SIZE];

/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)

int main(void)
{   
    // Initialize Hardware
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    
    MPU9250_Init();
    
    X_PWM_Start();
    Y_PWM_Start();
    Z_PWM_Start();
    
    ADC_Start();
    AMux_Start();
    AMux_Select(X_STATE);
    ADC_StartConvert();
    
    // Configure Interrupts
    UPDATE_UI_StartEx(UPDATE_UI);
    UPDATE_UI_SetPriority(LOW_PRIORITY);
    ADC_SAMPLECURRSENSOR_StartEx(SAMPLE_CURRSENSOR);
    ADC_SAMPLECURRSENSOR_SetPriority(HIGH_PRIORITY);
    CyGlobalIntEnable;

    // Main Program Loop
    for(;;)
    {   
        // --- Read in I/O and Sensor Data to Program Variables ---        
        // UI Controls       
        count = QuadDec_GetCounter();
        
        // Read Magnetic Field
        magField.x = MPU9250_ReadMagX()*1.66666;
        magField.y = MPU9250_ReadMagY()*1.66666;
        magField.z = MPU9250_ReadMagZ()*1.66666;

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
            while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN_EP_NUM))
            {
            }
            sprintf((char *) USB_Buff_IN,"$XMAG,%f*00\n",magField.x);
            USBFS_LoadInEP(IN_EP_NUM, USB_Buff_IN, USBUART_BUFFER_SIZE);
        }
        
        // --- Update UI ---
        if(update == 1){
            
            sprintf(LCD_MSG[0],"I_x: %.3f", currCoil.x);
            sprintf(LCD_MSG[1],"I_y: %.3f", currCoil.y);
            
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

CY_ISR(SAMPLE_CURRSENSOR){
     switch(currReadState){
        case X_STATE:
            currCoil.x = (ADC_GetResult32() + (currOffsetVector[0]))*currScaleMatrix[0][0];
            AMux_Select(Y_STATE);
            currReadState = Y_STATE;
            break;
        case Y_STATE:
            currCoil.x = (ADC_GetResult32() + (currOffsetVector[1]))*currScaleMatrix[1][1];
            AMux_Select(Z_STATE);
            currReadState = Z_STATE;
            break;
        case Z_STATE:
            currCoil.y = (ADC_GetResult32() + (currOffsetVector[2]))*currScaleMatrix[2][2];
            AMux_Select(X_STATE);
            currReadState = X_STATE;
            break;
        default:
            break;
    }
    ADC_StartConvert();
}

/* [] END OF FILE */

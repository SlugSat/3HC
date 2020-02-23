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
uint8 buffer0[USBUART_BUFFER_SIZE];
unsigned char update = 1;
int count;
int currentRaw;
float xCurr;
uint16 length;


/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)

/* Size of SRAM buffer to store endpoint data. */
#define BUFFER_SIZE   (64u)

#if (USBFS_16BITS_EP_ACCESS_ENABLE)
    /* To use the 16-bit APIs, the buffer has to be:
    *  1. The buffer size must be multiple of 2 (when endpoint size is odd).
    *     For example: the endpoint size is 63, the buffer size must be 64.
    *  2. The buffer has to be aligned to 2 bytes boundary to not cause exception
    *     while 16-bit access.
    */
    #ifdef CY_ALIGN
        /* Compiler supports alignment attribute: __ARMCC_VERSION and __GNUC__ */
        CY_ALIGN(2) uint8 buffer[BUFFER_SIZE];
    #else
        /* Complier uses pragma for alignment: __ICCARM__ */
        #pragma data_alignment = 2
        uint8 buffer[BUFFER_SIZE];
    #endif /* (CY_ALIGN) */
#else
    /* There are no specific requirements to the buffer size and alignment for 
    * the 8-bit APIs usage.
    */
    uint8 buffer[BUFFER_SIZE];
#endif /* (USBFS_GEN_16BITS_EP_ACCESS) */

int main(void)
{   
    // Initialize Hardware
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    UpdateTimer_UI_Start();
    QuadDec_Start();
    LCD_Start();
    //MPU9250_Init();
    
    X_PWM_Start();
    ADC_Start();
    
    // Configure Interrupts
    UPDATE_UI_StartEx(UPDATE_UI);
    UPDATE_UI_SetPriority(LOW_PRIORITY);
    CyGlobalIntEnable;

    
    // Configure USB
    while (0u == USBFS_GetConfiguration())
    {
    }
    /* Enable OUT endpoint to receive data from host. */
    USBFS_EnableOutEP(OUT_EP_NUM);

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
        xCurr = (currentRaw - 33572)/(-859.49);

        
        // --- Processing ---
        X_PWM_WriteCompare(count % 4096);
        
        
        
        
        // --- Communications to Host ---


        for(;;)
        {
            
            /* Check if configuration is changed. */
            if (0u != USBFS_IsConfigurationChanged())
            {
                /* Re-enable endpoint when device is configured. */
                if (0u != USBFS_GetConfiguration())
                {
                    /* Enable OUT endpoint to receive data from host. */
                    USBFS_EnableOutEP(OUT_EP_NUM);
                }
            }

            /* Check if data was received. */
            if (USBFS_OUT_BUFFER_FULL == USBFS_GetEPState(OUT_EP_NUM))
            {
                /* Read number of received data bytes. */
                length = USBFS_GetEPCount(OUT_EP_NUM);

                /* Trigger DMA to copy data from OUT endpoint buffer. */
            #if (USBFS_16BITS_EP_ACCESS_ENABLE)
                USBFS_ReadOutEP16(OUT_EP_NUM, buffer, length);
            #else
                USBFS_ReadOutEP(OUT_EP_NUM, buffer0, length);
            #endif /* (USBFS_GEN_16BITS_EP_ACCESS) */

                /* Wait until DMA completes copying data from OUT endpoint buffer. */
                while (USBFS_OUT_BUFFER_FULL == USBFS_GetEPState(OUT_EP_NUM))
                {
                }
                
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(OUT_EP_NUM);

                /* Wait until IN buffer becomes empty (host has read data). */
                while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN_EP_NUM))
                {
                }
                
                sprintf((char *) buffer,"I'M HERE BITCH\n");
            /* Trigger DMA to copy data into IN endpoint buffer.
            * After data has been copied, IN endpoint is ready to be read by the
            * host.
            */
            #if (USBFS_16BITS_EP_ACCESS_ENABLE)
                USBFS_LoadInEP16(IN_EP_NUM, buffer, length);
            #else
                USBFS_LoadInEP(IN_EP_NUM, buffer, 2 ;
            #endif /* (USBFS_GEN_16BITS_EP_ACCESS) */
            }
        }
        
        // --- Update UI ---
        if(update == 1){
            
            sprintf(LCD_MSG[0],"X: %d uT", MPU9250_ReadMagX());
            //sprintf(LCD_MSG[1],"Y: %d uT", MPU9250_ReadMagY());
            sprintf(LCD_MSG[1],"I: %f", xCurr);
            //sprintf(LCD_MSG[1],"Duty: %i", (count % 4095));
            
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

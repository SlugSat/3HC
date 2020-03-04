/*******************************************************************************
* File Name: Mux_CurrentSense.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_Mux_CurrentSense_H)
#define CY_AMUX_Mux_CurrentSense_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void Mux_CurrentSense_Start(void) ;
#define Mux_CurrentSense_Init() Mux_CurrentSense_Start()
void Mux_CurrentSense_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void Mux_CurrentSense_Stop(void); */
/* void Mux_CurrentSense_Select(uint8 channel); */
/* void Mux_CurrentSense_Connect(uint8 channel); */
/* void Mux_CurrentSense_Disconnect(uint8 channel); */
/* void Mux_CurrentSense_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define Mux_CurrentSense_CHANNELS  3u
#define Mux_CurrentSense_MUXTYPE   1
#define Mux_CurrentSense_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define Mux_CurrentSense_NULL_CHANNEL 0xFFu
#define Mux_CurrentSense_MUX_SINGLE   1
#define Mux_CurrentSense_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if Mux_CurrentSense_MUXTYPE == Mux_CurrentSense_MUX_SINGLE
# if !Mux_CurrentSense_ATMOSTONE
#  define Mux_CurrentSense_Connect(channel) Mux_CurrentSense_Set(channel)
# endif
# define Mux_CurrentSense_Disconnect(channel) Mux_CurrentSense_Unset(channel)
#else
# if !Mux_CurrentSense_ATMOSTONE
void Mux_CurrentSense_Connect(uint8 channel) ;
# endif
void Mux_CurrentSense_Disconnect(uint8 channel) ;
#endif

#if Mux_CurrentSense_ATMOSTONE
# define Mux_CurrentSense_Stop() Mux_CurrentSense_DisconnectAll()
# define Mux_CurrentSense_Select(channel) Mux_CurrentSense_FastSelect(channel)
void Mux_CurrentSense_DisconnectAll(void) ;
#else
# define Mux_CurrentSense_Stop() Mux_CurrentSense_Start()
void Mux_CurrentSense_Select(uint8 channel) ;
# define Mux_CurrentSense_DisconnectAll() Mux_CurrentSense_Start()
#endif

#endif /* CY_AMUX_Mux_CurrentSense_H */


/* [] END OF FILE */

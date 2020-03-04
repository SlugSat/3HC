/*******************************************************************************
* File Name: ENC_Backlight.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_ENC_Backlight_H) /* CY_CONTROL_REG_ENC_Backlight_H */
#define CY_CONTROL_REG_ENC_Backlight_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} ENC_Backlight_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    ENC_Backlight_Write(uint8 control) ;
uint8   ENC_Backlight_Read(void) ;

void ENC_Backlight_SaveConfig(void) ;
void ENC_Backlight_RestoreConfig(void) ;
void ENC_Backlight_Sleep(void) ; 
void ENC_Backlight_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define ENC_Backlight_Control        (* (reg8 *) ENC_Backlight_Sync_ctrl_reg__CONTROL_REG )
#define ENC_Backlight_Control_PTR    (  (reg8 *) ENC_Backlight_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_ENC_Backlight_H */


/* [] END OF FILE */

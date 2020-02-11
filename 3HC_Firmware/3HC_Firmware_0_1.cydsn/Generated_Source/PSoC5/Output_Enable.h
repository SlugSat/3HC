/*******************************************************************************
* File Name: Output_Enable.h  
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

#if !defined(CY_CONTROL_REG_Output_Enable_H) /* CY_CONTROL_REG_Output_Enable_H */
#define CY_CONTROL_REG_Output_Enable_H

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

} Output_Enable_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    Output_Enable_Write(uint8 control) ;
uint8   Output_Enable_Read(void) ;

void Output_Enable_SaveConfig(void) ;
void Output_Enable_RestoreConfig(void) ;
void Output_Enable_Sleep(void) ; 
void Output_Enable_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Output_Enable_Control        (* (reg8 *) Output_Enable_Sync_ctrl_reg__CONTROL_REG )
#define Output_Enable_Control_PTR    (  (reg8 *) Output_Enable_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_Output_Enable_H */


/* [] END OF FILE */

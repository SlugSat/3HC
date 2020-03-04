/*******************************************************************************
* File Name: XYZ_Direction.h  
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

#if !defined(CY_CONTROL_REG_XYZ_Direction_H) /* CY_CONTROL_REG_XYZ_Direction_H */
#define CY_CONTROL_REG_XYZ_Direction_H

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

} XYZ_Direction_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    XYZ_Direction_Write(uint8 control) ;
uint8   XYZ_Direction_Read(void) ;

void XYZ_Direction_SaveConfig(void) ;
void XYZ_Direction_RestoreConfig(void) ;
void XYZ_Direction_Sleep(void) ; 
void XYZ_Direction_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define XYZ_Direction_Control        (* (reg8 *) XYZ_Direction_Sync_ctrl_reg__CONTROL_REG )
#define XYZ_Direction_Control_PTR    (  (reg8 *) XYZ_Direction_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_XYZ_Direction_H */


/* [] END OF FILE */

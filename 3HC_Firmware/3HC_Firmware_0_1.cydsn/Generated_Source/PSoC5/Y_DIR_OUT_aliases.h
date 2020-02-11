/*******************************************************************************
* File Name: Y_DIR_OUT.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Y_DIR_OUT_ALIASES_H) /* Pins Y_DIR_OUT_ALIASES_H */
#define CY_PINS_Y_DIR_OUT_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Y_DIR_OUT_0			(Y_DIR_OUT__0__PC)
#define Y_DIR_OUT_0_INTR	((uint16)((uint16)0x0001u << Y_DIR_OUT__0__SHIFT))

#define Y_DIR_OUT_INTR_ALL	 ((uint16)(Y_DIR_OUT_0_INTR))

#endif /* End Pins Y_DIR_OUT_ALIASES_H */


/* [] END OF FILE */

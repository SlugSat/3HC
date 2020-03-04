/*******************************************************************************
* File Name: ENC_Backlight_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "ENC_Backlight.h"

/* Check for removal by optimization */
#if !defined(ENC_Backlight_Sync_ctrl_reg__REMOVED)

static ENC_Backlight_BACKUP_STRUCT  ENC_Backlight_backup = {0u};

    
/*******************************************************************************
* Function Name: ENC_Backlight_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ENC_Backlight_SaveConfig(void) 
{
    ENC_Backlight_backup.controlState = ENC_Backlight_Control;
}


/*******************************************************************************
* Function Name: ENC_Backlight_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void ENC_Backlight_RestoreConfig(void) 
{
     ENC_Backlight_Control = ENC_Backlight_backup.controlState;
}


/*******************************************************************************
* Function Name: ENC_Backlight_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ENC_Backlight_Sleep(void) 
{
    ENC_Backlight_SaveConfig();
}


/*******************************************************************************
* Function Name: ENC_Backlight_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ENC_Backlight_Wakeup(void)  
{
    ENC_Backlight_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

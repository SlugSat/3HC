/*******************************************************************************
* File Name: Output_Enable_PM.c
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

#include "Output_Enable.h"

/* Check for removal by optimization */
#if !defined(Output_Enable_Sync_ctrl_reg__REMOVED)

static Output_Enable_BACKUP_STRUCT  Output_Enable_backup = {0u};

    
/*******************************************************************************
* Function Name: Output_Enable_SaveConfig
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
void Output_Enable_SaveConfig(void) 
{
    Output_Enable_backup.controlState = Output_Enable_Control;
}


/*******************************************************************************
* Function Name: Output_Enable_RestoreConfig
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
void Output_Enable_RestoreConfig(void) 
{
     Output_Enable_Control = Output_Enable_backup.controlState;
}


/*******************************************************************************
* Function Name: Output_Enable_Sleep
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
void Output_Enable_Sleep(void) 
{
    Output_Enable_SaveConfig();
}


/*******************************************************************************
* Function Name: Output_Enable_Wakeup
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
void Output_Enable_Wakeup(void)  
{
    Output_Enable_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

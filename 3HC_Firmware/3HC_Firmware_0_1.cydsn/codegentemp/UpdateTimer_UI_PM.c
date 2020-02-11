/*******************************************************************************
* File Name: UpdateTimer_UI_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "UpdateTimer_UI.h"

static UpdateTimer_UI_backupStruct UpdateTimer_UI_backup;


/*******************************************************************************
* Function Name: UpdateTimer_UI_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  UpdateTimer_UI_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void UpdateTimer_UI_SaveConfig(void) 
{
    #if (!UpdateTimer_UI_UsingFixedFunction)
        UpdateTimer_UI_backup.TimerUdb = UpdateTimer_UI_ReadCounter();
        UpdateTimer_UI_backup.InterruptMaskValue = UpdateTimer_UI_STATUS_MASK;
        #if (UpdateTimer_UI_UsingHWCaptureCounter)
            UpdateTimer_UI_backup.TimerCaptureCounter = UpdateTimer_UI_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED)
            UpdateTimer_UI_backup.TimerControlRegister = UpdateTimer_UI_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: UpdateTimer_UI_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  UpdateTimer_UI_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void UpdateTimer_UI_RestoreConfig(void) 
{   
    #if (!UpdateTimer_UI_UsingFixedFunction)

        UpdateTimer_UI_WriteCounter(UpdateTimer_UI_backup.TimerUdb);
        UpdateTimer_UI_STATUS_MASK =UpdateTimer_UI_backup.InterruptMaskValue;
        #if (UpdateTimer_UI_UsingHWCaptureCounter)
            UpdateTimer_UI_SetCaptureCount(UpdateTimer_UI_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED)
            UpdateTimer_UI_WriteControlRegister(UpdateTimer_UI_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: UpdateTimer_UI_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  UpdateTimer_UI_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void UpdateTimer_UI_Sleep(void) 
{
    #if(!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(UpdateTimer_UI_CTRL_ENABLE == (UpdateTimer_UI_CONTROL & UpdateTimer_UI_CTRL_ENABLE))
        {
            /* Timer is enabled */
            UpdateTimer_UI_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            UpdateTimer_UI_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    UpdateTimer_UI_Stop();
    UpdateTimer_UI_SaveConfig();
}


/*******************************************************************************
* Function Name: UpdateTimer_UI_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  UpdateTimer_UI_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void UpdateTimer_UI_Wakeup(void) 
{
    UpdateTimer_UI_RestoreConfig();
    #if(!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED)
        if(UpdateTimer_UI_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                UpdateTimer_UI_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

/*******************************************************************************
* File Name: ValveTimer_PM.c
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

#include "ValveTimer.h"

static ValveTimer_backupStruct ValveTimer_backup;


/*******************************************************************************
* Function Name: ValveTimer_SaveConfig
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
*  ValveTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void ValveTimer_SaveConfig(void) 
{
    #if (!ValveTimer_UsingFixedFunction)
        ValveTimer_backup.TimerUdb = ValveTimer_ReadCounter();
        ValveTimer_backup.InterruptMaskValue = ValveTimer_STATUS_MASK;
        #if (ValveTimer_UsingHWCaptureCounter)
            ValveTimer_backup.TimerCaptureCounter = ValveTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!ValveTimer_UDB_CONTROL_REG_REMOVED)
            ValveTimer_backup.TimerControlRegister = ValveTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: ValveTimer_RestoreConfig
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
*  ValveTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void ValveTimer_RestoreConfig(void) 
{   
    #if (!ValveTimer_UsingFixedFunction)

        ValveTimer_WriteCounter(ValveTimer_backup.TimerUdb);
        ValveTimer_STATUS_MASK =ValveTimer_backup.InterruptMaskValue;
        #if (ValveTimer_UsingHWCaptureCounter)
            ValveTimer_SetCaptureCount(ValveTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!ValveTimer_UDB_CONTROL_REG_REMOVED)
            ValveTimer_WriteControlRegister(ValveTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: ValveTimer_Sleep
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
*  ValveTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void ValveTimer_Sleep(void) 
{
    #if(!ValveTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(ValveTimer_CTRL_ENABLE == (ValveTimer_CONTROL & ValveTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            ValveTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            ValveTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    ValveTimer_Stop();
    ValveTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: ValveTimer_Wakeup
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
*  ValveTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ValveTimer_Wakeup(void) 
{
    ValveTimer_RestoreConfig();
    #if(!ValveTimer_UDB_CONTROL_REG_REMOVED)
        if(ValveTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                ValveTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

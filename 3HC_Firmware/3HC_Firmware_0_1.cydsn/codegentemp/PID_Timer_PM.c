/*******************************************************************************
* File Name: PID_Timer_PM.c
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

#include "PID_Timer.h"

static PID_Timer_backupStruct PID_Timer_backup;


/*******************************************************************************
* Function Name: PID_Timer_SaveConfig
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
*  PID_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PID_Timer_SaveConfig(void) 
{
    #if (!PID_Timer_UsingFixedFunction)
        PID_Timer_backup.TimerUdb = PID_Timer_ReadCounter();
        PID_Timer_backup.InterruptMaskValue = PID_Timer_STATUS_MASK;
        #if (PID_Timer_UsingHWCaptureCounter)
            PID_Timer_backup.TimerCaptureCounter = PID_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!PID_Timer_UDB_CONTROL_REG_REMOVED)
            PID_Timer_backup.TimerControlRegister = PID_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: PID_Timer_RestoreConfig
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
*  PID_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PID_Timer_RestoreConfig(void) 
{   
    #if (!PID_Timer_UsingFixedFunction)

        PID_Timer_WriteCounter(PID_Timer_backup.TimerUdb);
        PID_Timer_STATUS_MASK =PID_Timer_backup.InterruptMaskValue;
        #if (PID_Timer_UsingHWCaptureCounter)
            PID_Timer_SetCaptureCount(PID_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!PID_Timer_UDB_CONTROL_REG_REMOVED)
            PID_Timer_WriteControlRegister(PID_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: PID_Timer_Sleep
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
*  PID_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void PID_Timer_Sleep(void) 
{
    #if(!PID_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(PID_Timer_CTRL_ENABLE == (PID_Timer_CONTROL & PID_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            PID_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            PID_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    PID_Timer_Stop();
    PID_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: PID_Timer_Wakeup
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
*  PID_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PID_Timer_Wakeup(void) 
{
    PID_Timer_RestoreConfig();
    #if(!PID_Timer_UDB_CONTROL_REG_REMOVED)
        if(PID_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                PID_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

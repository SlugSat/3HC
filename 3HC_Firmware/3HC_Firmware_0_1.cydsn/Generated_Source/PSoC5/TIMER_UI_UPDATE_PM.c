/*******************************************************************************
* File Name: TIMER_UI_UPDATE_PM.c
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

#include "TIMER_UI_UPDATE.h"

static TIMER_UI_UPDATE_backupStruct TIMER_UI_UPDATE_backup;


/*******************************************************************************
* Function Name: TIMER_UI_UPDATE_SaveConfig
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
*  TIMER_UI_UPDATE_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TIMER_UI_UPDATE_SaveConfig(void) 
{
    #if (!TIMER_UI_UPDATE_UsingFixedFunction)
        TIMER_UI_UPDATE_backup.TimerUdb = TIMER_UI_UPDATE_ReadCounter();
        TIMER_UI_UPDATE_backup.InterruptMaskValue = TIMER_UI_UPDATE_STATUS_MASK;
        #if (TIMER_UI_UPDATE_UsingHWCaptureCounter)
            TIMER_UI_UPDATE_backup.TimerCaptureCounter = TIMER_UI_UPDATE_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TIMER_UI_UPDATE_UDB_CONTROL_REG_REMOVED)
            TIMER_UI_UPDATE_backup.TimerControlRegister = TIMER_UI_UPDATE_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TIMER_UI_UPDATE_RestoreConfig
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
*  TIMER_UI_UPDATE_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TIMER_UI_UPDATE_RestoreConfig(void) 
{   
    #if (!TIMER_UI_UPDATE_UsingFixedFunction)

        TIMER_UI_UPDATE_WriteCounter(TIMER_UI_UPDATE_backup.TimerUdb);
        TIMER_UI_UPDATE_STATUS_MASK =TIMER_UI_UPDATE_backup.InterruptMaskValue;
        #if (TIMER_UI_UPDATE_UsingHWCaptureCounter)
            TIMER_UI_UPDATE_SetCaptureCount(TIMER_UI_UPDATE_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TIMER_UI_UPDATE_UDB_CONTROL_REG_REMOVED)
            TIMER_UI_UPDATE_WriteControlRegister(TIMER_UI_UPDATE_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TIMER_UI_UPDATE_Sleep
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
*  TIMER_UI_UPDATE_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TIMER_UI_UPDATE_Sleep(void) 
{
    #if(!TIMER_UI_UPDATE_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TIMER_UI_UPDATE_CTRL_ENABLE == (TIMER_UI_UPDATE_CONTROL & TIMER_UI_UPDATE_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TIMER_UI_UPDATE_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TIMER_UI_UPDATE_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TIMER_UI_UPDATE_Stop();
    TIMER_UI_UPDATE_SaveConfig();
}


/*******************************************************************************
* Function Name: TIMER_UI_UPDATE_Wakeup
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
*  TIMER_UI_UPDATE_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TIMER_UI_UPDATE_Wakeup(void) 
{
    TIMER_UI_UPDATE_RestoreConfig();
    #if(!TIMER_UI_UPDATE_UDB_CONTROL_REG_REMOVED)
        if(TIMER_UI_UPDATE_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TIMER_UI_UPDATE_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

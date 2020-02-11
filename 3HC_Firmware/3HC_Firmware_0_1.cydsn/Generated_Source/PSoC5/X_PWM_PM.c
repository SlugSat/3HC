/*******************************************************************************
* File Name: X_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "X_PWM.h"

static X_PWM_backupStruct X_PWM_backup;


/*******************************************************************************
* Function Name: X_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  X_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void X_PWM_SaveConfig(void) 
{

    #if(!X_PWM_UsingFixedFunction)
        #if(!X_PWM_PWMModeIsCenterAligned)
            X_PWM_backup.PWMPeriod = X_PWM_ReadPeriod();
        #endif /* (!X_PWM_PWMModeIsCenterAligned) */
        X_PWM_backup.PWMUdb = X_PWM_ReadCounter();
        #if (X_PWM_UseStatus)
            X_PWM_backup.InterruptMaskValue = X_PWM_STATUS_MASK;
        #endif /* (X_PWM_UseStatus) */

        #if(X_PWM_DeadBandMode == X_PWM__B_PWM__DBM_256_CLOCKS || \
            X_PWM_DeadBandMode == X_PWM__B_PWM__DBM_2_4_CLOCKS)
            X_PWM_backup.PWMdeadBandValue = X_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(X_PWM_KillModeMinTime)
             X_PWM_backup.PWMKillCounterPeriod = X_PWM_ReadKillTime();
        #endif /* (X_PWM_KillModeMinTime) */

        #if(X_PWM_UseControl)
            X_PWM_backup.PWMControlRegister = X_PWM_ReadControlRegister();
        #endif /* (X_PWM_UseControl) */
    #endif  /* (!X_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: X_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  X_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void X_PWM_RestoreConfig(void) 
{
        #if(!X_PWM_UsingFixedFunction)
            #if(!X_PWM_PWMModeIsCenterAligned)
                X_PWM_WritePeriod(X_PWM_backup.PWMPeriod);
            #endif /* (!X_PWM_PWMModeIsCenterAligned) */

            X_PWM_WriteCounter(X_PWM_backup.PWMUdb);

            #if (X_PWM_UseStatus)
                X_PWM_STATUS_MASK = X_PWM_backup.InterruptMaskValue;
            #endif /* (X_PWM_UseStatus) */

            #if(X_PWM_DeadBandMode == X_PWM__B_PWM__DBM_256_CLOCKS || \
                X_PWM_DeadBandMode == X_PWM__B_PWM__DBM_2_4_CLOCKS)
                X_PWM_WriteDeadTime(X_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(X_PWM_KillModeMinTime)
                X_PWM_WriteKillTime(X_PWM_backup.PWMKillCounterPeriod);
            #endif /* (X_PWM_KillModeMinTime) */

            #if(X_PWM_UseControl)
                X_PWM_WriteControlRegister(X_PWM_backup.PWMControlRegister);
            #endif /* (X_PWM_UseControl) */
        #endif  /* (!X_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: X_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  X_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void X_PWM_Sleep(void) 
{
    #if(X_PWM_UseControl)
        if(X_PWM_CTRL_ENABLE == (X_PWM_CONTROL & X_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            X_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            X_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (X_PWM_UseControl) */

    /* Stop component */
    X_PWM_Stop();

    /* Save registers configuration */
    X_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: X_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  X_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void X_PWM_Wakeup(void) 
{
     /* Restore registers values */
    X_PWM_RestoreConfig();

    if(X_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        X_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

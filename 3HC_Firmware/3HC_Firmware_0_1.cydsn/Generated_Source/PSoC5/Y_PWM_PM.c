/*******************************************************************************
* File Name: Y_PWM_PM.c
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

#include "Y_PWM.h"

static Y_PWM_backupStruct Y_PWM_backup;


/*******************************************************************************
* Function Name: Y_PWM_SaveConfig
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
*  Y_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Y_PWM_SaveConfig(void) 
{

    #if(!Y_PWM_UsingFixedFunction)
        #if(!Y_PWM_PWMModeIsCenterAligned)
            Y_PWM_backup.PWMPeriod = Y_PWM_ReadPeriod();
        #endif /* (!Y_PWM_PWMModeIsCenterAligned) */
        Y_PWM_backup.PWMUdb = Y_PWM_ReadCounter();
        #if (Y_PWM_UseStatus)
            Y_PWM_backup.InterruptMaskValue = Y_PWM_STATUS_MASK;
        #endif /* (Y_PWM_UseStatus) */

        #if(Y_PWM_DeadBandMode == Y_PWM__B_PWM__DBM_256_CLOCKS || \
            Y_PWM_DeadBandMode == Y_PWM__B_PWM__DBM_2_4_CLOCKS)
            Y_PWM_backup.PWMdeadBandValue = Y_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Y_PWM_KillModeMinTime)
             Y_PWM_backup.PWMKillCounterPeriod = Y_PWM_ReadKillTime();
        #endif /* (Y_PWM_KillModeMinTime) */

        #if(Y_PWM_UseControl)
            Y_PWM_backup.PWMControlRegister = Y_PWM_ReadControlRegister();
        #endif /* (Y_PWM_UseControl) */
    #endif  /* (!Y_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Y_PWM_RestoreConfig
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
*  Y_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Y_PWM_RestoreConfig(void) 
{
        #if(!Y_PWM_UsingFixedFunction)
            #if(!Y_PWM_PWMModeIsCenterAligned)
                Y_PWM_WritePeriod(Y_PWM_backup.PWMPeriod);
            #endif /* (!Y_PWM_PWMModeIsCenterAligned) */

            Y_PWM_WriteCounter(Y_PWM_backup.PWMUdb);

            #if (Y_PWM_UseStatus)
                Y_PWM_STATUS_MASK = Y_PWM_backup.InterruptMaskValue;
            #endif /* (Y_PWM_UseStatus) */

            #if(Y_PWM_DeadBandMode == Y_PWM__B_PWM__DBM_256_CLOCKS || \
                Y_PWM_DeadBandMode == Y_PWM__B_PWM__DBM_2_4_CLOCKS)
                Y_PWM_WriteDeadTime(Y_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Y_PWM_KillModeMinTime)
                Y_PWM_WriteKillTime(Y_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Y_PWM_KillModeMinTime) */

            #if(Y_PWM_UseControl)
                Y_PWM_WriteControlRegister(Y_PWM_backup.PWMControlRegister);
            #endif /* (Y_PWM_UseControl) */
        #endif  /* (!Y_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Y_PWM_Sleep
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
*  Y_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Y_PWM_Sleep(void) 
{
    #if(Y_PWM_UseControl)
        if(Y_PWM_CTRL_ENABLE == (Y_PWM_CONTROL & Y_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Y_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Y_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Y_PWM_UseControl) */

    /* Stop component */
    Y_PWM_Stop();

    /* Save registers configuration */
    Y_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Y_PWM_Wakeup
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
*  Y_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Y_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Y_PWM_RestoreConfig();

    if(Y_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Y_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

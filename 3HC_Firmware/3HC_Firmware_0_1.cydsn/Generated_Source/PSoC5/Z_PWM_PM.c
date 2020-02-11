/*******************************************************************************
* File Name: Z_PWM_PM.c
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

#include "Z_PWM.h"

static Z_PWM_backupStruct Z_PWM_backup;


/*******************************************************************************
* Function Name: Z_PWM_SaveConfig
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
*  Z_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Z_PWM_SaveConfig(void) 
{

    #if(!Z_PWM_UsingFixedFunction)
        #if(!Z_PWM_PWMModeIsCenterAligned)
            Z_PWM_backup.PWMPeriod = Z_PWM_ReadPeriod();
        #endif /* (!Z_PWM_PWMModeIsCenterAligned) */
        Z_PWM_backup.PWMUdb = Z_PWM_ReadCounter();
        #if (Z_PWM_UseStatus)
            Z_PWM_backup.InterruptMaskValue = Z_PWM_STATUS_MASK;
        #endif /* (Z_PWM_UseStatus) */

        #if(Z_PWM_DeadBandMode == Z_PWM__B_PWM__DBM_256_CLOCKS || \
            Z_PWM_DeadBandMode == Z_PWM__B_PWM__DBM_2_4_CLOCKS)
            Z_PWM_backup.PWMdeadBandValue = Z_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Z_PWM_KillModeMinTime)
             Z_PWM_backup.PWMKillCounterPeriod = Z_PWM_ReadKillTime();
        #endif /* (Z_PWM_KillModeMinTime) */

        #if(Z_PWM_UseControl)
            Z_PWM_backup.PWMControlRegister = Z_PWM_ReadControlRegister();
        #endif /* (Z_PWM_UseControl) */
    #endif  /* (!Z_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Z_PWM_RestoreConfig
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
*  Z_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Z_PWM_RestoreConfig(void) 
{
        #if(!Z_PWM_UsingFixedFunction)
            #if(!Z_PWM_PWMModeIsCenterAligned)
                Z_PWM_WritePeriod(Z_PWM_backup.PWMPeriod);
            #endif /* (!Z_PWM_PWMModeIsCenterAligned) */

            Z_PWM_WriteCounter(Z_PWM_backup.PWMUdb);

            #if (Z_PWM_UseStatus)
                Z_PWM_STATUS_MASK = Z_PWM_backup.InterruptMaskValue;
            #endif /* (Z_PWM_UseStatus) */

            #if(Z_PWM_DeadBandMode == Z_PWM__B_PWM__DBM_256_CLOCKS || \
                Z_PWM_DeadBandMode == Z_PWM__B_PWM__DBM_2_4_CLOCKS)
                Z_PWM_WriteDeadTime(Z_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Z_PWM_KillModeMinTime)
                Z_PWM_WriteKillTime(Z_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Z_PWM_KillModeMinTime) */

            #if(Z_PWM_UseControl)
                Z_PWM_WriteControlRegister(Z_PWM_backup.PWMControlRegister);
            #endif /* (Z_PWM_UseControl) */
        #endif  /* (!Z_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Z_PWM_Sleep
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
*  Z_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Z_PWM_Sleep(void) 
{
    #if(Z_PWM_UseControl)
        if(Z_PWM_CTRL_ENABLE == (Z_PWM_CONTROL & Z_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Z_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Z_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Z_PWM_UseControl) */

    /* Stop component */
    Z_PWM_Stop();

    /* Save registers configuration */
    Z_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Z_PWM_Wakeup
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
*  Z_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Z_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Z_PWM_RestoreConfig();

    if(Z_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Z_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

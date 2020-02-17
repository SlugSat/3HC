/*******************************************************************************
* File Name: ADC_CURR_SENSE_PM.c
* Version 3.30
*
* Description:
*  This file provides the power manager source code to the API for the
*  Delta-Sigma ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_CURR_SENSE.h"

static ADC_CURR_SENSE_BACKUP_STRUCT ADC_CURR_SENSE_backup =
{
    ADC_CURR_SENSE_DISABLED,
    ADC_CURR_SENSE_CFG1_DEC_CR
};


/*******************************************************************************
* Function Name: ADC_CURR_SENSE_SaveConfig
********************************************************************************
*
* Summary:
*  Save the register configuration which are not retention.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_CURR_SENSE_backup:  This global structure variable is used to store
*  configuration registers which are non retention whenever user wants to go
*  sleep mode by calling Sleep() API.
*
*******************************************************************************/
void ADC_CURR_SENSE_SaveConfig(void) 
{
    ADC_CURR_SENSE_backup.deccr = ADC_CURR_SENSE_DEC_CR_REG;
}


/*******************************************************************************
* Function Name: ADC_CURR_SENSE_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations which are not retention.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_CURR_SENSE_backup:  This global structure variable is used to restore
*  configuration registers which are non retention whenever user wants to switch
*  to active power mode by calling Wakeup() API.
*
*******************************************************************************/
void ADC_CURR_SENSE_RestoreConfig(void) 
{
    ADC_CURR_SENSE_DEC_CR_REG = ADC_CURR_SENSE_backup.deccr;
}


/*******************************************************************************
* Function Name: ADC_CURR_SENSE_Sleep
********************************************************************************
*
* Summary:
*  Stops the operation of the block and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_CURR_SENSE_backup:  The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_CURR_SENSE_Sleep(void) 
{
    /* Save ADC enable state */
    if((ADC_CURR_SENSE_ACT_PWR_DEC_EN == (ADC_CURR_SENSE_PWRMGR_DEC_REG & ADC_CURR_SENSE_ACT_PWR_DEC_EN)) &&
       (ADC_CURR_SENSE_ACT_PWR_DSM_EN == (ADC_CURR_SENSE_PWRMGR_DSM_REG & ADC_CURR_SENSE_ACT_PWR_DSM_EN)))
    {
        /* Component is enabled */
        ADC_CURR_SENSE_backup.enableState = ADC_CURR_SENSE_ENABLED;
        if((ADC_CURR_SENSE_DEC_CR_REG & ADC_CURR_SENSE_DEC_START_CONV) != 0u)
        {   
            /* Conversion is started */
            ADC_CURR_SENSE_backup.enableState |= ADC_CURR_SENSE_STARTED;
        }
		
        /* Stop the configuration */
        ADC_CURR_SENSE_Stop();
    }
    else
    {
        /* Component is disabled */
        ADC_CURR_SENSE_backup.enableState = ADC_CURR_SENSE_DISABLED;
    }

    /* Save the user configuration */
    ADC_CURR_SENSE_SaveConfig();
}


/*******************************************************************************
* Function Name: ADC_CURR_SENSE_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and enables the power to the block.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_CURR_SENSE_backup:  The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_CURR_SENSE_Wakeup(void) 
{
    /* Restore the configuration */
    ADC_CURR_SENSE_RestoreConfig();

    /* Enables the component operation */
    if(ADC_CURR_SENSE_backup.enableState != ADC_CURR_SENSE_DISABLED)
    {
        ADC_CURR_SENSE_Enable();
        if((ADC_CURR_SENSE_backup.enableState & ADC_CURR_SENSE_STARTED) != 0u)
        {
            ADC_CURR_SENSE_StartConvert();
        }
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */

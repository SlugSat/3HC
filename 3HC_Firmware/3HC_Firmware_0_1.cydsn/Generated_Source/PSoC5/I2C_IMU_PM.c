/*******************************************************************************
* File Name: I2C_IMU_PM.c
* Version 3.50
*
* Description:
*  This file provides low power mode APIs for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_IMU_PVT.h"

I2C_IMU_BACKUP_STRUCT I2C_IMU_backup =
{
    I2C_IMU_DISABLE,

#if (I2C_IMU_FF_IMPLEMENTED)
    I2C_IMU_DEFAULT_XCFG,
    I2C_IMU_DEFAULT_CFG,
    I2C_IMU_DEFAULT_ADDR,
    LO8(I2C_IMU_DEFAULT_DIVIDE_FACTOR),
    HI8(I2C_IMU_DEFAULT_DIVIDE_FACTOR),
#else  /* (I2C_IMU_UDB_IMPLEMENTED) */
    I2C_IMU_DEFAULT_CFG,
#endif /* (I2C_IMU_FF_IMPLEMENTED) */

#if (I2C_IMU_TIMEOUT_ENABLED)
    I2C_IMU_DEFAULT_TMOUT_PERIOD,
    I2C_IMU_DEFAULT_TMOUT_INTR_MASK,
#endif /* (I2C_IMU_TIMEOUT_ENABLED) */
};

#if ((I2C_IMU_FF_IMPLEMENTED) && (I2C_IMU_WAKEUP_ENABLED))
    volatile uint8 I2C_IMU_wakeupSource;
#endif /* ((I2C_IMU_FF_IMPLEMENTED) && (I2C_IMU_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: I2C_IMU_SaveConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Stores the component non-retention configuration registers.
*   Checked:   Disables the master, if it was enabled before, and enables
*              backup regulator of the I2C hardware. If a transaction intended
*              for component executes during this function call, it waits until
*              the current transaction is completed and I2C hardware is ready
*              to enter sleep mode. All subsequent I2C traffic is NAKed until
*              the device is put into sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  I2C_IMU_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            entering the sleep mode.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_IMU_SaveConfig(void) 
{
#if (I2C_IMU_FF_IMPLEMENTED)
    #if (I2C_IMU_WAKEUP_ENABLED)
        uint8 intState;
    #endif /* (I2C_IMU_WAKEUP_ENABLED) */

    /* Store registers before enter low power mode */
    I2C_IMU_backup.cfg     = I2C_IMU_CFG_REG;
    I2C_IMU_backup.xcfg    = I2C_IMU_XCFG_REG;
    I2C_IMU_backup.addr    = I2C_IMU_ADDR_REG;
    I2C_IMU_backup.clkDiv1 = I2C_IMU_CLKDIV1_REG;
    I2C_IMU_backup.clkDiv2 = I2C_IMU_CLKDIV2_REG;

#if (I2C_IMU_WAKEUP_ENABLED)
    /* Disable master */
    I2C_IMU_CFG_REG &= (uint8) ~I2C_IMU_ENABLE_MASTER;

    /* Enable backup regulator to keep block powered in low power mode */
    intState = CyEnterCriticalSection();
    I2C_IMU_PWRSYS_CR1_REG |= I2C_IMU_PWRSYS_CR1_I2C_REG_BACKUP;
    CyExitCriticalSection(intState);

    /* 1) Set force NACK to ignore I2C transactions;
    *  2) Wait unti I2C is ready go to Sleep; !!
    *  3) These bits are cleared on wake up.
    */
    /* Wait when block is ready for sleep */
    I2C_IMU_XCFG_REG |= I2C_IMU_XCFG_FORCE_NACK;
    while (0u == (I2C_IMU_XCFG_REG & I2C_IMU_XCFG_RDY_TO_SLEEP))
    {
    }

    /* Setup wakeup interrupt */
    I2C_IMU_DisableInt();
    (void) CyIntSetVector(I2C_IMU_ISR_NUMBER, &I2C_IMU_WAKEUP_ISR);
    I2C_IMU_wakeupSource = 0u;
    I2C_IMU_EnableInt();
#endif /* (I2C_IMU_WAKEUP_ENABLED) */

#else
    /* Store only address match bit */
    I2C_IMU_backup.control = (I2C_IMU_CFG_REG & I2C_IMU_CTRL_ANY_ADDRESS_MASK);
#endif /* (I2C_IMU_FF_IMPLEMENTED) */

#if (I2C_IMU_TIMEOUT_ENABLED)
    I2C_IMU_TimeoutSaveConfig();
#endif /* (I2C_IMU_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_IMU_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component before device enters
*  sleep mode. The Enable wakeup from Sleep Mode selection influences this
*  function implementation:
*   Unchecked: Checks current I2C component state, saves it, and disables the
*              component by calling I2C_Stop() if it is currently enabled.
*              I2C_SaveConfig() is then called to save the component
*              non-retention configuration registers.
*   Checked:   If a transaction intended for component executes during this
*              function call, it waits until the current transaction is
*              completed. All subsequent I2C traffic intended for component
*              is NAKed until the device is put to sleep mode. The address
*              match event wakes up the device.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_IMU_Sleep(void) 
{
#if (I2C_IMU_WAKEUP_ENABLED)
    /* Do not enable block after exit low power mode if it is wakeup source */
    I2C_IMU_backup.enableState = I2C_IMU_DISABLE;

    #if (I2C_IMU_TIMEOUT_ENABLED)
        I2C_IMU_TimeoutStop();
    #endif /* (I2C_IMU_TIMEOUT_ENABLED) */

#else
    /* Store enable state */
    I2C_IMU_backup.enableState = (uint8) I2C_IMU_IS_ENABLED;

    if (0u != I2C_IMU_backup.enableState)
    {
        I2C_IMU_Stop();
    }
#endif /* (I2C_IMU_WAKEUP_ENABLED) */

    I2C_IMU_SaveConfig();
}


/*******************************************************************************
* Function Name: I2C_IMU_RestoreConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              to the state they were in before I2C_Sleep() or I2C_SaveConfig()
*              was called.
*   Checked:   Disables the backup regulator of the I2C hardware. Sets up the
*              regular component interrupt handler and generates the component
*              interrupt if it was wake up source to release the bus and
*              continue in-coming I2C transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  I2C_IMU_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            exiting the sleep mode.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before I2C_IMU_SaveConfig() or
*  I2C_IMU_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void I2C_IMU_RestoreConfig(void) 
{
#if (I2C_IMU_FF_IMPLEMENTED)
    uint8 intState;

    if (I2C_IMU_CHECK_PWRSYS_I2C_BACKUP)
    /* Low power mode was Sleep - backup regulator is enabled */
    {
        /* Enable backup regulator in active mode */
        intState = CyEnterCriticalSection();
        I2C_IMU_PWRSYS_CR1_REG &= (uint8) ~I2C_IMU_PWRSYS_CR1_I2C_REG_BACKUP;
        CyExitCriticalSection(intState);

        /* Restore master */
        I2C_IMU_CFG_REG = I2C_IMU_backup.cfg;
    }
    else
    /* Low power mode was Hibernate - backup regulator is disabled. All registers are cleared */
    {
    #if (I2C_IMU_WAKEUP_ENABLED)
        /* Disable power to block before register restore */
        intState = CyEnterCriticalSection();
        I2C_IMU_ACT_PWRMGR_REG  &= (uint8) ~I2C_IMU_ACT_PWR_EN;
        I2C_IMU_STBY_PWRMGR_REG &= (uint8) ~I2C_IMU_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable component in I2C_Wakeup() after register restore */
        I2C_IMU_backup.enableState = I2C_IMU_ENABLE;
    #endif /* (I2C_IMU_WAKEUP_ENABLED) */

        /* Restore component registers after Hibernate */
        I2C_IMU_XCFG_REG    = I2C_IMU_backup.xcfg;
        I2C_IMU_CFG_REG     = I2C_IMU_backup.cfg;
        I2C_IMU_ADDR_REG    = I2C_IMU_backup.addr;
        I2C_IMU_CLKDIV1_REG = I2C_IMU_backup.clkDiv1;
        I2C_IMU_CLKDIV2_REG = I2C_IMU_backup.clkDiv2;
    }

#if (I2C_IMU_WAKEUP_ENABLED)
    I2C_IMU_DisableInt();
    (void) CyIntSetVector(I2C_IMU_ISR_NUMBER, &I2C_IMU_ISR);
    if (0u != I2C_IMU_wakeupSource)
    {
        /* Generate interrupt to process incoming transaction */
        I2C_IMU_SetPendingInt();
    }
    I2C_IMU_EnableInt();
#endif /* (I2C_IMU_WAKEUP_ENABLED) */

#else
    I2C_IMU_CFG_REG = I2C_IMU_backup.control;
#endif /* (I2C_IMU_FF_IMPLEMENTED) */

#if (I2C_IMU_TIMEOUT_ENABLED)
    I2C_IMU_TimeoutRestoreConfig();
#endif /* (I2C_IMU_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2C_IMU_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component for active mode
*  operation (when device exits sleep mode). The Enable wakeup from Sleep Mode
*  selection influences this function implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              by calling I2C_RestoreConfig(). If the component was enabled
*              before the I2C_Sleep() function was called, I2C_Wakeup()
*              re-enables it.
*   Checked:   Enables  master functionality if it was enabled before sleep,
*              and disables the backup regulator of the I2C hardware.
*              The incoming transaction continues as soon as the regular
*              I2C interrupt handler is set up (global interrupts has to be
*              enabled to service I2C component interrupt).
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before I2C_IMU_SaveConfig() or
*  I2C_IMU_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void I2C_IMU_Wakeup(void) 
{
    I2C_IMU_RestoreConfig();

    /* Restore component enable state */
    if (0u != I2C_IMU_backup.enableState)
    {
        I2C_IMU_Enable();
        I2C_IMU_EnableInt();
    }
    else
    {
    #if (I2C_IMU_TIMEOUT_ENABLED)
        I2C_IMU_TimeoutEnable();
    #endif /* (I2C_IMU_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */

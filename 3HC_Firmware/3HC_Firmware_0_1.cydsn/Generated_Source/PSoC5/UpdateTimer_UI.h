/*******************************************************************************
* File Name: UpdateTimer_UI.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_UpdateTimer_UI_H)
#define CY_TIMER_UpdateTimer_UI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 UpdateTimer_UI_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define UpdateTimer_UI_Resolution                 8u
#define UpdateTimer_UI_UsingFixedFunction         1u
#define UpdateTimer_UI_UsingHWCaptureCounter      0u
#define UpdateTimer_UI_SoftwareCaptureMode        0u
#define UpdateTimer_UI_SoftwareTriggerMode        0u
#define UpdateTimer_UI_UsingHWEnable              0u
#define UpdateTimer_UI_EnableTriggerMode          0u
#define UpdateTimer_UI_InterruptOnCaptureCount    0u
#define UpdateTimer_UI_RunModeUsed                0u
#define UpdateTimer_UI_ControlRegRemoved          0u

#if defined(UpdateTimer_UI_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define UpdateTimer_UI_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (UpdateTimer_UI_UsingFixedFunction)
    #define UpdateTimer_UI_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define UpdateTimer_UI_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End UpdateTimer_UI_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!UpdateTimer_UI_UsingFixedFunction)

        uint8 TimerUdb;
        uint8 InterruptMaskValue;
        #if (UpdateTimer_UI_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}UpdateTimer_UI_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    UpdateTimer_UI_Start(void) ;
void    UpdateTimer_UI_Stop(void) ;

void    UpdateTimer_UI_SetInterruptMode(uint8 interruptMode) ;
uint8   UpdateTimer_UI_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define UpdateTimer_UI_GetInterruptSource() UpdateTimer_UI_ReadStatusRegister()

#if(!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED)
    uint8   UpdateTimer_UI_ReadControlRegister(void) ;
    void    UpdateTimer_UI_WriteControlRegister(uint8 control) ;
#endif /* (!UpdateTimer_UI_UDB_CONTROL_REG_REMOVED) */

uint8  UpdateTimer_UI_ReadPeriod(void) ;
void    UpdateTimer_UI_WritePeriod(uint8 period) ;
uint8  UpdateTimer_UI_ReadCounter(void) ;
void    UpdateTimer_UI_WriteCounter(uint8 counter) ;
uint8  UpdateTimer_UI_ReadCapture(void) ;
void    UpdateTimer_UI_SoftwareCapture(void) ;

#if(!UpdateTimer_UI_UsingFixedFunction) /* UDB Prototypes */
    #if (UpdateTimer_UI_SoftwareCaptureMode)
        void    UpdateTimer_UI_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!UpdateTimer_UI_UsingFixedFunction) */

    #if (UpdateTimer_UI_SoftwareTriggerMode)
        void    UpdateTimer_UI_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (UpdateTimer_UI_SoftwareTriggerMode) */

    #if (UpdateTimer_UI_EnableTriggerMode)
        void    UpdateTimer_UI_EnableTrigger(void) ;
        void    UpdateTimer_UI_DisableTrigger(void) ;
    #endif /* (UpdateTimer_UI_EnableTriggerMode) */


    #if(UpdateTimer_UI_InterruptOnCaptureCount)
        void    UpdateTimer_UI_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (UpdateTimer_UI_InterruptOnCaptureCount) */

    #if (UpdateTimer_UI_UsingHWCaptureCounter)
        void    UpdateTimer_UI_SetCaptureCount(uint8 captureCount) ;
        uint8   UpdateTimer_UI_ReadCaptureCount(void) ;
    #endif /* (UpdateTimer_UI_UsingHWCaptureCounter) */

    void UpdateTimer_UI_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void UpdateTimer_UI_Init(void)          ;
void UpdateTimer_UI_Enable(void)        ;
void UpdateTimer_UI_SaveConfig(void)    ;
void UpdateTimer_UI_RestoreConfig(void) ;
void UpdateTimer_UI_Sleep(void)         ;
void UpdateTimer_UI_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define UpdateTimer_UI__B_TIMER__CM_NONE 0
#define UpdateTimer_UI__B_TIMER__CM_RISINGEDGE 1
#define UpdateTimer_UI__B_TIMER__CM_FALLINGEDGE 2
#define UpdateTimer_UI__B_TIMER__CM_EITHEREDGE 3
#define UpdateTimer_UI__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define UpdateTimer_UI__B_TIMER__TM_NONE 0x00u
#define UpdateTimer_UI__B_TIMER__TM_RISINGEDGE 0x04u
#define UpdateTimer_UI__B_TIMER__TM_FALLINGEDGE 0x08u
#define UpdateTimer_UI__B_TIMER__TM_EITHEREDGE 0x0Cu
#define UpdateTimer_UI__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define UpdateTimer_UI_INIT_PERIOD             29u
#define UpdateTimer_UI_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << UpdateTimer_UI_CTRL_CAP_MODE_SHIFT))
#define UpdateTimer_UI_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << UpdateTimer_UI_CTRL_TRIG_MODE_SHIFT))
#if (UpdateTimer_UI_UsingFixedFunction)
    #define UpdateTimer_UI_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << UpdateTimer_UI_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << UpdateTimer_UI_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define UpdateTimer_UI_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << UpdateTimer_UI_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << UpdateTimer_UI_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << UpdateTimer_UI_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (UpdateTimer_UI_UsingFixedFunction) */
#define UpdateTimer_UI_INIT_CAPTURE_COUNT      (2u)
#define UpdateTimer_UI_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << UpdateTimer_UI_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (UpdateTimer_UI_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define UpdateTimer_UI_STATUS         (*(reg8 *) UpdateTimer_UI_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define UpdateTimer_UI_STATUS_MASK    (*(reg8 *) UpdateTimer_UI_TimerHW__SR0 )
    #define UpdateTimer_UI_CONTROL        (*(reg8 *) UpdateTimer_UI_TimerHW__CFG0)
    #define UpdateTimer_UI_CONTROL2       (*(reg8 *) UpdateTimer_UI_TimerHW__CFG1)
    #define UpdateTimer_UI_CONTROL2_PTR   ( (reg8 *) UpdateTimer_UI_TimerHW__CFG1)
    #define UpdateTimer_UI_RT1            (*(reg8 *) UpdateTimer_UI_TimerHW__RT1)
    #define UpdateTimer_UI_RT1_PTR        ( (reg8 *) UpdateTimer_UI_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define UpdateTimer_UI_CONTROL3       (*(reg8 *) UpdateTimer_UI_TimerHW__CFG2)
        #define UpdateTimer_UI_CONTROL3_PTR   ( (reg8 *) UpdateTimer_UI_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define UpdateTimer_UI_GLOBAL_ENABLE  (*(reg8 *) UpdateTimer_UI_TimerHW__PM_ACT_CFG)
    #define UpdateTimer_UI_GLOBAL_STBY_ENABLE  (*(reg8 *) UpdateTimer_UI_TimerHW__PM_STBY_CFG)

    #define UpdateTimer_UI_CAPTURE_LSB         (* (reg16 *) UpdateTimer_UI_TimerHW__CAP0 )
    #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg16 *) UpdateTimer_UI_TimerHW__CAP0 )
    #define UpdateTimer_UI_PERIOD_LSB          (* (reg16 *) UpdateTimer_UI_TimerHW__PER0 )
    #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg16 *) UpdateTimer_UI_TimerHW__PER0 )
    #define UpdateTimer_UI_COUNTER_LSB         (* (reg16 *) UpdateTimer_UI_TimerHW__CNT_CMP0 )
    #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg16 *) UpdateTimer_UI_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define UpdateTimer_UI_BLOCK_EN_MASK                     UpdateTimer_UI_TimerHW__PM_ACT_MSK
    #define UpdateTimer_UI_BLOCK_STBY_EN_MASK                UpdateTimer_UI_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define UpdateTimer_UI_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define UpdateTimer_UI_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define UpdateTimer_UI_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define UpdateTimer_UI_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define UpdateTimer_UI_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define UpdateTimer_UI_CTRL_ENABLE                        ((uint8)((uint8)0x01u << UpdateTimer_UI_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define UpdateTimer_UI_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define UpdateTimer_UI_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << UpdateTimer_UI_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define UpdateTimer_UI_CTRL_MODE_SHIFT                 0x01u
        #define UpdateTimer_UI_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << UpdateTimer_UI_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define UpdateTimer_UI_CTRL_RCOD_SHIFT        0x02u
        #define UpdateTimer_UI_CTRL_ENBL_SHIFT        0x00u
        #define UpdateTimer_UI_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define UpdateTimer_UI_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << UpdateTimer_UI_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define UpdateTimer_UI_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << UpdateTimer_UI_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define UpdateTimer_UI_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << UpdateTimer_UI_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define UpdateTimer_UI_CTRL_RCOD       ((uint8)((uint8)0x03u << UpdateTimer_UI_CTRL_RCOD_SHIFT))
        #define UpdateTimer_UI_CTRL_ENBL       ((uint8)((uint8)0x80u << UpdateTimer_UI_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define UpdateTimer_UI_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define UpdateTimer_UI_RT1_MASK                        ((uint8)((uint8)0x03u << UpdateTimer_UI_RT1_SHIFT))
    #define UpdateTimer_UI_SYNC                            ((uint8)((uint8)0x03u << UpdateTimer_UI_RT1_SHIFT))
    #define UpdateTimer_UI_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define UpdateTimer_UI_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << UpdateTimer_UI_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define UpdateTimer_UI_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << UpdateTimer_UI_SYNCDSI_SHIFT))

    #define UpdateTimer_UI_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << UpdateTimer_UI_CTRL_MODE_SHIFT))
    #define UpdateTimer_UI_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << UpdateTimer_UI_CTRL_MODE_SHIFT))
    #define UpdateTimer_UI_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << UpdateTimer_UI_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define UpdateTimer_UI_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define UpdateTimer_UI_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define UpdateTimer_UI_STATUS_TC_INT_MASK_SHIFT        (UpdateTimer_UI_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define UpdateTimer_UI_STATUS_CAPTURE_INT_MASK_SHIFT   (UpdateTimer_UI_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define UpdateTimer_UI_STATUS_TC                       ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_TC_SHIFT))
    #define UpdateTimer_UI_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define UpdateTimer_UI_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define UpdateTimer_UI_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define UpdateTimer_UI_STATUS              (* (reg8 *) UpdateTimer_UI_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define UpdateTimer_UI_STATUS_MASK         (* (reg8 *) UpdateTimer_UI_TimerUDB_rstSts_stsreg__MASK_REG)
    #define UpdateTimer_UI_STATUS_AUX_CTRL     (* (reg8 *) UpdateTimer_UI_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define UpdateTimer_UI_CONTROL             (* (reg8 *) UpdateTimer_UI_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(UpdateTimer_UI_Resolution <= 8u) /* 8-bit Timer */
        #define UpdateTimer_UI_CAPTURE_LSB         (* (reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define UpdateTimer_UI_PERIOD_LSB          (* (reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define UpdateTimer_UI_COUNTER_LSB         (* (reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(UpdateTimer_UI_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define UpdateTimer_UI_CAPTURE_LSB         (* (reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define UpdateTimer_UI_PERIOD_LSB          (* (reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define UpdateTimer_UI_COUNTER_LSB         (* (reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define UpdateTimer_UI_CAPTURE_LSB         (* (reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define UpdateTimer_UI_PERIOD_LSB          (* (reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define UpdateTimer_UI_COUNTER_LSB         (* (reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg16 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(UpdateTimer_UI_Resolution <= 24u)/* 24-bit Timer */
        #define UpdateTimer_UI_CAPTURE_LSB         (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define UpdateTimer_UI_PERIOD_LSB          (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define UpdateTimer_UI_COUNTER_LSB         (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define UpdateTimer_UI_CAPTURE_LSB         (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define UpdateTimer_UI_PERIOD_LSB          (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define UpdateTimer_UI_COUNTER_LSB         (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define UpdateTimer_UI_CAPTURE_LSB         (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define UpdateTimer_UI_CAPTURE_LSB_PTR       ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define UpdateTimer_UI_PERIOD_LSB          (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define UpdateTimer_UI_PERIOD_LSB_PTR        ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define UpdateTimer_UI_COUNTER_LSB         (* (reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define UpdateTimer_UI_COUNTER_LSB_PTR       ((reg32 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define UpdateTimer_UI_COUNTER_LSB_PTR_8BIT       ((reg8 *) UpdateTimer_UI_TimerUDB_sT8_timerdp_u0__A0_REG )
    
    #if (UpdateTimer_UI_UsingHWCaptureCounter)
        #define UpdateTimer_UI_CAP_COUNT              (*(reg8 *) UpdateTimer_UI_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define UpdateTimer_UI_CAP_COUNT_PTR          ( (reg8 *) UpdateTimer_UI_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define UpdateTimer_UI_CAPTURE_COUNT_CTRL     (*(reg8 *) UpdateTimer_UI_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define UpdateTimer_UI_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) UpdateTimer_UI_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (UpdateTimer_UI_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define UpdateTimer_UI_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define UpdateTimer_UI_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << UpdateTimer_UI_CTRL_INTCNT_SHIFT))
    #define UpdateTimer_UI_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << UpdateTimer_UI_CTRL_TRIG_MODE_SHIFT))
    #define UpdateTimer_UI_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << UpdateTimer_UI_CTRL_TRIG_EN_SHIFT))
    #define UpdateTimer_UI_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << UpdateTimer_UI_CTRL_CAP_MODE_SHIFT))
    #define UpdateTimer_UI_CTRL_ENABLE                    ((uint8)((uint8)0x01u << UpdateTimer_UI_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define UpdateTimer_UI_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define UpdateTimer_UI_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_STATUS_TC_INT_MASK_SHIFT       UpdateTimer_UI_STATUS_TC_SHIFT
    #define UpdateTimer_UI_STATUS_CAPTURE_INT_MASK_SHIFT  UpdateTimer_UI_STATUS_CAPTURE_SHIFT
    #define UpdateTimer_UI_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define UpdateTimer_UI_STATUS_FIFOFULL_INT_MASK_SHIFT UpdateTimer_UI_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define UpdateTimer_UI_STATUS_TC                      ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define UpdateTimer_UI_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define UpdateTimer_UI_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define UpdateTimer_UI_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define UpdateTimer_UI_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define UpdateTimer_UI_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define UpdateTimer_UI_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << UpdateTimer_UI_STATUS_FIFOFULL_SHIFT))

    #define UpdateTimer_UI_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define UpdateTimer_UI_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define UpdateTimer_UI_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define UpdateTimer_UI_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define UpdateTimer_UI_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define UpdateTimer_UI_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_UpdateTimer_UI_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: DelayTimer.h
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

#if !defined(CY_TIMER_DelayTimer_H)
#define CY_TIMER_DelayTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 DelayTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define DelayTimer_Resolution                 16u
#define DelayTimer_UsingFixedFunction         0u
#define DelayTimer_UsingHWCaptureCounter      0u
#define DelayTimer_SoftwareCaptureMode        0u
#define DelayTimer_SoftwareTriggerMode        0u
#define DelayTimer_UsingHWEnable              0u
#define DelayTimer_EnableTriggerMode          1u
#define DelayTimer_InterruptOnCaptureCount    0u
#define DelayTimer_RunModeUsed                1u
#define DelayTimer_ControlRegRemoved          0u

#if defined(DelayTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define DelayTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (DelayTimer_UsingFixedFunction)
    #define DelayTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define DelayTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End DelayTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!DelayTimer_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (DelayTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!DelayTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}DelayTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    DelayTimer_Start(void) ;
void    DelayTimer_Stop(void) ;

void    DelayTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   DelayTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define DelayTimer_GetInterruptSource() DelayTimer_ReadStatusRegister()

#if(!DelayTimer_UDB_CONTROL_REG_REMOVED)
    uint8   DelayTimer_ReadControlRegister(void) ;
    void    DelayTimer_WriteControlRegister(uint8 control) ;
#endif /* (!DelayTimer_UDB_CONTROL_REG_REMOVED) */

uint16  DelayTimer_ReadPeriod(void) ;
void    DelayTimer_WritePeriod(uint16 period) ;
uint16  DelayTimer_ReadCounter(void) ;
void    DelayTimer_WriteCounter(uint16 counter) ;
uint16  DelayTimer_ReadCapture(void) ;
void    DelayTimer_SoftwareCapture(void) ;

#if(!DelayTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (DelayTimer_SoftwareCaptureMode)
        void    DelayTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!DelayTimer_UsingFixedFunction) */

    #if (DelayTimer_SoftwareTriggerMode)
        void    DelayTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (DelayTimer_SoftwareTriggerMode) */

    #if (DelayTimer_EnableTriggerMode)
        void    DelayTimer_EnableTrigger(void) ;
        void    DelayTimer_DisableTrigger(void) ;
    #endif /* (DelayTimer_EnableTriggerMode) */


    #if(DelayTimer_InterruptOnCaptureCount)
        void    DelayTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (DelayTimer_InterruptOnCaptureCount) */

    #if (DelayTimer_UsingHWCaptureCounter)
        void    DelayTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   DelayTimer_ReadCaptureCount(void) ;
    #endif /* (DelayTimer_UsingHWCaptureCounter) */

    void DelayTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void DelayTimer_Init(void)          ;
void DelayTimer_Enable(void)        ;
void DelayTimer_SaveConfig(void)    ;
void DelayTimer_RestoreConfig(void) ;
void DelayTimer_Sleep(void)         ;
void DelayTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define DelayTimer__B_TIMER__CM_NONE 0
#define DelayTimer__B_TIMER__CM_RISINGEDGE 1
#define DelayTimer__B_TIMER__CM_FALLINGEDGE 2
#define DelayTimer__B_TIMER__CM_EITHEREDGE 3
#define DelayTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define DelayTimer__B_TIMER__TM_NONE 0x00u
#define DelayTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define DelayTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define DelayTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define DelayTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define DelayTimer_INIT_PERIOD             99u
#define DelayTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << DelayTimer_CTRL_CAP_MODE_SHIFT))
#define DelayTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)1u << DelayTimer_CTRL_TRIG_MODE_SHIFT))
#if (DelayTimer_UsingFixedFunction)
    #define DelayTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << DelayTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << DelayTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define DelayTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << DelayTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << DelayTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << DelayTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (DelayTimer_UsingFixedFunction) */
#define DelayTimer_INIT_CAPTURE_COUNT      (2u)
#define DelayTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << DelayTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (DelayTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define DelayTimer_STATUS         (*(reg8 *) DelayTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define DelayTimer_STATUS_MASK    (*(reg8 *) DelayTimer_TimerHW__SR0 )
    #define DelayTimer_CONTROL        (*(reg8 *) DelayTimer_TimerHW__CFG0)
    #define DelayTimer_CONTROL2       (*(reg8 *) DelayTimer_TimerHW__CFG1)
    #define DelayTimer_CONTROL2_PTR   ( (reg8 *) DelayTimer_TimerHW__CFG1)
    #define DelayTimer_RT1            (*(reg8 *) DelayTimer_TimerHW__RT1)
    #define DelayTimer_RT1_PTR        ( (reg8 *) DelayTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define DelayTimer_CONTROL3       (*(reg8 *) DelayTimer_TimerHW__CFG2)
        #define DelayTimer_CONTROL3_PTR   ( (reg8 *) DelayTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define DelayTimer_GLOBAL_ENABLE  (*(reg8 *) DelayTimer_TimerHW__PM_ACT_CFG)
    #define DelayTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) DelayTimer_TimerHW__PM_STBY_CFG)

    #define DelayTimer_CAPTURE_LSB         (* (reg16 *) DelayTimer_TimerHW__CAP0 )
    #define DelayTimer_CAPTURE_LSB_PTR       ((reg16 *) DelayTimer_TimerHW__CAP0 )
    #define DelayTimer_PERIOD_LSB          (* (reg16 *) DelayTimer_TimerHW__PER0 )
    #define DelayTimer_PERIOD_LSB_PTR        ((reg16 *) DelayTimer_TimerHW__PER0 )
    #define DelayTimer_COUNTER_LSB         (* (reg16 *) DelayTimer_TimerHW__CNT_CMP0 )
    #define DelayTimer_COUNTER_LSB_PTR       ((reg16 *) DelayTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define DelayTimer_BLOCK_EN_MASK                     DelayTimer_TimerHW__PM_ACT_MSK
    #define DelayTimer_BLOCK_STBY_EN_MASK                DelayTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define DelayTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define DelayTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define DelayTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define DelayTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define DelayTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define DelayTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << DelayTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define DelayTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define DelayTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << DelayTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define DelayTimer_CTRL_MODE_SHIFT                 0x01u
        #define DelayTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << DelayTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define DelayTimer_CTRL_RCOD_SHIFT        0x02u
        #define DelayTimer_CTRL_ENBL_SHIFT        0x00u
        #define DelayTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define DelayTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << DelayTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define DelayTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << DelayTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define DelayTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << DelayTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define DelayTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << DelayTimer_CTRL_RCOD_SHIFT))
        #define DelayTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << DelayTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define DelayTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define DelayTimer_RT1_MASK                        ((uint8)((uint8)0x03u << DelayTimer_RT1_SHIFT))
    #define DelayTimer_SYNC                            ((uint8)((uint8)0x03u << DelayTimer_RT1_SHIFT))
    #define DelayTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define DelayTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << DelayTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define DelayTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << DelayTimer_SYNCDSI_SHIFT))

    #define DelayTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << DelayTimer_CTRL_MODE_SHIFT))
    #define DelayTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << DelayTimer_CTRL_MODE_SHIFT))
    #define DelayTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << DelayTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define DelayTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define DelayTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define DelayTimer_STATUS_TC_INT_MASK_SHIFT        (DelayTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define DelayTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (DelayTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define DelayTimer_STATUS_TC                       ((uint8)((uint8)0x01u << DelayTimer_STATUS_TC_SHIFT))
    #define DelayTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << DelayTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define DelayTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << DelayTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define DelayTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << DelayTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define DelayTimer_STATUS              (* (reg8 *) DelayTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define DelayTimer_STATUS_MASK         (* (reg8 *) DelayTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define DelayTimer_STATUS_AUX_CTRL     (* (reg8 *) DelayTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define DelayTimer_CONTROL             (* (reg8 *) DelayTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(DelayTimer_Resolution <= 8u) /* 8-bit Timer */
        #define DelayTimer_CAPTURE_LSB         (* (reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define DelayTimer_CAPTURE_LSB_PTR       ((reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define DelayTimer_PERIOD_LSB          (* (reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define DelayTimer_PERIOD_LSB_PTR        ((reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define DelayTimer_COUNTER_LSB         (* (reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define DelayTimer_COUNTER_LSB_PTR       ((reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(DelayTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define DelayTimer_CAPTURE_LSB         (* (reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define DelayTimer_CAPTURE_LSB_PTR       ((reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define DelayTimer_PERIOD_LSB          (* (reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define DelayTimer_PERIOD_LSB_PTR        ((reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define DelayTimer_COUNTER_LSB         (* (reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define DelayTimer_COUNTER_LSB_PTR       ((reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define DelayTimer_CAPTURE_LSB         (* (reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define DelayTimer_CAPTURE_LSB_PTR       ((reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define DelayTimer_PERIOD_LSB          (* (reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define DelayTimer_PERIOD_LSB_PTR        ((reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define DelayTimer_COUNTER_LSB         (* (reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define DelayTimer_COUNTER_LSB_PTR       ((reg16 *) DelayTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(DelayTimer_Resolution <= 24u)/* 24-bit Timer */
        #define DelayTimer_CAPTURE_LSB         (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define DelayTimer_CAPTURE_LSB_PTR       ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define DelayTimer_PERIOD_LSB          (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define DelayTimer_PERIOD_LSB_PTR        ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define DelayTimer_COUNTER_LSB         (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define DelayTimer_COUNTER_LSB_PTR       ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define DelayTimer_CAPTURE_LSB         (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define DelayTimer_CAPTURE_LSB_PTR       ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define DelayTimer_PERIOD_LSB          (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define DelayTimer_PERIOD_LSB_PTR        ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define DelayTimer_COUNTER_LSB         (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define DelayTimer_COUNTER_LSB_PTR       ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define DelayTimer_CAPTURE_LSB         (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define DelayTimer_CAPTURE_LSB_PTR       ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define DelayTimer_PERIOD_LSB          (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define DelayTimer_PERIOD_LSB_PTR        ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define DelayTimer_COUNTER_LSB         (* (reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define DelayTimer_COUNTER_LSB_PTR       ((reg32 *) DelayTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define DelayTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) DelayTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (DelayTimer_UsingHWCaptureCounter)
        #define DelayTimer_CAP_COUNT              (*(reg8 *) DelayTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define DelayTimer_CAP_COUNT_PTR          ( (reg8 *) DelayTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define DelayTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) DelayTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define DelayTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) DelayTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (DelayTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define DelayTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define DelayTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define DelayTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define DelayTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define DelayTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define DelayTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << DelayTimer_CTRL_INTCNT_SHIFT))
    #define DelayTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << DelayTimer_CTRL_TRIG_MODE_SHIFT))
    #define DelayTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << DelayTimer_CTRL_TRIG_EN_SHIFT))
    #define DelayTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << DelayTimer_CTRL_CAP_MODE_SHIFT))
    #define DelayTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << DelayTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define DelayTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define DelayTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define DelayTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define DelayTimer_STATUS_TC_INT_MASK_SHIFT       DelayTimer_STATUS_TC_SHIFT
    #define DelayTimer_STATUS_CAPTURE_INT_MASK_SHIFT  DelayTimer_STATUS_CAPTURE_SHIFT
    #define DelayTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define DelayTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define DelayTimer_STATUS_FIFOFULL_INT_MASK_SHIFT DelayTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define DelayTimer_STATUS_TC                      ((uint8)((uint8)0x01u << DelayTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define DelayTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << DelayTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define DelayTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << DelayTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define DelayTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << DelayTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define DelayTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << DelayTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define DelayTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << DelayTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define DelayTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << DelayTimer_STATUS_FIFOFULL_SHIFT))

    #define DelayTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define DelayTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define DelayTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define DelayTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define DelayTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define DelayTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_DelayTimer_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: ValveTimer.h
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

#if !defined(CY_TIMER_ValveTimer_H)
#define CY_TIMER_ValveTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 ValveTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define ValveTimer_Resolution                 16u
#define ValveTimer_UsingFixedFunction         0u
#define ValveTimer_UsingHWCaptureCounter      0u
#define ValveTimer_SoftwareCaptureMode        0u
#define ValveTimer_SoftwareTriggerMode        0u
#define ValveTimer_UsingHWEnable              0u
#define ValveTimer_EnableTriggerMode          1u
#define ValveTimer_InterruptOnCaptureCount    0u
#define ValveTimer_RunModeUsed                1u
#define ValveTimer_ControlRegRemoved          0u

#if defined(ValveTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define ValveTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (ValveTimer_UsingFixedFunction)
    #define ValveTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define ValveTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End ValveTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!ValveTimer_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (ValveTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!ValveTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}ValveTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    ValveTimer_Start(void) ;
void    ValveTimer_Stop(void) ;

void    ValveTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   ValveTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define ValveTimer_GetInterruptSource() ValveTimer_ReadStatusRegister()

#if(!ValveTimer_UDB_CONTROL_REG_REMOVED)
    uint8   ValveTimer_ReadControlRegister(void) ;
    void    ValveTimer_WriteControlRegister(uint8 control) ;
#endif /* (!ValveTimer_UDB_CONTROL_REG_REMOVED) */

uint16  ValveTimer_ReadPeriod(void) ;
void    ValveTimer_WritePeriod(uint16 period) ;
uint16  ValveTimer_ReadCounter(void) ;
void    ValveTimer_WriteCounter(uint16 counter) ;
uint16  ValveTimer_ReadCapture(void) ;
void    ValveTimer_SoftwareCapture(void) ;

#if(!ValveTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (ValveTimer_SoftwareCaptureMode)
        void    ValveTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!ValveTimer_UsingFixedFunction) */

    #if (ValveTimer_SoftwareTriggerMode)
        void    ValveTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (ValveTimer_SoftwareTriggerMode) */

    #if (ValveTimer_EnableTriggerMode)
        void    ValveTimer_EnableTrigger(void) ;
        void    ValveTimer_DisableTrigger(void) ;
    #endif /* (ValveTimer_EnableTriggerMode) */


    #if(ValveTimer_InterruptOnCaptureCount)
        void    ValveTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (ValveTimer_InterruptOnCaptureCount) */

    #if (ValveTimer_UsingHWCaptureCounter)
        void    ValveTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   ValveTimer_ReadCaptureCount(void) ;
    #endif /* (ValveTimer_UsingHWCaptureCounter) */

    void ValveTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void ValveTimer_Init(void)          ;
void ValveTimer_Enable(void)        ;
void ValveTimer_SaveConfig(void)    ;
void ValveTimer_RestoreConfig(void) ;
void ValveTimer_Sleep(void)         ;
void ValveTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define ValveTimer__B_TIMER__CM_NONE 0
#define ValveTimer__B_TIMER__CM_RISINGEDGE 1
#define ValveTimer__B_TIMER__CM_FALLINGEDGE 2
#define ValveTimer__B_TIMER__CM_EITHEREDGE 3
#define ValveTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define ValveTimer__B_TIMER__TM_NONE 0x00u
#define ValveTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define ValveTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define ValveTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define ValveTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define ValveTimer_INIT_PERIOD             86u
#define ValveTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << ValveTimer_CTRL_CAP_MODE_SHIFT))
#define ValveTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)1u << ValveTimer_CTRL_TRIG_MODE_SHIFT))
#if (ValveTimer_UsingFixedFunction)
    #define ValveTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << ValveTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << ValveTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define ValveTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << ValveTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << ValveTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << ValveTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (ValveTimer_UsingFixedFunction) */
#define ValveTimer_INIT_CAPTURE_COUNT      (2u)
#define ValveTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << ValveTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (ValveTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define ValveTimer_STATUS         (*(reg8 *) ValveTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define ValveTimer_STATUS_MASK    (*(reg8 *) ValveTimer_TimerHW__SR0 )
    #define ValveTimer_CONTROL        (*(reg8 *) ValveTimer_TimerHW__CFG0)
    #define ValveTimer_CONTROL2       (*(reg8 *) ValveTimer_TimerHW__CFG1)
    #define ValveTimer_CONTROL2_PTR   ( (reg8 *) ValveTimer_TimerHW__CFG1)
    #define ValveTimer_RT1            (*(reg8 *) ValveTimer_TimerHW__RT1)
    #define ValveTimer_RT1_PTR        ( (reg8 *) ValveTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define ValveTimer_CONTROL3       (*(reg8 *) ValveTimer_TimerHW__CFG2)
        #define ValveTimer_CONTROL3_PTR   ( (reg8 *) ValveTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define ValveTimer_GLOBAL_ENABLE  (*(reg8 *) ValveTimer_TimerHW__PM_ACT_CFG)
    #define ValveTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) ValveTimer_TimerHW__PM_STBY_CFG)

    #define ValveTimer_CAPTURE_LSB         (* (reg16 *) ValveTimer_TimerHW__CAP0 )
    #define ValveTimer_CAPTURE_LSB_PTR       ((reg16 *) ValveTimer_TimerHW__CAP0 )
    #define ValveTimer_PERIOD_LSB          (* (reg16 *) ValveTimer_TimerHW__PER0 )
    #define ValveTimer_PERIOD_LSB_PTR        ((reg16 *) ValveTimer_TimerHW__PER0 )
    #define ValveTimer_COUNTER_LSB         (* (reg16 *) ValveTimer_TimerHW__CNT_CMP0 )
    #define ValveTimer_COUNTER_LSB_PTR       ((reg16 *) ValveTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define ValveTimer_BLOCK_EN_MASK                     ValveTimer_TimerHW__PM_ACT_MSK
    #define ValveTimer_BLOCK_STBY_EN_MASK                ValveTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define ValveTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define ValveTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define ValveTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define ValveTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define ValveTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define ValveTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << ValveTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define ValveTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define ValveTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << ValveTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define ValveTimer_CTRL_MODE_SHIFT                 0x01u
        #define ValveTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << ValveTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define ValveTimer_CTRL_RCOD_SHIFT        0x02u
        #define ValveTimer_CTRL_ENBL_SHIFT        0x00u
        #define ValveTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define ValveTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << ValveTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define ValveTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << ValveTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define ValveTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << ValveTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define ValveTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << ValveTimer_CTRL_RCOD_SHIFT))
        #define ValveTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << ValveTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define ValveTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define ValveTimer_RT1_MASK                        ((uint8)((uint8)0x03u << ValveTimer_RT1_SHIFT))
    #define ValveTimer_SYNC                            ((uint8)((uint8)0x03u << ValveTimer_RT1_SHIFT))
    #define ValveTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define ValveTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << ValveTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define ValveTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << ValveTimer_SYNCDSI_SHIFT))

    #define ValveTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << ValveTimer_CTRL_MODE_SHIFT))
    #define ValveTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << ValveTimer_CTRL_MODE_SHIFT))
    #define ValveTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << ValveTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define ValveTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define ValveTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define ValveTimer_STATUS_TC_INT_MASK_SHIFT        (ValveTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define ValveTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (ValveTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define ValveTimer_STATUS_TC                       ((uint8)((uint8)0x01u << ValveTimer_STATUS_TC_SHIFT))
    #define ValveTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << ValveTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define ValveTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << ValveTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define ValveTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << ValveTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define ValveTimer_STATUS              (* (reg8 *) ValveTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define ValveTimer_STATUS_MASK         (* (reg8 *) ValveTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define ValveTimer_STATUS_AUX_CTRL     (* (reg8 *) ValveTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define ValveTimer_CONTROL             (* (reg8 *) ValveTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(ValveTimer_Resolution <= 8u) /* 8-bit Timer */
        #define ValveTimer_CAPTURE_LSB         (* (reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define ValveTimer_CAPTURE_LSB_PTR       ((reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define ValveTimer_PERIOD_LSB          (* (reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define ValveTimer_PERIOD_LSB_PTR        ((reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define ValveTimer_COUNTER_LSB         (* (reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define ValveTimer_COUNTER_LSB_PTR       ((reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(ValveTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define ValveTimer_CAPTURE_LSB         (* (reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define ValveTimer_CAPTURE_LSB_PTR       ((reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define ValveTimer_PERIOD_LSB          (* (reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define ValveTimer_PERIOD_LSB_PTR        ((reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define ValveTimer_COUNTER_LSB         (* (reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define ValveTimer_COUNTER_LSB_PTR       ((reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define ValveTimer_CAPTURE_LSB         (* (reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define ValveTimer_CAPTURE_LSB_PTR       ((reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define ValveTimer_PERIOD_LSB          (* (reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define ValveTimer_PERIOD_LSB_PTR        ((reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define ValveTimer_COUNTER_LSB         (* (reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define ValveTimer_COUNTER_LSB_PTR       ((reg16 *) ValveTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(ValveTimer_Resolution <= 24u)/* 24-bit Timer */
        #define ValveTimer_CAPTURE_LSB         (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define ValveTimer_CAPTURE_LSB_PTR       ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define ValveTimer_PERIOD_LSB          (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define ValveTimer_PERIOD_LSB_PTR        ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define ValveTimer_COUNTER_LSB         (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define ValveTimer_COUNTER_LSB_PTR       ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define ValveTimer_CAPTURE_LSB         (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define ValveTimer_CAPTURE_LSB_PTR       ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define ValveTimer_PERIOD_LSB          (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define ValveTimer_PERIOD_LSB_PTR        ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define ValveTimer_COUNTER_LSB         (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define ValveTimer_COUNTER_LSB_PTR       ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define ValveTimer_CAPTURE_LSB         (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define ValveTimer_CAPTURE_LSB_PTR       ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define ValveTimer_PERIOD_LSB          (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define ValveTimer_PERIOD_LSB_PTR        ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define ValveTimer_COUNTER_LSB         (* (reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define ValveTimer_COUNTER_LSB_PTR       ((reg32 *) ValveTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define ValveTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) ValveTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (ValveTimer_UsingHWCaptureCounter)
        #define ValveTimer_CAP_COUNT              (*(reg8 *) ValveTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define ValveTimer_CAP_COUNT_PTR          ( (reg8 *) ValveTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define ValveTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) ValveTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define ValveTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) ValveTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (ValveTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define ValveTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define ValveTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define ValveTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define ValveTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define ValveTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define ValveTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << ValveTimer_CTRL_INTCNT_SHIFT))
    #define ValveTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << ValveTimer_CTRL_TRIG_MODE_SHIFT))
    #define ValveTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << ValveTimer_CTRL_TRIG_EN_SHIFT))
    #define ValveTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << ValveTimer_CTRL_CAP_MODE_SHIFT))
    #define ValveTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << ValveTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define ValveTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define ValveTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define ValveTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define ValveTimer_STATUS_TC_INT_MASK_SHIFT       ValveTimer_STATUS_TC_SHIFT
    #define ValveTimer_STATUS_CAPTURE_INT_MASK_SHIFT  ValveTimer_STATUS_CAPTURE_SHIFT
    #define ValveTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define ValveTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define ValveTimer_STATUS_FIFOFULL_INT_MASK_SHIFT ValveTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define ValveTimer_STATUS_TC                      ((uint8)((uint8)0x01u << ValveTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define ValveTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << ValveTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define ValveTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << ValveTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define ValveTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << ValveTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define ValveTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << ValveTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define ValveTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << ValveTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define ValveTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << ValveTimer_STATUS_FIFOFULL_SHIFT))

    #define ValveTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define ValveTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define ValveTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define ValveTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define ValveTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define ValveTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_ValveTimer_H */


/* [] END OF FILE */

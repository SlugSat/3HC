/*******************************************************************************
* File Name: Z_PWM_OUT.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Z_PWM_OUT_H) /* Pins Z_PWM_OUT_H */
#define CY_PINS_Z_PWM_OUT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Z_PWM_OUT_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Z_PWM_OUT__PORT == 15 && ((Z_PWM_OUT__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Z_PWM_OUT_Write(uint8 value);
void    Z_PWM_OUT_SetDriveMode(uint8 mode);
uint8   Z_PWM_OUT_ReadDataReg(void);
uint8   Z_PWM_OUT_Read(void);
void    Z_PWM_OUT_SetInterruptMode(uint16 position, uint16 mode);
uint8   Z_PWM_OUT_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Z_PWM_OUT_SetDriveMode() function.
     *  @{
     */
        #define Z_PWM_OUT_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Z_PWM_OUT_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Z_PWM_OUT_DM_RES_UP          PIN_DM_RES_UP
        #define Z_PWM_OUT_DM_RES_DWN         PIN_DM_RES_DWN
        #define Z_PWM_OUT_DM_OD_LO           PIN_DM_OD_LO
        #define Z_PWM_OUT_DM_OD_HI           PIN_DM_OD_HI
        #define Z_PWM_OUT_DM_STRONG          PIN_DM_STRONG
        #define Z_PWM_OUT_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Z_PWM_OUT_MASK               Z_PWM_OUT__MASK
#define Z_PWM_OUT_SHIFT              Z_PWM_OUT__SHIFT
#define Z_PWM_OUT_WIDTH              1u

/* Interrupt constants */
#if defined(Z_PWM_OUT__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Z_PWM_OUT_SetInterruptMode() function.
     *  @{
     */
        #define Z_PWM_OUT_INTR_NONE      (uint16)(0x0000u)
        #define Z_PWM_OUT_INTR_RISING    (uint16)(0x0001u)
        #define Z_PWM_OUT_INTR_FALLING   (uint16)(0x0002u)
        #define Z_PWM_OUT_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Z_PWM_OUT_INTR_MASK      (0x01u) 
#endif /* (Z_PWM_OUT__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Z_PWM_OUT_PS                     (* (reg8 *) Z_PWM_OUT__PS)
/* Data Register */
#define Z_PWM_OUT_DR                     (* (reg8 *) Z_PWM_OUT__DR)
/* Port Number */
#define Z_PWM_OUT_PRT_NUM                (* (reg8 *) Z_PWM_OUT__PRT) 
/* Connect to Analog Globals */                                                  
#define Z_PWM_OUT_AG                     (* (reg8 *) Z_PWM_OUT__AG)                       
/* Analog MUX bux enable */
#define Z_PWM_OUT_AMUX                   (* (reg8 *) Z_PWM_OUT__AMUX) 
/* Bidirectional Enable */                                                        
#define Z_PWM_OUT_BIE                    (* (reg8 *) Z_PWM_OUT__BIE)
/* Bit-mask for Aliased Register Access */
#define Z_PWM_OUT_BIT_MASK               (* (reg8 *) Z_PWM_OUT__BIT_MASK)
/* Bypass Enable */
#define Z_PWM_OUT_BYP                    (* (reg8 *) Z_PWM_OUT__BYP)
/* Port wide control signals */                                                   
#define Z_PWM_OUT_CTL                    (* (reg8 *) Z_PWM_OUT__CTL)
/* Drive Modes */
#define Z_PWM_OUT_DM0                    (* (reg8 *) Z_PWM_OUT__DM0) 
#define Z_PWM_OUT_DM1                    (* (reg8 *) Z_PWM_OUT__DM1)
#define Z_PWM_OUT_DM2                    (* (reg8 *) Z_PWM_OUT__DM2) 
/* Input Buffer Disable Override */
#define Z_PWM_OUT_INP_DIS                (* (reg8 *) Z_PWM_OUT__INP_DIS)
/* LCD Common or Segment Drive */
#define Z_PWM_OUT_LCD_COM_SEG            (* (reg8 *) Z_PWM_OUT__LCD_COM_SEG)
/* Enable Segment LCD */
#define Z_PWM_OUT_LCD_EN                 (* (reg8 *) Z_PWM_OUT__LCD_EN)
/* Slew Rate Control */
#define Z_PWM_OUT_SLW                    (* (reg8 *) Z_PWM_OUT__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Z_PWM_OUT_PRTDSI__CAPS_SEL       (* (reg8 *) Z_PWM_OUT__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Z_PWM_OUT_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Z_PWM_OUT__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Z_PWM_OUT_PRTDSI__OE_SEL0        (* (reg8 *) Z_PWM_OUT__PRTDSI__OE_SEL0) 
#define Z_PWM_OUT_PRTDSI__OE_SEL1        (* (reg8 *) Z_PWM_OUT__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Z_PWM_OUT_PRTDSI__OUT_SEL0       (* (reg8 *) Z_PWM_OUT__PRTDSI__OUT_SEL0) 
#define Z_PWM_OUT_PRTDSI__OUT_SEL1       (* (reg8 *) Z_PWM_OUT__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Z_PWM_OUT_PRTDSI__SYNC_OUT       (* (reg8 *) Z_PWM_OUT__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Z_PWM_OUT__SIO_CFG)
    #define Z_PWM_OUT_SIO_HYST_EN        (* (reg8 *) Z_PWM_OUT__SIO_HYST_EN)
    #define Z_PWM_OUT_SIO_REG_HIFREQ     (* (reg8 *) Z_PWM_OUT__SIO_REG_HIFREQ)
    #define Z_PWM_OUT_SIO_CFG            (* (reg8 *) Z_PWM_OUT__SIO_CFG)
    #define Z_PWM_OUT_SIO_DIFF           (* (reg8 *) Z_PWM_OUT__SIO_DIFF)
#endif /* (Z_PWM_OUT__SIO_CFG) */

/* Interrupt Registers */
#if defined(Z_PWM_OUT__INTSTAT)
    #define Z_PWM_OUT_INTSTAT            (* (reg8 *) Z_PWM_OUT__INTSTAT)
    #define Z_PWM_OUT_SNAP               (* (reg8 *) Z_PWM_OUT__SNAP)
    
	#define Z_PWM_OUT_0_INTTYPE_REG 		(* (reg8 *) Z_PWM_OUT__0__INTTYPE)
#endif /* (Z_PWM_OUT__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Z_PWM_OUT_H */


/* [] END OF FILE */

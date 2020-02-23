/*******************************************************************************
* File Name: X_CURR.h  
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

#if !defined(CY_PINS_X_CURR_H) /* Pins X_CURR_H */
#define CY_PINS_X_CURR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "X_CURR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 X_CURR__PORT == 15 && ((X_CURR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    X_CURR_Write(uint8 value);
void    X_CURR_SetDriveMode(uint8 mode);
uint8   X_CURR_ReadDataReg(void);
uint8   X_CURR_Read(void);
void    X_CURR_SetInterruptMode(uint16 position, uint16 mode);
uint8   X_CURR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the X_CURR_SetDriveMode() function.
     *  @{
     */
        #define X_CURR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define X_CURR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define X_CURR_DM_RES_UP          PIN_DM_RES_UP
        #define X_CURR_DM_RES_DWN         PIN_DM_RES_DWN
        #define X_CURR_DM_OD_LO           PIN_DM_OD_LO
        #define X_CURR_DM_OD_HI           PIN_DM_OD_HI
        #define X_CURR_DM_STRONG          PIN_DM_STRONG
        #define X_CURR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define X_CURR_MASK               X_CURR__MASK
#define X_CURR_SHIFT              X_CURR__SHIFT
#define X_CURR_WIDTH              1u

/* Interrupt constants */
#if defined(X_CURR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in X_CURR_SetInterruptMode() function.
     *  @{
     */
        #define X_CURR_INTR_NONE      (uint16)(0x0000u)
        #define X_CURR_INTR_RISING    (uint16)(0x0001u)
        #define X_CURR_INTR_FALLING   (uint16)(0x0002u)
        #define X_CURR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define X_CURR_INTR_MASK      (0x01u) 
#endif /* (X_CURR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define X_CURR_PS                     (* (reg8 *) X_CURR__PS)
/* Data Register */
#define X_CURR_DR                     (* (reg8 *) X_CURR__DR)
/* Port Number */
#define X_CURR_PRT_NUM                (* (reg8 *) X_CURR__PRT) 
/* Connect to Analog Globals */                                                  
#define X_CURR_AG                     (* (reg8 *) X_CURR__AG)                       
/* Analog MUX bux enable */
#define X_CURR_AMUX                   (* (reg8 *) X_CURR__AMUX) 
/* Bidirectional Enable */                                                        
#define X_CURR_BIE                    (* (reg8 *) X_CURR__BIE)
/* Bit-mask for Aliased Register Access */
#define X_CURR_BIT_MASK               (* (reg8 *) X_CURR__BIT_MASK)
/* Bypass Enable */
#define X_CURR_BYP                    (* (reg8 *) X_CURR__BYP)
/* Port wide control signals */                                                   
#define X_CURR_CTL                    (* (reg8 *) X_CURR__CTL)
/* Drive Modes */
#define X_CURR_DM0                    (* (reg8 *) X_CURR__DM0) 
#define X_CURR_DM1                    (* (reg8 *) X_CURR__DM1)
#define X_CURR_DM2                    (* (reg8 *) X_CURR__DM2) 
/* Input Buffer Disable Override */
#define X_CURR_INP_DIS                (* (reg8 *) X_CURR__INP_DIS)
/* LCD Common or Segment Drive */
#define X_CURR_LCD_COM_SEG            (* (reg8 *) X_CURR__LCD_COM_SEG)
/* Enable Segment LCD */
#define X_CURR_LCD_EN                 (* (reg8 *) X_CURR__LCD_EN)
/* Slew Rate Control */
#define X_CURR_SLW                    (* (reg8 *) X_CURR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define X_CURR_PRTDSI__CAPS_SEL       (* (reg8 *) X_CURR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define X_CURR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) X_CURR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define X_CURR_PRTDSI__OE_SEL0        (* (reg8 *) X_CURR__PRTDSI__OE_SEL0) 
#define X_CURR_PRTDSI__OE_SEL1        (* (reg8 *) X_CURR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define X_CURR_PRTDSI__OUT_SEL0       (* (reg8 *) X_CURR__PRTDSI__OUT_SEL0) 
#define X_CURR_PRTDSI__OUT_SEL1       (* (reg8 *) X_CURR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define X_CURR_PRTDSI__SYNC_OUT       (* (reg8 *) X_CURR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(X_CURR__SIO_CFG)
    #define X_CURR_SIO_HYST_EN        (* (reg8 *) X_CURR__SIO_HYST_EN)
    #define X_CURR_SIO_REG_HIFREQ     (* (reg8 *) X_CURR__SIO_REG_HIFREQ)
    #define X_CURR_SIO_CFG            (* (reg8 *) X_CURR__SIO_CFG)
    #define X_CURR_SIO_DIFF           (* (reg8 *) X_CURR__SIO_DIFF)
#endif /* (X_CURR__SIO_CFG) */

/* Interrupt Registers */
#if defined(X_CURR__INTSTAT)
    #define X_CURR_INTSTAT            (* (reg8 *) X_CURR__INTSTAT)
    #define X_CURR_SNAP               (* (reg8 *) X_CURR__SNAP)
    
	#define X_CURR_0_INTTYPE_REG 		(* (reg8 *) X_CURR__0__INTTYPE)
#endif /* (X_CURR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_X_CURR_H */


/* [] END OF FILE */

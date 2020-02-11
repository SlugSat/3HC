/*******************************************************************************
* File Name: Can_Sense.h  
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

#if !defined(CY_PINS_Can_Sense_H) /* Pins Can_Sense_H */
#define CY_PINS_Can_Sense_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Can_Sense_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Can_Sense__PORT == 15 && ((Can_Sense__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Can_Sense_Write(uint8 value);
void    Can_Sense_SetDriveMode(uint8 mode);
uint8   Can_Sense_ReadDataReg(void);
uint8   Can_Sense_Read(void);
void    Can_Sense_SetInterruptMode(uint16 position, uint16 mode);
uint8   Can_Sense_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Can_Sense_SetDriveMode() function.
     *  @{
     */
        #define Can_Sense_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Can_Sense_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Can_Sense_DM_RES_UP          PIN_DM_RES_UP
        #define Can_Sense_DM_RES_DWN         PIN_DM_RES_DWN
        #define Can_Sense_DM_OD_LO           PIN_DM_OD_LO
        #define Can_Sense_DM_OD_HI           PIN_DM_OD_HI
        #define Can_Sense_DM_STRONG          PIN_DM_STRONG
        #define Can_Sense_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Can_Sense_MASK               Can_Sense__MASK
#define Can_Sense_SHIFT              Can_Sense__SHIFT
#define Can_Sense_WIDTH              1u

/* Interrupt constants */
#if defined(Can_Sense__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Can_Sense_SetInterruptMode() function.
     *  @{
     */
        #define Can_Sense_INTR_NONE      (uint16)(0x0000u)
        #define Can_Sense_INTR_RISING    (uint16)(0x0001u)
        #define Can_Sense_INTR_FALLING   (uint16)(0x0002u)
        #define Can_Sense_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Can_Sense_INTR_MASK      (0x01u) 
#endif /* (Can_Sense__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Can_Sense_PS                     (* (reg8 *) Can_Sense__PS)
/* Data Register */
#define Can_Sense_DR                     (* (reg8 *) Can_Sense__DR)
/* Port Number */
#define Can_Sense_PRT_NUM                (* (reg8 *) Can_Sense__PRT) 
/* Connect to Analog Globals */                                                  
#define Can_Sense_AG                     (* (reg8 *) Can_Sense__AG)                       
/* Analog MUX bux enable */
#define Can_Sense_AMUX                   (* (reg8 *) Can_Sense__AMUX) 
/* Bidirectional Enable */                                                        
#define Can_Sense_BIE                    (* (reg8 *) Can_Sense__BIE)
/* Bit-mask for Aliased Register Access */
#define Can_Sense_BIT_MASK               (* (reg8 *) Can_Sense__BIT_MASK)
/* Bypass Enable */
#define Can_Sense_BYP                    (* (reg8 *) Can_Sense__BYP)
/* Port wide control signals */                                                   
#define Can_Sense_CTL                    (* (reg8 *) Can_Sense__CTL)
/* Drive Modes */
#define Can_Sense_DM0                    (* (reg8 *) Can_Sense__DM0) 
#define Can_Sense_DM1                    (* (reg8 *) Can_Sense__DM1)
#define Can_Sense_DM2                    (* (reg8 *) Can_Sense__DM2) 
/* Input Buffer Disable Override */
#define Can_Sense_INP_DIS                (* (reg8 *) Can_Sense__INP_DIS)
/* LCD Common or Segment Drive */
#define Can_Sense_LCD_COM_SEG            (* (reg8 *) Can_Sense__LCD_COM_SEG)
/* Enable Segment LCD */
#define Can_Sense_LCD_EN                 (* (reg8 *) Can_Sense__LCD_EN)
/* Slew Rate Control */
#define Can_Sense_SLW                    (* (reg8 *) Can_Sense__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Can_Sense_PRTDSI__CAPS_SEL       (* (reg8 *) Can_Sense__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Can_Sense_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Can_Sense__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Can_Sense_PRTDSI__OE_SEL0        (* (reg8 *) Can_Sense__PRTDSI__OE_SEL0) 
#define Can_Sense_PRTDSI__OE_SEL1        (* (reg8 *) Can_Sense__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Can_Sense_PRTDSI__OUT_SEL0       (* (reg8 *) Can_Sense__PRTDSI__OUT_SEL0) 
#define Can_Sense_PRTDSI__OUT_SEL1       (* (reg8 *) Can_Sense__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Can_Sense_PRTDSI__SYNC_OUT       (* (reg8 *) Can_Sense__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Can_Sense__SIO_CFG)
    #define Can_Sense_SIO_HYST_EN        (* (reg8 *) Can_Sense__SIO_HYST_EN)
    #define Can_Sense_SIO_REG_HIFREQ     (* (reg8 *) Can_Sense__SIO_REG_HIFREQ)
    #define Can_Sense_SIO_CFG            (* (reg8 *) Can_Sense__SIO_CFG)
    #define Can_Sense_SIO_DIFF           (* (reg8 *) Can_Sense__SIO_DIFF)
#endif /* (Can_Sense__SIO_CFG) */

/* Interrupt Registers */
#if defined(Can_Sense__INTSTAT)
    #define Can_Sense_INTSTAT            (* (reg8 *) Can_Sense__INTSTAT)
    #define Can_Sense_SNAP               (* (reg8 *) Can_Sense__SNAP)
    
	#define Can_Sense_0_INTTYPE_REG 		(* (reg8 *) Can_Sense__0__INTTYPE)
#endif /* (Can_Sense__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Can_Sense_H */


/* [] END OF FILE */

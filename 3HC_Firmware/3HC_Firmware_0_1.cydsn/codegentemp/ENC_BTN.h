/*******************************************************************************
* File Name: ENC_BTN.h  
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

#if !defined(CY_PINS_ENC_BTN_H) /* Pins ENC_BTN_H */
#define CY_PINS_ENC_BTN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ENC_BTN_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ENC_BTN__PORT == 15 && ((ENC_BTN__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ENC_BTN_Write(uint8 value);
void    ENC_BTN_SetDriveMode(uint8 mode);
uint8   ENC_BTN_ReadDataReg(void);
uint8   ENC_BTN_Read(void);
void    ENC_BTN_SetInterruptMode(uint16 position, uint16 mode);
uint8   ENC_BTN_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ENC_BTN_SetDriveMode() function.
     *  @{
     */
        #define ENC_BTN_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ENC_BTN_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ENC_BTN_DM_RES_UP          PIN_DM_RES_UP
        #define ENC_BTN_DM_RES_DWN         PIN_DM_RES_DWN
        #define ENC_BTN_DM_OD_LO           PIN_DM_OD_LO
        #define ENC_BTN_DM_OD_HI           PIN_DM_OD_HI
        #define ENC_BTN_DM_STRONG          PIN_DM_STRONG
        #define ENC_BTN_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ENC_BTN_MASK               ENC_BTN__MASK
#define ENC_BTN_SHIFT              ENC_BTN__SHIFT
#define ENC_BTN_WIDTH              1u

/* Interrupt constants */
#if defined(ENC_BTN__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ENC_BTN_SetInterruptMode() function.
     *  @{
     */
        #define ENC_BTN_INTR_NONE      (uint16)(0x0000u)
        #define ENC_BTN_INTR_RISING    (uint16)(0x0001u)
        #define ENC_BTN_INTR_FALLING   (uint16)(0x0002u)
        #define ENC_BTN_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ENC_BTN_INTR_MASK      (0x01u) 
#endif /* (ENC_BTN__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ENC_BTN_PS                     (* (reg8 *) ENC_BTN__PS)
/* Data Register */
#define ENC_BTN_DR                     (* (reg8 *) ENC_BTN__DR)
/* Port Number */
#define ENC_BTN_PRT_NUM                (* (reg8 *) ENC_BTN__PRT) 
/* Connect to Analog Globals */                                                  
#define ENC_BTN_AG                     (* (reg8 *) ENC_BTN__AG)                       
/* Analog MUX bux enable */
#define ENC_BTN_AMUX                   (* (reg8 *) ENC_BTN__AMUX) 
/* Bidirectional Enable */                                                        
#define ENC_BTN_BIE                    (* (reg8 *) ENC_BTN__BIE)
/* Bit-mask for Aliased Register Access */
#define ENC_BTN_BIT_MASK               (* (reg8 *) ENC_BTN__BIT_MASK)
/* Bypass Enable */
#define ENC_BTN_BYP                    (* (reg8 *) ENC_BTN__BYP)
/* Port wide control signals */                                                   
#define ENC_BTN_CTL                    (* (reg8 *) ENC_BTN__CTL)
/* Drive Modes */
#define ENC_BTN_DM0                    (* (reg8 *) ENC_BTN__DM0) 
#define ENC_BTN_DM1                    (* (reg8 *) ENC_BTN__DM1)
#define ENC_BTN_DM2                    (* (reg8 *) ENC_BTN__DM2) 
/* Input Buffer Disable Override */
#define ENC_BTN_INP_DIS                (* (reg8 *) ENC_BTN__INP_DIS)
/* LCD Common or Segment Drive */
#define ENC_BTN_LCD_COM_SEG            (* (reg8 *) ENC_BTN__LCD_COM_SEG)
/* Enable Segment LCD */
#define ENC_BTN_LCD_EN                 (* (reg8 *) ENC_BTN__LCD_EN)
/* Slew Rate Control */
#define ENC_BTN_SLW                    (* (reg8 *) ENC_BTN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ENC_BTN_PRTDSI__CAPS_SEL       (* (reg8 *) ENC_BTN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ENC_BTN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ENC_BTN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ENC_BTN_PRTDSI__OE_SEL0        (* (reg8 *) ENC_BTN__PRTDSI__OE_SEL0) 
#define ENC_BTN_PRTDSI__OE_SEL1        (* (reg8 *) ENC_BTN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ENC_BTN_PRTDSI__OUT_SEL0       (* (reg8 *) ENC_BTN__PRTDSI__OUT_SEL0) 
#define ENC_BTN_PRTDSI__OUT_SEL1       (* (reg8 *) ENC_BTN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ENC_BTN_PRTDSI__SYNC_OUT       (* (reg8 *) ENC_BTN__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ENC_BTN__SIO_CFG)
    #define ENC_BTN_SIO_HYST_EN        (* (reg8 *) ENC_BTN__SIO_HYST_EN)
    #define ENC_BTN_SIO_REG_HIFREQ     (* (reg8 *) ENC_BTN__SIO_REG_HIFREQ)
    #define ENC_BTN_SIO_CFG            (* (reg8 *) ENC_BTN__SIO_CFG)
    #define ENC_BTN_SIO_DIFF           (* (reg8 *) ENC_BTN__SIO_DIFF)
#endif /* (ENC_BTN__SIO_CFG) */

/* Interrupt Registers */
#if defined(ENC_BTN__INTSTAT)
    #define ENC_BTN_INTSTAT            (* (reg8 *) ENC_BTN__INTSTAT)
    #define ENC_BTN_SNAP               (* (reg8 *) ENC_BTN__SNAP)
    
	#define ENC_BTN_0_INTTYPE_REG 		(* (reg8 *) ENC_BTN__0__INTTYPE)
#endif /* (ENC_BTN__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ENC_BTN_H */


/* [] END OF FILE */

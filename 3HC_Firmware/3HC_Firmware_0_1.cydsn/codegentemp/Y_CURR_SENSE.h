/*******************************************************************************
* File Name: Y_CURR_SENSE.h  
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

#if !defined(CY_PINS_Y_CURR_SENSE_H) /* Pins Y_CURR_SENSE_H */
#define CY_PINS_Y_CURR_SENSE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Y_CURR_SENSE_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Y_CURR_SENSE__PORT == 15 && ((Y_CURR_SENSE__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Y_CURR_SENSE_Write(uint8 value);
void    Y_CURR_SENSE_SetDriveMode(uint8 mode);
uint8   Y_CURR_SENSE_ReadDataReg(void);
uint8   Y_CURR_SENSE_Read(void);
void    Y_CURR_SENSE_SetInterruptMode(uint16 position, uint16 mode);
uint8   Y_CURR_SENSE_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Y_CURR_SENSE_SetDriveMode() function.
     *  @{
     */
        #define Y_CURR_SENSE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Y_CURR_SENSE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Y_CURR_SENSE_DM_RES_UP          PIN_DM_RES_UP
        #define Y_CURR_SENSE_DM_RES_DWN         PIN_DM_RES_DWN
        #define Y_CURR_SENSE_DM_OD_LO           PIN_DM_OD_LO
        #define Y_CURR_SENSE_DM_OD_HI           PIN_DM_OD_HI
        #define Y_CURR_SENSE_DM_STRONG          PIN_DM_STRONG
        #define Y_CURR_SENSE_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Y_CURR_SENSE_MASK               Y_CURR_SENSE__MASK
#define Y_CURR_SENSE_SHIFT              Y_CURR_SENSE__SHIFT
#define Y_CURR_SENSE_WIDTH              1u

/* Interrupt constants */
#if defined(Y_CURR_SENSE__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Y_CURR_SENSE_SetInterruptMode() function.
     *  @{
     */
        #define Y_CURR_SENSE_INTR_NONE      (uint16)(0x0000u)
        #define Y_CURR_SENSE_INTR_RISING    (uint16)(0x0001u)
        #define Y_CURR_SENSE_INTR_FALLING   (uint16)(0x0002u)
        #define Y_CURR_SENSE_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Y_CURR_SENSE_INTR_MASK      (0x01u) 
#endif /* (Y_CURR_SENSE__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Y_CURR_SENSE_PS                     (* (reg8 *) Y_CURR_SENSE__PS)
/* Data Register */
#define Y_CURR_SENSE_DR                     (* (reg8 *) Y_CURR_SENSE__DR)
/* Port Number */
#define Y_CURR_SENSE_PRT_NUM                (* (reg8 *) Y_CURR_SENSE__PRT) 
/* Connect to Analog Globals */                                                  
#define Y_CURR_SENSE_AG                     (* (reg8 *) Y_CURR_SENSE__AG)                       
/* Analog MUX bux enable */
#define Y_CURR_SENSE_AMUX                   (* (reg8 *) Y_CURR_SENSE__AMUX) 
/* Bidirectional Enable */                                                        
#define Y_CURR_SENSE_BIE                    (* (reg8 *) Y_CURR_SENSE__BIE)
/* Bit-mask for Aliased Register Access */
#define Y_CURR_SENSE_BIT_MASK               (* (reg8 *) Y_CURR_SENSE__BIT_MASK)
/* Bypass Enable */
#define Y_CURR_SENSE_BYP                    (* (reg8 *) Y_CURR_SENSE__BYP)
/* Port wide control signals */                                                   
#define Y_CURR_SENSE_CTL                    (* (reg8 *) Y_CURR_SENSE__CTL)
/* Drive Modes */
#define Y_CURR_SENSE_DM0                    (* (reg8 *) Y_CURR_SENSE__DM0) 
#define Y_CURR_SENSE_DM1                    (* (reg8 *) Y_CURR_SENSE__DM1)
#define Y_CURR_SENSE_DM2                    (* (reg8 *) Y_CURR_SENSE__DM2) 
/* Input Buffer Disable Override */
#define Y_CURR_SENSE_INP_DIS                (* (reg8 *) Y_CURR_SENSE__INP_DIS)
/* LCD Common or Segment Drive */
#define Y_CURR_SENSE_LCD_COM_SEG            (* (reg8 *) Y_CURR_SENSE__LCD_COM_SEG)
/* Enable Segment LCD */
#define Y_CURR_SENSE_LCD_EN                 (* (reg8 *) Y_CURR_SENSE__LCD_EN)
/* Slew Rate Control */
#define Y_CURR_SENSE_SLW                    (* (reg8 *) Y_CURR_SENSE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Y_CURR_SENSE_PRTDSI__CAPS_SEL       (* (reg8 *) Y_CURR_SENSE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Y_CURR_SENSE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Y_CURR_SENSE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Y_CURR_SENSE_PRTDSI__OE_SEL0        (* (reg8 *) Y_CURR_SENSE__PRTDSI__OE_SEL0) 
#define Y_CURR_SENSE_PRTDSI__OE_SEL1        (* (reg8 *) Y_CURR_SENSE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Y_CURR_SENSE_PRTDSI__OUT_SEL0       (* (reg8 *) Y_CURR_SENSE__PRTDSI__OUT_SEL0) 
#define Y_CURR_SENSE_PRTDSI__OUT_SEL1       (* (reg8 *) Y_CURR_SENSE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Y_CURR_SENSE_PRTDSI__SYNC_OUT       (* (reg8 *) Y_CURR_SENSE__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Y_CURR_SENSE__SIO_CFG)
    #define Y_CURR_SENSE_SIO_HYST_EN        (* (reg8 *) Y_CURR_SENSE__SIO_HYST_EN)
    #define Y_CURR_SENSE_SIO_REG_HIFREQ     (* (reg8 *) Y_CURR_SENSE__SIO_REG_HIFREQ)
    #define Y_CURR_SENSE_SIO_CFG            (* (reg8 *) Y_CURR_SENSE__SIO_CFG)
    #define Y_CURR_SENSE_SIO_DIFF           (* (reg8 *) Y_CURR_SENSE__SIO_DIFF)
#endif /* (Y_CURR_SENSE__SIO_CFG) */

/* Interrupt Registers */
#if defined(Y_CURR_SENSE__INTSTAT)
    #define Y_CURR_SENSE_INTSTAT            (* (reg8 *) Y_CURR_SENSE__INTSTAT)
    #define Y_CURR_SENSE_SNAP               (* (reg8 *) Y_CURR_SENSE__SNAP)
    
	#define Y_CURR_SENSE_0_INTTYPE_REG 		(* (reg8 *) Y_CURR_SENSE__0__INTTYPE)
#endif /* (Y_CURR_SENSE__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Y_CURR_SENSE_H */


/* [] END OF FILE */

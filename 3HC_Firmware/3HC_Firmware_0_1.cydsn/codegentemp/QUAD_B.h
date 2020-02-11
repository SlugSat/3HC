/*******************************************************************************
* File Name: QUAD_B.h  
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

#if !defined(CY_PINS_QUAD_B_H) /* Pins QUAD_B_H */
#define CY_PINS_QUAD_B_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "QUAD_B_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 QUAD_B__PORT == 15 && ((QUAD_B__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    QUAD_B_Write(uint8 value);
void    QUAD_B_SetDriveMode(uint8 mode);
uint8   QUAD_B_ReadDataReg(void);
uint8   QUAD_B_Read(void);
void    QUAD_B_SetInterruptMode(uint16 position, uint16 mode);
uint8   QUAD_B_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the QUAD_B_SetDriveMode() function.
     *  @{
     */
        #define QUAD_B_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define QUAD_B_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define QUAD_B_DM_RES_UP          PIN_DM_RES_UP
        #define QUAD_B_DM_RES_DWN         PIN_DM_RES_DWN
        #define QUAD_B_DM_OD_LO           PIN_DM_OD_LO
        #define QUAD_B_DM_OD_HI           PIN_DM_OD_HI
        #define QUAD_B_DM_STRONG          PIN_DM_STRONG
        #define QUAD_B_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define QUAD_B_MASK               QUAD_B__MASK
#define QUAD_B_SHIFT              QUAD_B__SHIFT
#define QUAD_B_WIDTH              1u

/* Interrupt constants */
#if defined(QUAD_B__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in QUAD_B_SetInterruptMode() function.
     *  @{
     */
        #define QUAD_B_INTR_NONE      (uint16)(0x0000u)
        #define QUAD_B_INTR_RISING    (uint16)(0x0001u)
        #define QUAD_B_INTR_FALLING   (uint16)(0x0002u)
        #define QUAD_B_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define QUAD_B_INTR_MASK      (0x01u) 
#endif /* (QUAD_B__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define QUAD_B_PS                     (* (reg8 *) QUAD_B__PS)
/* Data Register */
#define QUAD_B_DR                     (* (reg8 *) QUAD_B__DR)
/* Port Number */
#define QUAD_B_PRT_NUM                (* (reg8 *) QUAD_B__PRT) 
/* Connect to Analog Globals */                                                  
#define QUAD_B_AG                     (* (reg8 *) QUAD_B__AG)                       
/* Analog MUX bux enable */
#define QUAD_B_AMUX                   (* (reg8 *) QUAD_B__AMUX) 
/* Bidirectional Enable */                                                        
#define QUAD_B_BIE                    (* (reg8 *) QUAD_B__BIE)
/* Bit-mask for Aliased Register Access */
#define QUAD_B_BIT_MASK               (* (reg8 *) QUAD_B__BIT_MASK)
/* Bypass Enable */
#define QUAD_B_BYP                    (* (reg8 *) QUAD_B__BYP)
/* Port wide control signals */                                                   
#define QUAD_B_CTL                    (* (reg8 *) QUAD_B__CTL)
/* Drive Modes */
#define QUAD_B_DM0                    (* (reg8 *) QUAD_B__DM0) 
#define QUAD_B_DM1                    (* (reg8 *) QUAD_B__DM1)
#define QUAD_B_DM2                    (* (reg8 *) QUAD_B__DM2) 
/* Input Buffer Disable Override */
#define QUAD_B_INP_DIS                (* (reg8 *) QUAD_B__INP_DIS)
/* LCD Common or Segment Drive */
#define QUAD_B_LCD_COM_SEG            (* (reg8 *) QUAD_B__LCD_COM_SEG)
/* Enable Segment LCD */
#define QUAD_B_LCD_EN                 (* (reg8 *) QUAD_B__LCD_EN)
/* Slew Rate Control */
#define QUAD_B_SLW                    (* (reg8 *) QUAD_B__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define QUAD_B_PRTDSI__CAPS_SEL       (* (reg8 *) QUAD_B__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define QUAD_B_PRTDSI__DBL_SYNC_IN    (* (reg8 *) QUAD_B__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define QUAD_B_PRTDSI__OE_SEL0        (* (reg8 *) QUAD_B__PRTDSI__OE_SEL0) 
#define QUAD_B_PRTDSI__OE_SEL1        (* (reg8 *) QUAD_B__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define QUAD_B_PRTDSI__OUT_SEL0       (* (reg8 *) QUAD_B__PRTDSI__OUT_SEL0) 
#define QUAD_B_PRTDSI__OUT_SEL1       (* (reg8 *) QUAD_B__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define QUAD_B_PRTDSI__SYNC_OUT       (* (reg8 *) QUAD_B__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(QUAD_B__SIO_CFG)
    #define QUAD_B_SIO_HYST_EN        (* (reg8 *) QUAD_B__SIO_HYST_EN)
    #define QUAD_B_SIO_REG_HIFREQ     (* (reg8 *) QUAD_B__SIO_REG_HIFREQ)
    #define QUAD_B_SIO_CFG            (* (reg8 *) QUAD_B__SIO_CFG)
    #define QUAD_B_SIO_DIFF           (* (reg8 *) QUAD_B__SIO_DIFF)
#endif /* (QUAD_B__SIO_CFG) */

/* Interrupt Registers */
#if defined(QUAD_B__INTSTAT)
    #define QUAD_B_INTSTAT            (* (reg8 *) QUAD_B__INTSTAT)
    #define QUAD_B_SNAP               (* (reg8 *) QUAD_B__SNAP)
    
	#define QUAD_B_0_INTTYPE_REG 		(* (reg8 *) QUAD_B__0__INTTYPE)
#endif /* (QUAD_B__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_QUAD_B_H */


/* [] END OF FILE */

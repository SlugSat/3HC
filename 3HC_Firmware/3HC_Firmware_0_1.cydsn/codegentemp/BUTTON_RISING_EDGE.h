/*******************************************************************************
* File Name: BUTTON_RISING_EDGE.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_BUTTON_RISING_EDGE_H)
#define CY_ISR_BUTTON_RISING_EDGE_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void BUTTON_RISING_EDGE_Start(void);
void BUTTON_RISING_EDGE_StartEx(cyisraddress address);
void BUTTON_RISING_EDGE_Stop(void);

CY_ISR_PROTO(BUTTON_RISING_EDGE_Interrupt);

void BUTTON_RISING_EDGE_SetVector(cyisraddress address);
cyisraddress BUTTON_RISING_EDGE_GetVector(void);

void BUTTON_RISING_EDGE_SetPriority(uint8 priority);
uint8 BUTTON_RISING_EDGE_GetPriority(void);

void BUTTON_RISING_EDGE_Enable(void);
uint8 BUTTON_RISING_EDGE_GetState(void);
void BUTTON_RISING_EDGE_Disable(void);

void BUTTON_RISING_EDGE_SetPending(void);
void BUTTON_RISING_EDGE_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the BUTTON_RISING_EDGE ISR. */
#define BUTTON_RISING_EDGE_INTC_VECTOR            ((reg32 *) BUTTON_RISING_EDGE__INTC_VECT)

/* Address of the BUTTON_RISING_EDGE ISR priority. */
#define BUTTON_RISING_EDGE_INTC_PRIOR             ((reg8 *) BUTTON_RISING_EDGE__INTC_PRIOR_REG)

/* Priority of the BUTTON_RISING_EDGE interrupt. */
#define BUTTON_RISING_EDGE_INTC_PRIOR_NUMBER      BUTTON_RISING_EDGE__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable BUTTON_RISING_EDGE interrupt. */
#define BUTTON_RISING_EDGE_INTC_SET_EN            ((reg32 *) BUTTON_RISING_EDGE__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the BUTTON_RISING_EDGE interrupt. */
#define BUTTON_RISING_EDGE_INTC_CLR_EN            ((reg32 *) BUTTON_RISING_EDGE__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the BUTTON_RISING_EDGE interrupt state to pending. */
#define BUTTON_RISING_EDGE_INTC_SET_PD            ((reg32 *) BUTTON_RISING_EDGE__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the BUTTON_RISING_EDGE interrupt. */
#define BUTTON_RISING_EDGE_INTC_CLR_PD            ((reg32 *) BUTTON_RISING_EDGE__INTC_CLR_PD_REG)


#endif /* CY_ISR_BUTTON_RISING_EDGE_H */


/* [] END OF FILE */

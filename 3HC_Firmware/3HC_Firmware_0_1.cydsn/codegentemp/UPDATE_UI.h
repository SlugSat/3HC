/*******************************************************************************
* File Name: UPDATE_UI.h
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
#if !defined(CY_ISR_UPDATE_UI_H)
#define CY_ISR_UPDATE_UI_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void UPDATE_UI_Start(void);
void UPDATE_UI_StartEx(cyisraddress address);
void UPDATE_UI_Stop(void);

CY_ISR_PROTO(UPDATE_UI_Interrupt);

void UPDATE_UI_SetVector(cyisraddress address);
cyisraddress UPDATE_UI_GetVector(void);

void UPDATE_UI_SetPriority(uint8 priority);
uint8 UPDATE_UI_GetPriority(void);

void UPDATE_UI_Enable(void);
uint8 UPDATE_UI_GetState(void);
void UPDATE_UI_Disable(void);

void UPDATE_UI_SetPending(void);
void UPDATE_UI_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the UPDATE_UI ISR. */
#define UPDATE_UI_INTC_VECTOR            ((reg32 *) UPDATE_UI__INTC_VECT)

/* Address of the UPDATE_UI ISR priority. */
#define UPDATE_UI_INTC_PRIOR             ((reg8 *) UPDATE_UI__INTC_PRIOR_REG)

/* Priority of the UPDATE_UI interrupt. */
#define UPDATE_UI_INTC_PRIOR_NUMBER      UPDATE_UI__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable UPDATE_UI interrupt. */
#define UPDATE_UI_INTC_SET_EN            ((reg32 *) UPDATE_UI__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the UPDATE_UI interrupt. */
#define UPDATE_UI_INTC_CLR_EN            ((reg32 *) UPDATE_UI__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the UPDATE_UI interrupt state to pending. */
#define UPDATE_UI_INTC_SET_PD            ((reg32 *) UPDATE_UI__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the UPDATE_UI interrupt. */
#define UPDATE_UI_INTC_CLR_PD            ((reg32 *) UPDATE_UI__INTC_CLR_PD_REG)


#endif /* CY_ISR_UPDATE_UI_H */


/* [] END OF FILE */

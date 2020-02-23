/*******************************************************************************
* File Name: ADC_CurrentReadingReady.h
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
#if !defined(CY_ISR_ADC_CurrentReadingReady_H)
#define CY_ISR_ADC_CurrentReadingReady_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ADC_CurrentReadingReady_Start(void);
void ADC_CurrentReadingReady_StartEx(cyisraddress address);
void ADC_CurrentReadingReady_Stop(void);

CY_ISR_PROTO(ADC_CurrentReadingReady_Interrupt);

void ADC_CurrentReadingReady_SetVector(cyisraddress address);
cyisraddress ADC_CurrentReadingReady_GetVector(void);

void ADC_CurrentReadingReady_SetPriority(uint8 priority);
uint8 ADC_CurrentReadingReady_GetPriority(void);

void ADC_CurrentReadingReady_Enable(void);
uint8 ADC_CurrentReadingReady_GetState(void);
void ADC_CurrentReadingReady_Disable(void);

void ADC_CurrentReadingReady_SetPending(void);
void ADC_CurrentReadingReady_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ADC_CurrentReadingReady ISR. */
#define ADC_CurrentReadingReady_INTC_VECTOR            ((reg32 *) ADC_CurrentReadingReady__INTC_VECT)

/* Address of the ADC_CurrentReadingReady ISR priority. */
#define ADC_CurrentReadingReady_INTC_PRIOR             ((reg8 *) ADC_CurrentReadingReady__INTC_PRIOR_REG)

/* Priority of the ADC_CurrentReadingReady interrupt. */
#define ADC_CurrentReadingReady_INTC_PRIOR_NUMBER      ADC_CurrentReadingReady__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ADC_CurrentReadingReady interrupt. */
#define ADC_CurrentReadingReady_INTC_SET_EN            ((reg32 *) ADC_CurrentReadingReady__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ADC_CurrentReadingReady interrupt. */
#define ADC_CurrentReadingReady_INTC_CLR_EN            ((reg32 *) ADC_CurrentReadingReady__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ADC_CurrentReadingReady interrupt state to pending. */
#define ADC_CurrentReadingReady_INTC_SET_PD            ((reg32 *) ADC_CurrentReadingReady__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ADC_CurrentReadingReady interrupt. */
#define ADC_CurrentReadingReady_INTC_CLR_PD            ((reg32 *) ADC_CurrentReadingReady__INTC_CLR_PD_REG)


#endif /* CY_ISR_ADC_CurrentReadingReady_H */


/* [] END OF FILE */

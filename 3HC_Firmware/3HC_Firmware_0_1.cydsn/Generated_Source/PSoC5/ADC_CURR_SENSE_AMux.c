/*******************************************************************************
* File Name: ADC_CURR_SENSE_AMux.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "ADC_CURR_SENSE_AMux.h"

static uint8 ADC_CURR_SENSE_AMux_lastChannel = ADC_CURR_SENSE_AMux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: ADC_CURR_SENSE_AMux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ADC_CURR_SENSE_AMux_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < ADC_CURR_SENSE_AMux_CHANNELS ; chan++)
    {
#if (ADC_CURR_SENSE_AMux_MUXTYPE == ADC_CURR_SENSE_AMux_MUX_SINGLE)
        ADC_CURR_SENSE_AMux_Unset(chan);
#else
        ADC_CURR_SENSE_AMux_CYAMUXSIDE_A_Unset(chan);
        ADC_CURR_SENSE_AMux_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    ADC_CURR_SENSE_AMux_lastChannel = ADC_CURR_SENSE_AMux_NULL_CHANNEL;
}


#if (!ADC_CURR_SENSE_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: ADC_CURR_SENSE_AMux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_CURR_SENSE_AMux_Select(uint8 channel) 
{
    ADC_CURR_SENSE_AMux_DisconnectAll();        /* Disconnect all previous connections */
    ADC_CURR_SENSE_AMux_Connect(channel);       /* Make the given selection */
    ADC_CURR_SENSE_AMux_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: ADC_CURR_SENSE_AMux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_CURR_SENSE_AMux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( ADC_CURR_SENSE_AMux_lastChannel != ADC_CURR_SENSE_AMux_NULL_CHANNEL)
    {
        ADC_CURR_SENSE_AMux_Disconnect(ADC_CURR_SENSE_AMux_lastChannel);
    }

    /* Make the new channel connection */
#if (ADC_CURR_SENSE_AMux_MUXTYPE == ADC_CURR_SENSE_AMux_MUX_SINGLE)
    ADC_CURR_SENSE_AMux_Set(channel);
#else
    ADC_CURR_SENSE_AMux_CYAMUXSIDE_A_Set(channel);
    ADC_CURR_SENSE_AMux_CYAMUXSIDE_B_Set(channel);
#endif


    ADC_CURR_SENSE_AMux_lastChannel = channel;   /* Update last channel */
}


#if (ADC_CURR_SENSE_AMux_MUXTYPE == ADC_CURR_SENSE_AMux_MUX_DIFF)
#if (!ADC_CURR_SENSE_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: ADC_CURR_SENSE_AMux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_CURR_SENSE_AMux_Connect(uint8 channel) 
{
    ADC_CURR_SENSE_AMux_CYAMUXSIDE_A_Set(channel);
    ADC_CURR_SENSE_AMux_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: ADC_CURR_SENSE_AMux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_CURR_SENSE_AMux_Disconnect(uint8 channel) 
{
    ADC_CURR_SENSE_AMux_CYAMUXSIDE_A_Unset(channel);
    ADC_CURR_SENSE_AMux_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (ADC_CURR_SENSE_AMux_ATMOSTONE)
/*******************************************************************************
* Function Name: ADC_CURR_SENSE_AMux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ADC_CURR_SENSE_AMux_DisconnectAll(void) 
{
    if(ADC_CURR_SENSE_AMux_lastChannel != ADC_CURR_SENSE_AMux_NULL_CHANNEL) 
    {
        ADC_CURR_SENSE_AMux_Disconnect(ADC_CURR_SENSE_AMux_lastChannel);
        ADC_CURR_SENSE_AMux_lastChannel = ADC_CURR_SENSE_AMux_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */

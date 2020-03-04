/*******************************************************************************
* File Name: Mux_CurrentSense.c
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

#include "Mux_CurrentSense.h"

static uint8 Mux_CurrentSense_lastChannel = Mux_CurrentSense_NULL_CHANNEL;


/*******************************************************************************
* Function Name: Mux_CurrentSense_Start
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
void Mux_CurrentSense_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < Mux_CurrentSense_CHANNELS ; chan++)
    {
#if (Mux_CurrentSense_MUXTYPE == Mux_CurrentSense_MUX_SINGLE)
        Mux_CurrentSense_Unset(chan);
#else
        Mux_CurrentSense_CYAMUXSIDE_A_Unset(chan);
        Mux_CurrentSense_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    Mux_CurrentSense_lastChannel = Mux_CurrentSense_NULL_CHANNEL;
}


#if (!Mux_CurrentSense_ATMOSTONE)
/*******************************************************************************
* Function Name: Mux_CurrentSense_Select
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
void Mux_CurrentSense_Select(uint8 channel) 
{
    Mux_CurrentSense_DisconnectAll();        /* Disconnect all previous connections */
    Mux_CurrentSense_Connect(channel);       /* Make the given selection */
    Mux_CurrentSense_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: Mux_CurrentSense_FastSelect
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
void Mux_CurrentSense_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( Mux_CurrentSense_lastChannel != Mux_CurrentSense_NULL_CHANNEL)
    {
        Mux_CurrentSense_Disconnect(Mux_CurrentSense_lastChannel);
    }

    /* Make the new channel connection */
#if (Mux_CurrentSense_MUXTYPE == Mux_CurrentSense_MUX_SINGLE)
    Mux_CurrentSense_Set(channel);
#else
    Mux_CurrentSense_CYAMUXSIDE_A_Set(channel);
    Mux_CurrentSense_CYAMUXSIDE_B_Set(channel);
#endif


    Mux_CurrentSense_lastChannel = channel;   /* Update last channel */
}


#if (Mux_CurrentSense_MUXTYPE == Mux_CurrentSense_MUX_DIFF)
#if (!Mux_CurrentSense_ATMOSTONE)
/*******************************************************************************
* Function Name: Mux_CurrentSense_Connect
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
void Mux_CurrentSense_Connect(uint8 channel) 
{
    Mux_CurrentSense_CYAMUXSIDE_A_Set(channel);
    Mux_CurrentSense_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: Mux_CurrentSense_Disconnect
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
void Mux_CurrentSense_Disconnect(uint8 channel) 
{
    Mux_CurrentSense_CYAMUXSIDE_A_Unset(channel);
    Mux_CurrentSense_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (Mux_CurrentSense_ATMOSTONE)
/*******************************************************************************
* Function Name: Mux_CurrentSense_DisconnectAll
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
void Mux_CurrentSense_DisconnectAll(void) 
{
    if(Mux_CurrentSense_lastChannel != Mux_CurrentSense_NULL_CHANNEL) 
    {
        Mux_CurrentSense_Disconnect(Mux_CurrentSense_lastChannel);
        Mux_CurrentSense_lastChannel = Mux_CurrentSense_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */

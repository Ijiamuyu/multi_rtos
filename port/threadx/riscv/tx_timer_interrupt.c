/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Timer                                                               */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define TX_SOURCE_CODE

/* Include necessary system files.  */

#include "tx_api.h"
#include "tx_timer.h"
#include "tx_thread.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tx_timer_interrupt                                RISC-V64/GNU     */
/*                                                           6.2.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function processes the hardware timer interrupt.  This         */
/*    processing includes incrementing the system clock and checking for  */
/*    time slice and/or timer expiration.  If either is found, the        */
/*    interrupt context save/restore functions are called along with the  */
/*    expiration functions.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_timer_expiration_process          Timer expiration processing   */
/*    _tx_thread_time_slice                 Time slice interrupted thread */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    interrupt vector                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  03-08-2023      Scott Larson            Initial Version 6.2.1         */
/*                                                                        */
/**************************************************************************/
VOID   _tx_timer_interrupt(VOID)
{
    /* Increment system clock. */
    _tx_timer_system_clock++;

    /* Test for time-slice expiration. */
    if (_tx_timer_time_slice)
    {
        /* Decrement the time_slice.  */
        _tx_timer_time_slice--;

        /* Check for expiration.  */
        if (_tx_timer_time_slice == 0)
        {

           /* Set the time-slice expired flag.  */
           _tx_timer_expired_time_slice =  TX_TRUE;
        }
    }

    /* Test for timer expiration.  */
    if (*_tx_timer_current_ptr)
    {

        /* Set expiration flag.  */
        _tx_timer_expired =  TX_TRUE;
    }
    else
    {

        /* No timer expired, increment the timer pointer.  */
        _tx_timer_current_ptr++;

        /* Check for wrap-around.  */
        if (_tx_timer_current_ptr == _tx_timer_list_end)
        {

            /* Wrap to beginning of list.  */
            _tx_timer_current_ptr =  _tx_timer_list_start;
        }
    }

    /* See if anything has expired.  */
    if ((_tx_timer_expired_time_slice) || (_tx_timer_expired))
    {

        /* Did a timer expire?  */
        if (_tx_timer_expired)
        {

            /* Process timer expiration.  */
            _tx_timer_expiration_process();
        }

        /* Did time slice expire?  */
        if (_tx_timer_expired_time_slice)
        {

            /* Time slice interrupted thread.  */
            _tx_thread_time_slice();
        }
    }
}

/***************************************************************************
                              tms9914/interrupt.c
                             -------------------

    begin                : Dec 2001
    copyright            : (C) 2001, 2002 by Frank Mori Hess
    email                : fmhess@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "board.h"
#include <asm/bitops.h>
#include <asm/dma.h>

/*
 *  interrupt service routine
 */

void tms9914_interrupt( gpib_board_t *board, tms9914_private_t *priv )
{
	int status0, status1;

	// read interrupt status (also clears status)
	status0 = read_byte( priv, ISR0 );
	status1 = read_byte( priv, ISR1 );
	
	tms9914_interrupt_have_status( board, priv, status0, status1 );
}

void tms9914_interrupt_have_status(gpib_board_t *board, tms9914_private_t *priv, int status0,
		int status1)
{
	spin_lock(&board->spinlock);


	// record address status change
	if(status0 & HR_RLC || status0 & HR_MAC)
	{
		update_status_nolock( board, priv );
		wake_up_interruptible(&board->wait); /* wake up sleeping process */
	}

	// record reception of END
	if(status0 & HR_END)
	{
		set_bit(RECEIVED_END_BN, &priv->state);
	}

	// get incoming data in PIO mode
	if((status0 & HR_BI))
	{
		set_bit(READ_READY_BN, &priv->state);
		wake_up_interruptible(&board->wait); /* wake up sleeping process */
	}

	if((status0 & HR_BO))
	{
		if(read_byte(priv, ADSR) & HR_ATN)
		{
			set_bit(COMMAND_READY_BN, &priv->state);
		}else
		{
			set_bit(WRITE_READY_BN, &priv->state);
		}
		wake_up_interruptible(&board->wait);
	}

	if( status0 & HR_SPAS )
	{
		priv->spoll_status &= ~request_service_bit;
	}

	// record service request in status
	if(status1 & HR_SRQ)
	{
		set_bit(SRQI_NUM, &board->status);
		wake_up_interruptible(&board->wait);
	}

	// have been addressed (with secondary addressing disabled)
	if(status1 & HR_MA)
	{
		update_status_nolock( board, priv );
		// clear dac holdoff
		write_byte(priv, AUX_VAL, AUXCR);
	}

	// unrecognized command received
	if(status1 & HR_UNC)
	{
		printk("gpib command pass thru 0x%x\n", read_byte(priv, CPTR));
		// clear dac holdoff
		write_byte(priv, AUX_INVAL, AUXCR);
	}

	if(status1 & HR_ERR)
	{
		printk("gpib error detected\n");
	}

	if( status1 & HR_IFC )
	{
		push_gpib_event( &board->event_queue, EventIFC );
	}

	if( status1 & HR_GET )
	{
		push_gpib_event( &board->event_queue, EventDevTrg );
		// clear dac holdoff
		write_byte(priv, AUX_VAL, AUXCR);
	}

	if( status1 & HR_DCAS )
	{
		push_gpib_event( &board->event_queue, EventDevClr );
		// clear dac holdoff
		write_byte(priv, AUX_VAL, AUXCR);
	}

	// check for being addressed with secondary addressing
	if( status1 & HR_APT )
	{
		if( board->sad < 0 )
		{
			printk( "tms9914: bug, APT interrupt without secondary addressing?\n" );
		}
		if( read_byte( priv, CPTR ) == MSA( board->sad ) )
		{
			int address_status;

			address_status = read_byte( priv, ADSR );

			if( address_status & HR_TPAS )
			{
				write_byte(priv, AUX_TON | AUX_CS, AUXCR);
			}else if( address_status & HR_LPAS )
			{
				write_byte(priv, AUX_LON | AUX_CS, AUXCR);
			}
		}
		update_status_nolock( board, priv );
		// clear dac holdoff
		write_byte(priv, AUX_VAL, AUXCR);
	}

	spin_unlock(&board->spinlock);

	if( ( status0 & priv->imr0_bits ) || ( status1 & priv->imr1_bits ) )
		GPIB_DPRINTK("isr0 0x%x, imr0 0x%x, isr1 0x%x, imr1 0x%x, status 0x%x\n",
			status0, priv->imr0_bits, status1, priv->imr1_bits, board->status);
}

EXPORT_SYMBOL(tms9914_interrupt);
EXPORT_SYMBOL(tms9914_interrupt_have_status);



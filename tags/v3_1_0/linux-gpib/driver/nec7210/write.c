/***************************************************************************
                              nec7210/write.c
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
#include <asm/dma.h>


static ssize_t pio_write(gpib_device_t *device, nec7210_private_t *priv, uint8_t *buffer, size_t length)
{
	size_t count = 0;
	ssize_t retval = 0;
	unsigned long flags;
	static spinlock_t lock = SPIN_LOCK_UNLOCKED;

	// enable 'data out' interrupts
	priv->imr1_bits |= HR_DOIE;
	priv->write_byte(priv, priv->imr1_bits, IMR1);

	while(count < length)
	{
		// wait until byte is ready to be sent
		if(wait_event_interruptible(device->wait, test_bit(WRITE_READY_BN, &priv->state) ||
			test_bit(TIMO_NUM, &device->status)))
		{
			printk("gpib write interrupted!\n");
			retval = -EINTR;
			break;
		}
		if(test_bit(TIMO_NUM, &device->status))
		{
			retval = -ETIMEDOUT;
			break;
		}

		spin_lock_irqsave(&lock, flags);
		clear_bit(WRITE_READY_BN, &priv->state);
		priv->write_byte(priv, buffer[count++], CDOR);
		spin_unlock_irqrestore(&lock, flags);
	}

	// disable 'data out' interrupts
	priv->imr1_bits &= ~HR_DOIE;
	priv->write_byte(priv, priv->imr1_bits, IMR1);

	if(retval)
		return retval;

	return length;
}

static ssize_t dma_write(gpib_device_t *device, nec7210_private_t *priv, uint8_t *buffer, size_t length)
{
	unsigned long flags;
	int residue = 0;

	/* program dma controller */
	flags = claim_dma_lock();
	disable_dma(priv->dma_channel);
	clear_dma_ff(priv->dma_channel);
	set_dma_count(priv->dma_channel, length);
	set_dma_addr(priv->dma_channel, virt_to_bus(buffer));
	set_dma_mode(priv->dma_channel, DMA_MODE_WRITE );
	enable_dma(priv->dma_channel);

	// enable board's dma for output
	priv->imr2_bits |= HR_DMAO;
	priv->write_byte(priv, priv->imr2_bits, IMR2);

	clear_bit(WRITE_READY_BN, &priv->state);
	set_bit(DMA_IN_PROGRESS_BN, &priv->state);

	release_dma_lock(flags);

	// enable 'data out' interrupts
	priv->imr1_bits |= HR_DOIE;
	priv->write_byte(priv, priv->imr1_bits, IMR1);

	// suspend until message is sent
	if(wait_event_interruptible(device->wait, test_bit(DMA_IN_PROGRESS_BN, &priv->state) == 0 ||
		test_bit(TIMO_NUM, &device->status)))
	{
		printk("gpib write interrupted!\n");
	}

	// disable board's dma
	priv->imr2_bits &= ~HR_DMAO;
	priv->write_byte(priv, priv->imr2_bits, IMR2);

	// disable 'data out' interrupts
	priv->imr1_bits &= ~HR_DOIE;
	priv->write_byte(priv, priv->imr1_bits, IMR1);

	flags = claim_dma_lock();
	clear_dma_ff(priv->dma_channel);
	disable_dma(priv->dma_channel);
	residue = get_dma_residue(priv->dma_channel);
	release_dma_lock(flags);

	if(residue)
		return -EIO;

	return length;
}

ssize_t nec7210_write(gpib_device_t *device, nec7210_private_t *priv, uint8_t *buffer, size_t length, int send_eoi)
{
	size_t count = 0;
	ssize_t retval = 0;

	if(length == 0) return 0;

	/* normal handshaking, XXX necessary?*/
//	priv->write_byte(priv, priv->auxa_bits, AUXMR);

	if(send_eoi)
	{
		length-- ; /* save the last byte for sending EOI */
	}

	if(length > 0)
	{
		if(priv->dma_channel)
		{	// isa dma transfer
			retval = dma_write(device, priv, buffer, length);
			if(retval < 0)
				return retval;
			else count += retval;
		}else
		{	// PIO transfer
			retval = pio_write(device, priv, buffer, length);
			if(retval < 0)
				return retval;
			else count += retval;
		}
	}
	if(send_eoi)
	{
		/*send EOI */
		priv->write_byte(priv, AUX_SEOI, AUXMR);

		retval = pio_write(device, priv, &buffer[count], 1);
		if(retval < 0)
			return retval;
		else
			count++;
	}

	return count ? count : -1;
}













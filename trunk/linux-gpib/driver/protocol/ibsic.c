#include <ibprot.h>
#include <linux/delay.h>

extern int drvstat,ib_opened;

/*
 * IBSIC
 * Send IFC for at least 100 microseconds.
 *
 * NOTE:
 *      1.  Ibsic must be called prior to the first call to
 *          ibcmd in order to initialize the bus and enable the
 *          interface to leave the controller idle state.
 */
int ibsic(gpib_device_t *device)
{
	if(!test_bit(CIC_NUM, &device->status))
	{

		pgmstat |= PS_SAC;
		/* set controller state */
		device->interface->interface_clear(device, 1);                   /* assert IFC */
		udelay(100);
		device->interface->interface_clear(device, 0);                   /* clear IFC */
	}
// is CIC going to be set by core or driver? XXX
	set_bit(CIC_NUM, &device->status);

	return 0;
}


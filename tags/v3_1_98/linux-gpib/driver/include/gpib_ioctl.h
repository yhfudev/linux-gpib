/***************************************************************************
                          gpib_ioctl.h  -  header file for gpib library
                             -------------------

    copyright            : (C) 2002 by Frank Mori Hess
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


#ifndef _GPIB_IOCTL_H
#define _GPIB_IOCTL_H

#include <asm/ioctl.h>
#define GPIB_CODE 160

typedef struct
{
	char name[100];
} board_type_ioctl_t;

/* argument for read/write/command ioctls */
typedef struct
{
	int handle;
	uint8_t *buffer;
	unsigned long count;
	int end;
} read_write_ioctl_t;

typedef struct
{
	unsigned int handle;
	unsigned int pad;
	int sad;
	unsigned is_board : 1;
} open_dev_ioctl_t;

typedef struct
{
	unsigned int handle;
} close_dev_ioctl_t;

typedef struct
{
	unsigned int pad;
	int sad;
	uint8_t status_byte;
} serial_poll_ioctl_t;

typedef struct
{
	int eos;
	int eos_flags;
} eos_ioctl_t;

typedef struct
{
	int handle;
	int wait_mask;
	int clear_mask;
	int set_mask;
	int ibsta;
	int pad;
	int sad;
	unsigned long usec_timeout;
} wait_ioctl_t;

typedef struct
{
	int online;
} online_ioctl_t;

typedef struct
{
	unsigned int num_bytes;
	unsigned int pad;
	int sad;
} spoll_bytes_ioctl_t;

typedef struct
{
	unsigned int pad;
	int sad;
	int parallel_poll_configuration;
	int autopolling;
	int is_system_controller;
	unsigned int t1_delay;
	unsigned ist : 1;
} board_info_ioctl_t;

typedef struct
{
	int pci_bus;
	int pci_slot;
} select_pci_ioctl_t;

typedef struct
{
	uint8_t config;
	unsigned set_ist : 1;
	unsigned clear_ist : 1;
}	ppoll_config_ioctl_t;

typedef struct
{
	unsigned int handle;
	unsigned int pad;
} pad_ioctl_t;

typedef struct
{
	unsigned int handle;
	int sad;
} sad_ioctl_t;

typedef short event_ioctl_t;
typedef int rsc_ioctl_t;
typedef unsigned int t1_delay_ioctl_t;
typedef short autospoll_ioctl_t;

/* Standard functions. */
enum gpib_ioctl
{
	IBRD = _IOWR( GPIB_CODE, 0, read_write_ioctl_t ),
	IBWRT = _IOWR( GPIB_CODE, 1, read_write_ioctl_t ),
	IBCMD = _IOWR( GPIB_CODE, 2, read_write_ioctl_t ),
	IBOPENDEV = _IOWR( GPIB_CODE, 3, open_dev_ioctl_t ),
	IBCLOSEDEV = _IOW( GPIB_CODE, 4, close_dev_ioctl_t ),
	IBWAIT = _IOWR( GPIB_CODE, 5, wait_ioctl_t ),
	IBRPP = _IOWR( GPIB_CODE, 6, uint8_t ),

	IBSIC = _IOW( GPIB_CODE, 9, unsigned int ),
	IBSRE = _IOW( GPIB_CODE, 10, int ),
	IBGTS = _IO( GPIB_CODE, 11 ),
	IBCAC = _IOW( GPIB_CODE, 12, int ),
	IBLINES = _IOR( GPIB_CODE, 14, short ),
	IBPAD = _IOW( GPIB_CODE, 15, pad_ioctl_t ),
	IBSAD = _IOW( GPIB_CODE, 16, sad_ioctl_t ),
	IBTMO = _IOW( GPIB_CODE, 17, unsigned int ),
	IBRSP = _IOWR( GPIB_CODE, 18, serial_poll_ioctl_t ),
	IBEOS = _IOW( GPIB_CODE, 19, eos_ioctl_t ),
	IBRSV = _IOW( GPIB_CODE, 20, uint8_t ),
	CFCBASE = _IOW( GPIB_CODE, 21, unsigned long ),
	CFCIRQ = _IOW( GPIB_CODE, 22, unsigned int ),
	CFCDMA = _IOW( GPIB_CODE, 23, unsigned int ),
	CFCBOARDTYPE = _IOW( GPIB_CODE, 24, board_type_ioctl_t ),

	IBMUTEX = _IOW( GPIB_CODE, 26, int ),
	IBSPOLL_BYTES = _IOWR( GPIB_CODE, 27, spoll_bytes_ioctl_t ),
	IBPPC = _IOW( GPIB_CODE, 28, ppoll_config_ioctl_t ),
	IBBOARD_INFO = _IOR( GPIB_CODE, 29, board_info_ioctl_t ),

	IBQUERY_BOARD_RSV = _IOR( GPIB_CODE, 31, int ),
	IBSELECT_PCI = _IOWR( GPIB_CODE, 32, select_pci_ioctl_t ),
	IBEVENT = _IOR( GPIB_CODE, 33, event_ioctl_t ),
	IBRSC = _IOW( GPIB_CODE, 34, rsc_ioctl_t ),
	IB_T1_DELAY = _IOW( GPIB_CODE, 35, t1_delay_ioctl_t ),
	IBLOC = _IO( GPIB_CODE, 36 ),
	IBONL = _IOW( GPIB_CODE, 37, online_ioctl_t ),
	IBAUTOSPOLL = _IOW( GPIB_CODE, 38, autospoll_ioctl_t )
};

#endif	/* _GPIB_IOCTL_H */

#include "ib_internal.h"
#include <ibP.h>

int ibeos(int ud, int v)
{
	ibConf_t *conf;

	conf = enter_library( ud, 0 );
	if( conf == NULL )
		return exit_library( ud, 1 );

	conf->eos = v & 0xff;
	conf->eos_flags = v & 0xff00;

	return exit_library( ud, 0 );
}

/*
 *  iblcleos()
 *  sets the eos modes of the unit description (local) or
 *  board description (if none)
 *
 */

int iblcleos( const ibConf_t *conf )
{
	eos_ioctl_t eos_cmd;
	const ibBoard_t *board = &ibBoard[ conf->board ];

	eos_cmd.eos = conf->eos;
	eos_cmd.eos_flags = conf->eos_flags;

	return ioctl( board->fileno, IBEOS, &eos_cmd );
}

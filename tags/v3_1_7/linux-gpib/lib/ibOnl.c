
#include "ib_internal.h"
#include <ibP.h>
#include <stdlib.h>

// XXX supposed to reset board/device if onl is nonzero
int ibonl( int ud, int onl )
{
	int oflags=0;
	ibConf_t *conf;
	int retval;
	ibBoard_t *board;
	online_ioctl_t online_cmd;

	conf = enter_library( ud, 0 );
	if( conf == NULL )
		return exit_library( ud, 1 );

	board = interfaceBoard( conf );

	// XXX
	if( conf->flags & CN_EXCLUSIVE )
		oflags |= O_EXCL;

	if( board->fileno < 0 )
	{
		if( ibBoardOpen( conf->board, oflags ) < 0 )
		{
			return exit_library( ud, 1 );
		}
		if( ibBdChrConfig( conf ) < 0 )
		{
			setIberr( EDVR );
			setIbcnt( errno );
			return exit_library( ud, 1 );
		}
	}

	if( !onl )
	{
		retval = close_gpib_device( board, conf );
		if( retval < 0 )
		{
			fprintf( stderr, "libgpib: failed to mark device as closed!\n" );
			setIberr( EDVR );
			setIbcnt( errno );
			return exit_library( ud, 1 );
		}
	}

	online_cmd.master = board->is_system_controller;
	online_cmd.online = onl;
	retval = ioctl( board->fileno, IBONL, &online_cmd );
	if( retval < 0 )
	{
		switch( errno )
		{
			default:
				setIberr( EDVR );
				break;
		}
		return exit_library( ud, 1 );
	}

	if( onl )
	{
		retval = open_gpib_device( board, conf );
		if( retval < 0 )
		{
			fprintf( stderr, "libgpib: failed to mark device as open\n" );
			setIberr( EDVR );
			setIbcnt( errno );
			return exit_library( ud, 1 );
		}
	}else
	{
		if( conf->is_interface )
			ibBoardClose( conf->board );
		free( ibConfigs[ ud ] );
		ibConfigs[ ud ] = NULL;
		setIbsta( 0 );
		return 0;
	}

	return exit_library( ud, 0 );
}


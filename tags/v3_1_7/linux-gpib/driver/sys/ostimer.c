
#include <ibsys.h>

extern unsigned long timeTable[];

/*
 * Timer functions
 */
void watchdog_timeout( unsigned long arg )
/* Watchdog timeout routine */
{
	gpib_board_t *board = (gpib_board_t*) arg;
	set_bit( TIMO_NUM, &board->status );
	wake_up( &board->wait );
}
// store number of jiffies to wait for various timeouts
unsigned int usec_to_jiffies( unsigned int usec )
{
	unsigned int usec_per_jiffy = 1000000 / HZ;

	return ( usec + usec_per_jiffy - 1) / usec_per_jiffy;
}

/* install timer interrupt handler */
void osStartTimer( gpib_board_t *board, unsigned int usec_timeout )
/* Starts the timeout task  */
/* v = index into timeTable */
{
	clear_bit( TIMO_NUM, &board->status );

	if( usec_timeout > 0 )
	{
		board->timer.expires = jiffies + usec_to_jiffies( usec_timeout );   /* set number of ticks */
		board->timer.function = watchdog_timeout;
		board->timer.data = (unsigned long) board;
		add_timer( &board->timer );              /* add timer           */
	}
}


void osRemoveTimer( gpib_board_t *board )
/* Removes the timeout task */
{
	if( timer_pending( &board->timer ) )
		del_timer_sync( &board->timer );
}

int io_timed_out( gpib_board_t *board )
{
	if( test_bit( TIMO_NUM, &board->status ) ) return 1;
	return 0;
}

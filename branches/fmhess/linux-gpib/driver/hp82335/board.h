
#include <gpibP.h>

extern uint16      ibbase;	/* base addr of GPIB interface registers  */
extern uint8       ibirq;	/* interrupt request line for GPIB (1-7)  */
extern uint8       ibdma ;      /* DMA channel                            */

extern volatile int noTimo;     /* timeout flag */
extern int          pgmstat;    /* Program state */
extern int          ccrbits;    /* static bits for card control register */

extern ibregs_t *ib;            /* Local pointer to IB registers */
#define IB ib              

#if DMAOP
#error This board does not support DMA (not implemented yet)
#endif







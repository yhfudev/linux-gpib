#include <board.h>

#if DMAOP
#include <asm/dma.h>


/*
 *  BdDMAwait
 *  This function checks or waits for "DMA operation complete".
 */
IBLCL void bdDMAwait(ibio_op_t *rwop, int noWait)
{
	DBGin("bdDMAwait");
	if (rwop->io_flags & IO_READ){
           WaitingFor( HR_DMAI );
	} else {
           WaitingFor( HR_DMAO );
        }
	DBGout();
}

/*
 *  BdDMAstart
 *  This function configures and starts the DMA controller.
 */
IBLCL void bdDMAstart(ibio_op_t *rwop)
{
	DBGin("bdDMAstart");

        /*
         * Enable DMATC INT for the desired direction
         */

        if (rwop->io_flags & IO_READ) {
		GPIBout(IMR2, HR_DMAI);
		GPIBout(IMR1, HR_ENDIE);
	} else {
		GPIBout(IMR1, HR_ERRIE);  /* set IMR1 before IMR2 */
		GPIBout(IMR2, HR_DMAO);
        }

        GPIBout(AUXMR, auxrabits | HR_HLDA); /* RFD Hold off on all Data */
        GPIBout(AUXMR, AUX_FH );             /* Finish hand shake   */
        GPIBout(AUXMR, auxrabits | HR_HLDE); /* RFD Hold Off on End */

	DBGout();
}




/*
 *  BdDMAstop
 *  This function halts the DMA controller.
 */
IBLCL int bdDMAstop(ibio_op_t *rwop)
{
        int resid;

	DBGin("bdDMAstop");

        /*
         * Set RFD hold off on All data
         */
        GPIBout(AUXMR, auxrabits | HR_HLDA);
        /*
         * Clear all interrupt enable bits
         */
	GPIBout(IMR2, 0);
	GPIBout(IMR1, 0);
        /*
         * get the number of byte remaining
         */ 
        resid = get_dma_residue( ibdma );
	DBGprint(DBG_DATA,("resid = %d",resid));
	DBGout();
        return resid;
}

#endif /* DMAOP */





/* Automatically created by mkproto.sh. DONT EDIT */
/***** Public Functions ******/
extern  void osDMAAdjCnt(ibio_op_t *rwop);
extern  void osPIOAdjCnt(ibio_op_t *rwop);
extern  int ibVFSwrite( LINUX_STD_PARAM, char *buffer, int count);
extern  int ibVFSread( LINUX_STD_PARAM, char *buffer, int count);
extern  int osInit(void);
extern  void osReset(void);
extern  void ibintr(int irq, struct pt_regs *registerp );
extern  void osWaitForInt( int imr3mask );
extern  void osMemInit(void); 
extern  void osMemRelease(void); 
extern  void ibtmintr(unsigned long unused);			
extern  void osStartTimer(int v);			
extern  void osRemoveTimer(void);
extern  void osSendEOI(void);
extern  void osSendEOI(void);
extern  uint8 osP8in(short in_addr);
extern  void osP8out(short out_addr,uint8 out_value);	
extern  uint16 osP16in(short in_addr);
extern  void osP16out(short out_addr, uint16 out_value);	
extern  void osChngBase(int new_base);
extern  void osChngIRQ(int new_irq);
extern  void osChngDMA(int new_dma);
extern  uint32 osRegAddr(io_addr);
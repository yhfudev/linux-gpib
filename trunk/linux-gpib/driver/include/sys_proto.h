/* Automatically created by mkproto.sh. DONT EDIT */
/***** Public Functions ******/
extern  void osDMAAdjCnt(ibio_op_t *rwop);
extern  void osPIOAdjCnt(ibio_op_t *rwop);
extern  int osDoDMA(ibio_op_t *rwop);
extern  int ibopen(struct inode *inode, struct file *filep);
extern  int ibclose(struct inode *inode, struct file *file);
extern  int ibioctl(struct inode *inode, struct file *filep, unsigned int cmd, unsigned long arg);
extern  int ibVFSwrite( struct file *filep, const char *buffer, size_t count, loff_t *offset);
extern  int ibVFSread(struct file *filep, char *buffer, size_t count, loff_t *offset);
extern  int osInit(void);
extern  void osReset(void);
extern  void osWaitForInt( int imr3mask );
extern  void osLockMutex( void );
extern  void osUnlockMutex( void );
extern  void osMemInit(void); 
extern  void osMemRelease(void); 
extern  char *osGetDMABuffer( int *size );
extern  void osFreeDMABuffer( char *buf );
extern  void ibtmintr(unsigned long unused);			
extern  void osStartTimer(int v);			
extern  void osRemoveTimer(void);
extern  void osSendEOI(void);
extern  void osSendEOI(void);
extern  void osChngBase(int new_base);
extern  void osChngIRQ(int new_irq);
extern  void osChngDMA(int new_dma);
extern  uint32 osRegAddr(faddr_t io_addr);

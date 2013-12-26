#ifndef MCD_TSK_INIT_H
#define MCD_TSK_INIT_H 1

/*
 * Do not edit!
 */


/*
 * Task 0
 */
void  MCD_startDmaChainNoEu(int *currBD, short srcIncr, short destIncr, int xferSize, short xferSizeIncr, int *cSave, volatile TaskTableEntry *taskTable, int channel);


/*
 * Task 1
 */
void  MCD_startDmaSingleNoEu(int8_t *srcAddr, short srcIncr, int8_t *destAddr, short destIncr, int dmaSize, short xferSizeIncr, int flags, int *currBD, int *cSave, volatile TaskTableEntry *taskTable, int channel);


/*
 * Task 2
 */
void  MCD_startDmaChainEu(int *currBD, short srcIncr, short destIncr, int xferSize, short xferSizeIncr, int *cSave, volatile TaskTableEntry *taskTable, int channel);


/*
 * Task 3
 */
void  MCD_startDmaSingleEu(int8_t *srcAddr, short srcIncr, int8_t *destAddr, short destIncr, int dmaSize, short xferSizeIncr, int flags, int *currBD, int *cSave, volatile TaskTableEntry *taskTable, int channel);


/*
 * Task 4
 */
void  MCD_startDmaENetRcv(int8_t *bDBase, int8_t *currBD, int8_t *rcvFifoPtr, volatile TaskTableEntry *taskTable, int channel);


/*
 * Task 5
 */
void  MCD_startDmaENetXmit(int8_t *bDBase, int8_t *currBD, int8_t *xmitFifoPtr, volatile TaskTableEntry *taskTable, int channel);

#endif  /* MCD_TSK_INIT_H */

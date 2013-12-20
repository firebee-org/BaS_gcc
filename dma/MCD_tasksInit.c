/*
 * File:        MCD_tasksInit.c
 * Purpose:     Functions for initializing variable tables of different
 *              types of tasks.
 *
 * Notes:
 */

/*
 * Do not edit!
 */

#include "MCD_dma.h"
#include "MCD_tasksInit.h"

extern dmaRegs *MCD_dmaBar;


/*
 * Task 0
 */
 
void  MCD_startDmaChainNoEu(int *currBD, short srcIncr, short destIncr, int xferSize, short xferSizeIncr, int *cSave, volatile TaskTableEntry *taskTable, int channel)
{

    MCD_SET_VAR(taskTable+channel, 2, (uint32_t)currBD); /* var[2] */
    MCD_SET_VAR(taskTable+channel, 25, (uint32_t)(0xe000 << 16) | (0xffff & srcIncr));   /* inc[1] */
    MCD_SET_VAR(taskTable+channel, 24, (uint32_t)(0xe000 << 16) | (0xffff & destIncr));  /* inc[0] */
    MCD_SET_VAR(taskTable+channel, 11, (uint32_t)xferSize);  /* var[11] */
    MCD_SET_VAR(taskTable+channel, 26, (uint32_t)(0x2000 << 16) | (0xffff & xferSizeIncr));  /* inc[2] */
    MCD_SET_VAR(taskTable+channel, 0, (uint32_t)cSave);  /* var[0] */
    MCD_SET_VAR(taskTable+channel, 1, (uint32_t)0x00000000); /* var[1] */
    MCD_SET_VAR(taskTable+channel, 3, (uint32_t)0x00000000); /* var[3] */
    MCD_SET_VAR(taskTable+channel, 4, (uint32_t)0x00000000); /* var[4] */
    MCD_SET_VAR(taskTable+channel, 5, (uint32_t)0x00000000); /* var[5] */
    MCD_SET_VAR(taskTable+channel, 6, (uint32_t)0x00000000); /* var[6] */
    MCD_SET_VAR(taskTable+channel, 7, (uint32_t)0x00000000); /* var[7] */
    MCD_SET_VAR(taskTable+channel, 8, (uint32_t)0x00000000); /* var[8] */
    MCD_SET_VAR(taskTable+channel, 9, (uint32_t)0x00000000); /* var[9] */
    MCD_SET_VAR(taskTable+channel, 10, (uint32_t)0x00000000);    /* var[10] */
    MCD_SET_VAR(taskTable+channel, 12, (uint32_t)0x00000000);    /* var[12] */
    MCD_SET_VAR(taskTable+channel, 13, (uint32_t)0x80000000);    /* var[13] */
    MCD_SET_VAR(taskTable+channel, 14, (uint32_t)0x00000010);    /* var[14] */
    MCD_SET_VAR(taskTable+channel, 15, (uint32_t)0x00000004);    /* var[15] */
    MCD_SET_VAR(taskTable+channel, 16, (uint32_t)0x08000000);    /* var[16] */
    MCD_SET_VAR(taskTable+channel, 27, (uint32_t)0x00000000);    /* inc[3] */
    MCD_SET_VAR(taskTable+channel, 28, (uint32_t)0x80000000);    /* inc[4] */
    MCD_SET_VAR(taskTable+channel, 29, (uint32_t)0x80000001);    /* inc[5] */
    MCD_SET_VAR(taskTable+channel, 30, (uint32_t)0x40000000);    /* inc[6] */

    /* Set the task's Enable bit in its Task Control Register */
    MCD_dmaBar->taskControl[channel] |= (uint16_t)0x8000;
}


/*
 * Task 1
 */
 
void  MCD_startDmaSingleNoEu(char *srcAddr, short srcIncr, char *destAddr, short destIncr, int dmaSize, short xferSizeIncr, int flags, int *currBD, int *cSave, volatile TaskTableEntry *taskTable, int channel)
{

    MCD_SET_VAR(taskTable+channel, 7, (uint32_t)srcAddr);    /* var[7] */
    MCD_SET_VAR(taskTable+channel, 25, (uint32_t)(0xe000 << 16) | (0xffff & srcIncr));   /* inc[1] */
    MCD_SET_VAR(taskTable+channel, 2, (uint32_t)destAddr);   /* var[2] */
    MCD_SET_VAR(taskTable+channel, 24, (uint32_t)(0xe000 << 16) | (0xffff & destIncr));  /* inc[0] */
    MCD_SET_VAR(taskTable+channel, 3, (uint32_t)dmaSize);    /* var[3] */
    MCD_SET_VAR(taskTable+channel, 26, (uint32_t)(0x2000 << 16) | (0xffff & xferSizeIncr));  /* inc[2] */
    MCD_SET_VAR(taskTable+channel, 5, (uint32_t)flags);  /* var[5] */
    MCD_SET_VAR(taskTable+channel, 1, (uint32_t)currBD); /* var[1] */
    MCD_SET_VAR(taskTable+channel, 0, (uint32_t)cSave);  /* var[0] */
    MCD_SET_VAR(taskTable+channel, 4, (uint32_t)0x00000000); /* var[4] */
    MCD_SET_VAR(taskTable+channel, 6, (uint32_t)0x00000000); /* var[6] */
    MCD_SET_VAR(taskTable+channel, 8, (uint32_t)0x00000000); /* var[8] */
    MCD_SET_VAR(taskTable+channel, 9, (uint32_t)0x00000004); /* var[9] */
    MCD_SET_VAR(taskTable+channel, 10, (uint32_t)0x08000000);    /* var[10] */
    MCD_SET_VAR(taskTable+channel, 27, (uint32_t)0x00000000);    /* inc[3] */
    MCD_SET_VAR(taskTable+channel, 28, (uint32_t)0x80000001);    /* inc[4] */
    MCD_SET_VAR(taskTable+channel, 29, (uint32_t)0x40000000);    /* inc[5] */

    /* Set the task's Enable bit in its Task Control Register */
    MCD_dmaBar->taskControl[channel] |= (uint16_t)0x8000;
}


/*
 * Task 2
 */
 
void  MCD_startDmaChainEu(int *currBD, short srcIncr, short destIncr, int xferSize, short xferSizeIncr, int *cSave, volatile TaskTableEntry *taskTable, int channel)
{

    MCD_SET_VAR(taskTable+channel, 3, (uint32_t)currBD); /* var[3] */
    MCD_SET_VAR(taskTable+channel, 25, (uint32_t)(0xe000 << 16) | (0xffff & srcIncr));   /* inc[1] */
    MCD_SET_VAR(taskTable+channel, 24, (uint32_t)(0xe000 << 16) | (0xffff & destIncr));  /* inc[0] */
    MCD_SET_VAR(taskTable+channel, 12, (uint32_t)xferSize);  /* var[12] */
    MCD_SET_VAR(taskTable+channel, 26, (uint32_t)(0x2000 << 16) | (0xffff & xferSizeIncr));  /* inc[2] */
    MCD_SET_VAR(taskTable+channel, 0, (uint32_t)cSave);  /* var[0] */
    MCD_SET_VAR(taskTable+channel, 1, (uint32_t)0x00000000); /* var[1] */
    MCD_SET_VAR(taskTable+channel, 2, (uint32_t)0x00000000); /* var[2] */
    MCD_SET_VAR(taskTable+channel, 4, (uint32_t)0x00000000); /* var[4] */
    MCD_SET_VAR(taskTable+channel, 5, (uint32_t)0x00000000); /* var[5] */
    MCD_SET_VAR(taskTable+channel, 6, (uint32_t)0x00000000); /* var[6] */
    MCD_SET_VAR(taskTable+channel, 7, (uint32_t)0x00000000); /* var[7] */
    MCD_SET_VAR(taskTable+channel, 8, (uint32_t)0x00000000); /* var[8] */
    MCD_SET_VAR(taskTable+channel, 9, (uint32_t)0x00000000); /* var[9] */
    MCD_SET_VAR(taskTable+channel, 10, (uint32_t)0x00000000);    /* var[10] */
    MCD_SET_VAR(taskTable+channel, 11, (uint32_t)0x00000000);    /* var[11] */
    MCD_SET_VAR(taskTable+channel, 13, (uint32_t)0x00000000);    /* var[13] */
    MCD_SET_VAR(taskTable+channel, 14, (uint32_t)0x80000000);    /* var[14] */
    MCD_SET_VAR(taskTable+channel, 15, (uint32_t)0x00000010);    /* var[15] */
    MCD_SET_VAR(taskTable+channel, 16, (uint32_t)0x00000001);    /* var[16] */
    MCD_SET_VAR(taskTable+channel, 17, (uint32_t)0x00000004);    /* var[17] */
    MCD_SET_VAR(taskTable+channel, 18, (uint32_t)0x08000000);    /* var[18] */
    MCD_SET_VAR(taskTable+channel, 27, (uint32_t)0x00000000);    /* inc[3] */
    MCD_SET_VAR(taskTable+channel, 28, (uint32_t)0x80000000);    /* inc[4] */
    MCD_SET_VAR(taskTable+channel, 29, (uint32_t)0xc0000000);    /* inc[5] */
    MCD_SET_VAR(taskTable+channel, 30, (uint32_t)0x80000001);    /* inc[6] */
    MCD_SET_VAR(taskTable+channel, 31, (uint32_t)0x40000000);    /* inc[7] */

    /* Set the task's Enable bit in its Task Control Register */
    MCD_dmaBar->taskControl[channel] |= (uint16_t)0x8000;
}


/*
 * Task 3
 */
 
void  MCD_startDmaSingleEu(char *srcAddr, short srcIncr, char *destAddr, short destIncr, int dmaSize, short xferSizeIncr, int flags, int *currBD, int *cSave, volatile TaskTableEntry *taskTable, int channel)
{

    MCD_SET_VAR(taskTable+channel, 8, (uint32_t)srcAddr);    /* var[8] */
    MCD_SET_VAR(taskTable+channel, 25, (uint32_t)(0xe000 << 16) | (0xffff & srcIncr));   /* inc[1] */
    MCD_SET_VAR(taskTable+channel, 3, (uint32_t)destAddr);   /* var[3] */
    MCD_SET_VAR(taskTable+channel, 24, (uint32_t)(0xe000 << 16) | (0xffff & destIncr));  /* inc[0] */
    MCD_SET_VAR(taskTable+channel, 4, (uint32_t)dmaSize);    /* var[4] */
    MCD_SET_VAR(taskTable+channel, 26, (uint32_t)(0x2000 << 16) | (0xffff & xferSizeIncr));  /* inc[2] */
    MCD_SET_VAR(taskTable+channel, 6, (uint32_t)flags);  /* var[6] */
    MCD_SET_VAR(taskTable+channel, 2, (uint32_t)currBD); /* var[2] */
    MCD_SET_VAR(taskTable+channel, 0, (uint32_t)cSave);  /* var[0] */
    MCD_SET_VAR(taskTable+channel, 1, (uint32_t)0x00000000); /* var[1] */
    MCD_SET_VAR(taskTable+channel, 5, (uint32_t)0x00000000); /* var[5] */
    MCD_SET_VAR(taskTable+channel, 7, (uint32_t)0x00000000); /* var[7] */
    MCD_SET_VAR(taskTable+channel, 9, (uint32_t)0x00000000); /* var[9] */
    MCD_SET_VAR(taskTable+channel, 10, (uint32_t)0x00000001);    /* var[10] */
    MCD_SET_VAR(taskTable+channel, 11, (uint32_t)0x00000004);    /* var[11] */
    MCD_SET_VAR(taskTable+channel, 12, (uint32_t)0x08000000);    /* var[12] */
    MCD_SET_VAR(taskTable+channel, 27, (uint32_t)0x00000000);    /* inc[3] */
    MCD_SET_VAR(taskTable+channel, 28, (uint32_t)0xc0000000);    /* inc[4] */
    MCD_SET_VAR(taskTable+channel, 29, (uint32_t)0x80000000);    /* inc[5] */
    MCD_SET_VAR(taskTable+channel, 30, (uint32_t)0x80000001);    /* inc[6] */
    MCD_SET_VAR(taskTable+channel, 31, (uint32_t)0x40000000);    /* inc[7] */

    /* Set the task's Enable bit in its Task Control Register */
    MCD_dmaBar->taskControl[channel] |= (uint16_t)0x8000;
}


/*
 * Task 4
 */
 
void  MCD_startDmaENetRcv(char *bDBase, char *currBD, char *rcvFifoPtr, volatile TaskTableEntry *taskTable, int channel)
{

    MCD_SET_VAR(taskTable+channel, 0, (uint32_t)bDBase); /* var[0] */
    MCD_SET_VAR(taskTable+channel, 3, (uint32_t)currBD); /* var[3] */
    MCD_SET_VAR(taskTable+channel, 6, (uint32_t)rcvFifoPtr); /* var[6] */
    MCD_SET_VAR(taskTable+channel, 1, (uint32_t)0x00000000); /* var[1] */
    MCD_SET_VAR(taskTable+channel, 2, (uint32_t)0x00000000); /* var[2] */
    MCD_SET_VAR(taskTable+channel, 4, (uint32_t)0x00000000); /* var[4] */
    MCD_SET_VAR(taskTable+channel, 5, (uint32_t)0x00000000); /* var[5] */
    MCD_SET_VAR(taskTable+channel, 7, (uint32_t)0x00000000); /* var[7] */
    MCD_SET_VAR(taskTable+channel, 8, (uint32_t)0x00000000); /* var[8] */
    MCD_SET_VAR(taskTable+channel, 9, (uint32_t)0x0000ffff); /* var[9] */
    MCD_SET_VAR(taskTable+channel, 10, (uint32_t)0x30000000);    /* var[10] */
    MCD_SET_VAR(taskTable+channel, 11, (uint32_t)0x0fffffff);    /* var[11] */
    MCD_SET_VAR(taskTable+channel, 12, (uint32_t)0x00000008);    /* var[12] */
    MCD_SET_VAR(taskTable+channel, 24, (uint32_t)0x00000000);    /* inc[0] */
    MCD_SET_VAR(taskTable+channel, 25, (uint32_t)0x60000000);    /* inc[1] */
    MCD_SET_VAR(taskTable+channel, 26, (uint32_t)0x20000004);    /* inc[2] */
    MCD_SET_VAR(taskTable+channel, 27, (uint32_t)0x40000000);    /* inc[3] */

    /* Set the task's Enable bit in its Task Control Register */
    MCD_dmaBar->taskControl[channel] |= (uint16_t)0x8000;
}


/*
 * Task 5
 */
 
void  MCD_startDmaENetXmit(char *bDBase, char *currBD, char *xmitFifoPtr, volatile TaskTableEntry *taskTable, int channel)
{

    MCD_SET_VAR(taskTable+channel, 0, (uint32_t)bDBase); /* var[0] */
    MCD_SET_VAR(taskTable+channel, 3, (uint32_t)currBD); /* var[3] */
    MCD_SET_VAR(taskTable+channel, 11, (uint32_t)xmitFifoPtr);   /* var[11] */
    MCD_SET_VAR(taskTable+channel, 1, (uint32_t)0x00000000); /* var[1] */
    MCD_SET_VAR(taskTable+channel, 2, (uint32_t)0x00000000); /* var[2] */
    MCD_SET_VAR(taskTable+channel, 4, (uint32_t)0x00000000); /* var[4] */
    MCD_SET_VAR(taskTable+channel, 5, (uint32_t)0x00000000); /* var[5] */
    MCD_SET_VAR(taskTable+channel, 6, (uint32_t)0x00000000); /* var[6] */
    MCD_SET_VAR(taskTable+channel, 7, (uint32_t)0x00000000); /* var[7] */
    MCD_SET_VAR(taskTable+channel, 8, (uint32_t)0x00000000); /* var[8] */
    MCD_SET_VAR(taskTable+channel, 9, (uint32_t)0x00000000); /* var[9] */
    MCD_SET_VAR(taskTable+channel, 10, (uint32_t)0x00000000);    /* var[10] */
    MCD_SET_VAR(taskTable+channel, 12, (uint32_t)0x00000000);    /* var[12] */
    MCD_SET_VAR(taskTable+channel, 13, (uint32_t)0x0000ffff);    /* var[13] */
    MCD_SET_VAR(taskTable+channel, 14, (uint32_t)0xffffffff);    /* var[14] */
    MCD_SET_VAR(taskTable+channel, 15, (uint32_t)0x00000004);    /* var[15] */
    MCD_SET_VAR(taskTable+channel, 16, (uint32_t)0x00000008);    /* var[16] */
    MCD_SET_VAR(taskTable+channel, 24, (uint32_t)0x00000000);    /* inc[0] */
    MCD_SET_VAR(taskTable+channel, 25, (uint32_t)0x60000000);    /* inc[1] */
    MCD_SET_VAR(taskTable+channel, 26, (uint32_t)0x40000000);    /* inc[2] */
    MCD_SET_VAR(taskTable+channel, 27, (uint32_t)0xc000fffc);    /* inc[3] */
    MCD_SET_VAR(taskTable+channel, 28, (uint32_t)0xe0000004);    /* inc[4] */
    MCD_SET_VAR(taskTable+channel, 29, (uint32_t)0x80000000);    /* inc[5] */
    MCD_SET_VAR(taskTable+channel, 30, (uint32_t)0x4000ffff);    /* inc[6] */
    MCD_SET_VAR(taskTable+channel, 31, (uint32_t)0xe0000001);    /* inc[7] */

    /* Set the task's Enable bit in its Task Control Register */
    MCD_dmaBar->taskControl[channel] |= (uint16_t)0x8000;
}

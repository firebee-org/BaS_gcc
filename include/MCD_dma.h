/*
 * File:        MCD_dma.h
 * Purpose:     Main header file for multi-channel DMA API.
 *
 * Notes:
 */
#ifndef _MCD_API_H
#define _MCD_API_H

#include "bas_types.h"

/*
 * Turn Execution Unit tasks ON (#define) or OFF (#undef)
 */
#undef MCD_INCLUDE_EU
//#define MCD_INCLUDE_EU

/*
 * Number of DMA channels
 */
#define NCHANNELS 16

/*
 * Total number of variants
 */
#ifdef MCD_INCLUDE_EU
#define NUMOFVARIANTS   6
#else
#define NUMOFVARIANTS   4
#endif

/*
 * Define sizes of the various tables
 */
#define TASK_TABLE_SIZE     (NCHANNELS*32)
#define VAR_TAB_SIZE        (128)
#define CONTEXT_SAVE_SIZE   (128)
#define FUNCDESC_TAB_SIZE   (256)

#ifdef MCD_INCLUDE_EU
#define FUNCDESC_TAB_NUM    16
#else
#define FUNCDESC_TAB_NUM    1
#endif


#ifndef DEFINESONLY

/*
 * Portability typedefs
 */

/*
 * These structures represent the internal registers of the
 * multi-channel DMA
 */
struct dmaRegs_s {
   uint32_t taskbar;         /* task table base address register */
   uint32_t currPtr;
   uint32_t endPtr;
   uint32_t varTablePtr;
   uint16_t dma_rsvd0;
   uint16_t ptdControl;      /* ptd control */
   uint32_t intPending;      /* interrupt pending register */
   uint32_t intMask;         /* interrupt mask register */
   uint16_t taskControl[16]; /* task control registers */
   uint8_t  priority[32];    /* priority registers */
   uint32_t initiatorMux;    /* initiator mux control */
   uint32_t taskSize0;       /* task size control register 0. */
   uint32_t taskSize1;       /* task size control register 1. */
   uint32_t dma_rsvd1;       /* reserved */
   uint32_t dma_rsvd2;       /* reserved */
   uint32_t debugComp1;      /* debug comparator 1 */
   uint32_t debugComp2;      /* debug comparator 2 */
   uint32_t debugControl;    /* debug control */
   uint32_t debugStatus;     /* debug status */
   uint32_t ptdDebug;        /* priority task decode debug */
   uint32_t dma_rsvd3[31];   /* reserved */
};
typedef volatile struct dmaRegs_s dmaRegs;

#endif

/*
 * PTD contrl reg bits
 */
#define PTD_CTL_TSK_PRI         0x8000
#define PTD_CTL_COMM_PREFETCH   0x0001

/*
 * Task Control reg bits and field masks
 */
#define TASK_CTL_EN             0x8000
#define TASK_CTL_VALID          0x4000
#define TASK_CTL_ALWAYS         0x2000
#define TASK_CTL_INIT_MASK      0x1f00
#define TASK_CTL_ASTRT          0x0080
#define TASK_CTL_HIPRITSKEN     0x0040
#define TASK_CTL_HLDINITNUM     0x0020
#define TASK_CTL_ASTSKNUM_MASK  0x000f

/*
 * Priority reg bits and field masks
 */
#define PRIORITY_HLD            0x80
#define PRIORITY_PRI_MASK       0x07

/*
 * Debug Control reg bits and field masks
 */
#define DBG_CTL_BLOCK_TASKS_MASK    0xffff0000
#define DBG_CTL_AUTO_ARM            0x00008000
#define DBG_CTL_BREAK               0x00004000
#define DBG_CTL_COMP1_TYP_MASK      0x00003800
#define DBG_CTL_COMP2_TYP_MASK      0x00000070
#define DBG_CTL_EXT_BREAK           0x00000004
#define DBG_CTL_INT_BREAK           0x00000002

/*
 * PTD Debug reg selector addresses
 * This reg must be written with a value to show the contents of
 * one of the desired internal register.
 */
#define PTD_DBG_REQ             0x00 /* shows the state of 31 initiators */
#define PTD_DBG_TSK_VLD_INIT    0x01 /* shows which 16 tasks are valid and
                                        have initiators asserted */


/*
 * General return values
 */
#define MCD_OK                   0
#define MCD_ERROR               -1
#define MCD_TABLE_UNALIGNED     -2
#define MCD_CHANNEL_INVALID     -3

/*
 * MCD_initDma input flags
 */
#define MCD_RELOC_TASKS         0x00000001
#define MCD_NO_RELOC_TASKS      0x00000000
#define MCD_COMM_PREFETCH_EN    0x00000002  /* Commbus Prefetching - MCF547x/548x ONLY */

/*
 * MCD_dmaStatus Status Values for each channel
 */
#define MCD_NO_DMA  1 /* No DMA has been requested since reset */
#define MCD_IDLE    2 /* DMA active, but the initiator is currently inactive */
#define MCD_RUNNING 3 /* DMA active, and the initiator is currently active */
#define MCD_PAUSED  4 /* DMA active but it is currently paused */
#define MCD_HALTED  5 /* the most recent DMA has been killed with MCD_killTask() */
#define MCD_DONE    6 /* the most recent DMA has completed. */


/*
 * MCD_startDma parameter defines
 */

/*
 * Constants for the funcDesc parameter
 */
/* Byte swapping: */
#define MCD_NO_BYTE_SWAP    0x00045670  /* to disable byte swapping. */
#define MCD_BYTE_REVERSE    0x00076540  /* to reverse the bytes of each uint32_t of the DMAed data. */
#define MCD_U16_REVERSE     0x00067450  /* to reverse the 16-bit halves of
                                           each 32-bit data value being DMAed.*/
#define MCD_U16_BYTE_REVERSE    0x00054760 /* to reverse the byte halves of each
                                            16-bit half of each 32-bit data value DMAed */
#define MCD_NO_BIT_REV  0x00000000  /* do not reverse the bits of each byte DMAed. */
#define MCD_BIT_REV     0x00088880  /* reverse the bits of each byte DMAed */
/* CRCing: */
#define MCD_CRC16       0xc0100000  /* to perform CRC-16 on DMAed data. */
#define MCD_CRCCCITT    0xc0200000  /* to perform CRC-CCITT on DMAed data. */
#define MCD_CRC32       0xc0300000  /* to perform CRC-32 on DMAed data. */
#define MCD_CSUMINET    0xc0400000  /* to perform internet checksums on DMAed data.*/
#define MCD_NO_CSUM     0xa0000000  /* to perform no checksumming. */

#define MCD_FUNC_NOEU1 (MCD_NO_BYTE_SWAP | MCD_NO_BIT_REV | MCD_NO_CSUM)
#define MCD_FUNC_NOEU2 (MCD_NO_BYTE_SWAP | MCD_NO_CSUM)

/*
 * Constants for the flags parameter
 */
#define MCD_TT_FLAGS_RL   0x00000001 /* Read line */
#define MCD_TT_FLAGS_CW   0x00000002 /* Combine Writes */
#define MCD_TT_FLAGS_SP   0x00000004 /* Speculative prefetch(XLB) MCF547x/548x ONLY  */
#define MCD_TT_FLAGS_MASK 0x000000ff
#define MCD_TT_FLAGS_DEF  (MCD_TT_FLAGS_RL | MCD_TT_FLAGS_CW)

#define MCD_SINGLE_DMA  0x00000100 /* Unchained DMA */
#define MCD_CHAIN_DMA              /* TBD */
#define MCD_EU_DMA                 /* TBD */
#define MCD_FECTX_DMA   0x00001000 /* FEC TX ring DMA */
#define MCD_FECRX_DMA   0x00002000 /* FEC RX ring DMA */


/* these flags are valid for MCD_startDma and the chained buffer descriptors */
#define MCD_BUF_READY   0x80000000 /* indicates that this buffer is now under the DMA's control */
#define MCD_WRAP        0x20000000 /* to tell the FEC Dmas to wrap to the first BD */
#define MCD_INTERRUPT   0x10000000 /* to generate an interrupt after completion of the DMA. */
#define MCD_END_FRAME   0x08000000 /* tell the DMA to end the frame when transferring
                                      last byte of data in buffer */
#define MCD_CRC_RESTART 0x40000000 /* to empty out the accumulated checksum
                                      prior to performing the DMA. */

/* Defines for the FEC buffer descriptor control/status word*/
#define MCD_FEC_BUF_READY   0x8000
#define MCD_FEC_WRAP        0x2000
#define MCD_FEC_INTERRUPT   0x1000
#define MCD_FEC_END_FRAME   0x0800


/*
 * Defines for general intuitiveness
 */

#define MCD_TRUE  1
#define MCD_FALSE 0

/*
 * Three different cases for destination and source.
 */
#define MINUS1          -1
#define ZERO            0
#define PLUS1           1

#ifndef DEFINESONLY

/* Task Table Entry struct*/
typedef struct {
    uint32_t TDTstart;   /* task descriptor table start */
    uint32_t TDTend;     /* task descriptor table end */
    uint32_t varTab;     /* variable table start */
    uint32_t FDTandFlags;    /* function descriptor table start and flags */
    volatile uint32_t descAddrAndStatus;
    volatile uint32_t modifiedVarTab;
    uint32_t contextSaveSpace;   /* context save space start */
    uint32_t literalBases;
} TaskTableEntry;


/* Chained buffer descriptor */
typedef volatile struct MCD_bufDesc_struct MCD_bufDesc;
struct MCD_bufDesc_struct {
   uint32_t flags;         /* flags describing the DMA */
   uint32_t csumResult;    /* checksum from checksumming performed since last checksum reset */
   int8_t  *srcAddr;      /* the address to move data from */
   int8_t  *destAddr;     /* the address to move data to */
   int8_t  *lastDestAddr; /* the last address written to */
   uint32_t dmaSize;       /* the number of bytes to transfer independent of the transfer size */
   MCD_bufDesc *next; /* next buffer descriptor in chain */
   uint32_t info;          /* private information about this descriptor;  DMA does not affect it */
};

/* Progress Query struct */
typedef volatile struct MCD_XferProg_struct {
   int8_t *lastSrcAddr;         /* the most-recent or last, post-increment source address */
   int8_t *lastDestAddr;        /* the most-recent or last, post-increment destination address */
   uint32_t  dmaSize;            /* the amount of data transferred for the current buffer */
   MCD_bufDesc *currBufDesc;/* pointer to the current buffer descriptor being DMAed */
} MCD_XferProg;


/* FEC buffer descriptor */
typedef volatile struct MCD_bufDescFec_struct {
    uint16_t statCtrl;
    uint16_t length;
    uint32_t dataPointer;
} MCD_bufDescFec;


/*************************************************************************/
/*
 * API function Prototypes  - see MCD_dmaApi.c for further notes
 */

/*
 * MCD_startDma starts a particular kind of DMA .
 */
int MCD_startDma (
   int channel,   /* the channel on which to run the DMA */
   int8_t  *srcAddr,  /* the address to move data from, or buffer-descriptor address */
   int16_t srcIncr,   /* the amount to increment the source address per transfer */
   int8_t  *destAddr, /* the address to move data to */
   int16_t destIncr,  /* the amount to increment the destination address per transfer */
   uint32_t dmaSize,   /* the number of bytes to transfer independent of the transfer size */
   uint32_t xferSize,  /* the number bytes in of each data movement (1, 2, or 4) */
   uint32_t initiator, /* what device initiates the DMA */
   int priority,  /* priority of the DMA */
   uint32_t flags,     /* flags describing the DMA */
   uint32_t funcDesc   /* a description of byte swapping, bit swapping, and CRC actions */
);

/* 
 * MCD_initDma() initializes the DMA API by setting up a pointer to the DMA 
 * registers, relocating and creating the appropriate task structures, and 
 * setting up some global settings
 */
int MCD_initDma (dmaRegs *sDmaBarAddr, void *taskTableDest, uint32_t flags);

/* 
 * MCD_dmaStatus() returns the status of the DMA on the requested channel.
 */
int MCD_dmaStatus (int channel);

/* 
 * MCD_XferProgrQuery() returns progress of DMA on requested channel
 */
int MCD_XferProgrQuery (int channel, MCD_XferProg *progRep);

/* 
 * MCD_killDma() halts the DMA on the requested channel, without any
 * intention of resuming the DMA.
 */
int MCD_killDma (int channel);

/* 
 * MCD_continDma() continues a DMA which as stopped due to encountering an
 * unready buffer descriptor.
 */
int MCD_continDma (int channel);

/* 
 * MCD_pauseDma() pauses the DMA on the given channel ( if any DMA is
 * running on that channel). 
 */
int MCD_pauseDma (int channel);

/* 
 * MCD_resumeDma() resumes the DMA on a given channel (if any DMA is
 * running on that channel).
 */
int MCD_resumeDma (int channel);

/* 
 * MCD_csumQuery provides the checksum/CRC after performing a non-chained DMA
 */
int MCD_csumQuery (int channel, uint32_t *csum);

/* 
 * MCD_getCodeSize provides the packed size required by the microcoded task
 * and structures.
 */
int MCD_getCodeSize(void);

/*
 * MCD_getVersion provides a pointer to a version string and returns a
 * version number.
 */
int MCD_getVersion(char **longVersion);

/* macro for setting a location in the variable table */
#define MCD_SET_VAR(taskTab,idx,value) ((uint32_t *)(taskTab)->varTab)[idx] = value
   /* Note that MCD_SET_VAR() is invoked many times in firing up a DMA function,
      so I'm avoiding surrounding it with "do {} while(0)" */

#endif  /* DEFINESONLY */

#endif /* _MCD_API_H */

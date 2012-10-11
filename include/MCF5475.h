/* Coldfire C Header File
 * Copyright Freescale Semiconductor Inc
 * All rights reserved.
 *
 * 2008/05/23 Revision: 0.81
 *
 * (c) Copyright UNIS, a.s. 1997-2008
 * UNIS, a.s.
 * Jundrovska 33
 * 624 00 Brno
 * Czech Republic
 * http      : www.processorexpert.com
 * mail      : info@processorexpert.com
 */

#ifndef __MCF5475_H__
#define __MCF5475_H__


/********************************************************************/
/*
 * The basic data types
 */

typedef unsigned char           uint8;   /*  8 bits */
typedef unsigned short int      uint16;  /* 16 bits */
typedef unsigned long int       uint32;  /* 32 bits */

typedef signed char             int8;    /*  8 bits */
typedef signed short int        int16;   /* 16 bits */
typedef signed long int         int32;   /* 32 bits */

typedef volatile uint8          vuint8;  /*  8 bits */
typedef volatile uint16         vuint16; /* 16 bits */
typedef volatile uint32         vuint32; /* 32 bits */

#ifdef __cplusplus
extern "C" {
#endif

#pragma define_section system ".system" far_absolute RW

/***
 * MCF5475 Derivative Memory map definitions from linker command files:
 * __MBAR, __MMUBAR, __RAMBAR0, __RAMBAR0_SIZE, __RAMBAR1, __RAMBAR1_SIZE
 * linker symbols must be defined in the linker command file.
 */

extern __declspec(system) uint8 __MBAR[];
extern __declspec(system) uint8 __MMUBAR[];
extern __declspec(system) uint8 __RAMBAR0[];
extern __declspec(system) uint8 __RAMBAR0_SIZE[];
extern __declspec(system) uint8 __RAMBAR1[];
extern __declspec(system) uint8 __RAMBAR1_SIZE[];

#define MBAR_ADDRESS    (uint32)__MBAR
#define MMUBAR_ADDRESS  (uint32)__MMUBAR
#define RAMBAR0_ADDRESS (uint32)__RAMBAR0
#define RAMBAR0_SIZE    (uint32)__RAMBAR0_SIZE
#define RAMBAR1_ADDRESS (uint32)__RAMBAR1
#define RAMBAR1_SIZE    (uint32)__RAMBAR1_SIZE


#include "MCF5475_SIU.h"
#include "MCF5475_MMU.h"
#include "MCF5475_SDRAMC.h"
#include "MCF5475_XLB.h"
#include "MCF5475_CLOCK.h"
#include "MCF5475_FBCS.h"
#include "MCF5475_INTC.h"
#include "MCF5475_GPT.h"
#include "MCF5475_SLT.h"
#include "MCF5475_GPIO.h"
#include "MCF5475_PAD.h"
#include "MCF5475_PCI.h"
#include "MCF5475_PCIARB.h"
#include "MCF5475_EPORT.h"
#include "MCF5475_CTM.h"
#include "MCF5475_DMA.h"
#include "MCF5475_PSC.h"
#include "MCF5475_DSPI.h"
#include "MCF5475_I2C.h"
#include "MCF5475_FEC.h"
#include "MCF5475_USB.h"
#include "MCF5475_SRAM.h"
#include "MCF5475_SEC.h"

#ifdef __cplusplus
}
#endif


#endif /* __MCF5475_H__ */

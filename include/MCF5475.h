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

#include <stdint.h>
/***
 * MCF5475 Derivative Memory map definitions from linker command files:
 * __MBAR, __MMUBAR, __RAMBAR0, __RAMBAR0_SIZE, __RAMBAR1, __RAMBAR1_SIZE
 * linker symbols must be defined in the linker command file.
 */

extern uint8_t __MBAR[];
extern uint8_t __MMUBAR[];
extern uint8_t __RAMBAR0[];
extern uint8_t __RAMBAR0_SIZE[];
extern uint8_t __RAMBAR1[];
extern uint8_t __RAMBAR1_SIZE[];

#define MBAR_ADDRESS    (uint32_t)__MBAR
#define MMUBAR_ADDRESS  (uint32_t)__MMUBAR
#define RAMBAR0_ADDRESS (uint32_t)__RAMBAR0
#define RAMBAR0_SIZE    (uint32_t)__RAMBAR0_SIZE
#define RAMBAR1_ADDRESS (uint32_t)__RAMBAR1
#define RAMBAR1_SIZE    (uint32_t)__RAMBAR1_SIZE


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

#endif /* __MCF5475_H__ */

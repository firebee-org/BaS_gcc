/*
 * spidma.h *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#ifndef _DMA_H_
#define _DMA_H_

#include "MCF5475.h"
#include "MCD_dma.h"
#include "bas_string.h"

void *dma_memcpy(void *dst, void *src, size_t n);
extern int dma_init(void);
extern int dma_get_channel(int requestor);
extern int dma_set_channel(int, void (*)(void));
extern void dma_free_channel(int requestor);
extern void dma_clear_channel(int channel);
extern uint32_t dma_get_initiator(int requestor);
extern int dma_set_initiator(int initiator);
extern void dma_free_initiator(int initiator);
extern void dma_irq_enable(void);
extern void dma_irq_disable(void);
extern bool dma_interrupt_handler(void *arg1, void *arg2);


#endif /* _DMA_H_ */

/*
 * File:        net_timer.c
 * Purpose:     Provide a timer use by the dBUG network as a timeout
 *              indicator
 *
 * Notes:
 */
#include "net_timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "MCF5475.h"
#include "interrupts.h"

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#else
#error unknown machine!
#endif

static NET_TIMER net_timer[4] = {{0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0}};


int timer_default_isr(void *not_used, NET_TIMER *t)
{
    (void) not_used;
    
    /* 
     * Clear the pending event 
     */
    MCF_GPT_GMS(t->ch) = 0;

    /*
     * Clear the reference - the desired seconds have expired
     */
    t->reference = 0;

    return 1;
}

void timer_irq_enable(uint8_t ch)
{
    /*
     * Setup the appropriate ICR
     */
    MCF_INTC_ICR(TIMER_VECTOR(ch) - 64) =
		(uint8_t)(0
				| MCF_INTC_ICR_IP(net_timer[ch].pri)
				| MCF_INTC_ICR_IL(net_timer[ch].lvl));

    /*
     * Unmask the FEC interrupt in the interrupt controller
     */
    if (ch == 3)
        MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK59;
    else if (ch == 2)
        MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK60;
    else if (ch == 1)
        MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK61;
    else
        MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK62;
}

bool timer_set_secs(uint8_t ch, uint32_t secs)
{
    uint16_t timeout;
    
    /*
     * Reset the timer
     */
    MCF_GPT_GMS(ch) = 0;

    /* 
     * Get the timeout in seconds 
     */
    timeout = (uint16_t)(secs * net_timer[ch].cnt);

    /*
     * Set the reference indicating that we have not yet reached the
     * desired timeout
     */
    net_timer[ch].reference = 1;

    /*
     * Enable timer interrupt to the processor
     */
    timer_irq_enable(ch);

    /*
     * Enable the timer using the pre-calculated values
     */
    MCF_GPT_GCIR(ch) = (0
                        | MCF_GPT_GCIR_CNT(timeout)
                        | MCF_GPT_GCIR_PRE(net_timer[ch].pre)
                        );
    MCF_GPT_GMS(ch) = net_timer[ch].gms;

    return true;
}

uint32_t timer_get_reference(uint8_t ch)
{   
    return (uint32_t) net_timer[ch].reference;
}

bool timer_init(uint8_t ch, uint8_t lvl, uint8_t pri)
{
    /* 
     * Initialize the timer to expire after one second
     * 
     * This routine should only be called by the project (board) specific
     * initialization code.
     */
    if (!((ch <= 3) && (lvl <= 7) && (lvl >= 1) && (pri <= 7)))
		return false;

    /*
     * Reset the timer
     */
    MCF_GPT_GMS(ch) = 0;

    /* 
     * Save off the channel, and interrupt lvl/pri information
     */
    net_timer[ch].ch = ch;
    net_timer[ch].lvl = lvl;
    net_timer[ch].pri = pri;

    /*
     * Register the timer interrupt handler
     */
    if (!isr_register_handler(ISR_DBUG_ISR, 
                              TIMER_VECTOR(ch), 
                              (int (*)(void *,void *)) timer_default_isr,
                              NULL, 
                              (void *) &net_timer[ch])
                              )
    {
        return false;
    }

    /*
     * Calculate the require CNT value to get a 1 second timeout
     *
     * 1 sec = CNT * Clk Period * PRE
     * CNT = 1 sec / (Clk Period * PRE)
     * CNT = Clk Freq / PRE
     *
     * The system clock frequency is defined as SYSTEM_CLOCK and
     * is given in MHz. We need to multiple it by 1000000 to get the 
     * true value.  If we assume PRE to be the maximum of 0xFFFF, 
     * then the CNT value needed to achieve a 1 second timeout is 
     * given by:
     *
     * CNT = SYSTEM_CLOCK * (1000000/0xFFFF)
     */
    net_timer[ch].pre = 0xFFFF;
    net_timer[ch].cnt = (uint16_t) (SYSCLK * (1000000 / 0xFFFF));

    /* 
     * Save off the appropriate mode select register value 
     */
    net_timer[ch].gms = (0 
                     | MCF_GPT_GMS_TMS_GPIO
                     | MCF_GPT_GMS_IEN
                     | MCF_GPT_GMS_SC
                     | MCF_GPT_GMS_CE
                     );

    return true;
}

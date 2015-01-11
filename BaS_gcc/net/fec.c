/*
 * File:    fec.c
 * Purpose: Driver for the Fast Ethernet Controller (FEC)
 *
 * Notes:
 */

#include "net.h"
#include "fec.h"
#include "fecbd.h"
#include "exceptions.h"
#include "interrupts.h"
#include "MCF5475.h"
#include "MCD_dma.h"
#include "mcd_initiators.h"
#include "dma.h"
#include "bas_string.h"
#include "bas_printf.h"
#include "util.h"
#include "wait.h"
#include "am79c874.h"
//#include "bcm5222.h"
#include <stdbool.h>

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error Unknown machine!
#endif

#define DBG_FEC
#ifdef DBG_FEC
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_FEC */


FEC_EVENT_LOG fec_log[2];

/*
 * Write a value to a PHY's MII register.
 *
 * Parameters:
 *  ch          FEC channel
 *  phy_addr    Address of the PHY.
 *  reg_addr    Address of the register in the PHY.
 *  data        Data to be written to the PHY register.
 *
 * Return Values:
 *  1 on failure
 *  0 on success.
 *
 * Please refer to your PHY manual for registers and their meanings.
 * mii_write() polls for the FEC's MII interrupt event (which should
 * be masked from the interrupt handler) and clears it. If after a
 * suitable amount of time the event isn't triggered, a value of 0
 * is returned.
 */
int fec_mii_write(uint8_t ch, uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
    int timeout;
    uint32_t eimr;

    /*
     * Clear the MII interrupt bit
     */
    MCF_FEC_EIR(ch) = MCF_FEC_EIR_MII;

    /*
     * Write to the MII Management Frame Register to kick-off
     * the MII write
     */
    MCF_FEC_MMFR(ch) = 0
        | MCF_FEC_MMFR_ST_01
        | MCF_FEC_MMFR_OP_WRITE
        | MCF_FEC_MMFR_PA(phy_addr)
        | MCF_FEC_MMFR_RA(reg_addr)
        | MCF_FEC_MMFR_TA_10
        | MCF_FEC_MMFR_DATA(data);

    /*
     * Mask the MII interrupt
     */
    eimr = MCF_FEC_EIMR(ch);
    MCF_FEC_EIMR(ch) &= ~MCF_FEC_EIMR_MII;

    /*
     * Poll for the MII interrupt (interrupt should be masked)
     */
    for (timeout = 0; timeout < FEC_MII_TIMEOUT; timeout++)
    {
        wait(1);
        if (MCF_FEC_EIR(ch) & MCF_FEC_EIR_MII)
            break;
    }

    if (timeout == FEC_MII_TIMEOUT)
        return 0;

    /*
     * Clear the MII interrupt bit
     */
    MCF_FEC_EIR(ch) = MCF_FEC_EIR_MII;

    /*
     * Restore the EIMR
     */
    MCF_FEC_EIMR(ch) = eimr;

    return 1;
}

/*
 * Read a value from a PHY's MII register.
 *
 * Parameters:
 *  ch          FEC channel
 *  phy_addr    Address of the PHY.
 *  reg_addr    Address of the register in the PHY.
 *  data        Pointer to storage for the Data to be read
 *              from the PHY register (passed by reference)
 *
 * Return Values:
 *  1 on failure
 *  0 on success.
 *
 * Please refer to your PHY manual for registers and their meanings.
 * mii_read() polls for the FEC's MII interrupt event (which should
 * be masked from the interrupt handler) and clears it. If after a
 * suitable amount of time the event isn't triggered, a value of 0
 * is returned.
 */
int fec_mii_read(uint8_t ch, uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
{
    int timeout;

    /*
     * Clear the MII interrupt bit
     */
    MCF_FEC_EIR(ch) = MCF_FEC_EIR_MII;

    /*
     * Write to the MII Management Frame Register to kick-off
     * the MII read
     */
    MCF_FEC_MMFR(ch) = 0
        | MCF_FEC_MMFR_ST_01
        | MCF_FEC_MMFR_OP_READ
        | MCF_FEC_MMFR_PA(phy_addr)
        | MCF_FEC_MMFR_RA(reg_addr)
        | MCF_FEC_MMFR_TA_10;

    /*
     * Poll for the MII interrupt (interrupt should be masked)
     */
    for (timeout = 0; timeout < FEC_MII_TIMEOUT; timeout++)
    {
        wait(1);
        if (MCF_FEC_EIR(ch) & MCF_FEC_EIR_MII)
            break;
    }

    if (timeout == FEC_MII_TIMEOUT)
        return 0;

    /*
     * Clear the MII interrupt bit
     */
    MCF_FEC_EIR(ch) = MCF_FEC_EIR_MII;

    *data = (uint16_t)(MCF_FEC_MMFR(ch) & 0x0000FFFF);

    return 1;
}

/*
 * Initialize the MII interface controller
 *
 * Parameters:
 *  ch      FEC channel
 *  sys_clk System Clock Frequency (in MHz)
 */
void fec_mii_init(uint8_t ch, uint32_t sys_clk)
{
    /*
     * Initialize the MII clock (EMDC) frequency
     *
     * Desired MII clock is 2.5MHz
     * MII Speed Setting = System_Clock / (2.5MHz * 2)
     * (plus 1 to make sure we round up)
     */
    MCF_FEC_MSCR(ch) = MCF_FEC_MSCR_MII_SPEED((sys_clk / 5) + 1);
}

/* Initialize the MIB counters
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_mib_init(uint8_t ch)
{
    //To do
}

/*
 * Display the MIB counters
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_mib_dump(uint8_t ch)
{
    //To do
}

/*
 * Initialize the FEC log
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_log_init(uint8_t ch)
{
    memset(&fec_log[ch], 0, sizeof(FEC_EVENT_LOG));
}

/*
 * Display the FEC log
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_log_dump(uint8_t ch)
{
    dbg("\r\n   FEC%d Log\r\n", __FUNCTION__, ch);
    dbg("   ---------------\r\n", __FUNCTION__);
    dbg(" Total: %4d\r\n", fec_log[ch].total);
    dbg(" hberr: %4d\r\n", fec_log[ch].hberr);
    dbg(" babr:  %4d\r\n", fec_log[ch].babr);
    dbg(" babt:  %4d\r\n", fec_log[ch].babt);
    dbg(" gra:   %4d\r\n", fec_log[ch].gra);
    dbg(" txf:   %4d\r\n", fec_log[ch].txf);
    dbg(" mii:   %4d\r\n", fec_log[ch].mii);
    dbg(" lc:    %4d\r\n", fec_log[ch].lc);
    dbg(" rl:    %4d\r\n", fec_log[ch].rl);
    dbg(" xfun:  %4d\r\n", fec_log[ch].xfun);
    dbg(" xferr: %4d\r\n", fec_log[ch].xferr);
    dbg(" rferr: %4d\r\n", fec_log[ch].rferr);
    dbg(" dtxf:  %4d\r\n", fec_log[ch].dtxf);
    dbg(" drxf:  %4d\r\n", fec_log[ch].drxf);
    dbg(" \r\nRFSW:\r\n");
    dbg(" inv:   %4d\r\n", fec_log[ch].rfsw_inv);
    dbg(" m:     %4d\r\n", fec_log[ch].rfsw_m);
    dbg(" bc:    %4d\r\n", fec_log[ch].rfsw_bc);
    dbg(" mc:    %4d\r\n", fec_log[ch].rfsw_mc);
    dbg(" lg:    %4d\r\n", fec_log[ch].rfsw_lg);
    dbg(" no:    %4d\r\n", fec_log[ch].rfsw_no);
    dbg(" cr:    %4d\r\n", fec_log[ch].rfsw_cr);
    dbg(" ov:    %4d\r\n", fec_log[ch].rfsw_ov);
    dbg(" tr:    %4d\r\n", fec_log[ch].rfsw_tr);
    dbg(" ---------------\r\n\r\n");
}

/*
 * Display some of the registers for debugging
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_debug_dump(uint8_t ch)
{
    dbg("\r\n------------- FEC%d -------------\r\n",ch);
    dbg("EIR      %08x        \r\n", MCF_FEC_EIR(ch));
    dbg("EIMR     %08x        \r\n", MCF_FEC_EIMR(ch));
    dbg("ECR      %08x        \r\n", MCF_FEC_ECR(ch));
    dbg("RCR      %08x        \r\n", MCF_FEC_RCR(ch));
    dbg("R_HASH   %08x        \r\n", MCF_FEC_RHR_HASH(ch));
    dbg("TCR      %08x        \r\n", MCF_FEC_TCR(ch));
    dbg("FECTFWR  %08x        \r\n", MCF_FEC_FECTFWR(ch));
    dbg("FECRFSR  %08x        \r\n", MCF_FEC_FECRFSR(ch));
    dbg("FECRFCR  %08x        \r\n", MCF_FEC_FECRFCR(ch));
    dbg("FECRLRFP %08x        \r\n", MCF_FEC_FECRLRFP(ch));
    dbg("FECRLWFP %08x        \r\n", MCF_FEC_FECRLWFP(ch));
    dbg("FECRFAR  %08x        \r\n", MCF_FEC_FECRFAR(ch));
    dbg("FECRFRP  %08x        \r\n", MCF_FEC_FECRFRP(ch));
    dbg("FECRFWP  %08x        \r\n", MCF_FEC_FECRFWP(ch));
    dbg("FECTFSR  %08x        \r\n", MCF_FEC_FECTFSR(ch));
    dbg("FECTFCR  %08x        \r\n", MCF_FEC_FECTFCR(ch));
    dbg("FECTLRFP %08x        \r\n", MCF_FEC_FECTLRFP(ch));
    dbg("FECTLWFP %08x        \r\n", MCF_FEC_FECTLWFP(ch));
    dbg("FECTFAR  %08x        \r\n", MCF_FEC_FECTFAR(ch));
    dbg("FECTFRP  %08x        \r\n", MCF_FEC_FECTFRP(ch));
    dbg("FECTFWP  %08x        \r\n", MCF_FEC_FECTFWP(ch));
    dbg("FRST     %08x        \r\n", MCF_FEC_FECFRST(ch));
    dbg("--------------------------------\r\n\r\n");
}

/*
 * Set the duplex on the selected FEC controller
 *
 * Parameters:
 *  ch      FEC channel
 *  duplex  FEC_MII_FULL_DUPLEX or FEC_MII_HALF_DUPLEX
 */
void fec_duplex(uint8_t ch, uint8_t duplex)
{
    switch (duplex)
    {
        case FEC_MII_HALF_DUPLEX:
            MCF_FEC_RCR(ch) |= MCF_FEC_RCR_DRT;
            MCF_FEC_TCR(ch) &= (uint32_t) ~MCF_FEC_TCR_FDEN;
            break;
        case FEC_MII_FULL_DUPLEX:
        default:
            MCF_FEC_RCR(ch) &= (uint32_t) ~MCF_FEC_RCR_DRT;
            MCF_FEC_TCR(ch) |= MCF_FEC_TCR_FDEN;
            break;
    }
}

/*
 * Generate the hash table settings for the given address
 *
 * Parameters:
 *  addr    48-bit (6 byte) Address to generate the hash for
 *
 * Return Value:
 *  The 6 most significant bits of the 32-bit CRC result
 */
uint8_t fec_hash_address(const uint8_t *addr)
{
    uint32_t crc;
    uint8_t byte;
    int i, j;

    crc = 0xFFFFFFFF;
    for (i = 0; i < 6; ++i)
    {
        byte = addr[i];
        for (j = 0; j < 8; ++j)
        {
            if ((byte & 0x01) ^ (crc & 0x01))
            {
                crc >>= 1;
                crc = crc ^ 0xEDB88320;
            }
            else
                crc >>= 1;
            byte >>= 1;
        }
    }
    return (uint8_t)(crc >> 26);
}

/*
 * Set the Physical (Hardware) Address and the Individual Address
 * Hash in the selected FEC
 *
 * Parameters:
 *  ch  FEC channel
 *  pa  Physical (Hardware) Address for the selected FEC
 */
void fec_set_address(uint8_t ch, const uint8_t *pa)
{
    uint8_t crc;

    /*
     * Set the Physical Address
     */
    MCF_FEC_PALR(ch) = (uint32_t) ((pa[0] << 24) | (pa[1] << 16) | (pa[2] << 8) | pa[3]);
    MCF_FEC_PAHR(ch) = (uint32_t) ((pa[4] << 24) | (pa[5] << 16));

    /*
     * Calculate and set the hash for given Physical Address
     * in the  Individual Address Hash registers
     */
    crc = fec_hash_address(pa);
    if(crc >= 32)
        MCF_FEC_IAUR(ch) |= (uint32_t) (1 << (crc - 32));
    else
        MCF_FEC_IALR(ch) |= (uint32_t) (1 << crc);
}

/*
 * Reset the selected FEC controller
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_reset(uint8_t ch)
{
    int i;

    /* Clear any events in the FIFO status registers */
    MCF_FEC_FECRFSR(ch) = (0
            | MCF_FEC_FECRFSR_OF
            | MCF_FEC_FECRFSR_UF
            | MCF_FEC_FECRFSR_RXW
            | MCF_FEC_FECRFSR_FAE
            | MCF_FEC_FECRFSR_IP);
    MCF_FEC_FECTFSR(ch) = (0
            | MCF_FEC_FECTFSR_OF
            | MCF_FEC_FECTFSR_UF
            | MCF_FEC_FECTFSR_TXW
            | MCF_FEC_FECTFSR_FAE
            | MCF_FEC_FECTFSR_IP);

    /* Reset the FIFOs */
    MCF_FEC_FECFRST(ch) |= MCF_FEC_FECFRST_SW_RST;
    MCF_FEC_FECFRST(ch) &= ~MCF_FEC_FECFRST_SW_RST;

    /* Set the Reset bit and clear the Enable bit */
    MCF_FEC_ECR(ch) = MCF_FEC_ECR_RESET;

    /* Wait at least 8 clock cycles */
    for (i = 0; i < 10; ++i)
        NOP();
}

/*
 * Initialize the selected FEC
 *
 * Parameters:
 *  ch      FEC channel
 *  mode    External interface mode (MII, 7-wire, or internal loopback)
 *  pa      Physical (Hardware) Address for the selected FEC
 */
void fec_init(uint8_t ch, uint8_t mode, const uint8_t *pa)
{
    /*
     * Enable all the external interface signals
     */
    if (mode == FEC_MODE_7WIRE)
    {
        if (ch == 1)
            MCF_PAD_PAR_FECI2CIRQ |= MCF_PAD_PAR_FECI2CIRQ_PAR_E17;
        else
            MCF_PAD_PAR_FECI2CIRQ |= MCF_PAD_PAR_FECI2CIRQ_PAR_E07;
    }
    else if (mode == FEC_MODE_MII)
    {
        if (ch == 1)
            MCF_PAD_PAR_FECI2CIRQ |= 0
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDC_E1MDC
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDIO_E1MDIO
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E1MII
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E17;
        else
            MCF_PAD_PAR_FECI2CIRQ |= 0
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E0MDC
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E0MDIO
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E0MII
                | MCF_PAD_PAR_FECI2CIRQ_PAR_E07;
    }

    /*
     * Clear the Individual and Group Address Hash registers
     */
    MCF_FEC_IALR(ch) = 0;
    MCF_FEC_IAUR(ch) = 0;
    MCF_FEC_GALR(ch) = 0;
    MCF_FEC_GAUR(ch) = 0;

    /*
     * Set the Physical Address for the selected FEC
     */
    fec_set_address(ch, pa);

    /*
     * Mask all FEC interrupts
     */
    MCF_FEC_EIMR(ch) = MCF_FEC_EIMR_MASK_ALL;

    /*
     * Clear all FEC interrupt events
     */
    MCF_FEC_EIR(ch) = MCF_FEC_EIR_CLEAR_ALL;

    /*
     * Initialize the Receive Control Register
     */
    MCF_FEC_RCR(ch) = 0
        | MCF_FEC_RCR_MAX_FL(ETH_MAX_FRM)
        | MCF_FEC_RCR_PROM
        | MCF_FEC_RCR_FCE;

    if (mode == FEC_MODE_MII)
        MCF_FEC_RCR(ch) |= MCF_FEC_RCR_MII_MODE;

    else if (mode == FEC_MODE_LOOPBACK)
        MCF_FEC_RCR(ch) |= MCF_FEC_RCR_LOOP;

    /*
     * Initialize the Transmit Control Register
     */
    MCF_FEC_TCR(ch) = MCF_FEC_TCR_FDEN;

    /*
     * Set Rx FIFO alarm and granularity
     */
    MCF_FEC_FECRFCR(ch) = 0
        | MCF_FEC_FECRFCR_FRMEN
        | MCF_FEC_FECRFCR_RXW_MSK
        | MCF_FEC_FECRFCR_GR(7);
    MCF_FEC_FECRFAR(ch) = MCF_FEC_FECRFAR_ALARM(768);

    /*
     * Set Tx FIFO watermark, alarm and granularity
     */
    MCF_FEC_FECTFCR(ch) = 0
        | MCF_FEC_FECTFCR_FRMEN
        | MCF_FEC_FECTFCR_TXW_MASK
        | MCF_FEC_FECTFCR_GR(7);
    MCF_FEC_FECTFAR(ch) = MCF_FEC_FECTFAR_ALARM(256);
    MCF_FEC_FECTFWR(ch) = MCF_FEC_FECTFWR_X_WMRK_256;

    /*
     * Enable the transmitter to append the CRC
     */
    MCF_FEC_FECCTCWR(ch) = 0
        | MCF_FEC_FECCTCWR_TFCW
        | MCF_FEC_FECCTCWR_CRC;
}

/*
 * Start the FEC Rx DMA task
 *
 * Parameters:
 *  ch      FEC channel
 *  rxbd    First Rx buffer descriptor in the chain
 */
void fec_rx_start(uint8_t ch, int8_t *rxbd)
{
    uint32_t initiator;
    int channel;
#ifdef DBG_FEC
    int res;
#endif

    /*
     * Make the initiator assignment
     */
#if defined(DBG_FEC)
    res =
#else
    (void)
#endif
    dma_set_initiator(DMA_FEC_RX(ch));
    dbg("dma_set_initiator(DMA_FEC_RX(%d)): %d\r\n", ch, res);

    /*
     * Grab the initiator number
     */
    initiator = dma_get_initiator(DMA_FEC_RX(ch));
    dbg("dma_get_initiator(DMA_FEC_RX(%d)) = %d\r\n", ch, initiator);

    /*
     * Determine the DMA channel running the task for the
     * selected FEC
     */
    channel = dma_set_channel(DMA_FEC_RX(ch),
            (ch == 0) ? fec0_rx_frame : fec1_rx_frame);
    dbg("DMA channel for FEC%1d: %d\r\n", ch, channel);

    /*
     * Start the Rx DMA task
     */
    MCD_startDma(channel,
            (s8 *) rxbd,
            0,
            (s8 *) MCF_FEC_FECRFDR(ch),
            0,
            RX_BUF_SZ,
            0,
            initiator,
            FECRX_DMA_PRI(ch),
            0
            | MCD_FECRX_DMA
            | MCD_INTERRUPT
            | MCD_TT_FLAGS_CW
            | MCD_TT_FLAGS_RL
            | MCD_TT_FLAGS_SP
            ,
            0
            | MCD_NO_CSUM
            | MCD_NO_BYTE_SWAP
            );
    dbg("Rx DMA task for FEC%1d started\r\n", ch);
}

/*
 * Continue the Rx DMA task
 *
 * This routine is called after the DMA task has halted after
 * encountering an Rx buffer descriptor that wasn't marked as
 * ready. There is no harm in calling the DMA continue routine
 * if the DMA is not halted.
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_rx_continue(uint8_t ch)
{
    int channel;

    /*
     * Determine the DMA channel running the task for the
     * selected FEC
     */
    channel = dma_get_channel(DMA_FEC_RX(ch));

    dbg("RX DMA channel for FEC%1d is %d\r\n", ch, channel);

    /*
     * Continue/restart the DMA task
     */
    MCD_continDma(channel);
    dbg("RX dma on channel %d continued\r\n", channel);
}

/*
 * Stop all frame receptions on the selected FEC
 *
 * Parameters:
 *  ch  FEC channel
 */
void fec_rx_stop (uint8_t ch)
{
    uint32_t mask;
    int channel;

    /* Save off the EIMR value */
    mask = MCF_FEC_EIMR(ch);

    /* Mask all interrupts */
    MCF_FEC_EIMR(ch) = 0;

    /*
     * Determine the DMA channel running the task for the
     * selected FEC
     */
    channel = dma_get_channel(DMA_FEC_RX(ch));

    /* Kill the FEC Rx DMA task */
    MCD_killDma(channel);

    /*
     * Free up the FEC requestor from the software maintained
     * initiator list
     */
    dma_free_initiator(DMA_FEC_RX(ch));

    /* Free up the DMA channel */
    dma_free_channel(DMA_FEC_RX(ch));

    /* Restore the interrupt mask register value */
    MCF_FEC_EIMR(ch) = mask;
}

/*
 * Receive Frame interrupt handler - this handler is called by the
 * DMA interrupt handler indicating that a packet was successfully
 * transferred out of the Rx FIFO.
 *
 * Parameters:
 *  nif     Pointer to Network Interface structure
 *  ch      FEC channel
 */
void fec_rx_frame(uint8_t ch, NIF *nif)
{
    ETH_HDR *eth_hdr;
    FECBD *pRxBD;
    NBUF *cur_nbuf, *new_nbuf;
    int keep;

    dbg("started\r\n");

    while ((pRxBD = fecbd_rx_alloc(ch)) != NULL)
    {
        fec_log[ch].drxf++;
        keep = true;

        /*
         * Check the Receive Frame Status Word for errors
         *  - The L bit should always be set
         *  - No undefined bits should be set
         *  - The upper 5 bits of the length should be cleared
         */
        if (!(pRxBD->status & RX_BD_L) || (pRxBD->status & 0x0608)
                || (pRxBD->length & 0xF800))
        {
            keep = false;
            fec_log[ch].rfsw_inv++;
        }
        else if (pRxBD->status & RX_BD_ERROR)
        {
            keep = false;
            if (pRxBD->status & RX_BD_NO)
                fec_log[ch].rfsw_no++;
            if (pRxBD->status & RX_BD_CR)
                fec_log[ch].rfsw_cr++;
            if (pRxBD->status & RX_BD_OV)
                fec_log[ch].rfsw_ov++;
            if (pRxBD->status & RX_BD_TR)
                fec_log[ch].rfsw_tr++;
        }
        else
        {
            if (pRxBD->status & RX_BD_LG)
                fec_log[ch].rfsw_lg++;
            if (pRxBD->status & RX_BD_M)
                fec_log[ch].rfsw_m++;
            if (pRxBD->status & RX_BD_BC)
                fec_log[ch].rfsw_bc++;
            if (pRxBD->status & RX_BD_MC)
                fec_log[ch].rfsw_mc++;
        }

        if (keep)
        {
            /*
             * Pull the network buffer off the Rx ring queue
             */
            cur_nbuf = nbuf_remove(NBUF_RX_RING);

            /*
             * Copy the buffer descriptor information to the network buffer
             */
            cur_nbuf->length = (pRxBD->length - (ETH_HDR_LEN + ETH_CRC_LEN));
            cur_nbuf->offset = ETH_HDR_LEN;

            /*
             * Get a new buffer pointer for this buffer descriptor
             */
            new_nbuf = nbuf_alloc();
            if (new_nbuf == NULL)
            {
                dbg("nbuf_alloc() failed\n");

                /*
                 * Can't allocate a new network buffer, so we
                 * have to trash the received data and reuse the buffer
                 * hoping that some buffers will free up in the system
                 * and this frame will be re-transmitted by the host
                 */
                pRxBD->length = RX_BUF_SZ;
                pRxBD->status &= (RX_BD_W | RX_BD_INTERRUPT);
                pRxBD->status |= RX_BD_E;
                nbuf_add(NBUF_RX_RING, cur_nbuf);
                fec_rx_continue(ch);

                continue;
            }

            /*
             * Add the new network buffer to the Rx ring queue
             */
            nbuf_add(NBUF_RX_RING, new_nbuf);

            /*
             * Re-initialize the buffer descriptor - pointing it
             * to the new data buffer.  The previous data buffer
             * will be passed up the stack
             */
            pRxBD->data = new_nbuf->data;
            pRxBD->length = RX_BUF_SZ;
            pRxBD->status &= (RX_BD_W | RX_BD_INTERRUPT);
            pRxBD->status |= RX_BD_E;


            /*
             * Let the DMA know that there is a new Rx BD (in case the
             * ring was full and the DMA was waiting for an empty one)
             */
            fec_rx_continue(ch);

            /*
             * Get pointer to the frame data inside the network buffer
             */
            eth_hdr = (ETH_HDR *) cur_nbuf->data;

            /*
             * Pass the received packet up the network stack if the
             * protocol is supported in our network interface (NIF)
             */
            if (nif_protocol_exist(nif, eth_hdr->type))
            {
                hexdump((uint8_t *) eth_hdr, ETH_MAX_FRM);
                nif_protocol_handler(nif, eth_hdr->type, cur_nbuf);
            }
            else
            {
                nbuf_free(cur_nbuf);
                dbg("got unsupported packet %d, trashed it\r\n", eth_hdr->type);
            }
        }
        else
        {
            /*
             * This frame isn't a keeper
             * Reset the status and length, but don't need to get another
             * buffer since we are trashing the data in the current one
             */
            pRxBD->length = RX_BUF_SZ;
            pRxBD->status &= (RX_BD_W | RX_BD_INTERRUPT);
            pRxBD->status |= RX_BD_E;

            /*
             * Move the current buffer from the beginning to the end of the
             * Rx ring queue
             */
            cur_nbuf = nbuf_remove(NBUF_RX_RING);
            nbuf_add(NBUF_RX_RING, cur_nbuf);

            /*
             * Let the DMA know that there are new Rx BDs (in case
             * it is waiting for an empty one)
             */
            fec_rx_continue(ch);
        }
    }
}

void fec0_rx_frame(void)
{
    extern NIF nif1;

    fec_rx_frame(0, &nif1);
}

void fec1_rx_frame(void)
{
    extern NIF nif1;

    fec_rx_frame(1, &nif1);
}

/*
 * Start the FEC Tx DMA task
 *
 * Parameters:
 *  ch      FEC channel
 *  txbd    First Tx buffer descriptor in the chain
 */
void fec_tx_start(uint8_t ch, int8_t *txbd)
{
    uint32_t initiator;
    int channel;
    void fec0_tx_frame(void);
    void fec1_tx_frame(void);
#ifdef DBG_FEC
    int res;
#endif

    /*
     * Make the initiator assignment
     */
#ifdef DBG_FEC
    res =
#else
    (void)
#endif
    dma_set_initiator(DMA_FEC_TX(ch));
    dbg("dma_set_initiator(%d) = %d\r\n", ch, res);

    /*
     * Grab the initiator number
     */
    initiator = dma_get_initiator(DMA_FEC_TX(ch));
    dbg("dma_get_initiator(%d) = %d\r\n", ch, initiator);


    /*
     * Determine the DMA channel running the task for the
     * selected FEC
     */
    channel = dma_set_channel(DMA_FEC_TX(ch),
            (ch == 0) ? fec0_tx_frame : fec1_tx_frame);
    dbg("dma_set_channel(%d, ...) = %d\r\n", ch, channel);

    /*
     * Start the Tx DMA task
     */
    MCD_startDma(channel,
            (s8 *) txbd,
            0,
            (s8 *) MCF_FEC_FECTFDR(ch),
            0,
            ETH_MTU,
            0,
            initiator,
            FECTX_DMA_PRI(ch),
            0
            | MCD_FECTX_DMA
            | MCD_INTERRUPT
            | MCD_TT_FLAGS_CW
            | MCD_TT_FLAGS_RL
            | MCD_TT_FLAGS_SP
            ,
            0
            | MCD_NO_CSUM
            | MCD_NO_BYTE_SWAP
            );
    dbg("DMA tx task started\r\n");
}

/*
 * Continue the Tx DMA task
 *
 * This routine is called after the DMA task has halted after
 * encountering an Tx buffer descriptor that wasn't marked as
 * ready.  There is no harm in calling the continue DMA routine
 * if the DMA was not paused.
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_tx_continue(uint8_t ch)
{
    int channel;

    /*
     * Determine the DMA channel running the task for the
     * selected FEC
     */
    channel = dma_get_channel(DMA_FEC_TX(ch));
    dbg("dma_get_channel(DMA_FEC_TX(%d)) = %d\r\n", ch, channel);

    /*
     * Continue/restart the DMA task
     */
    MCD_continDma(channel);
    dbg("DMA TX task continue\r\n");
}

/*
 * Stop all transmissions on the selected FEC and kill the DMA task
 *
 * Parameters:
 *  ch  FEC channel
 */
void fec_tx_stop(uint8_t ch)
{
    uint32_t mask;
    int channel;


    /* Save off the EIMR value */
    mask = MCF_FEC_EIMR(ch);

    /* Mask all interrupts */
    MCF_FEC_EIMR(ch) = 0;

    /* If the Ethernet is still enabled... */
    if (MCF_FEC_ECR(ch) & MCF_FEC_ECR_ETHER_EN)
    {
        /* Issue the Graceful Transmit Stop */
        MCF_FEC_TCR(ch) |= MCF_FEC_TCR_GTS;

        /* Wait for the Graceful Stop Complete interrupt */
        while (!(MCF_FEC_EIR(ch) & MCF_FEC_EIR_GRA))
        {
            if (!(MCF_FEC_ECR(ch) & MCF_FEC_ECR_ETHER_EN))
                break;
        }

        /* Clear the Graceful Stop Complete interrupt */
        MCF_FEC_EIR(ch) = MCF_FEC_EIR_GRA;
    }

    /*
     * Determine the DMA channel running the task for the
     * selected FEC
     */
    channel = dma_get_channel(DMA_FEC_TX(ch));

    /* Kill the FEC Tx DMA task */
    MCD_killDma(channel);

    /*
     * Free up the FEC requestor from the software maintained
     * initiator list
     */
    dma_free_initiator(DMA_FEC_TX(ch));

    /* Free up the DMA channel */
    dma_free_channel(DMA_FEC_TX(ch));

    /* Restore the interrupt mask register value */
    MCF_FEC_EIMR(ch) = mask;
}

/*
 * Trasmit Frame interrupt handler - this handler is called by the
 * DMA interrupt handler indicating that a packet was successfully
 * transferred to the Tx FIFO.
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_tx_frame(uint8_t ch)
{
    FECBD *pTxBD;
    NBUF *pNbuf;
    bool is_empty = true;

    dbg("\r\n");
    while ((pTxBD = fecbd_tx_free(ch)) != NULL)
    {
        fec_log[ch].dtxf++;

        /*
         * Grab the network buffer associated with this buffer descriptor
         */
        pNbuf = nbuf_remove(NBUF_TX_RING);

        /*
         * Free up the network buffer that was just transmitted
         */
        nbuf_free(pNbuf);
        dbg("free buffer %p from TX ring\r\n", pNbuf);

        /*
         * Re-initialize the Tx BD
         */
        pTxBD->data = NULL;
        pTxBD->length = 0;
        is_empty = false;

    }
    if (is_empty)
        dbg("transmit queue was empty!\r\n");
}

void fec0_tx_frame(void)
{
    fec_tx_frame(0);
}

void fec1_tx_frame(void)
{
    fec_tx_frame(1);
}

/*
 * Send a packet out the selected FEC
 *
 * Parameters:
 *  ch      FEC channel
 *  nif     Pointer to Network Interface (NIF) structure
 *  dst     Destination MAC Address
 *  src     Source MAC Address
 *  type    Ethernet Frame Type
 *  length  Number of bytes to be transmitted (doesn't include type,
 *          src, or dest byte count)
 *  pkt     Pointer packet network buffer
 *
 * Return Value:
 *  1       success
 *  0       otherwise
 */
int fec_send(uint8_t ch, NIF *nif, uint8_t *dst, uint8_t *src, uint16_t type, NBUF *nbuf)
{
    FECBD *pTxBD;

    /* Check the length */
    if ((nbuf->length + ETH_HDR_LEN) > ETH_MTU)
    {
        dbg("nbuf->length (%d) + ETH_HDR_LEN (%d) exceeds ETH_MTU (%d)\r\n",
                nbuf->length, ETH_HDR_LEN, ETH_MTU);
        return 0;
    }

    /*
     * Copy the destination address, source address, and Ethernet
     * type into the packet
     */
    memcpy(&nbuf->data[0],  dst,   6);
    memcpy(&nbuf->data[6],  src,   6);
    memcpy(&nbuf->data[12], &type, 2);

    /*
     * Grab the next available Tx Buffer Descriptor
     */
    while ((pTxBD = fecbd_tx_alloc(ch)) == NULL) {};

    /*
     * Put the network buffer into the Tx waiting queue
     */
    nbuf_add(NBUF_TX_RING, nbuf);

    /*
     * Setup the buffer descriptor for transmission
     */
    pTxBD->data = nbuf->data;
    pTxBD->length = nbuf->length + ETH_HDR_LEN;
    pTxBD->status |= (TX_BD_R | TX_BD_L);

    /*
     * Continue the Tx DMA task (in case it was waiting for a new
     * TxBD to be ready
     */
    fec_tx_continue(ch);

    return 1;
}

int fec0_send(NIF *nif, uint8_t *dst, uint8_t *src, uint16_t type, NBUF *nbuf)
{
    return fec_send(0, nif, dst, src, type, nbuf);
}

int fec1_send(NIF *nif, uint8_t *dst, uint8_t *src, uint16_t type, NBUF *nbuf)
{
    return fec_send(1, nif, dst, src, type, nbuf);
}

/*
 * Enable interrupts on the selected FEC
 *
 * Parameters:
 *  ch      FEC channel
 *  pri     Interrupt Priority
 *  lvl     Interrupt Level
 */
void fec_irq_enable(uint8_t ch, uint8_t lvl, uint8_t pri)
{
    /*
     * Setup the appropriate ICR
     */
    MCF_INTC_ICR((ch == 0) ? 39 : 38) = MCF_INTC_ICR_IP(pri) |
                                        MCF_INTC_ICR_IL(lvl);

    /*
     * Clear any pending FEC interrupt events
     */
    MCF_FEC_EIR(ch) = MCF_FEC_EIR_CLEAR_ALL;

    /*
     * Unmask all FEC interrupts
     */
    MCF_FEC_EIMR(ch) = MCF_FEC_EIMR_UNMASK_ALL;

    /*
     * Unmask the FEC interrupt in the interrupt controller
     */
    if (ch == 0)
        MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK39;
    else
        MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK38;
}


/*
 * Disable interrupts on the selected FEC
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_irq_disable(uint8_t ch)
{

    /*
     * Mask all FEC interrupts
     */
    MCF_FEC_EIMR(ch) = MCF_FEC_EIMR_MASK_ALL;

    /*
     * Mask the FEC interrupt in the interrupt controller
     */
    if (ch == 0)
        MCF_INTC_IMRH |= MCF_INTC_IMRH_INT_MASK39;
    else
        MCF_INTC_IMRH |= MCF_INTC_IMRH_INT_MASK38;
}

/*
 * FEC interrupt handler
 * All interrupts are multiplexed into a single vector for each
 * FEC module. The lower level interrupt handler passes in the
 * channel to this handler. Note that the receive interrupt is
 * generated by the Multi-channel DMA FEC Rx task.
 *
 * Parameters:
 * ch       FEC channel
 */
static bool fec_irq_handler(uint8_t ch)
{
    uint32_t event, eir;

    /*
     * Determine which interrupt(s) asserted by AND'ing the
     * pending interrupts with those that aren't masked.
     */
    eir = MCF_FEC_EIR(ch);
    event = eir & MCF_FEC_EIMR(ch);

    if (event != eir)
        dbg("pending but not enabled: 0x%08x\r\n", (event ^ eir));

    /*
     * Clear the event(s) in the EIR immediately
     */
    MCF_FEC_EIR(ch) = event;

    if (event & MCF_FEC_EIR_RFERR)
    {
        fec_log[ch].total++;
        fec_log[ch].rferr++;
        dbg("RFERR\r\n");
        dbg("FECRFSR%d = 0x%08x\r\n", ch, MCF_FEC_FECRFSR(ch));
        //fec_eth_stop(ch);
    }

    if (event & MCF_FEC_EIR_XFERR)
    {
        fec_log[ch].total++;
        fec_log[ch].xferr++;
        dbg("XFERR\r\n");
    }

    if (event & MCF_FEC_EIR_XFUN)
    {
        fec_log[ch].total++;
        fec_log[ch].xfun++;
        dbg("XFUN\r\n");
        //fec_eth_stop(ch);
    }

    if (event & MCF_FEC_EIR_RL)
    {
        fec_log[ch].total++;
        fec_log[ch].rl++;
        dbg("RL\r\n");
    }

    if (event & MCF_FEC_EIR_LC)
    {
        fec_log[ch].total++;
        fec_log[ch].lc++;
        dbg("LC\r\n");
    }

    if (event & MCF_FEC_EIR_MII)
    {
        fec_log[ch].mii++;
        dbg("MII\r\n");
    }

    if (event & MCF_FEC_EIR_TXF)
    {
        fec_log[ch].txf++;
        dbg("TXF\r\n");
    }

    if (event & MCF_FEC_EIR_GRA)
    {
        fec_log[ch].gra++;
        dbg("GRA\r\n");
    }

    if (event & MCF_FEC_EIR_BABT)
    {
        fec_log[ch].total++;
        fec_log[ch].babt++;
        dbg("BABT\r\n");
    }

    if (event & MCF_FEC_EIR_BABR)
    {
        fec_log[ch].total++;
        fec_log[ch].babr++;
        dbg("BABR\r\n");
    }

    if (event & MCF_FEC_EIR_HBERR)
    {
        fec_log[ch].total++;
        fec_log[ch].hberr++;
        dbg("HBERR\r\n");
    }

    return true;
}

/*
 * handler for FEC interrupts
 * arg2 is a pointer to the nif in this case
 */
bool fec0_interrupt_handler(void* arg1, void* arg2)
{
    bool res;

    (void) arg1;    /* not used */
    (void) arg2;

    res = fec_irq_handler(0);

    return res;
}

bool fec1_interrupt_handler(void* arg1, void* arg2)
{
    bool res;

    (void) arg1;    /* not used */
    (void) arg2;

    res = fec_irq_handler(1);

    return res;
}

/*
 * Configure the selected Ethernet port and enable all operations
 *
 * Parameters:
 *  ch      FEC channel
 *  trcvr   Transceiver mode (MII, 7-Wire or internal loopback)
 *  speed   Maximum operating speed (MII only)
 *  duplex  Full or Half-duplex (MII only)
 *  mac     Physical (MAC) Address
 */
void fec_eth_setup(uint8_t ch, uint8_t trcvr, uint8_t speed, uint8_t duplex, const uint8_t *mac)
{
    /*
     * Disable FEC interrupts
     */
    fec_irq_disable(ch);

    /*
     * Initialize the event log
     */
    fec_log_init(ch);

    /*
     * Initialize the network buffers and fec buffer descriptors
     */
    nbuf_init();
    fecbd_init(ch);

    /*
     * Initialize the FEC
     */
    fec_reset(ch);
    fec_init(ch, trcvr, mac);

    if (trcvr == FEC_MODE_MII)
    {
        /*
         * Initialize the MII interface
         */
#if defined(MACHINE_FIREBEE)
        if (am79c874_init(0, 0, speed, duplex))
            dbg("PHY init completed\r\n");
        else
            dbg("PHY init failed\r\n");
#elif defined(MACHINE_M548X)
        bcm_5222_init(0, 0, speed, duplex);
#else
        fec_mii_init(ch, SYSCLK / 1000);
#endif /* MACHINE_FIREBEE */
    }

    /*
     * Initialize and enable FEC interrupts
     */
    fec_irq_enable(ch, FEC_INTC_LVL(ch), FEC_INTC_PRI(ch));

    /*
     * Enable the multi-channel DMA tasks
     */
    fec_rx_start(ch, (int8_t*) fecbd_get_start(ch, Rx));
    fec_tx_start(ch, (int8_t*) fecbd_get_start(ch, Tx));

    /*
     * Enable the FEC channel
     */
    MCF_FEC_ECR(ch) |= MCF_FEC_ECR_ETHER_EN;
}

/*
 * Reset the selected Ethernet port
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_eth_reset(uint8_t ch)
{
    // To do
}


/*
 * Stop the selected Ethernet port
 *
 * Parameters:
 *  ch      FEC channel
 */
void fec_eth_stop(uint8_t ch)
{
    int level;

    /*
     * Disable interrupts
     */
    level = set_ipl(7);

    dbg("fec %d stopped\r\n", ch);
    /*
     * Gracefully disable the receiver and transmitter
     */
    fec_tx_stop(ch);
    fec_rx_stop(ch);

    /*
     * Disable FEC interrupts
     */
    fec_irq_disable(ch);

    /*
     * Disable the FEC channel
     */
    MCF_FEC_ECR(ch) &= ~MCF_FEC_ECR_ETHER_EN;

#ifdef DBG_FEC
    nbuf_debug_dump();
    fec_log_dump(ch);
#endif

    /*
     * Flush the network buffers
     */
    nbuf_flush();

    /*
     * Restore interrupt level
     */
    set_ipl(level);
}


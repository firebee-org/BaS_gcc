/*
 * File:        am79c874.c
 * Purpose:     Driver for the AMD AM79C874 10/100 Ethernet PHY
 */

#include "net.h"
#include "fec.h"
#include "am79c874.h"

#include "bas_printf.h"

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M5475EVB)
#include "m5475e.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine!"
#endif

#include <debug.h>
// #define DEBUG

/* Initialize the AM79C874 PHY
 *
 * This function sets up the Auto-Negotiate Advertisement register
 * within the PHY and then forces the PHY to auto-negotiate for
 * it's settings.
 *
 * Params:
 *  fec_ch      FEC channel
 *  phy_addr    Address of the PHY.
 *  speed       Desired speed (10BaseT or 100BaseTX)
 *  duplex      Desired duplex (Full or Half)
 *
 * Return Value:
 *  0 if MII commands fail
 *  1 otherwise
 */
int am79c874_init(uint8_t fec_ch, uint8_t phy_addr, uint8_t speed, uint8_t duplex)
{
    int timeout;
    uint16_t settings;
    if (speed); /* to do */
    if (duplex); /* to do */

    /* Initialize the MII interface */
    fec_mii_init(fec_ch, SYSCLK / 1000);
    dbg("%s: PHY reset\r\n", __FUNCTION__);

    /* Reset the PHY */
    if (!fec_mii_write(fec_ch, phy_addr, MII_AM79C874_CR, MII_AM79C874_CR_RESET))
        return 0;

    /* Wait for the PHY to reset */
    for (timeout = 0; timeout < FEC_MII_TIMEOUT; timeout++)
    {
        fec_mii_read(fec_ch, phy_addr, MII_AM79C874_CR, &settings);
        if (!(settings & MII_AM79C874_CR_RESET))
            break;
    }

    if (timeout >= FEC_MII_TIMEOUT)
    {
        dbg("%s: PHY reset failed\r\n", __FUNCTION__);
        return 0;
    };
    dbg("%s: PHY reset OK\r\n", __FUNCTION__);
    dbg("%s: PHY Enable Auto-Negotiation\r\n", __FUNCTION__);

    /* Enable Auto-Negotiation */
    if (!fec_mii_write(fec_ch, phy_addr, MII_AM79C874_CR, MII_AM79C874_CR_AUTON | MII_AM79C874_CR_RST_NEG))
        return 0;

    dbg("%s:PHY Wait for auto-negotiation to complete\r\n", __FUNCTION__);

    /* Wait for auto-negotiation to complete */
    for (timeout = 0; timeout < FEC_MII_TIMEOUT; timeout++)
    {
        settings = 0;
        fec_mii_read(fec_ch, phy_addr, MII_AM79C874_SR, &settings);
        if ((settings & AUTONEGLINK) == AUTONEGLINK)
            break;
    }

    if (timeout >= FEC_MII_TIMEOUT)
    {
        dbg("%s: Auto-negotiation failed (timeout). Set default mode (100Mbps, full duplex)\r\n", __FUNCTION__);

        /* Set the default mode (Full duplex, 100 Mbps) */
        if (!fec_mii_write(fec_ch, phy_addr, MII_AM79C874_CR, MII_AM79C874_CR_100MB | MII_AM79C874_CR_DPLX))
        {
            dbg("%s: forced setting 100Mbps/full failed.\r\n", __FUNCTION__);
            return 0;
        }
    }

#ifdef DBG_AM79
    settings = 0;

    fec_mii_read(fec_ch, phy_addr, MII_AM79C874_DR, &settings);

    dbg("%s: PHY Mode:\r\n", __FUNCTION__);
    if (settings & MII_AM79C874_DR_DATA_RATE)
        dbg("%s: 100Mbps", __FUNCTION__);
    else
        dbg("%s: 10Mbps ", __FUNCTION__);

    if (settings & MII_AM79C874_DR_DPLX)
        dbg("%s: Full-duplex\r\n", __FUNCTION__);
    else
        dbg("%s: Half-duplex\r\n", __FUNCTION__);

    dbg("%s:PHY auto-negotiation complete\r\n", __FUNCTION__);
#endif /* DBG_AM79 */

    return 1;
}

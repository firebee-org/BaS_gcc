#include <stddef.h>
#include "bas_types.h"

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M5475EVB)
#include "m5475e.h"
#endif /* MACHINE_FIREBEE */

#define AMD_FLASH_BUS_SHIFT     1
#define AMD_FLASH_CELL          volatile uint16_t
#define AMD_FLASH_CELL_BYTES    2
#define AMD_FLASH_CELL_MASK     0x1
#define AMD_FLASH_CMD_DATA(x)   ((uint16_t) x)

struct amd_flash_sector_info
{
    uint32_t size; /* sector size in bytes */
    uint32_t offset; /* offset from base of device */
};

/*
 * AM29LV640D flash layout (bottom boot as used in the Firebee )
 */
static struct amd_flash_sector_info sector[] =
{
    {  8 * 1024, 0x00000000 }, /* SA0 */
    {  8 * 1024, 0x00008000 }, /* SA1 */
    {  8 * 1024, 0x00010000 }, /* SA2 */
    {  8 * 1024, 0x00018000 }, /* SA3 */
    {  8 * 1024, 0x00020000 }, /* SA4 */
    {  8 * 1024, 0x00028000 }, /* SA5 */
    {  8 * 1024, 0x00030000 }, /* SA6 */
    {  8 * 1024, 0x00038000 }, /* SA7 */
    {  8 * 1024, 0x00040000 }, /* SA8 */
    { 64 * 1024, 0x00048000 }, /* SA9 */
    { 64 * 1024, 0x00050000 }, /* SA10 */
    { 64 * 1024, 0x00058000 }, /* SA11 */
    { 64 * 1024, 0x00060000 }, /* SA12 */
    { 64 * 1024, 0x00068000 }, /* SA13 */
    { 64 * 1024, 0x00070000 }, /* SA14 */
    { 64 * 1024, 0x00078000 }, /* SA15 */
    { 64 * 1024, 0x00080000 }, /* SA16 */
    { 64 * 1024, 0x00088000 }, /* SA17 */
    { 64 * 1024, 0x00090000 }, /* SA18 */
    { 64 * 1024, 0x00098000 }, /* SA19 */
    { 64 * 1024, 0x000a0000 }, /* SA20 */
    { 64 * 1024, 0x000a8000 }, /* SA21 */
    { 64 * 1024, 0x000b0000 }, /* SA22 */
    { 64 * 1024, 0x000b8000 }, /* SA23 */
    { 64 * 1024, 0x000c0000 }, /* SA24 */
    { 64 * 1024, 0x000c8000 }, /* SA25 */
    { 64 * 1024, 0x000d0000 }, /* SA26 */
    { 64 * 1024, 0x000d8000 }, /* SA27 */
    { 64 * 1024, 0x000e0000 }, /* SA28 */
    { 64 * 1024, 0x000e8000 }, /* SA29 */
    { 64 * 1024, 0x000f0000 }, /* SA30 */
    { 64 * 1024, 0x000f8000 }, /* SA31 */
    { 64 * 1024, 0x00100000 }, /* SA32 */
    { 64 * 1024, 0x00108000 }, /* SA32 */
    { 64 * 1024, 0x00110000 }, /* SA34 */
    { 64 * 1024, 0x00118000 }, /* SA35 */
    { 64 * 1024, 0x00120000 }, /* SA36 */
    { 64 * 1024, 0x00128000 }, /* SA37 */
    { 64 * 1024, 0x00130000 }, /* SA38 */
    { 64 * 1024, 0x00138000 }, /* SA39 */
    { 64 * 1024, 0x00140000 }, /* SA40 */
    { 64 * 1024, 0x00148000 }, /* SA41 */
    { 64 * 1024, 0x00150000 }, /* SA42 */
    { 64 * 1024, 0x00158000 }, /* SA43 */
    { 64 * 1024, 0x00160000 }, /* SA44 */
    { 64 * 1024, 0x00168000 }, /* SA45 */
    { 64 * 1024, 0x00170000 }, /* SA46 */
    { 64 * 1024, 0x00178000 }, /* SA47 */
    { 64 * 1024, 0x00180000 }, /* SA48 */
    { 64 * 1024, 0x00188000 }, /* SA49 */
    { 64 * 1024, 0x00190000 }, /* SA50 */
    { 64 * 1024, 0x00198000 }, /* SA51 */
    { 64 * 1024, 0x001a0000 }, /* SA52 */
    { 64 * 1024, 0x001a8000 }, /* SA53 */
    { 64 * 1024, 0x001b0000 }, /* SA54 */
    { 64 * 1024, 0x001b8000 }, /* SA55 */
    { 64 * 1024, 0x001c0000 }, /* SA56 */
    { 64 * 1024, 0x001c8000 }, /* SA57 */
    { 64 * 1024, 0x001d0000 }, /* SA58 */
    { 64 * 1024, 0x001d8000 }, /* SA59 */
    { 64 * 1024, 0x001e0000 }, /* SA60 */
    { 64 * 1024, 0x001e8000 }, /* SA61 */
    { 64 * 1024, 0x001f0000 }, /* SA62 */
    { 64 * 1024, 0x001f8000 }, /* SA63 */
    { 64 * 1024, 0x00200000 }, /* SA64 */
    { 64 * 1024, 0x00208000 }, /* SA65 */
    { 64 * 1024, 0x00210000 }, /* SA66 */
    { 64 * 1024, 0x00218000 }, /* SA67 */
    { 64 * 1024, 0x00220000 }, /* SA68 */
    { 64 * 1024, 0x00228000 }, /* SA69 */
    { 64 * 1024, 0x00230000 }, /* SA70 */
    { 64 * 1024, 0x00238000 }, /* SA71 */
    { 64 * 1024, 0x00240000 }, /* SA72 */
    { 64 * 1024, 0x00248000 }, /* SA73 */
    { 64 * 1024, 0x00250000 }, /* SA74 */
    { 64 * 1024, 0x00258000 }, /* SA75 */
    { 64 * 1024, 0x00260000 }, /* SA76 */
    { 64 * 1024, 0x00268000 }, /* SA77 */
    { 64 * 1024, 0x00270000 }, /* SA78 */
    { 64 * 1024, 0x00278000 }, /* SA79 */
    { 64 * 1024, 0x00280000 }, /* SA80 */
    { 64 * 1024, 0x00288000 }, /* SA81 */
    { 64 * 1024, 0x00290000 }, /* SA82 */
    { 64 * 1024, 0x00298000 }, /* SA83 */
    { 64 * 1024, 0x002a0000 }, /* SA84 */
    { 64     * 1024, 0x002a8000 }, /* SA85 */
    { 64 * 1024, 0x002b0000 }, /* SA86 */
    { 64 * 1024, 0x002b8000 }, /* SA87 */
    { 64 * 1024, 0x002c0000 }, /* SA88 */
    { 64 * 1024, 0x002c8000 }, /* SA89 */
    { 64 * 1024, 0x002d0000 }, /* SA90 */
    { 64 * 1024, 0x002d8000 }, /* SA91 */
    { 64 * 1024, 0x002e0000 }, /* SA92 */
    { 64 * 1024, 0x002e8000 }, /* SA93 */
    { 64 * 1024, 0x002f0000 }, /* SA94 */
    { 64 * 1024, 0x002f8000 }, /* SA95 */
    { 64 * 1024, 0x00300000 }, /* SA96 */
    { 64 * 1024, 0x00308000 }, /* SA97 */
    { 64 * 1024, 0x00310000 }, /* SA98 */
    { 64 * 1024, 0x00318000 }, /* SA99 */
    { 64 * 1024, 0x00320000 }, /* SA100 */
    { 64 * 1024, 0x00328000 }, /* SA101 */
    { 64 * 1024, 0x00330000 }, /* SA102 */
    { 64 * 1024, 0x00338000 }, /* SA103 */
    { 64 * 1024, 0x00340000 }, /* SA104 */
    { 64 * 1024, 0x00348000 }, /* SA105 */
    { 64 * 1024, 0x00350000 }, /* SA106 */
    { 64 * 1024, 0x00358000 }, /* SA107 */
    { 64 * 1024, 0x00360000 }, /* SA108 */
    { 64 * 1024, 0x00368000 }, /* SA109 */
    { 64 * 1024, 0x00370000 }, /* SA110 */
    { 64 * 1024, 0x00378000 }, /* SA111 */
    { 64 * 1024, 0x00380000 }, /* SA112 */
    { 64 * 1024, 0x00388000 }, /* SA113 */
    { 64 * 1024, 0x00390000 }, /* SA114 */
    { 64 * 1024, 0x00398000 }, /* SA115 */
    { 64 * 1024, 0x003a0000 }, /* SA116 */
    { 64 * 1024, 0x003a8000 }, /* SA117 */
    { 64 * 1024, 0x003b0000 }, /* SA118 */
    { 64 * 1024, 0x003b8000 }, /* SA119 */
    { 64 * 1024, 0x003c0000 }, /* SA120 */
    { 64 * 1024, 0x003c8000 }, /* SA121 */
    { 64 * 1024, 0x003d0000 }, /* SA122 */
    { 64 * 1024, 0x003d8000 }, /* SA123 */
    { 64 * 1024, 0x003e0000 }, /* SA124 */
    { 64 * 1024, 0x003e8000 }, /* SA125 */
    { 64 * 1024, 0x003f0000 }, /* SA126 */
    { 64 * 1024, 0x003f8000 }, /* SA127 */
};

static const int AMD_FLASH_SECTORS = sizeof(sector)	/ sizeof(struct amd_flash_sector_info);

#define SOFFSET(n)  (sector[n].offset)
#define SADDR(n)    (SOFFSET(n) >> AMD_FLASH_BUS_SHIFT)
#define SSIZE(n)	(sector[n].size)

#define AMD_FLASH_DEVICES	1

static AMD_FLASH_CELL *pFlash;

typedef struct romram
{
    uint32_t flash_address;
    uint32_t ram_address;
    char *name;
} ROMRAM;

static const struct romram flash_areas[] =
{
    { 0xe0000000, 0x00e00000, "BaS" },		/* BaS */
    { 0xe0600000, 0x00e00000, "EmuTOS" }, 	/* EmuTOS */
    { 0xe0400000, 0x00e00000, "FireTOS" }, 	/* FireTOS */
    { 0xe0700000, 0x00e00000, "FPGA" },		/* FPGA config */
};
static const int num_flash_areas = sizeof(flash_areas) / sizeof(struct romram);

#define FLASH_ADDRESS BOOTFLASH_BASE_ADDRESS

/*
 * erase a flash sector
 *
 * sector_num is the index into the sector table above.
 *
 * FIXME: need to disable data cache to ensure proper operation
 */
void amd_flash_sector_erase(int n)
{
    volatile AMD_FLASH_CELL status;

    (void) num_flash_areas;     /* to make compiler happy */

    pFlash[0x555] = AMD_FLASH_CMD_DATA(0xAA);
    pFlash[0x2AA] = AMD_FLASH_CMD_DATA(0x55);
    pFlash[0x555] = AMD_FLASH_CMD_DATA(0x80);
    pFlash[0x555] = AMD_FLASH_CMD_DATA(0xAA);
    pFlash[0x2AA] = AMD_FLASH_CMD_DATA(0x55);
    pFlash[SADDR(n)] = AMD_FLASH_CMD_DATA(0x30);

    do
        status = pFlash[SADDR(n)];
    while ((status & AMD_FLASH_CMD_DATA(0x80)) != AMD_FLASH_CMD_DATA(0x80));

    /*
     * Place device in read mode
     */
    pFlash[0] = AMD_FLASH_CMD_DATA(0xAA);
    pFlash[0] = AMD_FLASH_CMD_DATA(0x55);
    pFlash[0] = AMD_FLASH_CMD_DATA(0xF0);
}

int amd_flash_erase(void *start, int bytes, void (*putchar)(int))
{
    int i, ebytes = 0;

    if (bytes == 0)
        return 0;

    for (i = 0; i < AMD_FLASH_SECTORS; i++)
    {
        if (start >= (void *)((void *) pFlash + SOFFSET(i)) &&
                start <= (void *)((void *) pFlash + SOFFSET(i) + (SSIZE(i) - 1)))
        {
            break;
        }
    }

    while (ebytes < bytes)
    {
        if (putchar != NULL)
        {
            putchar('.');
        }
        amd_flash_sector_erase(i);
        ebytes += SSIZE(i);
        i++;
    }

    if (putchar != NULL)
    {
        putchar(10);    /* LF */
        putchar(13);    /* CR */
    }

    return ebytes;
}

void amd_flash_program_cell(AMD_FLASH_CELL *dst, AMD_FLASH_CELL data)
{
    volatile AMD_FLASH_CELL status;
    int retry;

    pFlash[0x555] = AMD_FLASH_CMD_DATA(0xAA);
    pFlash[0x2AA] = AMD_FLASH_CMD_DATA(0x55);
    pFlash[0x555] = AMD_FLASH_CMD_DATA(0xA0);
    *dst = data;

    /*
     * Wait for program operation to finish
     *  (Data# Polling Algorithm)
     */
    retry = 0;
    while (1)
    {
        status = *dst;
        if ((status & AMD_FLASH_CMD_DATA(0x80)) ==
                (data & AMD_FLASH_CMD_DATA(0x80)))
        {
            break;
        }
        if (status & AMD_FLASH_CMD_DATA(0x20))
        {
            status = *dst;
            if ((status & AMD_FLASH_CMD_DATA(0x80)) ==
                    (data & AMD_FLASH_CMD_DATA(0x80)))
            {
                break;
            }
            if (++retry > 1024)
            {
                break;
            }
        }
    }
}


int amd_flash_program(void *dest, void *source, int bytes, int erase, void (*func)(void), void (*putchar)(int))
{
    AMD_FLASH_CELL *src;
    AMD_FLASH_CELL *dst;
    int hashi = 1;
    int hashj = 0;
    char hash[5];

    hash[0] = 8;  /* Backspace */
    hash[1] = 124;/* "|" */
    hash[2] = 47; /* "/" */
    hash[3] = 45; /* "-" */
    hash[4] = 92; /* "\" */

    src = (AMD_FLASH_CELL *)source;
    dst = (AMD_FLASH_CELL *)dest;

    /*
     * Place device in read mode
     */
    pFlash[0] = AMD_FLASH_CMD_DATA(0xAA);
    pFlash[0] = AMD_FLASH_CMD_DATA(0x55);
    pFlash[0] = AMD_FLASH_CMD_DATA(0xF0);

    /*
     * Erase device if necessary
     */
    if (erase)
    {
        amd_flash_erase(dest, bytes, putchar);
    }

    /*
     * Program device
     */
    while (bytes > 0)
    {
        amd_flash_program_cell(dst, *src);

        /* Verify Write */
        if (*dst == *src)
        {
            bytes -= AMD_FLASH_CELL_BYTES;
            *dst++, *src++;

            if ((putchar != NULL))
            {
                /* Hash marks to indicate progress */
                if (hashj == 0x1000)
                {
                    hashj = -1;
                    putchar(hash[0]);
                    putchar(hash[hashi]);

                    hashi++;
                    if (hashi == 5)
                    {
                        hashi = 1;
                    }

                }
                hashj++;
            }
        }
        else
            break;
    }

    /*
     * Place device in read mode
     */
    pFlash[0] = AMD_FLASH_CMD_DATA(0xAA);
    pFlash[0] = AMD_FLASH_CMD_DATA(0x55);
    pFlash[0] = AMD_FLASH_CMD_DATA(0xF0);

    if (putchar != NULL)
    {
        putchar(hash[0]);
    }

    /*
     * If a function was passed in, call it now
     */
    if ((func != NULL))
    {
        func();
    }

    return ((int)src - (int)source);
}


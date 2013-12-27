#ifndef _DRIVER_MEM_H_
#define _DRIVER_MEM_H_

/*
 * the driver_mem module provides a block of _uncached_ memory for USB and other drivers as
 * well as some memory handling functions for it
 */
extern int driver_mem_init(void);
extern void *driver_mem_alloc(long amount);
extern int driver_mem_free(void *addr);
extern void driver_mem_release(void);

#endif /* _DRIVER_MEM_H_ */

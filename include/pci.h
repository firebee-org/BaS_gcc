#ifndef _PCI_H_
#define _PCI_H_

#define	 PCI_MEMORY_OFFSET	(0x80000000)
#define	 PCI_MEMORY_SIZE	(0x40000000)
#define	 PCI_IO_OFFSET		(0xD0000000)
#define	 PCI_IO_SIZE		(0x10000000)

extern void init_eport(void);
extern void init_xlbus_arbiter(void);
extern void init_pci(void);

#endif /* _PCI_H_ */

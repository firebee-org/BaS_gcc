/*
 * pci.S
 *
 * Purpose:		PCI configuration for the Coldfire builtin PCI bridge.
 *
 * Notes:
 *
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
 *  Created on: 08.05.2014
 *      Author: David Galvez
 */

.global _wrapper_find_pci_device
.global _wrapper_find_pci_classcode

.global _wrapper_read_config_longword
.global _wrapper_read_config_word
.global _wrapper_read_config_byte

.global _wrapper_fast_read_config_byte
.global _wrapper_fast_read_config_word
.global _wrapper_fast_read_config_longword

.global _wrapper_write_config_longword
.global _wrapper_write_config_word
.global _wrapper_write_config_byte

.global _wrapper_get_resource
.global _wrapper_hook_interrupt
.global _wrapper_unhook_interrupt

.global _wrapper_special_cycle
.global _wrapper_get_routing
.global _wrapper_set_interrupt
.global _wrapper_get_resource
.global _wrapper_get_card_used
.global _wrapper_set_card_used

.global _wrapper_read_mem_byte
.global _wrapper_read_mem_word
.global _wrapper_read_mem_longword

.global _wrapper_fast_read_mem_byte
.global _wrapper_fast_read_mem_word
.global _wrapper_fast_read_mem_longword

.global _wrapper_write_mem_byte
.global _wrapper_write_mem_word
.global _wrapper_write_mem_longword

.global _wrapper_read_io_byte
.global _wrapper_read_io_word
.global _wrapper_read_io_longword

.global _wrapper_fast_read_io_byte
.global _wrapper_fast_read_io_word
.global _wrapper_fast_read_io_longword

.global _wrapper_write_io_byte
.global _wrapper_write_io_word
.global _wrapper_write_io_longword

.global _wrapper_get_machine_id
.global _wrapper_get_pagesize

.global _wrapper_virt_to_bus
.global _wrapper_bus_to_virt
.global _wrapper_virt_to_phys
.global _wrapper_phys_to_virt


_wrapper_find_pci_device:
	move.l D1,-(SP)			// index
	move.l D0,-(SP)			// Vendor ID
	move.l #16,D1
	lsr.l D1,D0
	move.l D0,-(SP)			// Device ID
	jsr _pci_find_device
	add.l #12,SP
	rts

_wrapper_find_pci_classcode:
	move.l D1,-(SP)			// index
	move.l D0,-(SP)			// ID
	jsr _pci_find_classcode
	addq.l #8,SP
	rts

_wrapper_read_config_byte:
	move.l A0,-(SP)			// pointer to space for read data
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_read_config_byte
	move.l 8(SP),A0			// PCI_BIOS expects value in memory
	move.l D0,(A0)
	add.l #12,SP
	move.l #0,D0
	rts

_wrapper_read_config_word:
	move.l A0,-(SP)			// pointer to space for read data
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_read_config_word
	move.l 8(SP),A0			// little to big endian
	move.l D0,(A0)
	mvz.b 1(A0),D0
	lsl.l #8,D0
	move.b (A0),D0
	move.l D0,(A0)			// PCI_BIOS expects value in memory, not in D0
	add.l #12,SP
	move.l #0,D0
	rts

_wrapper_read_config_longword:
	move.l A0,-(SP)			// pointer to space for read data
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_read_config_longword
	move.l 8(SP),A0			// little to big endian
	move.l D0,(A0)
	mvz.b 3(A0),D0
	lsl.l #8,D0
	move.b 2(A0),D0
	lsl.l #8,D0
	move.b 1(A0),D0
	lsl.l #8,D0
	move.b (A0),D0
	move.l D0,(A0)			// PCI_BIOS expects value in memory, not in D0
	add.l #12,SP
	move.l #0,D0
	rts

/* Not implemented */
_wrapper_fast_read_config_byte:
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_fast_read_config_byte
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_fast_read_config_word:
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_fast_read_config_word
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_fast_read_config_longword:
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_fast_read_config_longword
	addq.l #8,SP
	rts

_wrapper_write_config_byte:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_write_config_byte
	add.l #12,SP
	rts

_wrapper_write_config_word:
	move.l D0,-(SP)			// make data little endian
	moveq #0,D1
	move.w D2,D1
	lsr.l #8,D1
	asl.l #8,D2
	or.l D1,D2
	move.l (SP)+,D0
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_write_config_word
	add.l #12,SP
	rts

_wrapper_write_config_longword:
	move.l D0,-(SP)
	move.l D2,D0			// make data little endian
	lsr.l #8,D0
	asl.l #8,D2
	and.l #0x00FF00FF,D0
	and.l #0xFF00FF00,D2
	or.l D0,D2
	swap D2
	move.l (SP)+,D0
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// PCI register
	move.l D0,-(SP)			// handle
	jsr _pci_write_config_longword
	add.l #12,SP
	rts

_wrapper_hook_interrupt:
	move.l A1,-(SP)			// parameter for interrupt handler
	move.l A0,-(SP)			// pointer to interrupt handler
	move.l D0,-(SP)			// handle
	jsr _pci_hook_interrupt
	add.l #12,SP
	rts

_wrapper_unhook_interrupt:
	move.l D0,-(SP)			// handle
	jsr _pci_unhook_interrupt
	addq.l #4,SP
	rts

/* Not implemented */
_wrapper_special_cycle:
	move.l D1,-(SP)			// special cycle data
	move.l D0,-(SP)			// bus number
	jsr _pci_special_cycle
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_get_routing:
	move.l D0,-(SP)			// handle
	jsr _pci_get_routing
	addq.l #4,SP
	rts

/* Not implemented */
_wrapper_set_interrupt:
	move.l D1,-(SP)			// mode
	move.l D0,-(SP)			// handle
	jsr _pci_set_interrupt
	addq.l #8,SP
	rts

_wrapper_get_resource:
	move.l D0,-(SP)			// handle
	jsr _pci_get_resource
	addq.l #4,SP
	rts

/* Not implemented */
_wrapper_get_card_used:
	move.l D1,-(SP)			// address
	move.l D0,-(SP)			// handle
	jsr _pci_get_card_used
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_set_card_used:
	move.l A0,-(SP)			// callback
	move.l D0,-(SP)			// handle
	jsr _pci_set_card_used
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_read_mem_byte:
	move.l A0,-(SP)			// pointer to data in memory
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_mem_byte
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_read_mem_word:
	move.l A0,-(SP)			// pointer to data in memory
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_mem_word
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_read_mem_longword:
	move.l A0,-(SP)			// pointer to data in memory
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_mem_longword
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_fast_read_mem_byte:
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_mem_byte
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_fast_read_mem_word:
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_mem_word
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_fast_read_mem_longword:
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_mem_longword
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_write_mem_byte:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_write_mem_byte
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_write_mem_word:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_write_mem_word
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_write_mem_longword:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// address to access (in PCI memory address space)
	move.l D0,-(SP)			// handle
	jsr _pci_write_mem_longword
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_read_io_byte:
	move.l A0,-(SP)			// pointer to data in memory
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_io_byte
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_read_io_word:
	move.l A0,-(SP)			// pointer to data in memory
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_io_word
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_read_io_longword:
	move.l A0,-(SP)			// pointer to data in memory
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_io_longword
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_fast_read_io_byte:
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_io_byte
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_fast_read_io_word:
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_io_word
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_fast_read_io_longword:
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_read_io_longword
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_write_io_byte:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_write_io_byte
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_write_io_word:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_write_io_word
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_write_io_longword:
	move.l D2,-(SP)			// data to write
	move.l D1,-(SP)			// address to access (in PCI I/O address space)
	move.l D0,-(SP)			// handle
	jsr _pci_write_io_longword
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_get_machine_id:
	jsr _pci_get_machine_id
	rts

/* Not implemented */
_wrapper_get_pagesize:
	jsr _pci_get_pagesize
	rts

/* Not implemented */
_wrapper_virt_to_bus:
	move.l A0,-(SP)			// ptr
	move.l D1,-(SP)			// address in virtual CPU space
	move.l D0,-(SP)			// handle
	jsr _pci_virt_to_bus
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_bus_to_virt:
	move.l A0,-(SP)			// ptr
	move.l D1,-(SP)			// PCI bus address
	move.l D0,-(SP)			// handle
	jsr _pci_bus_to_virt
	add.l #12,SP
	rts

/* Not implemented */
_wrapper_virt_to_phys:
	move.l A0,-(SP)			// ptr
	move.l D0,-(SP)			// address in virtual CPU space
	jsr _pci_virt_to_phys
	addq.l #8,SP
	rts

/* Not implemented */
_wrapper_phys_to_virt:
	move.l A0,-(SP)			// ptr
	move.l D0,-(SP)			// physical CPU address 
	jsr _pci_phys_to_virt
	addq.l #8,SP
	rts

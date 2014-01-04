# # Makefile for Firebee BaS
#
# This Makefile is meant for cross compiling the BaS with Vincent Riviere's cross compilers.
# If you want to compile native on an Atari (you will need at least GCC 4.6.3), set
# TCPREFIX to be empty.
# If you want to compile with the m68k-elf- toolchain, set TCPREFIX accordingly. Requires an extra
# installation, but allows source level debugging over BDM with a recent gdb (tested with 7.5),
# the m68k BDM tools from sourceforge (http://bdm.sourceforge.net) and a BDM pod (TBLCF and P&E tested).

# can be either "Y" or "N" (without quotes). "Y" for using the m68k-elf-, "N" for using the m68k-atari-mint
# toolchain
COMPILE_ELF=Y

ifeq (Y,$(COMPILE_ELF))
TCPREFIX=m68k-elf-
EXE=elf
FORMAT=elf32-m68k
else 
TCPREFIX=m68k-atari-mint-
EXE=s19
FORMAT=srec
endif

CC=$(TCPREFIX)gcc
LD=$(TCPREFIX)ld
CPP=$(TCPREFIX)cpp
OBJCOPY=$(TCPREFIX)objcopy
AR=$(TCPREFIX)ar
RANLIB=$(TCPREFIX)ranlib
NATIVECC=gcc

INCLUDE=-Iinclude
CFLAGS=-mcpu=5474 \
		-Wall \
		-Os \
		-g \
		-fomit-frame-pointer \
		-ffreestanding \
		-fleading-underscore \
		-Wa,--register-prefix-optional
CFLAGS_OPTIMIZED = -mcpu=5474 \
		-Wall \
		-O2 \
		-g \
		-fomit-frame-pointer \
		-ffreestanding \
		-fleading-underscore \
		-Wa,--register-prefix-optional

TRGTDIRS= ./firebee ./m5484lite
OBJDIRS=$(patsubst %, %/objs,$(TRGTDIRS))
TOOLDIR=util

VPATH=dma exe flash fs if kbd pci spi sys usb net util video radeon x86emu xhdi

# Linker control file. The final $(LDCFILE) is intermediate only (preprocessed version of $(LDCSRC)
LDCFILE=bas.lk
LDRFILE=ram.lk
LDCSRC=bas.lk.in
LDCBSRC=basflash.lk.in
LDCBFS=bashflash.lk

# this Makefile can create the BaS to flash or an arbitrary ram address (for BDM debugging). See
# below for the definition of TARGET_ADDRESS
FLASH_EXEC=bas.$(EXE)
RAM_EXEC=ram.$(EXE)
BASFLASH_EXEC=basflash.$(EXE)

CSRCS= \
	sysinit.c \
	init_fpga.c \
	fault_vectors.c \
	interrupts.c \
	bas_printf.c \
	bas_string.c \
	BaS.c \
	cache.c \
	mmu.c \
	mmc.c \
	unicode.c \
	ff.c \
	sd_card.c \
	wait.c \
	s19reader.c \
	flash.c \
	dma.c \
	xhdi_sd.c \
	xhdi_interface.c \
	pci.c \
	dspi.c \
	driver_vec.c \
	driver_mem.c \
	MCD_dmaApi.c \
	MCD_tasks.c \
	MCD_tasksInit.c \
	usb.c \
	ohci-hcd.c \
	ehci-hcd.c \
	usb_mouse.c \
	ikbd.c \
	\
	nbuf.c \
	queue.c \
	net_timer.c \
	am79c874.c \
	bcm5222.c \
	nif.c \
	fecbd.c \
	fec.c \
	ip.c \
	udp.c \
	arp.c \
	bootp.c \
	tftp.c \
	\
	fbmem.c \
	fbmon.c \
	fbmodedb.c \
	offscreen.c \
	\
	videl.c \
	video.c \
	\
	radeon_base.c \
	radeon_accel.c \
	radeon_cursor.c \
	radeon_monitor.c \
	\
	x86decode.c \
	x86sys.c \
	x86debug.c \
	x86prim_ops.c \
	x86ops.c \
	x86ops2.c \
	x86fpu.c \
	x86biosemu.c \
	x86pcibios.c \
	\
	basflash.c \
	basflash_start.c 


ASRCS= \
	startcf.S \
	printf_helper.S \
	exceptions.S \
	xhdi_vec.S

SRCS=$(ASRCS) $(CSRCS)
COBJS=$(patsubst %.c,%.o,$(CSRCS))
AOBJS=$(patsubst %.S,%.o,$(ASRCS))

OBJS=$(COBJS) $(AOBJS)
LIBBAS=libbas.a

LIBS=$(patsubst %,%/$(LIBBAS),$(TRGTDIRS))

all: fls ram bfl lib
fls: $(patsubst %,%/$(FLASH_EXEC),$(TRGTDIRS))
ram: $(patsubst %,%/$(RAM_EXEC),$(TRGTDIRS))
bfl: $(patsubst %,%/$(BASFLASH_EXEC),$(TRGTDIRS))
lib: $(LIBS)


.PHONY: clean
clean:
	for d in $(TRGTDIRS);\
		do rm -f $$d/*.map $$d/*.s19 $$d/*.elf $$d/*.lk $$d/*.a $$d/objs/* $$d/depend;\
	done
	rm -f tags


# flags for targets
m5484lite/bas.$(EXE): MACHINE=MACHINE_M5484LITE
firebee/bas.$(EXE): MACHINE=MACHINE_FIREBEE
m5484lite/ram.$(EXE): MACHINE=MACHINE_M5484LITE
firebee/ram.$(EXE): MACHINE=MACHINE_FIREBEE
m5484lite/basflash.$(EXE): MACHINE=MACHINE_M5484LITE
firebee/basflash.$(EXE): MACHINE=MACHINE_FIREBEE

#
# generate pattern rules for different object files
#
define CC_TEMPLATE
ifeq (firebee,$(1))
	MACHINE=MACHINE_FIREBEE
else
	MACHINE=MACHINE_M5484LITE
endif

# always optimize x86 emulator objects
$(1)/objs/x86decode.o:	CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86sys.o:		CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86debug.o:	CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86prim_ops.o:CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86ops.o:		CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86ops2.o:	CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86fpu.o:		CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86biosemu.o:	CFLAGS=$(CFLAGS_OPTIMIZED)
$(1)/objs/x86pcibios.o:	CFLAGS=$(CFLAGS_OPTIMIZED)

$(1)/objs/%.o:%.c
	$(CC) $$(CFLAGS) -D$$(MACHINE) $(INCLUDE) -c $$< -o $$@

$(1)/objs/%.o:%.S
	$(CC) $$(CFLAGS) -Wa,--bitwise-or -D$$(MACHINE) $(INCLUDE) -c $$< -o $$@
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call CC_TEMPLATE,$(DIR))))

# rules for depend
define DEP_TEMPLATE
ifneq (clean,$$(MAKECMDGOALS))
include $(1)/depend
endif

ifeq (firebee,$(1))
	MACHINE=MACHINE_FIREBEE
else
	MACHINE=MACHINE_M5484LITE
endif
$(1)/depend:$(SRCS)
	$(CC) $$(CFLAGS) -D$$(MACHINE) $(INCLUDE) -M $$^ | sed -e "s#^\(.*\).o:#"$(1)"/objs/\1.o:#" > $$@
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call DEP_TEMPLATE,$(DIR))))


#
# generate pattern rules for libraries
#
define AR_TEMPLATE
$(1)_OBJS=$(patsubst %,$(1)/objs/%,$(OBJS))
$(1)/$(LIBBAS): $$($(1)_OBJS)
	$(AR) rv $$@ $$?
	$(RANLIB) $$@
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call AR_TEMPLATE,$(DIR))))

ifeq ($(COMPILE_ELF),Y)
	FORMAT_ELF=1
else
	FORMAT_ELF=0
endif
#
# define pattern rules for binaries
#
define EX_TEMPLATE
# pattern rule for flash
$(1)_MAPFILE=$(1)/$$(basename $$(FLASH_EXEC)).map
$(1)/$$(FLASH_EXEC): $(1)/$(LIBBAS) $(LDCSRC)
	$(CPP) $(INCLUDE) -DOBJDIR=$(1)/objs -P -DFORMAT_ELF=$(FORMAT_ELF) -D$$(MACHINE) $(LDCSRC) -o $(1)/$$(LDCFILE)
	$(LD) --oformat $$(FORMAT) -Map $$($(1)_MAPFILE) --cref -T $(1)/$$(LDCFILE) -o $$@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $$@ $$(basename $$@).s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $$@ $$(basename $$@).elf
endif

# pattern rule for RAM
$(1)_MAPFILE_RAM=$(1)/$$(basename $$(RAM_EXEC)).map
$(1)/$$(RAM_EXEC): $(1)/$(LIBBAS) $(LDCSRC)
	$(CPP) $(INCLUDE) -DCOMPILE_RAM -DOBJDIR=$(1)/objs -P -DFORMAT_ELF=$(FORMAT_ELF) -D$$(MACHINE) $(LDCSRC) -o $(1)/$$(LDRFILE)
	$(LD) --oformat $$(FORMAT) -Map $$($(1)_MAPFILE_RAM) --cref -T $(1)/$$(LDRFILE) -o $$@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $$@ $$(basename $$@).s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $$@ $$(basename $$@).elf
endif

# pattern rule for basflash
$(1)_MAPFILE_BFL=$(1)/$$(basename $$(BASFLASH_EXEC)).map
$(1)/$$(BASFLASH_EXEC): $(1)/objs/basflash.o $(1)/objs/basflash_start.o $(1)/$(LIBBAS) $(LDCBFL)
	$(CPP) $(INCLUDE) -P -DOBJDIR=$(1)/objs -DFORMAT_ELF=$(FORMAT_ELF) -D$$(MACHINE) $(LDCBSRC) -o $(1)/$$(LDCBFS)
	$(LD) --oformat $$(FORMAT) -Map $$($(1)_MAPFILE_BFL) --cref -T $(1)/$$(LDCFILE) -L$(1) -lbas -o $$@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $$@ $$(basename $$@).s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $$@ $$(basename $$@).elf
endif
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call EX_TEMPLATE,$(DIR))))


indent: $(CSRCS)
	indent $<
	
.PHONY: tags
tags:
	ctags $(patsubst %,%/*,$(VPATH))
	
.PHONY: printvars
printvars:
	@$(foreach V,$(.VARIABLES), $(if $(filter-out environment% default automatic, $(origin $V)),$(warning $V=$($V))))
ifeq (MACHINE_M5484LITE,$$(MACHINE))
	MNAME=m5484lite
else ifeq (MACHINE_FIREBEE,$(MACHINE))
   	MNAME=firebee
endif

tools:
	$(NATIVECC) $(INCLUDE) -c $(TOOLDIR)/s19header.c -o $(TOOLDIR)/s19header.o 
	$(NATIVECC) -o $(TOOLDIR)/s19header $(TOOLDIR)/s19header.o


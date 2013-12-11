#
# Makefile for Firebee BaS
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

SRCDIR=sources
TRGTDIRS= ./firebee ./m5484lite
OBJDIRS=$(patsubst %, %/objs,$(TRGTDIRS))
TOOLDIR=util

# Linker control file. The final $(LDCFILE) is intermediate only (preprocessed  version of $(LDCSRC)
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
	$(SRCDIR)/sysinit.c \
	$(SRCDIR)/init_fpga.c \
	$(SRCDIR)/mmu.c \
	$(SRCDIR)/fault_vectors.c \
	$(SRCDIR)/interrupts.c \
	$(SRCDIR)/bas_printf.c \
	$(SRCDIR)/bas_string.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/cache.c \
	$(SRCDIR)/mmc.c \
	$(SRCDIR)/unicode.c \
	$(SRCDIR)/ff.c \
	$(SRCDIR)/sd_card.c \
	$(SRCDIR)/wait.c \
	$(SRCDIR)/s19reader.c \
	$(SRCDIR)/flash.c \
	$(SRCDIR)/dma.c \
	$(SRCDIR)/xhdi_sd.c \
	$(SRCDIR)/xhdi_interface.c \
	$(SRCDIR)/pci.c \
	$(SRCDIR)/dspi.c \
	$(SRCDIR)/driver_vec.c \
	$(SRCDIR)/MCD_dmaApi.c \
	$(SRCDIR)/MCD_tasks.c \
	$(SRCDIR)/MCD_tasksInit.c \
	\
	$(SRCDIR)/usb.c \
	$(SRCDIR)/usb_mem.c \
	$(SRCDIR)/ohci-hcd.c \
	$(SRCDIR)/ehci-hcd.c \
	\
	$(SRCDIR)/ikbd.c \
	\
	$(SRCDIR)/basflash.c \
	$(SRCDIR)/basflash_start.c 

ASRCS= \
	$(SRCDIR)/startcf.S \
	$(SRCDIR)/printf_helper.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/illegal_instruction.S \
	$(SRCDIR)/xhdi_vec.S
	
COBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.S,%.o,$(ASRCS)))

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
		do rm -f $$d/*.map $$d/*.s19 $$d/*.elf $$d/*.lk $$d/*.a $$d/objs/* ;\
	done
	rm -f depend
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
$(1)/objs/%.o:$(SRCDIR)/%.c
	$(CC) $$(CFLAGS) -D$$(MACHINE) $(INCLUDE) -c $$< -o $$@

$(1)/objs/%.o:$(SRCDIR)/%.S
	$(CC) $$(CFLAGS) -Wa,--bitwise-or -D$$(MACHINE) $(INCLUDE) -c $$< -o $$@
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call CC_TEMPLATE,$(DIR))))

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

#
# define pattern rules for binaries
#
define EX_TEMPLATE
# pattern rule for flash
$(1)_MAPFILE=$(1)/$$(basename $$(FLASH_EXEC)).map
$(1)/$$(FLASH_EXEC): $(1)/$(LIBBAS) $(LDCSRC)
	$(CPP) $(INCLUDE) -DOBJDIR=$(1)/objs -P -DFORMAT=$$(FORMAT) -D$$(MACHINE) $(LDCSRC) -o $(1)/$$(LDCFILE)
	$(LD) --oformat $$(FORMAT) -Map $$($(1)_MAPFILE) --cref -T $(1)/$$(LDCFILE) -o $$@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $$@ $$(basename $$@).s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $$@ $$(basename $$@).elf
endif

# pattern rule for RAM
$(1)_MAPFILE_RAM=$(1)/$$(basename $$(RAM_EXEC)).map
$(1)/$$(RAM_EXEC): $(1)/$(LIBBAS) $(LDCSRC)
	$(CPP) $(INCLUDE) -DCOMPILE_RAM -DOBJDIR=$(1)/objs -P -DFORMAT=$$(FORMAT) -D$$(MACHINE) $(LDCSRC) -o $(1)/$$(LDRFILE)
	$(LD) --oformat $$(FORMAT) -Map $$($(1)_MAPFILE_RAM) --cref -T $(1)/$$(LDRFILE) -o $$@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $$@ $$(basename $$@).s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $$@ $$(basename $$@).elf
endif

# pattern rule for basflash
$(1)_MAPFILE_BFL=$(1)/$$(basename $$(BASFLASH_EXEC)).map
$(1)/$$(BASFLASH_EXEC): $(1)/objs/basflash.o $(1)/objs/basflash_start.o $(1)/$(LIBBAS) $(LDCBFL)
	$(CPP) $(INCLUDE) -P -DOBJDIR=$(1)/objs -DFORMAT=$$(FORMAT) -D$$(MACHINE) $(LDCBSRC) -o $(1)/$$(LDCBFS)
	$(LD) --oformat $$(FORMAT) -Map $$($(1)_MAPFILE_BFL) --cref -T $(1)/$$(LDCFILE) -L$(1) -lbas -o $$@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $$@ $$(basename $$@).s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $$@ $$(basename $$@).elf
endif
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call EX_TEMPLATE,$(DIR))))


depend: $(ASRCS) $(CSRCS)
	- rm -f depend
	for d in $(TRGTDIRS);\
		do $(CC) $(CFLAGS) $(INCLUDE) -M $(ASRCS) $(CSRCS) | sed -e "s#^\(.*\).o:#$$d/objs/\1.o:#" >> depend; \
	done

indent: $(CSRCS)
	indent $<
	
.PHONY: tags
tags:
	ctags sources/* include/*
	
ifneq (clean,$(MAKECMDGOALS))
-include depend
endif

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


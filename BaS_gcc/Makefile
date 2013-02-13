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

INCLUDE=-Iinclude
CFLAGS=-mcpu=5474\
	   -Wall\
	   -g\
	   -Wno-multichar\
	   -Winline\
	   -Os\
	   -fomit-frame-pointer\
	   -fno-strict-aliasing\
	   -ffreestanding\
	   -fleading-underscore\
	   -Wa,--register-prefix-optional

SRCDIR=sources
OBJDIR=objs

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
	$(SRCDIR)/bas_printf.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/cache.c \
	$(SRCDIR)/mmc.c \
	$(SRCDIR)/unicode.c \
	$(SRCDIR)/ff.c \
	$(SRCDIR)/sd_card.c \
	$(SRCDIR)/wait.c \
	$(SRCDIR)/s19reader.c \
	$(SRCDIR)/flash.c

ASRCS= \
	$(SRCDIR)/startcf.S \
	$(SRCDIR)/printf_helper.S \
	$(SRCDIR)/mmu.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/illegal_instruction.S
	
COBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.S,%.o,$(ASRCS)))

OBJS=$(COBJS) $(AOBJS)
LIBBAS=libbas.a

all: fls ram bfl lib
fls: $(FLASH_EXEC)
ram: $(RAM_EXEC)
bfl: $(BASFLASH_EXEC)
lib: $(LIBBAS)

.PHONY clean:
	@ rm -f $(FLASH_EXEC) $(FLASH_EXEC).elf $(FLASH_EXEC).s19 \
			$(RAM_EXEC) $(RAM_EXEC).elf $(RAM_EXEC).s19 \
			$(BASFLASH_EXEC) $(BASFLASH_EXEC).elf $(BASFLASH_EXEC).s19 $(OBJDIR)/basflash.o \
			$(OBJS) \
			bas.lk bas.map ram.lk ram.map basflash.lk basflash.map depend 

$(FLASH_EXEC): TARGET_ADDRESS=0xe0000000
$(FLASH_EXEC): LDCFILE=bas.lk
$(FLASH_EXEC): MAPFILE=bas.map

$(RAM_EXEC): TARGET_ADDRESS=0x10000000
$(RAM_EXEC): LDCFILE=ram.lk
$(RAM_EXEC): MAPFILE=ram.map

$(BASFLASH_EXEC): TARGET_ADDRESS=0x00020000
$(BASFLASH_EXEC): LDCFILE=basflash.lk
$(BASFLASH_EXEC): MAPFILE=basflash.map

# the final link stage
$(FLASH_EXEC) $(RAM_EXEC): $(LIBBAS) $(LDCSRC)
	$(CPP) -P -DTARGET_ADDRESS=$(TARGET_ADDRESS) -DFORMAT=$(FORMAT) $(LDCSRC) -o $(LDCFILE)
	$(LD) --oformat $(FORMAT) -Map $(MAPFILE) --cref -T $(LDCFILE) -o $@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $@ $@.s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $@ $@.elf
endif

# the basflash (SD-card executable called from BaS) final link stage
$(BASFLASH_EXEC): $(OBJDIR)/basflash.o $(LIBBAS) $(LDCBFL)
	$(CPP) -P -DTARGET_ADDRESS=$(TARGET_ADDRESS) -DFORMAT=$(FORMAT) $(LDCBSRC) -o $(LDCFILE)
	$(LD) --oformat $(FORMAT) -Map $(MAPFILE) --cref -T $(LDCFILE) -L. -lbas -o $@
ifeq ($(COMPILE_ELF),Y)
	$(OBJCOPY) -O srec $@ $@.s19
else
	objcopy -I srec -O elf32-big --alt-machine-code 4 $@ $@.elf
endif	

$(LIBBAS): $(OBJS)
	$(AR) rv $@ $(OBJS)
	$(RANLIB) $@
	
# compile init_fpga with -mbitfield for testing purposes
#$(OBJDIR)/init_fpga.o:	CFLAGS += -mbitfield

# compile printf pc-relative so it can be used as well before and after copy of BaS
$(OBJDIR)/bas_printf.o:	CFLAGS += -mpcrel
# the same for flush_and_invalidate_cache()
$(OBJDIR)/cache.o: CFLAGS += -mpcrel

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.S
	$(CC) -c $(CFLAGS) -Wa,--bitwise-or $(INCLUDE) $< -o $@

depend: $(ASRCS) $(CSRCS)
		$(CC) $(CFLAGS) $(INCLUDE) -M $(ASRCS) $(CSRCS) | sed -e 's/^\(.*\).o:/$(OBJDIR)\/\1.o:/' > depend

	
ifneq (clean,$(MAKECMDGOALS))
-include depend
endif

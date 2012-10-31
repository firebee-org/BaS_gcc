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
COMPILE_ELF=N

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

INCLUDE=-Iinclude
CFLAGS=-mcpu=5474\
	   -Wall\
	   -g\
	   -Wno-multichar\
	   -Os\
	   -fomit-frame-pointer\
	   -fno-strict-aliasing\
	   -fno-builtin\
	   -fleading-underscore\
	   -Wa,--register-prefix-optional

SRCDIR=sources
OBJDIR=objs

MAPFILE=bas.map

# Linker control file. The final $(LDCFILE) is intermediate only (preprocessed  version of $(LDCSRC)
LDCFILE=bas.lk
LDCSRC=bas.lk.in

# this Makefile can create the BaS to flash or an arbitrary ram address (for BDM debugging). See
# below for the definition of TARGET_ADDRESS
FLASH_EXEC=bas.$(EXE)
RAM_EXEC=ram.$(EXE)

CSRCS= \
	$(SRCDIR)/sysinit.c \
	$(SRCDIR)/init_fpga.c \
	$(SRCDIR)/bas_printf.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/cache.c \
	$(SRCDIR)/sd_card.c

ASRCS= \
	$(SRCDIR)/printf_helper.S \
	$(SRCDIR)/mmu.S \
	$(SRCDIR)/sd_card_asm.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/ewf.S \
	$(SRCDIR)/illegal_instruction.S 

STRT_SRC = startcf.S
STRT_OBJ = $(OBJDIR)/startcf.o

COBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.S,%.o,$(ASRCS)))

OBJS=$(COBJS) $(AOBJS)
	
all: $(FLASH_EXEC)
ram: $(RAM_EXEC)
.PHONY clean:
	@ rm -f $(FLASH_EXEC) $(FLASH_EXEC).elf $(RAM_EXEC) $(RAM_EXEC).elf $(STRT_OBJ) $(OBJS)\
			$(MAPFILE) $(LDCFILE) depend 

$(FLASH_EXEC): TARGET_ADDRESS=0xe0000000
$(RAM_EXEC): TARGET_ADDRESS=0x10000000

$(FLASH_EXEC) $(RAM_EXEC): $(STRT_OBJ) $(OBJS)
	$(CPP) -P -DTARGET_ADDRESS=$(TARGET_ADDRESS) -DFORMAT=$(FORMAT) $(LDCSRC) -o $(LDCFILE)
	$(LD) --oformat $(FORMAT) -Map $(MAPFILE) --cref -T $(LDCFILE) -o $@
ifneq ($(COMPILE_ELF),Y)
	objcopy -I srec -O elf32-big --alt-machine-code 4 $@ $@.elf
endif
	
# compile init_fpga with -mbitfield for testing purposes
$(OBJDIR)/init_fpga.o:	CFLAGS += -mbitfield

# compile printf pc-relative so it can be used as well before and after copy of BaS
$(OBJDIR)/printf.o:	CFLAGS += -mpcrel

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.S
	$(CC) -c $(CFLAGS) -Wa,--bitwise-or $(INCLUDE) $< -o $@

depend: $(ASRCS) $(CSRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -M $(ASRCS) $(CSRCS) > depend
	
ifneq (clean,$(MAKECMDGOALS))
-include depend
endif

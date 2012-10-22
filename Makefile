#
# Makefile for Firebee BaS
#
# requires the GNU toolchain
# either the m68k-atari-mint version which allows creating the binaries
#
# or
#
# the m68k-elf version which also allows to use gdb together with bdm tools for debugging
#

# This Makefile is meant for cross compiling the BaS with Vincent Riviere's cross compilers.
# If you want to compile native on an Atari (you will need at least GCC 4.6.3), set
# TCPREFIX to be empty. 
TCPREFIX=m68k-atari-mint-

CC=$(TCPREFIX)gcc
LD=$(TCPREFIX)ld
CPP=$(TCPREFIX)cpp
OBJCOPY=$(TCPREFIX)objcopy

INCLUDE=-Iinclude
CFLAGS=-mcpu=5474 -Wall -Wno-multichar -Os -fomit-frame-pointer -fno-strict-aliasing

SRCDIR=sources
OBJDIR=objs

MAPFILE=bas.map

# Linker control file. The final $(LDCFILE) is intermediate only (preprocessed  version of $(LDCSRC)
LDCFILE=bas.lk
LDCSRC=bas.lk.in

# this Makefile can create the BaS to flash or an arbitrary ram address (for BDM debugging). See
# below for the definition of TARGET_ADDRESS
FLASH_EXEC=bas.s19
RAM_EXEC=ram.s19

CSRCS= \
	$(SRCDIR)/sysinit.c \
	$(SRCDIR)/init_fpga.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/cache.c \
	$(SRCDIR)/sd_card.c

ASRCS= \
	$(SRCDIR)/mmu.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/ewf.S \
	$(SRCDIR)/illegal_instruction.S 

STRT_SRC = startcf.S
STRT_OBJ = $(OBJDIR)/startcf.o

COBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.S,%.o,$(ASRCS)))

OBJS=$(COBJS) $(AOBJS)
	
.PHONY all: $(FLASH_EXEC)
.PHONY ram: $(RAM_EXEC)
.PHONY clean:
	@ rm -f $(FLASH_EXEC) $(FLASH_EXEC).elf $(RAM_EXEC) $(RAM_EXEC).elf $(STRT_OBJ) $(OBJS)\
			$(MAPFILE) $(LDCFILE) depend 

$(FLASH_EXEC): TARGET_ADDRESS=0xe0000000
$(RAM_EXEC): TARGET_ADDRESS=0x01000000

$(FLASH_EXEC) $(RAM_EXEC): $(STRT_OBJ) $(OBJS)
	$(CPP) -P -DTARGET_ADDRESS=$(TARGET_ADDRESS) $(LDCSRC) -o $(LDCFILE)
	$(LD) --oformat srec -Map $(MAPFILE) --cref -T $(LDCFILE) -s -o $@
	objcopy -I srec -O elf32-big --alt-machine-code 4 $@ $@.elf
	
# compile init_fpga with -mbitfield for testing purposes
$(OBJDIR)/init_fpga.o:	CFLAGS += -mbitfield

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.S
	$(CC) -c $(CFLAGS) -Wa,--bitwise-or $(INCLUDE) $< -o $@

depend: $(ASRCS) $(CSRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -M $(ASRCS) $(CSRCS) > depend
	
ifneq (clean,$(MAKECMDGOALS))
-include depend
endif

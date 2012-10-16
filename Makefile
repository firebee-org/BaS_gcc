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

INCLUDE=-Iinclude
CFLAGS=-mcpu=5474 -Wall -Wno-multichar -Os -fomit-frame-pointer

SRCDIR=sources
OBJDIR=objs

MAPFILE=bas.map

# Linker control files. flash.lk is meant for BaS in flash, ram.lk (not written yet) for
# debugging in RAM.
LDCFILE=flash.lk
# LDCFILE=ram.lk

EXEC=bas.s19

CSRCS= \
	$(SRCDIR)/startcf.c \
	$(SRCDIR)/sysinit.c \
	$(SRCDIR)/init_fpga.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/cache.c \
	$(SRCDIR)/sd_card.c \
	$(SRCDIR)/last.c

ASRCS= \
	$(SRCDIR)/mmu.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/ewf.S \
	$(SRCDIR)/illegal_instruction.S 

COBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,$(OBJDIR)/%.o,$(patsubst %.S,%.o,$(ASRCS)))

OBJS=$(COBJS) $(AOBJS)
	
.PHONY all: $(EXEC)

.PHONY clean:
	@ rm -f $(EXEC) $(OBJS) $(MAPFILE) depend
	
$(EXEC): $(OBJS) $(LDCFILE)
	$(LD) --oformat srec -Map $(MAPFILE) --cref -T flash.lk -s -o $@ 

# compile init_fpga with -mbitfield for testing purposes
$(OBJDIR)/init_fpga.o:	CFLAGS += -mbitfield

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.S
	$(CC) -c $(CFLAGS) -Wa,--bitwise-or $(INCLUDE) $< -o $@

depend: $(ASRCS) $(CSRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -M $(ASRCS) $(CSRCS) > depend
	
include depend
	
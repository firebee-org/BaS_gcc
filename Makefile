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

TCPREFIX=m68k-atari-mint-

CC=$(TCPREFIX)gcc
LD=$(TCPREFIX)ld

INCLUDE=-Iinclude
CFLAGS=-mcfv4e -Wno-multichar -Os -fomit-frame-pointer
#CFLAGS=-mcfv4e -Wno-multichar -S -O3 -fomit-frame-pointer
SRCDIR=sources
OBJDIR=objs

EXEC=bas.hex

CSRCS= \
	$(SRCDIR)/sysinit.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/sd_card.c \
	$(SRCDIR)/last.c

ASRCS= \
	$(SRCDIR)/startcf.S \
	$(SRCDIR)/mmu.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/ewf.S \
	$(SRCDIR)/illegal_instruction.S 

COBJS=$(patsubst $(SRCDIR),$(OBJDIR),$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR),$(OBJDIR),$(patsubst %.S,%.o,$(ASRCS)))

OBJS=$(COBJS) $(SOBJS)

all: $(EXEC)

$(EXEC): $(OBJS)
	echo "generating executable"

clean:
	rm $(EXEC) *.o
	
.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@
	
.S.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

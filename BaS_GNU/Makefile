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
CFLAGS=-mcfv4e -Wno-multichar -O1 -fomit-frame-pointer
#CFLAGS=-mcfv4e -Wno-multichar -S -O3 -fomit-frame-pointer
SRCDIR=sources

EXEC=bas.hex

CSRCS= \
	$(SRCDIR)/sysinit.c \
	$(SRCDIR)/BaS.c \
	$(SRCDIR)/sd_card.c \
	$(SRCDIR)/last.c

ARCS= \
	$(SRCDIR)/startcf.S \
	$(SRCDIR)/mmu.S \
	$(SRCDIR)/exceptions.S \
	$(SRCDIR)/supervisor.S \
	$(SRCDIR)/ewf.S \
	$(SRCDIR)/illegal_instruction.S 

COBJS=$(CSRCS:.c=.o)
AOBJS=$(ASRCS:.S=.o)

OBJS=$(COBJS) $(SOBJS)

all: $(EXEC)

$(EXEC): $(OBJS)
	echo "generating executable"

clean:
	rm $(EXEC) *.o
	
.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $<
	
.S.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

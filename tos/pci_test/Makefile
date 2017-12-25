CROSS=Y

CROSSBINDIR_IS_Y=m68k-atari-mint-
CROSSBINDIR_IS_N=

CROSSBINDIR=$(CROSSBINDIR_IS_$(CROSS))

UNAME := $(shell uname)
ifeq ($(CROSS), Y)
ifeq ($(UNAME),Linux)
PREFIX=m68k-atari-mint
HATARI=hatari
else
PREFIX=m68k-atari-mint
HATARI=/usr/local/bin/hatari
endif
else
PREFIX=/usr
endif

DEPEND=depend
TOPDIR = ../..

#LIBCMINI=$(TOPDIR)/../libcmini/libcmini

#INCLUDE=-I$(LIBCMINI)/include -nostdlib
#LIBS=-lcmini -nostdlib -lgcc
CC=$(PREFIX)/bin/gcc

CC=$(CROSSBINDIR)gcc
STRIP=$(CROSSBINDIR)strip
STACK=$(CROSSBINDIR)stack

APP=pci_test.prg
TEST_APP=$(APP)

CFLAGS=\
	-O2\
	-fomit-frame-pointer \
	-g\
	-Wl,-Map,mapfile\
	-Wl,--defsym -Wl,__MBAR=0xff000000\
	-Wl,--defsym -Wl,__MMUBAR=0xff040000\
	-Wl,--defsym -Wl,__FPGA_JTAG_LOADED=0xff101000\
	-Wl,--defsym -Wl,__FPGA_JTAG_VALID=0xff101004\
	-Wl,--defsym -Wl,__VRAM=0x60000000\
	-Wall

SRCDIR=sources
INCDIR=include
INCLUDE+=-I$(INCDIR)

CSRCS=\
	$(SRCDIR)/pci_test.c

ASRCS=

COBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.S,%.o,$(ASRCS)))
OBJS=$(COBJS) $(AOBJS)

TRGTDIRS=./m5475
OBJDIRS=$(patsubst %,%/objs,$(TRGTDIRS))

#
# multilib flags. These must match m68k-atari-mint-gcc -print-multi-lib output
#
m5475/$(APP):CFLAGS += -mcpu=5475
m5475/mshort/$(APP): CFLAGS += -mcpu=5475 -mshort

all:$(patsubst %,%/$(APP),$(TRGTDIRS))
#
# generate pattern rules for multilib object files.
#
define CC_TEMPLATE
$(1)/objs/%.o:$(SRCDIR)/%.c
	@echo CC $$<
	@$(CC) $$(CFLAGS) $(INCLUDE) -c $$< -o $$@

$(1)/objs/%.o:$(SRCDIR)/%.S
	@echo CC $$<
	@$(CC) $$(CFLAGS) $(INCLUDE) -c $$< -o $$@

$(1)_OBJS=$(patsubst %,$(1)/objs/%,$(OBJS))
$(1)/$(APP): $$($(1)_OBJS)
	@echo CC $$@
	#@$(CC) $$(CFLAGS) -o $$@ $(LIBCMINI)/$(1)/startup.o $$($(1)_OBJS) -L$(LIBCMINI)/$(1) $(LIBS)
	@$(CC) $$(CFLAGS) -o $$@ $$($(1)_OBJS) $(LIBS)
	#@$(STRIP) $$@
endef
$(foreach DIR,$(TRGTDIRS),$(eval $(call CC_TEMPLATE,$(DIR))))

$(DEPEND): $(ASRCS) $(CSRCS)
	@-rm -f $(DEPEND)
	@for d in $(TRGTDIRS);\
		do $(CC) $(CFLAGS) $(INCLUDE) -M $(ASRCS) $(CSRCS) | sed -e "s#^\(.*\).o:#$$d/objs/\1.o:#" >> $(DEPEND); \
    done


clean:
	@rm -f $(patsubst %,%/objs/*.o,$(TRGTDIRS)) $(patsubst %,%/$(APP),$(TRGTDIRS))
	@rm -f $(DEPEND) mapfile

.PHONY: printvars
printvars:
	@$(foreach V,$(.VARIABLES), $(if $(filter-out environment% default automatic, $(origin $V)),$(warning $V=$($V))))

ifneq (clean,$(MAKECMDGOALS))
-include $(DEPEND)
endif

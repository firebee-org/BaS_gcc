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
TOPDIR= ../..

BAS_INCLUDE=-I$(TOPDIR)/../BaS_gcc/include

LIBCMINI=$(TOPDIR)/../libcmini/libcmini

INCLUDE=-I$(LIBCMINI)/include $(BAS_INCLUDE) -nostdlib
LIBS=-lcmini -nostdlib -lgcc
CC=$(PREFIX)/bin/gcc

CC=$(CROSSBINDIR)gcc
STRIP=$(CROSSBINDIR)strip
STACK=$(CROSSBINDIR)stack

APP=bascook.prg
TEST_APP=$(APP)

CFLAGS=\
	-Os\
	-g\
	-Wl,-Map,mapfile\
	-Wall

SRCDIR=sources

CSRCS=\
	$(SRCDIR)/bascook.c
ASRCS=

COBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.c,%.o,$(CSRCS)))
AOBJS=$(patsubst $(SRCDIR)/%.o,%.o,$(patsubst %.S,%.o,$(ASRCS)))
OBJS=$(COBJS) $(AOBJS)

TRGTDIRS=.
OBJDIRS=$(patsubst %,%/objs,$(TRGTDIRS))

#
# multilib flags. These must match m68k-atari-mint-gcc -print-multi-lib output
#
$(APP):CFLAGS += -mcpu=5475

all: $(TEST_APP)

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
	@echo CC $$<
	@$(CC) $$(CFLAGS) -o $$@ $(LIBCMINI)/m5475/startup.o $$($(1)_OBJS) -L$(LIBCMINI)/m5475 $(LIBS)
	@$(STRIP) $$@
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

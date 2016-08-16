TITLE_ID = EBSDMP001
TARGET = ebootSegs

OBJS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

LIBS = -lSceDisplay_stub -lSceGxm_stub -lSceCtrl_stub -lSceSysmodule_stub

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "$(TARGET)" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin $@ \
		--add sce_sys/icon0.png=sce_sys/icon0.png \
		--add sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \

eboot.bin: $(TARGET).velf
	vita-make-fself $< eboot.bin

%.velf: %.elf
	vita-elf-create $< $@

%.elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf *.velf *.elf *.vpk $(OBJS) param.sfo eboot.bin

CC=sh3eb-elf-gcc
OBJCOPY=sh3eb-elf-objcopy
MKG3A=mkg3a
RM=rm
CFLAGS=-m4a-nofpu -mb -flto -O2 -mhitachi -Wall -I../../include -lgcc -L../../lib -fuse-linker-plugin -Wall -Wextra
LDFLAGS=$(CFLAGS) -nostartfiles -T../../toolchain/prizm.x -Wl,-static -Wl,-gc-sections
OBJECTS=src/bitmaps.o src/draw.o src/engine.o src/graphic_functions.o src/intro.o src/key.o src/level.o src/main.o \
src/menu.o src/object.o src/rand.o src/save.o src/text.o src/tileset.o src/overclock.o
PROJ_NAME=GravityDuck
BIN=$(PROJ_NAME).bin
ELF=$(PROJ_NAME).elf
ADDIN=$(PROJ_NAME).g3a
 
all: $(ADDIN)
 
$(ADDIN): $(BIN)
	$(MKG3A) -n :"Gravity Duck" -i uns:unselected.bmp -i sel:selected.bmp $< $@
$(ELF): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(BIN)
.s.o:
	$(CC) -c $(CFLAGS) $< -o $@
 
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
 
clean:
	rm -f $(OBJECTS) $(PROJ_NAME).bin $(PROJ_NAME).elf $(ADDIN)

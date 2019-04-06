# Edit linker script here...
# linker= ../../toolchain/prizm.x
linker	= ../../common/prizm.ld

CFLAGS	= -mb -mhitachi -Wall -Wextra -I ../../include -O3
LDFLAGS	= $(CFLAGS) -T $(linker) -nostartfiles -lgcc  -Wl,-static,-gc-sections\
	  -L ../../lib

SRC	= $(wildcard src/*.[cs])
OBJ	= $(SRC:%=%.o)

NAME	= GravityDuck
BIN	= $(NAME).bin
ADDIN	= $(NAME).g3a

all: $(ADDIN)

$(BIN):	$(OBJ)
	sh3eb-elf-gcc $^ $(LDFLAGS) -o $@
$(ADDIN): $(BIN)
	mkg3a -n :$(NAME) -i uns:unselected.bmp -i sel:selected.bmp $< $@

%.c.o: %.c
	sh3eb-elf-gcc -c $(CFLAGS) $< -o $@
%.s.o: %.s
	sh3eb-elf-as -c $< -o $@

clean:
	rm -f $(OBJ)

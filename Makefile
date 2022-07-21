# VARIABLES

PREFIX := arm-none-eabi

CC := $(PREFIX)-gcc
ASM := $(PREFIX)-gcc
LD := $(PREFIX)-gcc
GDB := $(PREFIX)-gdb
OBJCOPY := $(PREFIX)-objcopy
OBJDUMP := $(PREFIX)-objdump
OPENOCD := openocd

LD_SCRIPT := nrf52.ld

COMMON_FLAGS := -mcpu=cortex-m4 -Os -flto
CC_FLAGS := $(COMMON_FLAGS) -DNRF52832_XXAA -DBOARD_PCA10040 -ffunction-sections -fdata-sections -ffreestanding -Os -Wall -Wextra -MMD -Isrc -g3
ASM_FLAGS := $(CC_FLAGS)
LD_FLAGS := $(COMMON_FLAGS) -static -nostdlib -Wl,--gc-sections -Wl,--strip-debug -T $(LD_SCRIPT)

SRCS := $(shell find src/ -type f -name "*.c")
ASMS := $(shell find src/ -type f -name "*.s")
OBJS := $(SRCS:src/%.c=build/obj/%.o) $(ASMS:src/%.s=build/obj/%.o)
DEP_FILES := $(OBJS:.o=.d)
EXEC := build/blinky



# RULES FOR HUMANS

default: $(EXEC) $(EXEC).bin

flash: $(EXEC)
	$(OPENOCD) -f openocd.cfg -d2 -c "halt" -c "nrf5 mass_erase" -c "program $^ verify" -c "reset" -c "exit"

flash-debug: $(EXEC)
	$(OPENOCD) -f openocd.cfg -d2 -c "halt" -c "nrf5 mass_erase" -c "program $^ verify" -c "reset" -c "halt"

openocd-gdb:
	$(GDB) $(EXEC) -ex "set architecture armv7e-m" -ex "target extended-remote localhost:3333"

disasm:
	$(OBJDUMP) -D $(EXEC) | less

clean:
	rm -r build

.PHONY: default flash flash-debug openocd-gdb disasm clean



# RULES FOR COMPUTERS

build/%.bin: build/%
	$(OBJCOPY) -O binary $< $@

$(EXEC): $(OBJS) $(LD_SCRIPT)
	$(LD) $(LD_FLAGS) $(OBJS) -o $@

build/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@

build/obj/%.o: src/%.s
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(ASM_FLAGS) -c $< -o $@

-include $(DEP_FILES)

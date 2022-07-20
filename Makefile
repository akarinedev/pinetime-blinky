# VARIABLES

PREFIX := arm-none-eabi

CC := $(PREFIX)-gcc
CC_FLAGS := -mcpu=cortex-m4 -DNRF52832_XXAA -DBOARD_PCA10040 -ffunction-sections -fdata-sections -ffreestanding -Os -Wall -Wextra -nostartfiles -MMD -Isrc
CC_REL_FLAGS := $(CC_FLAGS) -DNDEBUG
CC_DEB_FLAGS := $(CC_FLAGS) -g -DNDEBUG

LD := $(PREFIX)-gcc
LD_SCRIPT := nrf52.ld
LD_FLAGS := -mcpu=cortex-m4 -static -nostdlib -Wl,--gc-sections
LD_REL_FLAGS := $(LD_FLAGS) -Wl,--strip-debug -T $(LD_SCRIPT)
LD_DEB_FLAGS := $(LD_FLAGS)


SRCS := $(shell find src/ -type f -name "*.c")
ASMS := $(shell find src/ -type f -name "*.s")
REL_OBJS := $(SRCS:src/%.c=build/release/obj/%.o) $(ASMS:src/%.s=build/release/obj/%.o)
DEB_OBJS := $(SRCS:src/%.c=build/debug/obj/%.o) $(ASMS:src/%.s=build/debug/obj/%.o)
DEP_FILES := $(REL_OBJS:.o=.d) $(DEB_OBJS:.o=.d)

EXEC_DEB := build/debug/blinky
EXEC_REL := build/release/blinky

# Binutils
OBJCOPY := $(PREFIX)-objcopy
OBJDUMP := $(PREFIX)-objdump

# RULES FOR HUMANS

default: release debug

release: $(EXEC_REL) $(EXEC_REL).bin
debug: $(EXEC_DEB) $(EXEC_DEB).bin

flash: $(EXEC_REL).bin
	openocd -f openocd.cfg -d2 -c "halt" -c "nrf5 mass_erase" -c "program $(EXEC_REL) verify" -c "reset"

openocd-gdb:
	$(PREFIX)-gdb $(EXEC_REL) -ex "set architecture armv7e-m" -ex "target extended-remote localhost:3333"

disasm:
	$(OBJDUMP) -b binary -marm -D $(EXEC_REL).bin | less

qemu: $(EXEC_REL).bin
	qemu-system-arm -S -s -M virt -kernel $< -nographic

qemu-gdb:
	$(PREFIX)-gdb $(EXEC_REL) -ex "set architecture armv7e-m" -ex "target remote localhost:1234"

clean:
	rm -r build

.PHONY: default release debug flash qemu qemu-gdb clean

# RULES FOR COMPUTERS

build/%.bin: build/%
	$(OBJCOPY) -O binary $< $@

$(EXEC_REL): $(REL_OBJS) $(LD_SCRIPT)
	$(LD) $(LD_REL_FLAGS) $(REL_OBJS) -o $@

$(EXEC_DEB): $(DEB_OBJS) $(LD_SCRIPT)
	$(LD) $(LD_DEB_FLAGS) $(DEB_OBJS) -o $@

build/release/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_REL_FLAGS) -c $< -o $@

build/debug/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_DEB_FLAGS) -c $< -o $@

build/release/obj/%.o: src/%.s
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_REL_FLAGS) -c $< -o $@

build/debug/obj/%.o: src/%.s
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_DEB_FLAGS) -c $< -o $@

-include $(DEP_FILES)

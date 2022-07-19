PREFIX := arm-none-eabi

CC := $(PREFIX)-gcc
CC_FLAGS := -DNRF52832_XXAA -DBOARD_PCA10040 -ffunction-sections -fdata-sections -Os -Wall -Wextra -nostartfiles -MMD
CC_DEB_FLAGS := $(CC_FLAGS) -g3
CC_REL_FLAGS := $(CC_FLAGS) -DNDEBUG

LD := $(PREFIX)-gcc
LD_FLAGS := -nostartfiles
LD_DEB_FLAGS := $(LD_FLAGS) -g3
LD_REL_FLAGS := $(LD_FLAGS)

LD_SCRIPT := link.ld
LD_FLAGS_FINAL := -T $(LD_SCRIPT) -Wl,--gc-sections

FINAL_OBJECTS := main.o boards.o system_nrf52.o
FINAL_DEB_OBJECTS := ${FINAL_OBJECTS:%=build/debug/obj/%}
FINAL_REL_OBJECTS := ${FINAL_OBJECTS:%=build/release/obj/%}

EXEC_DEB := build/debug/blinky
EXEC_REL := build/release/blinky

OBJCOPY := $(PREFIX)-objcopy

default: $(EXEC_REL).bin $(EXEC_DEB).bin

.PHONY: qemu qemu-gdb clean
qemu: $(EXEC_DEB)
	qemu-system-arm -S -s -M virt -kernel $< -nographic

qemu-gdb:
	arm-none-eabi-gdb $(EXEC_DEB) -ex "target remote localhost:1234"

build/%.bin: build/%
	$(OBJCOPY) -O binary $< $@

$(EXEC_DEB): $(FINAL_DEB_OBJECTS) $(LD_SCRIPT)
	$(LD) $(LD_DEB_FLAGS) $(LD_FLAGS_FINAL) $(FINAL_DEB_OBJECTS) -o $@

$(EXEC_REL): $(FINAL_REL_OBJECTS) $(LD_SCRIPT)
	$(LD) $(LD_REL_FLAGS) $(LD_FLAGS_FINAL) -Wl,--strip-debug $(FINAL_REL_OBJECTS) -o $@

build/debug/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_DEB_FLAGS) -c $< -o $@

build/release/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_REL_FLAGS) -c $< -o $@

-include build/debug/obj/*.d
-include build/release/obj/*.d

clean:
	rm -r build


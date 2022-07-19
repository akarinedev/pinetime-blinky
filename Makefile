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
FINAL_DEB_OBJECTS := ${FINAL_OBJECTS:%=build/obj/debug/%}
FINAL_REL_OBJECTS := ${FINAL_OBJECTS:%=build/obj/release/%}

EXEC_REL := build/blinky-release
EXEC_DEB := build/blinky-debug

OBJCOPY := $(PREFIX)-objcopy

default: $(EXEC_REL).bin $(EXEC_DEB).bin

qemu: $(EXEC_DEB)
	qemu-system-arm -S -s -M virt -kernel $< -nographic

qemu-gdb:
	arm-none-eabi-gdb build/blinky-debug -ex "target remote localhost:1234"

build/%.bin: build/%
	$(OBJCOPY) -O binary $< $@

$(EXEC_DEB): $(FINAL_DEB_OBJECTS) $(LD_SCRIPT)
	$(LD) $(LD_DEB_FLAGS) $(LD_FLAGS_FINAL) $(FINAL_DEB_OBJECTS) -o $@

$(EXEC_REL): $(FINAL_REL_OBJECTS) $(LD_SCRIPT)
	$(LD) $(LD_REL_FLAGS) $(LD_FLAGS_FINAL) -Wl,--strip-debug -DNDEBUG $(FINAL_REL_OBJECTS) -o $@

build/obj/debug/%.o: src/%.c | build/obj/debug
	$(CC) $(CC_DEB_FLAGS) -c $< -o $@

build/obj/release/%.o: src/%.c | build/obj/release
	$(CC) $(CC_REL_FLAGS) -c $< -o $@

build/obj/debug: | build/obj
	mkdir build/obj/debug
build/obj/release: | build/obj
	mkdir build/obj/release
build/obj: | build
	mkdir build/obj
build:
	mkdir build/

-include build/obj/debug/*.d
-include build/obj/release/*.d

clean:
	rm -r build


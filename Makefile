# VARIABLES

PREFIX := arm-none-eabi

CC := $(PREFIX)-gcc
CC_FLAGS := -DNRF52832_XXAA -DBOARD_PCA10040 -ffunction-sections -fdata-sections -Os -Wall -Wextra -nostartfiles -MMD -Isrc
CC_REL_FLAGS := $(CC_FLAGS) -DNDEBUG
CC_DEB_FLAGS := $(CC_FLAGS) -g3

LD := $(PREFIX)-gcc
LD_FLAGS := -nostartfiles
LD_REL_FLAGS := $(LD_FLAGS)
LD_DEB_FLAGS := $(LD_FLAGS) -g3

LD_SCRIPT := link.ld
LD_FLAGS_FINAL := -T $(LD_SCRIPT) -Wl,--gc-sections

SRCS := $(shell find src/ -type f -name "*.c")
REL_OBJS := $(SRCS:src/%.c=build/release/obj/%.o)
DEB_OBJS := $(SRCS:src/%.c=build/debug/obj/%.o)
DEP_FILES := $(REL_OBJS:.o=.d) $(DEB_OBJS:.o=.d)

EXEC_DEB := build/debug/blinky
EXEC_REL := build/release/blinky

OBJCOPY := $(PREFIX)-objcopy

# RULES FOR HUMANS

default: release debug

release: $(EXEC_REL) $(EXEC_REL).bin
debug: $(EXEC_DEB) $(EXEC_DEB).bin

qemu: $(EXEC_DEB)
	qemu-system-arm -S -s -M virt -kernel $< -nographic

qemu-gdb:
	arm-none-eabi-gdb $(EXEC_DEB) -ex "target remote localhost:1234"

clean:
	rm -r build

.PHONY: default release debug qemu qemu-gdb clean

# RULES FOR COMPUTERS

build/%.bin: build/%
	$(OBJCOPY) -O binary $< $@

$(EXEC_REL): $(REL_OBJS) $(LD_SCRIPT)
	$(LD) $(LD_REL_FLAGS) $(LD_FLAGS_FINAL) -Wl,--strip-debug $(REL_OBJS) -o $@

$(EXEC_DEB): $(DEB_OBJS) $(LD_SCRIPT)
	$(LD) $(LD_DEB_FLAGS) $(LD_FLAGS_FINAL) $(DEB_OBJS) -o $@

build/release/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_REL_FLAGS) -c $< -o $@

build/debug/obj/%.o: src/%.c
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(CC) $(CC_DEB_FLAGS) -c $< -o $@

-include $(DEP_FILES)

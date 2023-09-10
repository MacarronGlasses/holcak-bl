AS:=nasm
CC:=i686-elf-gcc
LD:=i686-elf-ld
OBJCOPY:=i686-elf-objcopy

STAGE2:=$(subst source/stage2/,build/stage2/,$(addsuffix .o,$(shell find source/stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
STAGE3:=$(subst source/stage3/,build/stage3/,$(addsuffix .o,$(shell find source/stage3/ -type f \( -name '*.c' -o -name '*.asm' \))))
DRIVER:=$(subst source/driver/,build/driver/,$(addsuffix .o,$(shell find source/driver/ -type f \( -name '*.c' -o -name '*.asm' \))))
STDLIB:=$(subst source/stdlib/,build/stdlib/,$(addsuffix .o,$(shell find source/stdlib/ -type f \( -name '*.c' -o -name '*.asm' \))))

GENERATE:=$(addprefix source/,driver/isrg.h driver/isrg.c)

.PHONY: always clean start debug
always: ${GENERATE} build/build.img

build/build.img: build/stage1/build.bin build/stage2/build.bin tools/install.py
	dd if=/dev/zero of=$@ bs=512 count=65536
	./tools/install.py $@ build/stage1/build.bin build/stage2/build.bin
	./script/fdisk.sh $(abspath $@)
	mkfs.fat -F 16 $@ -n FAT16 --offset 2048
	mcopy -i build/build.img@@1M script/debug.gdb "::test.txt"

build/stage1/build.bin: $(shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \))
	mkdir -p ${@D}
	${AS} -fbin -I source/stage1/ -o $@ $(filter %.asm,$^)

build/stage2/build.bin: ${STAGE2} ${STAGE3} ${DRIVER} ${STDLIB}
	@mkdir -p ${@D}
	${LD} -T source/linker.ld --gc-sections -Map build/stage2/build.map -nostdlib -melf_i386 -o build/stage2/build.elf $(filter %.o,$^)
	${OBJCOPY} -O binary --strip-debug build/stage2/build.elf $@

ifeq (1,$(shell if [ -d build/stage2/ ]; then echo 1; fi))
    -include $(shell find build/stage2/ -type f -name '*.d')
endif

build/stage2/%.c.o: source/stage2/%.c
	@mkdir -p ${@D}
	${CC} -I ~/toolchain/gcc-build/gcc/include/ -I source/ -I source/stdlib/ -Wall -Wextra -pedantic -std=c17 -ggdb -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m16 -o $@ $<

build/stage2/%.asm.o: source/stage2/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -g -felf32 -Fdwarf -MD $(addsuffix .d,$(basename $@)) -o $@ $<

ifeq (1,$(shell if [ -d build/stage3/ ]; then echo 1; fi))
    -include $(shell find build/stage3/ -type f -name '*.d')
endif

build/stage3/%.c.o: source/stage3/%.c
	@mkdir -p ${@D}
	${CC} -I ~/toolchain/gcc-build/gcc/include/ -I source/ -I source/stdlib/ -Wall -Wextra -pedantic -std=c17 -ggdb -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stage3/%.asm.o: source/stage3/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -I source/stage3/ -I source/driver/ -g -felf32 -Fdwarf -MD $(addsuffix .d,$(basename $@)) -o $@ $<

ifeq (1,$(shell if [ -d build/driver/ ]; then echo 1; fi))
    -include $(shell find build/driver/ -type f -name '*.d')
endif

build/driver/%.c.o: source/driver/%.c
	@mkdir -p ${@D}
	${CC} -I ~/toolchain/gcc-build/gcc/include/ -I source/ -I source/stdlib/ -Wall -Wextra -pedantic -std=c17 -ggdb -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/driver/%.asm.o: source/driver/%.asm
	@mkdir -p ${@D}
	${AS} -I source/driver/ -g -felf32 -Fdwarf -MD $(addsuffix .d,$(basename $@)) -o $@ $<

ifeq (1,$(shell if [ -d build/stdlib/ ]; then echo 1; fi))
    -include $(shell find build/stdlib/ -type f -name '*.d')
endif

build/stdlib/%.c.o: source/stdlib/%.c
	@mkdir -p ${@D}
	${CC} -I ~/toolchain/gcc-build/gcc/include/ -I source/ -I source/stdlib/ -Wall -Wextra -pedantic -std=c17 -ggdb -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stdlib/%.asm.o: source/stdlib/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stdlib/ -g -felf32 -Fdwarf -MD $(addsuffix .d,$(basename $@)) -o $@ $<

${GENERATE}: tools/generate.py
	./$< $(abspath source/)

start: build/build.img
#	qemu-system-i386 -M q35 -m 256M -drive format=raw,file=build/build.img,media=disk -boot c
	qemu-system-i386 -M pc -m 256M -drive format=raw,file=build/build.img,media=disk -boot c
#	qemu-system-i386 -M pc -m 256M -drive format=raw,if=floppy,file=build/build.img

debug: build/build.img
	qemu-system-i386 -M q35 -m 256M -drive format=raw,file=build/build.img -boot c -s -S &
	gdb -x script/debug.gdb

clean:
	rm -rf build/

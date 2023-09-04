AS:=nasm
CC:=i686-elf-gcc
LD:=i686-elf-ld
OBJCOPY:=i686-elf-objcopy

.PHONY: clean start

build/build.img: build/stage1/build.bin build/stage2/build.bin tools/install.py
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1/build.bin of=$@ conv=notrunc
	./tools/install.py $@ build/stage1/build.bin build/stage2/build.bin

build/stage1/build.bin: $(shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \))
	mkdir -p ${@D}
	${AS} -fbin -I source/stage1/ -o $@ $(filter %.asm,$^)

build/stage2/build.bin: $(subst source/stage2/,build/stage2/,$(addsuffix .o,$(shell find source/stage2/ -type f \( -name '*.c' -o -name '*.asm' \)))) $(subst source/stage3/,build/stage3/,$(addsuffix .o,$(shell find source/stage3/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T source/linker.ld --gc-sections -Map build/stage2/build.map -nostdlib -melf_i386 -o build/stage2/build.elf $(filter %.o,$^)
	${OBJCOPY} -O binary --strip-debug build/stage2/build.elf $@

ifeq (1,$(shell if [ -d build/stage2/ ]; then echo 1; fi))
    -include $(shell find build/stage2/ -type f -name '*.d')
endif

build/stage2/%.c.o: source/stage2/%.c
	@mkdir -p ${@D}
	${CC} -I ~/toolchain/gcc-build/gcc/include/ -I source/global/ -Wall -Wextra -pedantic -std=c17 -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m16 -o $@ $<	

build/stage2/%.asm.o: source/stage2/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

build/stage3/%.c.o: source/stage3/%.c
	@mkdir -p ${@D}
	${CC} -I ~/toolchain/gcc-build/gcc/include/ -I source/global/ -Wall -Wextra -pedantic -std=c17 -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stage3/%.asm.o: source/stage3/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -I source/stage3/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

start: build/build.img
	qemu-system-i386 -M q35 -m 256M -drive format=raw,file=build/build.img -boot c
#	qemu-system-i386 -M q35 -m 256M -drive format=raw,if=floppy,file=build/build.img

clean:
	rm -rf build/

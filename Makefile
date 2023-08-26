AS:=nasm
CC:=i686-elf-gcc
LD:=i686-elf-ld
OBJCOPY:=i686-elf-objcopy

.PHONY: clean

build/build.img: build/stage1/build.bin build/stage2/build.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1/build.bin of=$@ conv=notrunc

build/stage1/build.bin: $(shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \))
	mkdir -p ${@D}
	${AS} -fbin -I source/stage1/ -o $@ $(filter %.asm,$^)

build/stage2/build.bin: ${DRIVERS} ${LIBRARY} $(subst source/stage2/,build/stage2/,$(addsuffix .o,$(shell find source/stage2/ -type f \( -name '*.c' -o -name '*.asm' \))))
	@mkdir -p ${@D}
	${LD} -T source/linker.ld --gc-sections -Map build/stage2/build.map -nostdlib -melf_i386 -o build/stage2/build.elf $(filter %.o,$^)
	${OBJCOPY} -O binary build/stage2/build.elf $@

ifeq (1,$(shell if [ -d build/stage2/ ]; then echo 1; fi))
    -include $(shell find build/stage2/ -type f -name '*.d')
endif

build/stage2/%.c.o: source/stage2/%.c
	@mkdir -p ${@D}
	${CC} -Wall -Wextra -pedantic -std=c17 -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -o $@ $<

build/stage2/%.asm.o: source/stage2/%.asm
	@mkdir -p ${@D}
	${AS} -I source/stage2/ -felf32 -MD $(addsuffix .d,$(basename $@)) -o $@ $<

clean:
	rm -rf build/

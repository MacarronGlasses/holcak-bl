# Toolchain configuration
TOOLCHAIN_VERSION_BINUTILS:=2.41
TOOLCHAIN_VERSION_GCC:=13.2.0

TOOLCHAIN_BUILD:=./.toolchain/${TARGET}/bin/${TARGET}
TOOLCHAIN_BUILD_LIBRARY:=.toolchain/i686-elf/lib/gcc/i686-elf/${TOOLCHAIN_VERSION_GCC}/include/
TOOLCHAIN_BUILD_DEPENDS:=.toolchain/${TARGET}/bin/${TARGET}-as .toolchain/${TARGET}/bin/${TARGET}-gcc
TOOLCHAIN_BUILD_THREADS:=8

# Processed configuration
PROCESSED_FILES:=source/driver/isrg.h source/driver/isrg.c
PROCESSED_FILES_SCRIPT:=script/process.py

# Target platform has to be known by setting the TARGET variable!
$(if ${TARGET},,$(error Error: Select target platform using the TARGET variable!))

# Compiles the whole folder recursively
# Parameters:
#   $1    - Source folder name
define compile
$(shell echo $1 | tr a-z A-Z):=$(subst source/$1/,build/$1/,$(addsuffix .o,$(shell find source/$1/ -type f \( -name '*.c' -o -name '*.asm' \))))

build/$1/%.c.o: source/$1/%.c
	@mkdir -p $${@D}
	gcc -Wall -Wextra -pedantic -std=c17 -ggdb -ffreestanding -nostartfiles -MMD -MP -fno-pie -fno-pic -nostdlib -nostdinc -c -m32 -I ${TOOLCHAIN_BUILD_LIBRARY} -I source/ -I source/stdlib/ -o $$@ $$<

build/$1/%.asm.o: source/$1/%.asm
	@mkdir -p $${@D}
	nasm -felf32 -g -Fdwarf -I source/$1/ -MP -MF $(addsuffix .d,$(basename $$@)) -o $$@ $$<

ifeq (1,$(shell if [ -d build/stage2/ ]; then echo 1; fi))
    -include $$(shell find build/stage2/ -type f -name '*.d')
endif
endef

.PHONY: build clean
build: ${TOOLCHAIN_BUILD_DEPENDS} ${PROCESSED_FILES} build/stage1/build.bin build/stage2/build.bin

# Creates rules for compiling all the required folders
$(eval $(call compile,stage2))
$(eval $(call compile,stage3))
$(eval $(call compile,driver))
$(eval $(call compile,stdlib))

build/stage1/build.bin: $(shell find source/stage1/ -type f \( -name '*.asm' -o -name '*.inc' \))
	@mkdir -p ${@D}
	nasm -fbin -I source/stage1/ -l ${@D}/build.lst -o $@ $(filter %.asm,$^)

build/stage2/build.bin: ${STAGE2} ${STAGE3} ${DRIVER} ${STDLIB}
	@mkdir -p ${@D}
	${TOOLCHAIN_BUILD}-ld -T source/linker.ld --gc-sections -Map ${@D}/build.map -nostdlib -melf_i386 -o ${@D}/build.elf $(filter %.o,$^)
	${TOOLCHAIN_BUILD}-objcopy -O binary --strip-debug ${@D}/build.elf $@

.toolchain/${TARGET}/bin/${TARGET}-as: .toolchain/.source/binutils/
	mkdir -p .toolchain/${TARGET}/.build/binutils/
	(cd .toolchain/${TARGET}/.build/binutils/ && ../../../.source/binutils/configure --target="${TARGET}" --prefix="$(shell pwd)/.toolchain/${TARGET}/" --with-sysroot --disable-nls --disable-werror && make -j ${TOOLCHAIN_BUILD_THREADS} && make install)

.toolchain/.source/binutils/: .toolchain/.source/binutils.tar.gz
	mkdir -p $@
	tar -xf $^ -C $@ --strip-components 1

.toolchain/.source/binutils.tar.gz:
	mkdir -p ${@D}
	wget -O $@ https://ftp.gnu.org/gnu/binutils/binutils-${TOOLCHAIN_VERSION_BINUTILS}.tar.gz

.toolchain/${TARGET}/bin/${TARGET}-gcc: .toolchain/.source/gcc/
	mkdir -p .toolchain/${TARGET}/.build/gcc/
	(cd .toolchain/${TARGET}/.build/gcc/ && ../../../.source/gcc/configure --target="${TARGET}" --prefix="$(shell pwd)/.toolchain/${TARGET}/" --disable-nls --enable-languages=c --without-headers --disable-werror && make all-gcc -j ${TOOLCHAIN_BUILD_THREADS} && make all-target-libgcc -j ${TOOLCHAIN_BUILD_THREADS} && make install-gcc && make install-target-libgcc)

.toolchain/.source/gcc/: .toolchain/.source/gcc.tar.gz
	mkdir -p $@
	tar -xf $^ -C $@ --strip-components 1

.toolchain/.source/gcc.tar.gz:
	mkdir -p ${@D}
	wget -O $@ https://ftp.gnu.org/gnu/gcc/gcc-${TOOLCHAIN_VERSION_GCC}/gcc-${TOOLCHAIN_VERSION_GCC}.tar.gz

${PROCESSED_FILES}: script/process.py
	./script/process.py $(abspath source/)

clean:
	rm -rf build/

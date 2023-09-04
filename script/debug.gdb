set disassembly-flavor intel
symbol-file build/stage2/build.elf
set tdesc filename script/target.xml
target remote localhost:1234
layout asm
layout regs

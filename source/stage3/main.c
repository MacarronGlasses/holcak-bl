#include <driver/idt.h>
#include <driver/isr.h>
#include <driver/pic.h>
#include <global.h>
#include "printf.h"
#include "info.h"

__cdecl void main(void) {
	printf("Hello, World!\n");
}

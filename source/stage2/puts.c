#include <stdint.h>
#include <stdarg.h>
#include "puts.h"

void putc(char chr) {
	__asm__ volatile ("push %%ax; mov $0x0E, %%ah; mov %0, %%al; int $0x10; pop %%ax" : : "r" (chr));
}

void puts(const char *str) {
	for (; *str; putc(*str++));
}

void putx(size_t val) {
	puts("0x");
	for (uint8_t i = sizeof(size_t) * 2; i > 0; i--) {
		char chr = ((val >> ((i - 1) * 4)) & 0x0F) + '0';
		putc(chr > '9' ? chr + 0x07 : chr);
	}
}

void printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	for (; *fmt; fmt++) {
		if (*fmt == '%') {
			switch (*++fmt) {
			case 'c':
				putc(va_arg(args, int));
				break;
			case 's':
				puts(va_arg(args, char*));
				break;
			case 'x':
			case 'p':
				putx(va_arg(args, size_t));
				break;
			case '%':
				putc('%');
				break;
			default:
				va_end(args);
				return;
			}
		} else {
			putc(*fmt);
		}
	}
	va_end(args);
}

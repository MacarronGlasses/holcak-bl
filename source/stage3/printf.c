#include <driver/vga.h>
#include <stdint.h>
#include "printf.h"

void printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

static void putc(char chr, uint16_t *cursor) {
	if (chr == '\n') {
		*cursor = (*cursor + VGA_COLS - 1) / VGA_COLS * VGA_COLS;
	}
	if (*cursor == VGA_ROWS * VGA_COLS) {
		*cursor = VGA_ROWS * VGA_COLS - VGA_COLS;
		vga_scroll(1);
	}
	if (chr != '\n') {
		VGA[*cursor] = (uint16_t)chr | VGA_FG(VGA_LIGHT_GREY) | VGA_BG(VGA_BLACK);
		*cursor += 1;
	}
}

static void puts(char *str, uint16_t *cursor) {
	for (; *str; putc(*str++, cursor));
}

static void putx(uint64_t value, uint8_t bytes, uint16_t *cursor) {
	puts("0x", cursor);
	for (; bytes > 0; bytes--) {
		char chr = ((value >> ((bytes - 1) << 2)) & 0x0F) + '0';
		putc(chr > '9' ? chr + 0x07 : chr, cursor);
	}
}

void vprintf(const char *fmt, va_list args) {
	uint16_t cursor = vga_cursor_get();
	for (; *fmt; fmt++) {
		if (*fmt == '%') {
			uint8_t length = 2;
		format:
			switch (*++fmt) {
				case 'c': {
					putc(va_arg(args, int), &cursor);
				} break;
				case 's': {
					puts(va_arg(args, char*), &cursor);
				} break;
				case 'h': {
					length--;
				} goto format;
				case 'l': {
					length++;
				} goto format;
				case 'x': {
					uint64_t value = length >= 3 ? va_arg(args, uint64_t) : va_arg(args, uint32_t);
					putx(value, 2 << length, &cursor);
				} break;
				case 'p': {
					putx(va_arg(args, uintptr_t), 8, &cursor);
				} break;
				case '%': {
					putc('%', &cursor);
				} break;
			}
		} else {
			putc(*fmt, &cursor);
		}
	}
	VGA[cursor] = VGA_FG(VGA_LIGHT_GREY) | VGA_BG(VGA_BLACK);
	vga_cursor_set(cursor);
}

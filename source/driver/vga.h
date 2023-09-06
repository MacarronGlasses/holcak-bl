#ifndef DRIVER_VGA_H_
#define DRIVER_VGA_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#define VGA ((uint16_t*)0xB8000)
#define VGA_ROWS (25)
#define VGA_COLS (80)

typedef enum {
	VGA_BLACK,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHT_GREY,
	VGA_DARK_GREY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_RED,
	VGA_LIGHT_MAGENTA,
	VGA_YELLOW,
	VGA_WHITE,
} vga_color_t;

#define VGA_RC(Row, Col) (((uint16_t)(Col)) + ((uint16_t)(Row) * VGA_COLS))
#define VGA_FG(S) ((S) << 0x08)
#define VGA_BG(S) ((S) << 0x0C)

void vga_clear(void);
void vga_scroll(uint8_t rows);
void vga_cursor_show(uint8_t shape);
void vga_cursor_hide(void);
void vga_cursor_set(uint16_t cursor);
uint16_t vga_cursor_get(void);

#endif//DRIVER_VGA_H_

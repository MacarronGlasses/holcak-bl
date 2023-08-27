#ifndef STAGE2_PUTS_H_
#define STAGE2_PUTS_H_
#include <stddef.h>

void putc(char chr);
void puts(const char *str);
void putx(size_t val);
void printf(const char *fmt, ...);

#endif//STAGE2_PUTS_H_

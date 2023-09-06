#ifndef STAGE3_PRINTF_H_
#define STAGE3_PRINTF_H_
#include <stdarg.h>

void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list args);

#endif//STAGE3_PRINTF_H_

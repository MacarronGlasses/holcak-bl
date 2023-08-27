#ifndef STAGE2_CPUID_H_
#define STAGE2_CPUID_H_
#include <stdbool.h>
#include <limits.h>
#include <cpuid.h>

typedef struct {
	uint32_t levels;
	char vendor[13];
	uint32_t features[4];
} cpuid_info_t;

cpuid_info_t cpuid_init(void);

#endif//STAGE2_CPUID_H_

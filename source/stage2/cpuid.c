#include <define.h>
#include <stdint.h>
#include <stddef.h>
#include "cpuid.h"

/*
 * References:
 *   https://www.felixcloutier.com/x86/cpuid
 *   https://en.wikipedia.org/wiki/CPUID
 */ 
cpuid_info_t cpuid_init(void) {
	cpuid_info_t info = (cpuid_info_t){0};
	info.levels = __get_cpuid_max(0x80000000, NULL);
	if (info.levels > 0x00) {
		info.vendor[12] = '\0';
		__get_cpuid(0x00, (unsigned int*)NULL, (unsigned int*)&info.vendor[0], (unsigned int*)&info.vendor[8], (unsigned int*)&info.vendor[4]);
		__get_cpuid(0x01, (unsigned int*)&info.features[0], (unsigned int*)&info.features[1], (unsigned int*)&info.features[2], (unsigned int*)&info.features[3]);
	}
	return info;
}

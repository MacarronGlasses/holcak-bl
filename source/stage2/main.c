#include <stdint.h>
#include <define.h>
#include "cpuid.h"
#include "puts.h"

__cdecl void start_16(uint32_t disk) {
	// TODO: `cpuid_init` should just return bool if cpuid exist!
	cpuid_info_t cpuid = cpuid_init();
	if (cpuid.levels > 0x00) {
		printf("CPUID detected:\n\r");
		printf("    Vendor:     %s\n\r", cpuid.vendor);
		printf("    Features A: %x\n\r", cpuid.features[0]);
		printf("    Features B: %x\n\r", cpuid.features[1]);
		printf("    Features C: %x\n\r", cpuid.features[2]);
		printf("    Features D: %x\n\r", cpuid.features[3]);
	} else {
		printf("CPUID not supported!\n\r");
	}
}

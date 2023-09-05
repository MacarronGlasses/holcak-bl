#include <define.h>
#include "info.h"

__cdecl void main(void) {
	*((char*)0xB8000) = 'A';
}

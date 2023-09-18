#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdint.h>

#ifndef _MSC_VER
#define __align(B) __attiribute__((aligned(B)))
#else //_MSC_VER
#define __align(B) __declspec(align(B))
#endif//_MSC_VER

#define __packed __attribute__((packed))
#define __cdecl  __attribute__((cdecl))

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

#endif//GLOBAL_H_

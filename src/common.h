#ifndef COMMON_H
#define COMMON_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
#ifdef _MSC_VER
typedef signed _int64 s64;
#else
typedef signed long long int s64;
#endif

#define GTA_ASIZE(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#endif // COMMON_H

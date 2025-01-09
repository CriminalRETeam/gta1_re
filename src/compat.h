#ifndef COMPAT_H
#define COMPAT_H

#if _MSC_VER < 1500
#ifdef _M_IX86
typedef unsigned int uintptr_t;
#else
#error "Unknown architecture"
#endif

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif

#endif

#endif // COMPAT_H

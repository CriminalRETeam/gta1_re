#ifndef COMPAT_H
#define COMPAT_H

#ifdef _WIN32
#define PSEP "\\"
#else
#define PSEP "/"
#endif

#if _MSC_VER < 1500
#ifdef _M_IX86
typedef unsigned int uintptr_t;
#else
#include <stdint.h>
#endif

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif

#ifdef _WIN32
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#endif

#endif // COMPAT_H

#ifndef DECOMP_H
#define DECOMP_H

#include <stdlib.h>

#ifdef MGL_ON_SDL3
#include <SDL3/SDL.h>
#define TRIGGER_BREAKPOINT() SDL_TriggerBreakpoint()
#else
#define TRIGGER_BREAKPOINT() abort()
#endif


typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

#define NOT_IMPLEMENTED() \
	do {							\
        TRIGGER_BREAKPOINT();       \
        abort();                    \
    } while (0)

#endif // DECOMP_H

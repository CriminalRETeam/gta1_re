#ifndef DECOMP_H
#define DECOMP_H

#include <SDL3/SDL.h>

typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

#define NOT_IMPLEMENTED() do {      \
        SDL_TriggerBreakpoint();    \
        abort();                    \
    } while (0)

#endif // DECOMP_H

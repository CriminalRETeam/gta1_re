#include "math_lut.h"

#include <math.h>

#include "compat.h"

// GLOBAL: GTA 0x00511a28
u32 g_Fixed_tan_table[256];

// GLOBAL: GTA 0x00511e28
u32 g_Fixed_sin_table[1024 + 256];


// FUNCTION: GTA 0x00430400
void InitFixedSinTanTables() {

    {
        for (int i = 0; i < GTA_ASIZE(g_Fixed_sin_table); i++) {
            g_Fixed_sin_table[i] = (s32)(sinf(i * 2 * M_PI / 1024.f) * (float)(1 << 16));
        }
    }

    {
        for (int i = 0; i < GTA_ASIZE(g_Fixed_tan_table); i++) {
            g_Fixed_tan_table[i] = (s32)(tanf(i * 2 * M_PI / 1024.f) * (float)(1 << 16));
        }
    }
}

// FUNCTION: GTA 0x00430490
float FixedToFloat(u32 fixed) {
    return (float)fixed * (1 / (float)(1 << 16));
}

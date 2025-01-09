#include "map.h"
#include "common.h"

#include <windows.h>

// GLOBAL: GTA 0x005c1c24
u8 g_Map_name_priority;

// GLOBAL: GTA 0x005bfbf0
BOOL g_Map_name_specified;

// GLOBAL: GTA 0x007750c4
u8 g_Style_priority;

// GLOBAL: GTA 0x007752d8
BOOL g_Style_index;

// GLOBAL: GTA 0x005c1c34
char g_Map_name[20];

// GLOBAL: GTA 0x007750c0
void *g_Style_memory_1;

// GLBOAL: GTA 0x00775530
void *g_Style_memory_2;

// GLOBAL: GTA 0x00775304
void *g_Style_memory_3;

// GLOBAL: GTA 0x0077552c
void *g_Style_memory_4;

// GLOBAL: GTA 0x00775538
BOOL g_Style_memory_1_allocated;

// GLOBAL: GTA 0x0077553c
BOOL g_Style_memory_2_allocated;

// GLOBAL: GTA 0x00775540
BOOL g_Style_memory_3_allocated;

// GLOBAL: GTA 0x00775544
bool g_Style_memory_4_allocated;

// GLOBAL: GTA 0x005c2c48
void *g_Sector_data;

// GLOBAL: GTA 0x005c2c74
bool g_Sector_data_allocated;


// FUNCTION: GTA 0x00438190
void ResetMapName(void) {
    g_Map_name_priority = 0;
    g_Map_name_specified = 0;
    return;
}

// FUNCTION: GTA 0x004381c0
void SetMapNameWithPriority(const char *track_filename, u8 priority) {

    if (track_filename != NULL && priority >= g_Map_name_priority) {
        strcpy(g_Map_name, track_filename);
        g_Map_name_priority = priority;
        g_Map_name_specified = 1;
    }
}

// FUNCTION: GTA 0x0047cee0
void SetStyleWithPriority(int style, u8 priority) {
    if (priority >= g_Style_priority && style != 0) {
        g_Style_index = style;
        g_Style_priority = priority;
    }
}

// FUNCTION: GTA 0x0047d3c0
void FreeStyleMemory() {
    if (g_Style_memory_1_allocated) {
        free(g_Style_memory_1);
        g_Style_memory_1_allocated = FALSE;
    }
    if (g_Style_memory_2_allocated) {
        free(g_Style_memory_2);
        g_Style_memory_2_allocated = FALSE;
    }
    if (g_Style_memory_3_allocated) {
        free(g_Style_memory_3);
        g_Style_memory_3_allocated = FALSE;
    }
    if (g_Style_memory_4_allocated) {
        free(g_Style_memory_4);
        g_Style_memory_4_allocated = false;
    }
}

void FreeMapSectors() {
    if (g_Sector_data_allocated) {
        free(g_Sector_data);
        g_Sector_data_allocated = false;
    }
}

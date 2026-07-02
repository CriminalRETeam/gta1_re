#ifndef MAP_H
#define MAP_H

#include "common.h"

extern void ResetMapName();

extern void SetMapNameWithPriority(const char *track_filename, u8 priority);

extern void SetStyleWithPriority(int style, u8 priority);

extern void FreeStyleMemory();

extern void FreeMapSectors();

#endif // MAP_H

#ifndef MAP_H
#define MAP_H

#include "common.h"

void ResetMapName(void);

void SetMapNameWithPriority(const char *track_filename, u8 priority);

void SetStyleWithPriority(int style, u8 priority);

void FreeStyleMemory();

void FreeMapSectors();

#endif // MAP_H

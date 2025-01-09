#ifndef MISSION_H
#define MISSION_H

#include "decomp.h"

#define DEBUGPRINT_NOARG()  reinterpret_cast<void*(*)()>(DebugPrint)

struct tMission_info {
    char name[81];
    undefined field_0x51;
    undefined field_0x52;
    undefined field_0x53;
    int field_0x54;
    char filename[13];
    undefined field_0x65;
    undefined field_0x66;
    undefined field_0x67;
    int style;
    char *content;
    char *content_pos_ptr;
    bool content_allocated;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
};

void DebugPrint(const char *message);

void SetCurrentMission(int mission);

void LoadMission();

void FreeMission();

#endif // MISSION_H

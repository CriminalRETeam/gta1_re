#include "mission.h"
#include "map.h"
#include "error.h"
#include "util.h"

#include <stdio.h>

// GLOBAL: GTA 0x006b3e28
int g_Current_mission_index;

// GLOBAL: GTA 0x006b3e30
tMission_info g_Mission_info;


// FUNCTION: GTA 0x0044ab40
void DebugPrint(const char *message) {

}

// FUNCTION: GTA 0x0044ab50
void SetCurrentMission(int mission) {
    if (mission >= 0 && mission <= 2000) {
        g_Current_mission_index = mission;
    } else {
        FatalError(eFatalError_actual_mission_D_not_available, 99, mission);
    }
}

// FUNCTION: GTA 0x0044ab90
void LoadMission() {
    static char buffer[16];
    if (!g_Mission_info.content_allocated) {
        g_Mission_info.content = (char *)CheckedMalloc(0x37001); // FIXME: magic value
        g_Mission_info.content_allocated = true;
    }
    if (g_Current_mission_index == 0) {
        DebugPrint(NULL);
        strcpy(g_Mission_info.name, "default mission");
        g_Mission_info.field_0x54 = 0;
        strcpy(g_Mission_info.filename, "level001.cmp");
        g_Mission_info.style = 0;
        g_Mission_info.content[0] = '\0';
    } else {
        char section_header[12];

        OpenTextFile("..\\gtadata\\mission.ini");
        sprintf(section_header, "[%d]", g_Current_mission_index);
        SkipUntilStringDelim(section_header);
        ReadCopyUntilDelim(g_Mission_info.name, sizeof(g_Mission_info.name), ',');
        g_Mission_info.field_0x54 = ReadInteger();
        ReadCopyUntilDelim(g_Mission_info.filename, sizeof(g_Mission_info.filename), ',');
        g_Mission_info.style = ReadInteger();
        ReadCopyUntilDelim(g_Mission_info.content, 0x37001, '['); // FIXME: magic value
        CloseTextFile();
        DebugPrint((const char *)g_Mission_info.field_0x54);
    }
    SetMapNameWithPriority(g_Mission_info. filename, 1);
    SetStyleWithPriority(g_Mission_info.style, 2);
    g_Mission_info.content_pos_ptr = g_Mission_info.content;
}

// FUNCTION: GTA 0x0044acc0
void FreeMission() {
    if (g_Mission_info.content_allocated) {
        free(g_Mission_info.content);
        g_Mission_info.content_allocated = false;
    }
}

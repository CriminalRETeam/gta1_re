#ifndef CITY_H
#define CITY_H

#include "common.h"
#include "decomp.h"

struct tCity {
    const char *name;
    u8 count_missions;
    u8 mission_save_indexes[4];
    undefined field_0x9;
    u16 missions[4];
    undefined field_0x12[2];
    undefined field_0x14[4];
    const char *mission_names[4];
    s8 index_last_mission;
    undefined field_0x29[3];
};

extern tCity g_Cities_singleplayer[3];

extern tCity g_Cities_multiplayer[3];

extern tCity *g_Current_cities;

extern s16 g_Current_cities_max_index;


void UpdateAvailableCitiesCount();

#endif //  CITY_H

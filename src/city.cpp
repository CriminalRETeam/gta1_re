#include "city.h"
#include "game.h"
#include "menu.h"
#include "save.h"

#include <stddef.h>

// GLOBAL: GTA 0x004af4a0
tCity g_Cities_singleplayer[3] = {
    {
        NULL,
        2,
        { 0, 1, 0, 0 },
        0,
        { 1, 2, 0, 0 },
        { 0, 0 },
        { 0, 0, 0, 0 },
        { NULL, NULL, NULL, NULL },
        -1,
        { 0, 0, 0 },
    },
    {
        NULL,
        2,
        { 2, 3, 0, 0 },
        0,
        { 102, 103, 0, 0 },
        { 0, 0 },
        { 0, 0, 0, 0 },
        { NULL, NULL, NULL, NULL },
        -1,
        { 0, 0, 0 },
    },
    {
        NULL,
        2,
        { 4, 5, 0, 0 },
        0,
        { 202, 203, 0, 0 },
        { 0, 0 },
        { 0, 0, 0, 0 },
        { NULL, NULL, NULL, NULL },
        -1,
        { 0, 0, 0 },
    },
};

// GLOBAL: GTA 0x004af418
tCity g_Cities_multiplayer[3] = {
    {
        NULL,
        4,
        { 0, 1, 2, 3 },
        0,
        { 1001, 1002, 1003, 1004 },
        { 0, 1 },
        { 1, 1, 0, 0 },
        { NULL, NULL, NULL, NULL },
        3,
        { 0, 0, 0 },
    },
    {
        NULL,
        4,
        { 4, 5, 6, 7 },
        0,
        { 1101, 1102, 1103, 1104 },
        { 0, 1 },
        { 1, 1, 0, 0 },
        { NULL, NULL, NULL, NULL },
        3,
        { 0, 0, 0 },
    },
    {
        NULL,
        4,
        { 8, 9, 10, 11 },
        0,
        { 1201, 1202, 1203, 1204 },
        { 0, 1 },
        { 1, 1, 0, 0 },
        { NULL, NULL, NULL, NULL },
        3,
        { 0, 0, 0 },
    },
};

// GLOBAL: GTA 0x00511084
tCity *g_Current_cities;

// GLOBAL: GTA 0x005101b8
s16 g_Current_cities_max_index;

// FUNCTION: GTA 0x00427630
void UpdateAvailableCitiesCount() {
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        g_Current_cities = g_Cities_singleplayer;
        g_Current_cities_max_index = -1;
        for (int i = 0; i < GTA_ASIZE(g_Cities_singleplayer); i++) {
            for (int j = g_Cities_singleplayer[i].count_missions - 1; j >= 0; j--) {
                if (g_Save_state.profiles[g_Save_state.active_profile_index].scores[g_Current_cities[i].mission_save_indexes[j]] != -1) {
                    g_Current_cities_max_index = i;
                    g_Current_cities->index_last_mission = j;
                    break;
                }
            }
        }
        if (g_Demo_version) {
            g_Current_cities_max_index = 0;
            g_Current_cities[0].index_last_mission = 0;
        }
    } else {
        g_Current_cities = g_Cities_multiplayer;
        g_Current_cities_max_index = 2;
    }

}

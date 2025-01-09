#include "save.h"
#include "error.h"
#include "game.h"
#include "text.h"

#include <stdio.h>

// GLOBAL: GTA 0x00510298
tSave_state g_Save_state;

// GLOBAL: GTA 0x00510298
tDefault_profile g_Default_profiles[8] = {
    { "..\\gtadata\\f_play1", "Ulrika", },
    { "..\\gtadata\\f_play2", "Travis", },
    { "..\\gtadata\\f_play3", "Katie", },
    { "..\\gtadata\\f_play4", "Mikki", },
    { "..\\gtadata\\f_play5", "Divine", },
    { "..\\gtadata\\f_play6", "Bubba", },
    { "..\\gtadata\\f_play7", "Troy", },
    { "..\\gtadata\\f_play8", "Kivlov", },
};

static bool TryLoadSaveGame() {
    FILE *f = fopen("..\\gtadata\\player_a.dat", "rb");
    if (f == NULL) {
        return false;
    }
    bool success = fread(&g_Save_state, 1, sizeof(g_Save_state), f) == sizeof(g_Save_state);
    fclose(f);
    return success;
}

void InitializeSaveState() {
    if (!TryLoadSaveGame()) {
        g_Save_state.active_profile_index = 1;
        g_Save_state.music_mode = g_Demo_version != 0;
        g_Save_state.sound_volume = 4;
        g_Save_state.music_volume = 6;
        g_Save_state.text_speed = eTextSpeed_normal;
        g_Save_state.field_0x4 = 0;
        g_Save_state.field_0x5 = 0;
        g_Save_state.transparency_effects = eTransparencyeffects_on;
        g_Save_state.multiplayer_target = eMultiplayerGameType_money;
        g_Save_state.multiplayer_money_target = 100000;
        g_Save_state.multiplayer_kill_target = 10;
        g_Save_state.language_id = -1;
        for (int i = 0; i < GTA_ASIZE(g_Save_state.profiles); i++) {
            strcpy(g_Save_state.profiles[i].name, g_Default_profiles[i].name);
            g_Save_state.profiles[i].scores[0] = 0;
            g_Save_state.profiles[i].selected_city[0] = 0;
            g_Save_state.profiles[i].selected_mission[0] = 0;
            g_Save_state.profiles[i].selected_city[1] = 0;
            g_Save_state.profiles[i].selected_mission[1] = 0;
            g_Save_state.profiles[i].scores[1] = -1;
            g_Save_state.profiles[i].scores[2] = -1;
            g_Save_state.profiles[i].scores[3] = -1;
            g_Save_state.profiles[i].scores[4] = -1;
            g_Save_state.profiles[i].scores[5] = -1;
            for (int j = 0; j < GTA_ASIZE(g_Save_state.profiles[i].missions_finished); j++) {
                g_Save_state.profiles[i].missions_finished[j] = FALSE;
            }
        }
        sprintf(g_Save_state.session_name, "GTA Game");
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == NULL) {
            FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
        }
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
        }
        fclose(f);
    }
    if (g_Save_state.language_id != -1) {
        InitLanguageFilePath(static_cast<tLanguage>(g_Save_state.language_id));
    }
}

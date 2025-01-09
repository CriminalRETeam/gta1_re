#ifndef SAVE_H
#define SAVE_H

#include "common.h"
#include "decomp.h"

enum tText_Speed {
    eTextSpeed_slow = 1,
    eTextSpeed_normal = 2,
    eTextSpeed_fast = 3
};

enum tMusic_mode {
    eMusicMode_radio = 0,
    eMusicMode_constant = 1
};

enum tTransparency_effects {
    eTransparencyEffects_off = 0,
    eTransparencyeffects_on = 1
};

enum tMultiplayer_game_type {
    eMultiplayerGameType_money = 0,
    eMultiplayerGameType_frags = 1,
    eMultiplayerGameType_race = 2,
    eSinglePlayerGameType = 255
};

struct tSave_state {
    u8 sound_volume;
    u8 music_volume;
    u8 text_speed;
    u8 music_mode;
    u8 field_0x4;
    u8 field_0x5;
    u8 field_0x6;
    u8 transparency_effects;
    u8 multiplayer_target;
    undefined field_0x9[3];
    s32 multiplayer_money_target;
    undefined4 multiplayer_kill_target;
    s8 language_id;
    undefined field_0x15[3];
    struct {
        s32 score;
        char name[16];
    } highscores[6][3];
    struct {
        char name[12]; /* maybe size 16? */
        undefined field_0xc[4];
        s32 scores[6];
        s32 selected_city[2]; /* index of selected city in city select screen ([0]: singleplayer, [1]: multiplayer) */
        s32 selected_mission[2]; /* index of selected mission in city select screen ([0]: singleplayer, [1]: multiplayer) */
        s32 missions_finished[6];
    } profiles[8];
    s8 active_profile_index; /* idx in player selection menu */
    char session_name[19];
};

struct tDefault_profile {
    const char *profile_pic_path;
    const char *name;
};

extern tSave_state g_Save_state;

extern tDefault_profile g_Default_profiles[8];


void InitializeSaveState();

#endif // SAVE_H

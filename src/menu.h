#ifndef MENU_H
#define MENU_H

#include "graphics.h"

enum tMenu_HostOrClient {
    eMenuMP_singleplayer = 0,
    eMenuMP_multiplayer_host = 1,
    eMenuMP_multiplayer_client = 2,
};

typedef u32 tMenu_action;
enum {
    eMenuAction_Up = 0x1,
    eMenuAction_Down = 0x2,
    eMenuAction_Left = 0x4,
    eMenuAction_Right = 0x8,
    eMenuAction_Enter = 0x10,
    eMenuAction_Escape = 0x20,
    eMenuAction_Char = 0x40,
    eMenuAction_Backspace = 0x80,
    eMenuAction_Shift = 0x100,
    eMenuAction_Space = 0x200,
};

enum tMenu_index {
    eMenu_connection_select = 0,
    eMenu_cutscene = 1,
    eMenu_error = 2,
    eMenu_host_wait_for_players = 3,
    eMenu_cd_check = 4,
    eMenu_client_wait_for_players = 5,
    eMenu_loading = 6,
    eMenu_main_menu = 7,
    eMenu_city_select = 8,
    eMenu_options = 9,
    eMenu_exit_credits = 10,
    eMenu_profile_select = 11,
    eMenu_profile_name_edit = 12,
    eMenu_post_game_summary = 13,
    eMenu_profile_reset = 14,
    eMenu_multiplayer_client = 15,
    eMenu_multiplayer_new_host_session = 16,
    eMenu_connection_failure = 17,
    eMenu_multiplayer_options = 18,
    eMenu_connection_version_mismatch_failure = 19,
    eMenu_svga_error = 20,
};

enum tMenu_result {
    eMenuRC_continue = 0,
    eMenuRC_game_singleplayer = 1,
    eMenuRC_game_multiplayer_host = 2,
    eMenuRC_game_multiplayer_client = 3,
    eMenuRC_exit = 4,
};

enum tEndOfRace_result{
    eEndOfRace_success = 1,
    eEndOfRace_failed = 2,
    eEndOfRace_dead = 3,
    eEndOfRace_arrest = 4,
    eEndOfRace_timeout = 5,
    eEndOfRace_timeover = 6,
    eEndOfRace_7 = 7,
    eEndOfRace_score = 8,
    eEndOfRace_demo_9 = 9,
    eEndOfRace_cannon = 10,
    eEndOfRace_demo = 11,
};

enum tPost_cutscene_menu {
    ePostCutscene_city_select = 0,
    ePostCutscene_game_summary = 1,
};

enum tNetwork_waiting_room_state {
    eWaiting_room_WaitingOnLetsGo = 0,
    eWaiting_room_NoLetsGoSent = 1,
    eWaiting_room_Unknown_2 = 2,
    eWaiting_room_LetsGoSentOrReceived = 3,
    eWaiting_room_NetworkError = 4,
    eWaiting_room_Canceled = 5,
    eWaiting_room_VersionMismatch = 6,
};

extern int g_Menu_bytes_per_pixel;

extern tEndOfRace_result g_End_of_game_result;

extern tMenu_HostOrClient g_Menu_host_or_client;

extern tPixelmap g_Menu_upper;

extern tPixelmap g_Menu_logos[10];

extern tPixelmap g_Menu_lowers[2];

extern tRect g_Menu_viewport;;


void InitPlayerData();

void InitMenus(tMenu_index menu);

void UnloadMenuLogos();

tMenu_result DoMenuStep(tMenu_action action);

#endif // MENU_H

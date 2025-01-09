#include "main.h"
#include "cutscene.h"
#include "error.h"
#include "font.h"
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "interface.h"
#include "map.h"
#include "menu.h"
#include "math_lut.h"
#include "mission.h"
#include "multiplayer.h"
#include "sound.h"
#include "text.h"

#include <stdio.h>

#ifdef _MSC_VER
#include <new.h>
#endif
#include <sys/types.h>
#include <sys/timeb.h>

#include <windows.h>

// GLOBAL: GTA 0x0054f294
HANDLE g_GTA_mutex;

// GLOBAL: GTA 0x0054f298
BOOL g_BOOL_0054f298;

// GLOBAL: GTA 0x005031d8
BOOL g_noMenus;

enum {
    eModKey_LShift = 0x1,
    eModKey_RShift = 0x2,
};

// GLOBAL: GTA 0x004a8b78
char g_Oem_keys_to_ascii[84] = {
    '\0',   '\0',
    '1',    '2',    '3',    '4',    '5',    '6',    '7',    '8',    '9',    '0',    '\0',   '\0',   '\0',   '\0',
    'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',    'o',    'p',    '[',    ']',    '\0',   '\0',
    'a',    's',    'd',    'f',    'g',    'h',    'j',    'k',    'l',    '\0',   '\0',   '\0',   '\0',   '\0',
    'z',    'x',    'c',    'v',    'b',    'n',    'm',
};

// FUNCTION: GTA 0x00437230
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#ifdef _MSC_VER
    _set_new_handler(OnOutOfMemory);
#endif
    tMenu_result menu_rc = eMenuRC_continue;
    g_GTA_mutex = CreateMutexA(NULL, TRUE, "Grand Theft Auto");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 0;
    }
    InitGraphDriver(&hInstance);
    InitError();
    g_BOOL_0054f298 = FALSE;
    ConfigureGameOptions(
        TRUE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        TRUE,
        TRUE,
        TRUE,
        TRUE,
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        FALSE);
    ResetMapName();
    SetMapNameWithPriority(NULL, 2);
    SetCurrentMission(0);
    DEBUGPRINT_NOARG();
    SetReplayPath("replay.rep");
    LoadControlsFromRegister();
    SetLanguageDataPath();
    InitFixedSinTanTables();
    ProcessEvents();
    InitMultiplayer();
    InitInput();
    DEBUGPRINT_NOARG();
    if (g_noMenus) {
        InitInterfaceStrings();
    } else {
        InitPlayerData();
    }
    InitDirectionTexts();
    SetMultiplayerProfileName(0, "Player", 0);
    if (g_noMenus) {
        LoadKanjiFont();
    } else {
        PlayDMAIntro();
        InitMenus(eMenu_cd_check);
    }
    do {
        u32 mod_keys = 0;
        ProcessEvents();
        if (!g_noMenus) {
            do {
                struct _timeb timeb;
                _ftime(&timeb);
                unsigned short start = timeb.millitm;
                u32 key_event = KeyboardEvent();
                tMenu_action action = 0;
                switch (key_event) {
                case 0x1:
                    action = eMenuAction_Escape;
                    break;
                case 0xe:
                case 0x53:
                case 0x153:
                    action = eMenuAction_Backspace;
                    break;
                case 0x1c:
                    action = eMenuAction_Enter;
                    break;
                case 0x148:
                    action = eMenuAction_Up;
                    break;
                case 0x14b:
                    action = eMenuAction_Left;
                    break;
                case 0x14d:
                    action = eMenuAction_Right;
                    break;
                case 0x150:
                    action = eMenuAction_Down;
                    break;
                case 0x39:
                    action = eMenuAction_Space;
                    break;
                case 0xaa:
                    mod_keys &= ~eModKey_LShift;
                    break;
                case 0x2a:
                    mod_keys |= eModKey_LShift;
                    break;
                case 0x36:
                    mod_keys |= eModKey_RShift;
                    break;
                case 0xb6:
                    mod_keys &= ~eModKey_RShift;
                    break;
                default:
                    if (key_event < 0x54 && g_Oem_keys_to_ascii[key_event]) {
                        action = (g_Oem_keys_to_ascii[key_event] << 16) | eMenuAction_Char;
                    }
                    break;
                }
                if (mod_keys) {
                    action |= eMenuAction_Shift;
                }
                menu_rc = DoMenuStep(action);
                _ftime(&timeb);
                int delta_time_ms = timeb.millitm - start;
                if (delta_time_ms < 35) {
                    if (delta_time_ms < 0) {
                        delta_time_ms = 0;
                    }
                    Sleep(35 - delta_time_ms);
                }
            } while (menu_rc == eMenuRC_continue);
            if (!g_noMenus) {
                UnloadMenuLogos();
            }
        }
        if (menu_rc == eMenuRC_exit) {
            break;
        }
        tGame_result game_rc;
        do {
            if (menu_rc == eMenuRC_game_multiplayer_client) {
                LoadMission();
            } else if (menu_rc != eMenuRC_game_multiplayer_host) {
                SetNotConnectedState();
                InitMultiplayerPlayerCounts();
            }
            game_rc = DoGame();
        } while (game_rc == eGameRC_restart);
        if (!g_noMenus) {
            tMenu_index next_menu;
            if (game_rc == eGameRC_connection_failure) {
                next_menu = eMenu_connection_failure;
            } else if (game_rc == eGameRC_svga_error) {
                next_menu = eMenu_svga_error;
            } else if (g_BOOL_005031f4) {
                g_BOOL_005031f4 = false;
                g_BOOL_005031f0 = FALSE;
                next_menu = eMenu_main_menu;
            } else {
                next_menu = eMenu_cutscene;
            }
            InitMenus(next_menu);
        }
    } while (g_noMenus != 1);
    QuitError();
    FreeGameFonts();
    FreeMission();
    FreeStyleMemory();
    FreeMapSectors();
    StopAndReleaseSpeedLimiter();
    DestroyMultiplayer();
    DEBUGPRINT_NOARG();
    StopAudio();
    FreeMilesBuffers();
    UnloadKanjiFont();
    FlushReplayBuffer();
    FreeLanguageData();
    ReleaseMutex(g_GTA_mutex);
    DEBUGPRINT_NOARG();
    FreeLanguageData();
    QuitGraphics();
    return 0;
}

// Function: GTA 0x004376a0
int OnOutOfMemory(unsigned int count) {
    if (count != 0) {
        FatalError(eFatalError_out_of_memory_allocating_D_bytes, 474, count);
    }
    return 0;
}

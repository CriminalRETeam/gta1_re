#ifndef GAME_H
#define GAME_H

#include "common.h"

#include <windows.h>

enum {
    eScancode_Esc = 0x01,
    eScancode_Tab = 0x0f,
    eScancode_Enter = 0x1c,
    eScancode_LCtrl = 0x1d,
    eScancode_LShift = 0x2a,
    eScancode_LAlt = 0x38,
    eScancode_Space = 0x39,
    eScancode_CapsLock = 0x3a,
    eScancode_KP7 = 0x47,
    eScancode_KP8 = 0x48,
    eScancode_KP9 = 0x49,
    eScancode_KP_MIN = 0x4a,
    eScancode_KP4 = 0x4b,
    eScancode_KP5 = 0x4c,
    eScancode_KP6 = 0x4d,
    eScancode_KP_PLUS = 0x4e,
    eScancode_KP1 = 0x4f,
    eScancode_KP2 = 0x50,
    eScancode_KP3 = 0x51,
    eScancode_KP0 = 0x52,
    eScancode_KP_DOT = 0x53,
};

enum tGame_result {
    eGameRC_continue = 0,
    eGameRC_restart = 3,
    eGameRC_connection_failure = 4,
    eGameRC_svga_error = 5,
};

extern BOOL g_BOOL_00502f6c;

extern BOOL g_BOOL_00502f58;

extern BOOL g_BOOL_00503180;

extern BOOL g_BOOL_00502f70;

extern BOOL g_BOOL_005031cc;

extern BOOL g_BOOL_005031c8;

extern BOOL g_BOOL_0050319c;

extern BOOL g_BOOL_00502f68;

extern BOOL g_BOOL_00502f40;

extern BOOL g_BOOL_005031a4;

extern BOOL g_BOOL_00502f50;

extern BOOL g_BOOL_005031a0;

extern BOOL g_BOOL_00502f64;

extern BOOL g_BOOL_00502f48;

extern BOOL g_BOOL_005031f0;

extern BOOL g_BOOL_00503190;

extern BOOL g_BOOL_005031b8;

extern BOOL g_BOOL_005031dc;

extern BOOL g_BOOL_00502f54;

extern BOOL g_BOOL_00502f44;

extern BOOL g_BOOL_00502f3c;

extern BOOL g_BOOL_005031e0;

extern BOOL g_Multiplayer_available;

extern BOOL g_BOOL_00503178;

extern BOOL g_BOOL_00502f60;

extern BOOL g_BOOL_00503188;

extern BOOL g_BOOL_00502f38;

extern BOOL g_BOOL_005031d0;

extern BOOL g_Debug_mode;

extern BOOL g_BOOL_005031e8;

extern BOOL g_BOOL_00502f4c;

extern BOOL g_Transparency_effects;

extern bool g_BOOL_005031e6;

extern bool g_No_police;

extern bool g_Unlimited_lives;

extern bool g_BOOL_0050317c;

extern bool g_BOOL_005031c4;

extern bool g_BOOL_00502f74;

extern bool g_BOOL_0050318c;

extern bool g_BOOL_005031e5;

extern bool g_Demo_version;

extern bool g_Enable_music;

extern bool g_All_weapons;

void ConfigureGameOptions(
        BOOL param_1,  BOOL param_2,  BOOL param_3,  BOOL param_4,  BOOL param_5,  BOOL param_6,
        BOOL param_7,  BOOL param_8,  BOOL param_9,  BOOL param_10, BOOL param_11, BOOL param_12,
        BOOL param_13, BOOL param_14, BOOL param_15, BOOL param_16, BOOL param_17, BOOL param_18,
        BOOL param_19, BOOL param_20, BOOL param_21, BOOL param_22, BOOL param_23, BOOL param_24,
        BOOL param_25, BOOL param_26, BOOL param_27, BOOL param_28, BOOL param_29, BOOL param_30,
        BOOL param_31, BOOL param_32, int param_33,  bool param_34, bool param_35, bool param_36,
        bool param_37, bool param_38, bool param_39, bool param_40, bool param_41, bool demo_version,
        bool param_43);

void ProcessEvents();

// x07f: scancode
// 0x080: key up
// 0x100: navigation key (KP0,1,2,3,4,6,7,8,9,0,.)
u32 KeyboardEvent();

tGame_result DoGame();

void SetTextSpeed(int text_speed);

#endif // GAME_H

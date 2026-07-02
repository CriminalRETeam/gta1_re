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

extern b32 g_BOOL_00502f6c;

extern b32 g_BOOL_00502f58;

extern b32 g_BOOL_00503180;

extern b32 g_BOOL_00502f70;

extern b32 g_BOOL_005031cc;

extern b32 g_BOOL_005031c8;

extern b32 g_BOOL_0050319c;

extern b32 g_BOOL_00502f68;

extern b32 g_BOOL_00502f40;

extern b32 g_BOOL_005031a4;

extern b32 g_BOOL_00502f50;

extern b32 g_BOOL_005031a0;

extern b32 g_BOOL_00502f64;

extern b32 g_BOOL_00502f48;

extern b32 g_BOOL_005031f0;

extern b32 g_BOOL_00503190;

extern b32 g_BOOL_005031b8;

extern b32 g_BOOL_005031dc;

extern b32 g_BOOL_00502f54;

extern b32 g_BOOL_00502f44;

extern b32 g_BOOL_00502f3c;

extern b32 g_BOOL_005031e0;

extern b32 g_Multiplayer_available;

extern b32 g_BOOL_00503178;

extern b32 g_BOOL_00502f60;

extern b32 g_BOOL_00503188;

extern b32 g_BOOL_00502f38;

extern b32 g_BOOL_005031d0;

extern b32 g_Debug_mode;

extern b32 g_BOOL_005031e8;

extern b32 g_BOOL_00502f4c;

extern b32 g_Transparency_effects;

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

extern void ConfigureGameOptions(
        b32 param_1,  b32 param_2,  b32 param_3,  b32 param_4,  b32 param_5,  b32 param_6,
        b32 param_7,  b32 param_8,  b32 param_9,  b32 param_10, b32 param_11, b32 param_12,
        b32 param_13, b32 param_14, b32 param_15, b32 param_16, b32 param_17, b32 param_18,
        b32 param_19, b32 param_20, b32 param_21, b32 param_22, b32 param_23, b32 param_24,
        b32 param_25, b32 param_26, b32 param_27, b32 param_28, b32 param_29, b32 param_30,
        b32 param_31, b32 param_32, int param_33,  bool param_34, bool param_35, bool param_36,
        bool param_37, bool param_38, bool param_39, bool param_40, bool param_41, bool demo_version,
        bool param_43);

extern void ProcessEvents();

// x07f: scancode
// 0x080: key up
// 0x100: navigation key (KP0,1,2,3,4,6,7,8,9,0,.)
extern u32 KeyboardEvent();

extern tGame_result DoGame();

extern void SetTextSpeed(int text_speed);

#endif // GAME_H

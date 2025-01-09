#include "game.h"
#include "error.h"
#include "main.h"
#include "save.h"

#include "decomp.h"

#include <mgraph.h>

// GLOBAL: GTA 0x00502f6c
BOOL g_BOOL_00502f6c;

// GLOBAL: GTA 0x00502f58
BOOL g_BOOL_00502f58;

// GLOBAL: GTA 0x00503180
BOOL g_BOOL_00503180;

// GLOBAL: GTA 0x00502f70
BOOL g_BOOL_00502f70;

// GLOBAL: GTA 0x005031cc
BOOL g_BOOL_005031cc;

// GLOBAL: GTA 0x005031c8
BOOL g_BOOL_005031c8;

// GLOBAL: GTA 0x0050319c
BOOL g_BOOL_0050319c;

// GLOBAL: GTA 0x00502f68
BOOL g_BOOL_00502f68;

// GLOBAL: GTA 0x00502f40
BOOL g_BOOL_00502f40;

// GLOBAL: GTA 0x005031a4
BOOL g_BOOL_005031a4;

// GLOBAL: GTA 0x00502f50
BOOL g_BOOL_00502f50;

// GLOBAL: GTA 0x005031a0
BOOL g_BOOL_005031a0;

// GLOBAL: GTA 0x00502f64
BOOL g_BOOL_00502f64;

// GLOBAL: GTA 0x00502f48
BOOL g_BOOL_00502f48;

// GLOBAL: GTA 0x005031f0
BOOL g_BOOL_005031f0;

// GLOBAL: GTA 0x00503190
BOOL g_BOOL_00503190;

// GLOBAL: GTA 0x005031b8
BOOL g_BOOL_005031b8;

// GLOBAL: GTA 0x005031dc
BOOL g_BOOL_005031dc;

// GLOBAL: GTA 0x00502f54
BOOL g_BOOL_00502f54;

// GLOBAL: GTA 0x00502f44
BOOL g_BOOL_00502f44;

// GLOBAL: GTA 0x00502f3c
BOOL g_BOOL_00502f3c;

// GLOBAL: GTA 0x005031e0
BOOL g_BOOL_005031e0;

// GLOBAL: GTA 0x00502f5c
BOOL g_Multiplayer_available;

// GLOBAL: GTA 0x00503178
BOOL g_BOOL_00503178;

// GLOBAL: GTA 0x00502f60
BOOL g_BOOL_00502f60;

// GLOBAL: GTA 0x00503188
BOOL g_BOOL_00503188;

// GLOBAL: GTA 0x00502f38
BOOL g_BOOL_00502f38;

// GLOBAL: GTA 0x005031d0
BOOL g_BOOL_005031d0;

// GLOBAL: GTA 0x00503194
BOOL g_Debug_mode;

// GLOBAL: GTA 0x005031e8
BOOL g_BOOL_005031e8;

// GLOBAL: GTA 0x00502f4c
BOOL g_BOOL_00502f4c;

// GLOBAL: GTA 0x005031e4
BOOL g_Transparency_effects;

// GLOBAL: GTA 0x005031e6
bool g_BOOL_005031e6;

// GLOBAL: GTA 0x005031ec
bool g_No_police;

// GLOBAL: GTA 0x00502f35
bool g_Unlimited_lives;

// GLOBAL: GTA 0x0050317c
bool g_BOOL_0050317c;

// GLOBAL: GTA 0x005031c4
bool g_BOOL_005031c4;

// GLOBAL: GTA 0x00502f74
bool g_BOOL_00502f74;

// GLOBAL: GTA 0x0050318c
bool g_BOOL_0050318c;

// GLOBAL: GTA 0x005031e5
bool g_BOOL_005031e5;

// GLOBAL: GTA 0x005031bc
bool g_Demo_version;

// GLOBAL: GTA 0x0050317d
bool g_Enable_music;

// GLOBAL: GTA 0x00503198
bool g_All_weapons;

// GLOBAL: GTA 0x004b3610
int g_INT_004b3610 = 2;

// GLOBAL: GTA 0x004b3614
int g_INT_004b3614 = 2;


// FUNCTION: GTA 0x005031d8
void ConfigureGameOptions(
        BOOL param_1,  BOOL param_2,  BOOL param_3,  BOOL param_4,  BOOL param_5,  BOOL param_6,
        BOOL param_7,  BOOL param_8,  BOOL param_9,  BOOL param_10, BOOL param_11, BOOL param_12,
        BOOL param_13, BOOL param_14, BOOL param_15, BOOL param_16, BOOL param_17, BOOL param_18,
        BOOL param_19, BOOL param_20, BOOL param_21, BOOL param_22, BOOL param_23, BOOL param_24,
        BOOL param_25, BOOL param_26, BOOL param_27, BOOL param_28, BOOL param_29, BOOL param_30,
        BOOL param_31, BOOL param_32, int param_33,  bool param_34, bool param_35, bool param_36,
        bool param_37, bool param_38, bool param_39, bool param_40, bool param_41, bool demo_version,
        bool param_43) {
    g_BOOL_00502f6c = param_4;
    g_BOOL_00502f58 = param_9;
    g_BOOL_00503180 = param_3;
    g_BOOL_00502f70 = param_12;
    g_BOOL_005031cc = param_6;
    g_noMenus = param_14;
    g_BOOL_005031c8 = param_13;
    g_BOOL_0050319c = param_15;
    g_BOOL_00502f68 = param_2;
    g_BOOL_00502f40 = param_1;
    g_BOOL_005031a4 = param_8;
    g_BOOL_00502f50 = param_5;
    g_BOOL_005031a0 = param_11;
    g_BOOL_00502f64 = param_10;
    g_BOOL_00502f48 = param_7;
    g_BOOL_005031f0 = param_16;
    g_BOOL_00503190 = param_17;
    g_BOOL_005031b8 = param_18;
    g_BOOL_005031dc = param_19;
    g_BOOL_00502f54 = param_20;
    g_BOOL_00502f44 = param_21;
    g_BOOL_00502f3c = param_22;
    g_BOOL_005031e0 = param_23;
    g_Multiplayer_available = param_24;
    g_BOOL_00503178 = param_26;
    g_BOOL_00502f60 = param_27;
    g_BOOL_00503188 = param_28;
    g_BOOL_00502f38 = param_29;
    g_BOOL_005031d0 = param_30;
    g_Debug_mode = param_25;
    g_BOOL_005031e8 = param_31;
    g_BOOL_00502f4c = param_32;
    g_Transparency_effects = param_33;
    g_BOOL_005031e6 = (bool)param_34;
    g_No_police = (bool)param_35;
    g_Unlimited_lives = (bool)param_36;
    g_BOOL_0050317c = (bool)param_37;
    g_BOOL_005031c4 = (bool)param_38;
    g_BOOL_00502f74 = (bool)param_39;
    g_BOOL_0050318c = (bool)param_40;
    g_BOOL_005031e5 = (bool)param_41;
    g_Demo_version = demo_version;
    g_Enable_music = param_43;
}

// FUNCTION: GTA 0x00414a70
void ProcessEvents() {
    EVT_flush(EVT_EVERYEVT);
}

// FUNCTION: GTA 0x00414a80
u32 KeyboardEvent() {
    event_t event;
    for (;;) {
        EVT_getNext(&event, EVT_EVERYEVT);
        if (event.what == EVT_KEYDOWN || event.what == EVT_KEYUP) {
            break;
        }
        if (event.what == EVT_NULLEVT) {
            return 0;
        }
    }
    u32 key_event = EVT_scanCode(event.message);
    switch (key_event) {
    case eScancode_KP7:
    case eScancode_KP8:
    case eScancode_KP9:
    case eScancode_KP4:
    case eScancode_KP6:
    case eScancode_KP1:
    case eScancode_KP2:
    case eScancode_KP3:
    case eScancode_KP0:
    case eScancode_KP_DOT:
        if (EVT_asciiCode(event.message) == '\0') {
            key_event += 0x100;
        }
        break;
    }
    if (event.what == EVT_KEYUP) {
        key_event += 0x80;
    }
    return key_event;
}

// STUB: GTA 0x004148a0
tGame_result DoGame() {
    // FIXME
    NOT_IMPLEMENTED();
    return eGameRC_connection_failure;
}

// FUNCTION: GTA 0x00482140
void SetTextSpeed(int text_speed) {
    switch (text_speed) {
    case 0:
        g_INT_004b3610 = 0;
        g_INT_004b3614 = 1;
        break;
    case eTextSpeed_slow:
        g_INT_004b3610 = 1;
        g_INT_004b3614 = 1;
        break;
    case eTextSpeed_normal:
        g_INT_004b3610 = 2;
        g_INT_004b3614 = 2;
        break;
    case eTextSpeed_fast:
        g_INT_004b3610 = 4;
        g_INT_004b3614 = 3;
        break;
    default:
        FatalError(eFatalError_invalid_case, 200, text_speed);
        break;
    }
}

#include "input.h"
#include "common.h"
#include "decomp.h"
#include "error.h"
#include "game.h"
#include "mission.h"
#include "text.h"
#include "util.h"

#include <stdio.h>
#include <string.h>

#include <windows.h>


// GLOBAL: GTA 0x005134e8
char g_Replay_path[256];

// GLOBAL: GTA 0x005135e8
u32 g_Replay_default_mask;

// GLOBAL: GTA 0x004b2f90
const char *g_Register_control_names[10] = {
        "Control 0",
        "Control 1",
        "Control 2",
        "Control 3",
        "Control 4",
        "Control 5",
        "Control 6",
        "Control 7",
        "Control 8",
        "Control 9",
};

// GLOBAL: GTA 0x00753784
undefined4 g_Controls_from_register[10];

// GLOBAL: GTA 0x004a8c58
int g_Register_control_swizzler[10] = {
        0, 1, 2, 3, 4, 6, 5, 8, 9, 7
};

// GLOBAL: GTA 0x00753780
char g_Language_id_suffix[2];


// GLOBAL: GTA 0x00513660
tJoystickThing g_JoystickCaps;

// GLOBAL: GTA 0x004b0bbc
const u8 g_Joystick_capability_flags[4] = {1, 2, 4, 8};

// GLOBAL: GTA 0x00513620
undefined4 g_Input_scancodes[10];

// GLOBAL: GTA 0x00513654
BOOL g_Input_with_joystick;

// GLOBAL: GTA 0x005134e0
BOOL g_Input_with_keyboard;

// GLOBAL: GTA 0x005031f4
bool g_BOOL_005031f4;

// GLOBAL: GTA 0x004b0bac
tReplayBuffer_state g_Replay_buffer_state = eReplayBuffer_synchronized;

// GLOBAL: GTA 0x005134dc
int g_Replay_buffer_pos;

// GLOBAL: GTA 0x00513648
tReplay_item *g_Replay_buffer;


// FUNCTION: GTA 0x00432c50
void SetReplayPath(const char *path) {
    sprintf(g_Replay_path, "..\\gtadata\\%s", path);
    g_Replay_default_mask = (strcmp("replay.rep", path) == 0) - 1;
}

// FUNCTION: GTA 0x00433b20
void InitJoystickCapsAtIndex(int index, int avg, int min, int max) {
    g_JoystickCaps.inputs[index].avg_flags = min;
    g_JoystickCaps.inputs[index].min_rng = avg - min;
    g_JoystickCaps.inputs[index].max_rng = max - min;
    g_JoystickCaps.inputs[index].min_step = (avg - min) / 10;
    g_JoystickCaps.inputs[index].max_step = (max - min) / 10;
    g_JoystickCaps.inputs[4].avg_flags |= g_Joystick_capability_flags[index];
}

// FUNCTION: GTA 0x004337f0
void InitJoystickCaps() {
    int i;

    for (i = 0; i < GTA_ASIZE(g_JoystickCaps.inputs); i++) {
        g_JoystickCaps.inputs[i].field_0x14 = 0;
    }
    g_JoystickCaps.inputs[5].avg_flags = 0;
    g_JoystickCaps.inputs[4].max_step = 0;
    JOYCAPS caps;
    if (joyGetDevCapsA(0, &caps, sizeof(caps)) == JOYERR_NOERROR) {
        g_JoystickCaps.inputs[0].avg_flags = (caps.wXmin + caps.wXmax) / 512 - 128;
        g_JoystickCaps.inputs[0].min_rng = caps.wXmin / 256 - g_JoystickCaps.inputs[0].avg_flags - 128;
        g_JoystickCaps.inputs[0].min_step = g_JoystickCaps.inputs[0].min_rng / 10;
        g_JoystickCaps.inputs[0].max_rng = caps.wXmax / 256 - g_JoystickCaps.inputs[0].avg_flags - 128;
        g_JoystickCaps.inputs[0].max_step = g_JoystickCaps.inputs[0].max_rng / 10;

        g_JoystickCaps.inputs[1].avg_flags = (caps.wYmin + caps.wYmax) / 512 - 128;
        g_JoystickCaps.inputs[1].min_rng = caps.wYmin / 256 - g_JoystickCaps.inputs[1].avg_flags - 128;
        g_JoystickCaps.inputs[1].min_step = g_JoystickCaps.inputs[1].min_rng / 10;
        g_JoystickCaps.inputs[1].max_rng = caps.wYmax / 256 - g_JoystickCaps.inputs[1].avg_flags - 128;
        g_JoystickCaps.inputs[1].max_step = g_JoystickCaps.inputs[1].max_rng / 10;

        u32 capability_flags = g_JoystickCaps.inputs[4].avg_flags | g_Joystick_capability_flags[0] | g_Joystick_capability_flags[1];

        if (caps.wCaps & JOYCAPS_HASZ) {
            g_JoystickCaps.inputs[5].max_rng = 1;
            g_JoystickCaps.inputs[2].avg_flags = (caps.wZmin + caps.wZmax) / 512 - 128;
            g_JoystickCaps.inputs[2].min_rng = caps.wZmin / 256 - g_JoystickCaps.inputs[2].avg_flags - 256;
            g_JoystickCaps.inputs[2].min_step = g_JoystickCaps.inputs[2].min_rng / 10;
            g_JoystickCaps.inputs[2].max_rng = caps.wZmax / 256 - g_JoystickCaps.inputs[2].avg_flags - 256;
            g_JoystickCaps.inputs[2].max_step = g_JoystickCaps.inputs[2].max_rng / 10;
            capability_flags |= g_Joystick_capability_flags[2];
        }
        if (caps.wCaps & JOYCAPS_HASR) {
            g_JoystickCaps.inputs[5].field_0x14 = 1;
            g_JoystickCaps.inputs[3].avg_flags = (caps.wRmin + caps.wRmax) / 512 - 128;
            g_JoystickCaps.inputs[3].min_rng = caps.wRmin / 256 - g_JoystickCaps.inputs[3].avg_flags - 128;
            g_JoystickCaps.inputs[3].min_step = g_JoystickCaps.inputs[3].min_rng / 10;
            g_JoystickCaps.inputs[3].max_rng = caps.wRmax / 256 - g_JoystickCaps.inputs[3].avg_flags - 128;
            g_JoystickCaps.inputs[3].max_step = g_JoystickCaps.inputs[3].max_rng / 10;
            capability_flags |= g_Joystick_capability_flags[3];
        }
        g_JoystickCaps.inputs[4].avg_flags = capability_flags;
        if (caps.wCaps & JOYCAPS_HASU) {
            g_JoystickCaps.has_u = 1;
            s32 avg = (caps.wUmin + caps.wUmax) / 512 - 128;
            g_JoystickCaps.inputs[4].min_rng = caps.wUmin / 256 - avg - 128;
            g_JoystickCaps.inputs[4].min_step = g_JoystickCaps.inputs[4].min_rng / 10;
            g_JoystickCaps.inputs[4].max_rng = caps.wUmax / 256 - avg - 256;
            g_JoystickCaps.inputs[4].max_step = g_JoystickCaps.inputs[4].max_rng / 10;
            g_JoystickCaps.inputs[4].avg_flags = avg | 0x2e;
        }
        if ((caps.wCaps & JOYCAPS_HASV) != JOYCAPS_HASV) {
            g_JoystickCaps.has_v = 1;
            InitJoystickCapsAtIndex(5, caps.wVmin / 256 - 128, (caps.wVmax + caps.wVmin) / 512 - 128, caps.wVmax / 256 - 128);
        }
    }
}

// FUNCTION: GTA 0x0046e820
void LoadControlsFromRegister() {
    HKEY hKey;
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design\\Grand Theft Auto", 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        FatalError(eFatalError_please_run_settings_first_error_open_registry_key, 426, 0);
    }
    DWORD size = sizeof(g_Language_id_007537ac);
    result = RegQueryValueExA(hKey, "Language", NULL, NULL, (BYTE *)&g_Language_id_007537ac, &size);
    if (result != ERROR_SUCCESS) {
        FatalError(eFatalError_please_run_settings_first_error_query_registry_value, 426, 0);
    }
    result = RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        FatalError(eFatalError_cannot_close_registry_key, 426, 0);
    }

    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\DMA Design\\Grand Theft Auto\\Controls", 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        FatalError(eFatalError_please_run_settings_first_error_open_registry_key, 426, 0);
    }
    for (int i = 0; i < GTA_ASIZE(g_Register_control_names); i++) {
        size = sizeof(g_Controls_from_register[i]);
        result = RegQueryValueExA(hKey, g_Register_control_names[i], NULL, NULL, (BYTE *)&g_Controls_from_register[i], &size);
        if (result != ERROR_SUCCESS) {
            FatalError(eFatalError_please_run_settings_first_error_query_registry_value, 426, 0);
        }
    }
    result = RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        FatalError(eFatalError_cannot_close_registry_key, 426, 0);
    }
}

// FUNCTION: GTA 0x0046e960
void GetScancodesFromRegister(undefined4 *keys) {

    for (int i = 0; i < GTA_ASIZE(g_Controls_from_register); i++) {
        undefined4 value = g_Controls_from_register[i];
        if (value < 0x100) {
            if (value >= 0x80) {
                value += 0x80;
            }
        } else if (value < 0x120) {
            value += 0x2f1;
        } else if (value >= 0x140 && value <= 0x145) {
            value += 0x2a9;
        } else {
            value = 0;
        }

        keys[g_Register_control_swizzler[i]] = value;
    }
}

// FUNCTION: GTA 0x0046e9c0
const char *GetLanguageSuffix() {
    g_Language_id_suffix[0] = '0' + g_Language_id_007537ac;
    return g_Language_id_suffix;
}

// FUNCTION: GTA 0x004337b0
void SetJoystickUAxisFlag(int index) {
    g_JoystickCaps.inputs[4].max_step = 1;
    g_JoystickCaps.inputs[index].field_0x14 = 1;
}

// FUNCTION: GTA 0x004337d0
void SetJoystickVAxisFlag(u8 flag) {
    g_JoystickCaps.inputs[5].avg_flags |= flag;
}

// FUNCTION: GTA 0x00432b40
void InitInput() {
    InitJoystickCaps();
    GetScancodesFromRegister(g_Input_scancodes);
    g_Input_with_keyboard = FALSE;
    g_Input_with_joystick = FALSE;
    for (int i = 0; i < GTA_ASIZE(g_Input_scancodes); i++) {
        if (g_Input_scancodes[i] <= 1000) {
            g_Input_with_keyboard = TRUE;
        } else {
            g_Input_with_joystick = TRUE;
            switch (g_Input_scancodes[i]) {
            case 1001:
            case 1002:
                SetJoystickUAxisFlag(0);
                break;
            case 1003:
            case 1004:
                SetJoystickUAxisFlag(1);
                break;
            case 1005:
            case 1006:
                SetJoystickUAxisFlag(2);
                break;
            case 1007:
            case 1008:
                SetJoystickUAxisFlag(3);
                break;
            case 1009:
                SetJoystickVAxisFlag(0x10);
                break;
            case 1010:
                SetJoystickVAxisFlag(0x20);
                break;
            case 1011:
                SetJoystickVAxisFlag(0x40);
                break;
            case 1012:
                SetJoystickVAxisFlag(0x80);
                break;
            }
        }
    }
    if (g_Input_with_joystick) {
        DEBUGPRINT_NOARG();
    }
}

// FUNCTION: GTA 0x00432dc0
void FlushReplayBuffer() {
    tReplayBuffer_state state = g_Replay_buffer_state;
    g_Replay_buffer_state = eReplayBuffer_synchronized;
    if (state == eReplayBuffer_empty && g_Replay_buffer_pos != 0 && !g_BOOL_00502f44) {
        WriteBufferToFile(g_Replay_path, g_Replay_buffer, g_Replay_buffer_pos * sizeof(tReplay_item));
    }
}

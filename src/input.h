#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include "decomp.h"

#include <windows.h>

struct tJoystickThing {
    struct tJoystick_input {
        s32 min_rng;
        s32 min_step;
        s32 avg_flags;
        s32 max_step;
        s32 max_rng;
        BOOL field_0x14;
    } inputs[6]; // x, y, z, r, u, v
    undefined4 has_u;
    undefined4 has_v;
};

struct tReplay_item {
    undefined4 field_0x0;
    undefined4 field_0x4;
};

enum tReplayBuffer_state {
    eReplayBuffer_empty = 0,
    eReplayBuffer_contains_data = 1,
    eReplayBuffer_synchronized = 2,
};

extern bool g_BOOL_005031f4;

void SetReplayPath(const char *path);

void LoadControlsFromRegister();

void GetScancodesFromRegister(undefined4 *keys);

const char *GetLanguageSuffix();

void InitInput();

void FlushReplayBuffer();

#endif // INPUT_H

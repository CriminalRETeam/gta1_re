#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "common.h"
#include "decomp.h"

#include <dplay.h>

enum tConnection_state {
    eMultiplayer_unavailable = 0,
    eMultiplayer_not_connected = 1,
    eMultiplayer_connected = 2,
};

struct tMultiplayer_player {
    undefined field_0x0;
    u8 field_0x1;
    undefined2 field_0x2;
    undefined4 field_0x4;
    undefined4 field_0x8;
    undefined4 field_0xc;
    undefined4 field_0x10;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined4 width_div_2;
    undefined4 height_div_2;
    undefined4 width;
    undefined4 height;
    undefined4 width_lshift_16;
    undefined4 height_lshift_16;
    undefined field_0x30[20];
    bool screen_not_4_by_3;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    undefined field_0x48;
    undefined field_0x49;
    undefined field_0x4a;
    undefined field_0x4b;
    undefined field_0x4c;
    undefined field_0x4d;
    undefined field_0x4e;
    undefined field_0x4f;
    undefined field_0x50;
    undefined field_0x51;
    undefined field_0x52;
    undefined field_0x53;
    undefined field_0x54;
    undefined field_0x55;
    undefined field_0x56;
    undefined field_0x57;
    undefined field_0x58;
    undefined field_0x59;
    undefined field_0x5a;
    undefined field_0x5b;
    undefined field_0x5c;
    undefined field_0x5d;
    undefined field_0x5e;
    undefined field_0x5f;
    undefined field_0x60;
    undefined field_0x61;
    undefined field_0x62;
    undefined field_0x63;
    undefined field_0x64;
    undefined field_0x65;
    undefined field_0x66;
    undefined field_0x67;
    undefined field_0x68;
    undefined field_0x69;
    undefined field_0x6a;
    undefined field_0x6b;
    undefined field_0x6c;
    undefined field_0x6d;
    undefined field_0x6e;
    undefined field_0x6f;
    undefined field_0x70;
    undefined field_0x71;
    undefined field_0x72;
    undefined field_0x73;
    undefined field_0x74;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
    undefined field_0x78;
    undefined field_0x79;
    undefined field_0x7a;
    undefined field_0x7b;
    undefined field_0x7c;
    undefined field_0x7d;
    undefined field_0x7e;
    undefined field_0x7f;
    undefined field_0x80;
    undefined field_0x81;
    undefined field_0x82;
    undefined field_0x83;
    undefined field_0x84;
    undefined field_0x85;
    undefined field_0x86;
    undefined field_0x87;
    undefined field_0x88;
    undefined field_0x89;
    undefined field_0x8a;
    undefined field_0x8b;
    undefined field_0x8c;
    undefined field_0x8d;
    undefined field_0x8e;
    undefined field_0x8f;
    undefined field_0x90;
    undefined field_0x91;
    undefined field_0x92;
    undefined field_0x93;
    undefined field_0x94;
    undefined field_0x95;
    undefined field_0x96;
    undefined field_0x97;
    undefined field_0x98;
    undefined field_0x99;
    undefined field_0x9a;
    undefined field_0x9b;
    undefined field_0x9c;
    undefined field_0x9d;
    undefined field_0x9e;
    undefined field_0x9f;
    undefined field_0xa0;
    undefined field_0xa1;
    undefined field_0xa2;
    undefined field_0xa3;
    undefined field_0xa4;
    undefined field_0xa5;
    undefined field_0xa6;
    undefined field_0xa7;
    undefined field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    struct {
        int field_0x0;
        int field_0x4;
        int field_0x8;
        undefined4 field3_0xc;
    } field_0xac;
    struct {
        undefined4 field_0x0;
        undefined2 field_0x4;
        undefined field_0x6;
        undefined field_0x7;
    } field_0xbc;
    undefined field_0xc4;
    undefined field_0xc5;
    undefined field_0xc6;
    undefined field_0xc7;
    undefined field_0xc8;
    undefined field_0xc9;
    undefined field_0xca;
    undefined field_0xcb;
    undefined field_0xcc;
    undefined field_0xcd;
    undefined field_0xce;
    undefined field_0xcf;
    undefined4 field_0xd0;
    undefined4 field_0xd4;
    undefined4 field_0xd8;
    undefined4 field_0xdc;
    undefined4 field_0xe0;
    char name[16];
    int score;
    undefined2 field_0xf8;
    undefined field_0xfa;
    undefined field_0xfb;
    undefined4 field_0xfc;
    undefined4 field_0x100;
    undefined4 field_0x104;
    undefined4 field_0x108;
    undefined4 field_0x10c;
    short field_0x110[10];
    char field_0x124[3];
    char field_0x127[3];
    undefined field_0x12a;
    undefined field_0x12b;
    undefined field_0x12c;
    undefined field_0x12d;
    undefined field_0x12e;
    undefined field_0x12f;
    undefined field_0x130;
    char field_0x131[10];
    undefined field_0x13b;
    undefined4 field_0x13c;
    undefined4 field_0x140;
    undefined4 field_0x144;
    undefined4 field_0x148;
    undefined2 field_0x14c;
    undefined field_0x14e;
    undefined field_0x14f;
    undefined2 field_0x150;
    undefined field_0x152;
    undefined field_0x153;
    undefined field_0x154[4];
    u8 field_0x158[4];
    undefined field_0x15c;
    undefined field_0x15d;
    undefined field_0x15e;
    undefined field_0x15f;
    undefined4 field_0x160;
    undefined4 field_0x164;
    undefined field_0x168;
    undefined field_0x169;
    undefined field_0x16a;
    undefined field_0x16b;
    short field_0x16c;
    undefined field_0x16e;
    undefined field_0x16f;
    undefined4 field_0x170;
    undefined2 field_0x174;
    undefined field_0x176;
    undefined field_0x177;
    undefined4 field_0x178;
    undefined2 field_0x17c;
    undefined2 field_0x17e;
    undefined2 field_0x180;
    undefined field_0x182;
    undefined field_0x183;
    int player_is_inputing_text;
    u8 config_number;
    undefined field_0x189;
    undefined field_0x18a;
    undefined field_0x18b;
    undefined4 field_0x18c;
    undefined field_0x190;
    undefined field_0x191;
    undefined field_0x192;
    undefined field_0x193;
    u8 rotation_speed_from_input;
    s8 direction_factor_from_input;
    undefined field_0x196;
    undefined field_0x197;
    undefined field_0x198;
    undefined field_0x199;
    undefined field_0x19a;
    undefined field_0x19b;
    undefined4 frags;
    undefined4 field_0x1a0;
    undefined4 field_0x1a4;
    undefined2 field_0x1a8;
    undefined2 field_0x1aa;
    undefined2 field_0x1ac;
    undefined2 field_0x1ae;
    undefined2 field_0x1b0;
    u8 field_0x1b2;
    u8 field_0x1b3;
    undefined profile_index;
    undefined field_0x1b5;
    undefined field_0x1b6;
    undefined field_0x1b7;
    undefined4 field_0x1b8;
};

extern int g_Multiplayer_player_id;

struct tDirectPlay_multiplayer {
    DPID player_id;
    BOOL valid;
};

void InitMultiplayer();

void InitMultiplayerBackend();

void InitDirectPlay();

void SetMultiplayerProfileName(int index, const char *name, u8 profile_index);

void SetPlayerScreenDimension(int player, int width, int height);

int GetMultiplayerScore_00462b70(int player);

int GetMultiplayerScore_004643b0(int player);

const char *GetMultiplayerName(int player);

int GetMultiplayerFrags(int player);

int GetMultiplayerRacePosition(int player);

int GetEndOfMultiplayerGameResult(char *buffer);

void SetNotConnectedState();

void InitMultiplayerPlayerCounts();

void DestroyMultiplayer();

BOOL IsMultiplayerAvailable();

void Multiplayer_FUN_0044b8a0(int arg);

BOOL EnumerateMultiplayerProviders();

int GetCountMultiplayerProviders();

const char *GetMultiplayerProviderName(int provider_index);

int GetCountMultiplayerSessions();

BOOL ConnectMultiplayerProvider(int provider_index);

BOOL FindMultiplayerSessions();

BOOL CreateMultiplayerSession(char *session_name);

BOOL RegisterMultiplayerPlayer(char *name);

#endif // MULTIPLAYER_H

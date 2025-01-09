#define INITGUID

#include "multiplayer.h"
#include "common.h"
#include "decomp.h"
#include "game.h"
#include "interface.h"
#include "menu.h"
#include "text.h"

#include <windows.h>
#include <dplay.h>

#include <stdio.h>
#include <stdlib.h>

struct tMultiplayer_provider {
    GUID guid;
    char *name;
    tMultiplayer_provider *next;
};

struct tMultiplayer_session {
    GUID guid;
    char name[32];
    tMultiplayer_session *next;
};

// {22DF3B40-3612-11CF-AEC8-0000C06691AD}
static const GUID g_GTA_Game_GUID =
{ 0x22df3b40, 0x3612, 0x11cf, { 0xae, 0xc8, 0x00, 0x00, 0xc0, 0x66, 0x91, 0xad } };

// GLOBAL: GTA 0x00501d7c
tConnection_state g_Connection_state;

// GLOBAL: GTA 0x00501d78
BOOL g_DirectPlay_started;

// GLOBAL: GTA 0x007852c8
BOOL g_BOOL_007852c8;

// GLOBAL: GTA 0x00785180
tMultiplayer_session *g_Multiplayer_sessions;

// GLOBAL: GTA 0x0074f148
tMultiplayer_player g_Multiplayer_players[4];

// GLOBAL: GTA 0x0078518c
LPDIRECTPLAY g_DirectPlay;

// GLOBAL: GTA 0x007851b8
DPID g_DirectPlay_player_id;

// GLOBAL: GTA 0x007851bc
BOOL g_DirectPlay_player_created;

// GLOBAL: GTA 0x007851c0
LPDIRECTPLAY2 g_DirectPlay2;

// GLOBAL: GTA 0x007851c4
tMultiplayer_provider *g_Multiplayer_providers;

// GLOBAL: GTA 0x0074f83c
int g_Multiplayer_player_id;

// GLOBAL: GTA 0x007710f5
bool g_MultiplayerRacePositionUpdated;

// GLOBAL: GTA 0x007710fc
s16 g_Multiplayer_race_position[4];

// GLOBAL: GTA 0x006b400c
s8 g_Input_network_cycle;

// GLOBAL: GTA 0x0074f838
int g_Count_players;

// GLOBAL: GTA 0x0074f84c
int g_Count_players_2;

// GLOBAL: GTA 0x00785190
tDirectPlay_multiplayer g_DirectPlay_multiplayers[4];

// GLOBAL: GTA 0x007851b0
int g_Count_multi_players;

// GLOBAL: GTA 0x007851b4
BOOL g_DirectPlay_failed;

// GLOBAL: GTA 0x006b4064
int g_INT_006b4064;

// GLOBAL: GTA 0x00785184
int g_Count_multiplayer_providers;

// GLOBAL: GTA 0x00785188
int g_Count_multiplayer_sessions;


// FUNCTION: GTA 0x0044b920
void InitMultiplayer() {
    if (g_Multiplayer_available) {
        InitMultiplayerBackend();
    }
}

// FUNCTION: GTA 0x00412960
void InitMultiplayerBackend() {
    InitDirectPlay();
    g_Connection_state = eMultiplayer_not_connected;
    g_DirectPlay_started = TRUE;
}

// FUNCTION: GTA 0x00487240
void InitDirectPlay() {
    g_BOOL_007852c8 = TRUE;
    g_DirectPlay = NULL;
    g_DirectPlay2 = NULL;
    g_DirectPlay_player_id = 0;
    g_DirectPlay_player_created = FALSE;
    g_Multiplayer_providers = NULL;
    g_Multiplayer_sessions = NULL;
}

// FUNCTION: GTA 0x00462b90
void SetMultiplayerProfileName(int index, const char *name, u8 profile_index) {

    strcpy(g_Multiplayer_players[index].name, name);
    g_Multiplayer_players[index].profile_index = profile_index;
}

// FUNCTION: GTA 0x00464500
void SetPlayerScreenDimension(int player, int width, int height) {
    g_Multiplayer_players[player].height_lshift_16 = height << 16;
    g_Multiplayer_players[player].width_lshift_16 = width << 16;
    g_Multiplayer_players[player].height = height;
    g_Multiplayer_players[player].width = width;
    g_Multiplayer_players[player].width_div_2 = width / 2;
    g_Multiplayer_players[player].height_div_2 = height / 2;
    g_Multiplayer_players[player].screen_not_4_by_3 = width * 3 != height * 4;
}

// STUB: GTA 0x00474a50
void CalculateMultiplayerRacePositions() {
    // FIXME
    NOT_IMPLEMENTED();
}

// FUNCTION: GTA 0x00462b70
int GetMultiplayerScore_00462b70(int player) {
    return g_Multiplayer_players[player].score;
}

// FUNCTION: GTA 0x004643b0
int GetMultiplayerScore_004643b0(int player) {
    return g_Multiplayer_players[player].score;
}

// FUNCTION: GTA 0x00462b70
const char *GetMultiplayerName(int player) {
    return g_Multiplayer_players[player].name;
}

// FUNCTION: GTA 0x00464420
int GetMultiplayerFrags(int player) {
    return g_Multiplayer_players[player].frags;
}

// FUNCTION: GTA 0x00474bd0
int GetMultiplayerRacePosition(int player) {
    if (!g_MultiplayerRacePositionUpdated) {
        CalculateMultiplayerRacePositions();
    }
    for (int i = 0; i < 4; i++) {
        if (g_Multiplayer_race_position[i] == player) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: GTA 0x00445670
int GetEndOfMultiplayerGameResult(char *buffer) {
    int runnerup_id = -1;
    int runnerup_score = -1;
    int winner_id = -1;
    int winner_score = -1;
    for (int i = 0; i < 4; i++) {
        int score = GetMultiplayerScore_00462b70(i);
        if (score > winner_score) {
            runnerup_id = runnerup_id;
            runnerup_score = winner_score;
            winner_id = i;
            winner_score = score;
        } else if (score > runnerup_score) {
            runnerup_id = i;
            runnerup_score = score;
        }
    }
    const char *winner_name = GetMultiplayerName(winner_id);
    int my_score = GetMultiplayerScore_00462b70(g_Multiplayer_player_id);
    switch (g_End_of_game_result) {
    case eEndOfRace_success:
        strcpy(buffer, GetTranslatedString("m22success"));
        break;
    case eEndOfRace_failed:
        strcpy(buffer, GetTranslatedString("m22failed"));
        break;
    case eEndOfRace_dead:
        strcpy(buffer, GetTranslatedString("m22dead"));
        break;
    case eEndOfRace_arrest:
        strcpy(buffer, GetTranslatedString("m22arrest"));
        break;
    case eEndOfRace_timeout:
        strcpy(buffer, GetTranslatedString("m22timeout"));
        break;
    case eEndOfRace_timeover:
        sprintf(g_Load_temporary_buffer, GetTranslatedString("m22timeover"), winner_name, winner_score, GetMultiplayerName(runnerup_id), runnerup_score);
        strcpy(buffer, g_Load_temporary_buffer);
        break;
    case eEndOfRace_score:
        sprintf(g_Load_temporary_buffer, GetTranslatedString("m22score"), winner_name, winner_score);
        strcpy(buffer, g_Load_temporary_buffer);
        break;
    case eEndOfRace_cannon:
        sprintf(g_Load_temporary_buffer, GetTranslatedString("m22cannon"), winner_name);
        strcpy(buffer, g_Load_temporary_buffer);
        break;
    case eEndOfRace_demo:
        strcpy(buffer, GetTranslatedString("m22demo"));
        break;
    default:
        strcpy(buffer, GetTranslatedString("m22incomplete"));
        break;
    }
    return my_score;
}

// FUNCTION: GTA 0x00412d00
void SetNotConnectedState() {
    if (g_Connection_state == eMultiplayer_connected) {
        g_Connection_state = eMultiplayer_not_connected;
    }
}

// FUNCTION: GTA 0x004876b0
int GetDirectPlayPlayerIndex() {
    for (int i = 0; i < GTA_ASIZE(g_DirectPlay_multiplayers); i++) {
        if (g_DirectPlay_multiplayers[i].valid && g_DirectPlay_multiplayers[i].player_id == g_DirectPlay_player_id) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: GTA 0x00412af0
int GetMultiplayerIndex() {
    if (g_Connection_state != eMultiplayer_connected) {
        return 0;
    }
    return GetDirectPlayPlayerIndex();
}

// FUNCTION: GTA 0x004876a0
int GetRemoteDirectPlayPlayerCount() {
    return g_Count_multi_players - 1;
}

// FUNCTION: GTA 0x00412ad0
int GetRemotePlayerCount() {
    if (g_Connection_state != eMultiplayer_connected) {
        return 0;
    }
    return GetRemoteDirectPlayPlayerCount();
}

// FUNCTION: GTA 0x0044b8b0
void InitMultiplayerPlayerCounts() {
    if (g_Multiplayer_available) {
        g_Multiplayer_player_id = GetMultiplayerIndex();
        int count = GetRemotePlayerCount();
        g_Count_players = count + 1;
        g_Count_players_2 = count + 1;
        g_Input_network_cycle = 0;
    } else {
        g_Multiplayer_player_id = 0;
        g_Count_players = 1;
        g_Count_players_2 = 1;
        g_Input_network_cycle = 0;
    }
}

static BOOL inline CheckDirectPlayResult(HRESULT result) {
    switch (result) {
        case DP_OK:
        case DPERR_USERCANCEL:
        case DPERR_NOSESSIONS:
        case DPERR_TIMEOUT:
            g_DirectPlay_failed = FALSE;
            break;
        default:
            g_DirectPlay_failed = TRUE;
            break;
    }
    return !g_DirectPlay_failed;
}

void StopDirectPlay() {
    if (g_DirectPlay == NULL) {
        return;
    }
    if (g_DirectPlay_player_created) {
        HRESULT result = g_DirectPlay2->DestroyPlayer(g_DirectPlay_player_id);
        CheckDirectPlayResult(result);
        g_DirectPlay_player_id = 0;
        g_DirectPlay_player_created = FALSE;
    }
    HRESULT result = g_DirectPlay2->Close();
    CheckDirectPlayResult(result);
    g_DirectPlay2->Release();
    g_DirectPlay->Release();
    g_DirectPlay2 = NULL;
    g_DirectPlay = NULL;
}

// FUNCTION: GTA 0x00412980
void StopMultiplayer() {
    if (g_DirectPlay_started) {
        if (g_Connection_state != eMultiplayer_unavailable) {
            StopDirectPlay();
        }
        g_DirectPlay_started = FALSE;
    }
}

// FUNCTION: GTA 0x0044b910
void DestroyMultiplayer() {
    if (g_Multiplayer_available) {
        StopMultiplayer();
    }
}

// FUNCTION: GTA 0x00412950
BOOL IsMultiplayerAvailable() {
    return g_Connection_state != eMultiplayer_unavailable;
}

// FUNCTION: GTA 0x0044b8a0
void Multiplayer_FUN_0044b8a0(int arg) {
    g_INT_006b4064 = arg;
}

// FUNCTION: GTA 0x00486910
WINBOOL CALLBACK MultiplayerProviderCallback(LPGUID lpguidSP, LPSTR lpSPName, DWORD dwMajorVersion, DWORD dwMinorVersion, LPVOID lpContext) {
    tMultiplayer_provider *next = g_Multiplayer_providers;
    g_Multiplayer_providers = new tMultiplayer_provider;
    g_Multiplayer_providers->guid = *lpguidSP;
    g_Multiplayer_providers->name = new char[strlen(lpSPName) + 1];
    strcpy(g_Multiplayer_providers->name, lpSPName);
    g_Multiplayer_providers->next = next;
    g_Count_multiplayer_providers += 1;
    return TRUE;
}

// FUNCTION: GTA 0x00486880
BOOL EnumerateMultiplayerProviders() {
    while (g_Multiplayer_providers != NULL) {
        tMultiplayer_provider *next = g_Multiplayer_providers;
        delete g_Multiplayer_providers->name;
        delete g_Multiplayer_providers;
        g_Multiplayer_providers = next;
    }
    g_Multiplayer_providers = NULL;
    g_Count_multiplayer_providers = 0;
    HRESULT hres = DirectPlayEnumerateA(MultiplayerProviderCallback, NULL);
    return CheckDirectPlayResult(hres);
}

// FUNCTION: GTA 0x004869a0
int GetCountMultiplayerProviders() {
    return g_Count_multiplayer_providers;
}

// FUNCTION: GTA 0x004869b0
const char *GetMultiplayerProviderName(int provider_index) {
    tMultiplayer_provider *provider = g_Multiplayer_providers;
    while (provider_index > 0) {
        provider = provider->next;
        provider_index--;
    }
    if (provider == NULL) {
        return NULL;
    }
    return provider->name;
}

// FUNCTION: GTA 0x00486e10
int GetCountMultiplayerSessions() {
    return g_Count_multiplayer_sessions;
}

// FUNCTION: GTA 0x004869d0
BOOL ConnectMultiplayerProvider(int provider_index) {
    HRESULT result;
    tMultiplayer_provider *provider = g_Multiplayer_providers;
    while (provider_index > 0) {
        provider = provider->next;
        provider_index -= 1;
    }
    if (g_DirectPlay != NULL) {
        if (g_DirectPlay_player_created) {
            result = g_DirectPlay2->DestroyPlayer(g_DirectPlay_player_id);
            CheckDirectPlayResult(result);
            g_DirectPlay_player_id = 0;
            g_DirectPlay_player_created = FALSE;
        }
        result = g_DirectPlay2->Close();
        CheckDirectPlayResult(result);
        g_DirectPlay2->Release();
        g_DirectPlay->Release();
        g_DirectPlay = NULL;
        g_DirectPlay2 = NULL;
    }
    result = DirectPlayCreate(&provider->guid, &g_DirectPlay, NULL);
    if (!CheckDirectPlayResult(result)) {
        if (g_DirectPlay != NULL) {
            g_DirectPlay->Release();
            g_DirectPlay = NULL;
        }
        return FALSE;
    }
    result = g_DirectPlay->QueryInterface(IID_IDirectPlay2A, (void **) &g_DirectPlay2);
    if (!CheckDirectPlayResult(result)) {
        if (g_DirectPlay2 != NULL) {
            g_DirectPlay2->Release();
            g_DirectPlay->Release();
            g_DirectPlay = NULL;
            g_DirectPlay2 = NULL;
        }
        return FALSE;
    }
    return TRUE;
}

// FUNCTION: GTA 0x00486c40
WINBOOL CALLBACK MultiplayerEnumSeessionsCallback(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext) {
    tMultiplayer_session *next = g_Multiplayer_sessions;
    if (dwFlags & DPESC_TIMEDOUT) {
        return FALSE;
    }
    g_Multiplayer_sessions = new tMultiplayer_session;
    g_Multiplayer_sessions->guid = lpThisSD->guidInstance;
    strcpy(g_Multiplayer_sessions->name, lpThisSD->lpszSessionNameA);
    g_Multiplayer_sessions->next = next;
    g_Count_multiplayer_sessions += 1;
    return TRUE;
}

// FUNCTION: GTA 0x00486b60
BOOL FindMultiplayerSessions() {
    DPSESSIONDESC2 session_desc;
    memset(&session_desc, 0, sizeof(session_desc));
    session_desc.dwSize = sizeof(session_desc);
    session_desc.guidApplication = g_GTA_Game_GUID;
    if (g_Multiplayer_sessions != NULL) {
        tMultiplayer_session *next = g_Multiplayer_sessions->next;
        delete g_Multiplayer_sessions;
        g_Multiplayer_sessions = next;
    }
    g_Multiplayer_sessions = NULL;
    g_Count_multiplayer_sessions = 0;
    ShowCursor(TRUE);
    HRESULT result = g_DirectPlay2->EnumSessions(&session_desc, 0, MultiplayerEnumSeessionsCallback, NULL, 0);
    BOOL success = CheckDirectPlayResult(result);
    ShowCursor(FALSE);
    return success;
}

// FUNCTION: GTA 0x00486cc0
BOOL CreateMultiplayerSession(char *session_name) {
    DPSESSIONDESC2 session_desc;
    memset(&session_desc, 0, sizeof(session_desc));
    session_desc.dwSize = sizeof(session_desc);
    session_desc.guidApplication = g_GTA_Game_GUID;
    session_desc.dwFlags = 0;
    session_desc.dwMaxPlayers = 4;
    session_desc.lpszSessionNameA = session_name;
    ShowCursor(TRUE);
    HRESULT result = g_DirectPlay2->Open(&session_desc, DPOPEN_CREATE);
    ShowCursor(FALSE);
    if (!CheckDirectPlayResult(result)) {
        result = g_DirectPlay2->DestroyPlayer(g_DirectPlay_player_id);
        CheckDirectPlayResult(result);
        g_DirectPlay_player_id = 0;
        g_DirectPlay_player_created = FALSE;
        result = g_DirectPlay2->Close();
        CheckDirectPlayResult(result);
        return FALSE;
    }
    g_BOOL_007852c8 = TRUE;
    return TRUE;
}

// FUNCTION: GTA 0x004870f0
BOOL RegisterMultiplayerPlayer(char *name) {
    DPNAME dp_name;
    dp_name.dwSize = sizeof(dp_name);
    dp_name.dwFlags = 0;
    dp_name.lpszShortNameA = name;
    dp_name.lpszLongNameA = NULL;
    HRESULT result;
    if (g_DirectPlay_player_created) {
        result = g_DirectPlay2->DestroyPlayer(g_DirectPlay_player_id);
        CheckDirectPlayResult(result);
        g_DirectPlay_player_id = 0;
    }
    g_DirectPlay_player_created = TRUE;
    result = g_DirectPlay2->CreatePlayer(&g_DirectPlay_player_id, &dp_name, NULL, NULL, 0, 0);
    if (!CheckDirectPlayResult(result)) {
        result = g_DirectPlay2->DestroyPlayer(g_DirectPlay_player_id);
        CheckDirectPlayResult(result);
        g_DirectPlay_player_id = 0;
        g_DirectPlay_player_created = 0;
        result = g_DirectPlay2->Close();
        CheckDirectPlayResult(result);
        return FALSE;
    }
    return TRUE;
}

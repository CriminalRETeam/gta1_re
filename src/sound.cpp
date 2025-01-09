#include "sound.h"
#include "error.h"
#include "game.h"
#include "mission.h"
#include "save.h"

#include <stdio.h>
#include <time.h>

#include <mss.h>

struct tStream_play_track_info {
    s32 prev_position;
    s32 length;
    s32 track_position;
};

struct tSample_info {
    int offset;
    int size;
    int sample_rate;
};

// GLOBAL: GTA 0x004bddf0
bool g_Miles_running;

// GLOBAL: GTA 0x004bddf3
bool g_Cutscene_music_running;

// GLOBAL: GTA 0x004ab2dc
bool g_BOOL_004ab2dc = TRUE;

// GLOBAL: GTA 0x00501aa8
int g_Music_track;

// GLOBAL: GTA 0x005015f8
tStream_play_track_info g_Music_track_infos[99];

// GLOBAL: GTA 0x00501578
tStream_play_track_info g_Audio_track_infos_2[9];

// GLOBAL: GTA 0x00501aa0
bool g_Music_initialized;

// GLOBAL: GTA 0x00501aa4
BOOL g_Always_music;

// GLBOAL: GTA 0x00501aac
int g_Music_state;

// GLOBAL: GTA 0x004ab040
bool g_Music_enabled = true;

// GLOBAL: GTA 0x004ab041
bool g_Sound_enabled = true;

// GLOBAL: GTA 0x004bdb08
struct StashedSound {
    StashedSound() {
        stash = g_Sound_enabled;
    }
    bool stash;
} g_Stashed_sound_enabled;

// GLOBAL: GTA 0x004ab048
u8 g_Music_volume_preset_values[8] =
    { 0, 18, 36, 54, 73, 91, 109, 127 };

// GLOBAL: GTA 0x004ab050
u8 g_Sound_volume_preset_values[8] =
    { 0, 18, 36, 54, 73, 91, 109, 127 };

// GLOBAL: GTA 0x004ab042
s8 g_Master_music_volume = 127;

// GLOBAL: GTA 0x004ab043
s8 g_Master_sound_volume = 127;

// GLOBAL: GTA 0x00771078
bool g_Miles_samples_allocated;

// GLOBAL: GTA 0x00771010
FILE *g_File_vocalcom;

// GLOBAL: GTA 0x00771000
HDIGDRIVER g_Miles_device;

// GLOBAL: GTA 0x00501a9c
HSTREAM g_Music_stream;

// GLOBAL: GTA 0x004b3114
s32 g_Master_volume_level = 64;

// GLOBAL: GTA 0x004bddf1
bool g_Unknown_music_bool;

// GLOBAL: GTA 0004abd38
int g_Music_stream_volume = 50;

// GLOBAL: GTA 0x005031f8
int g_Audio_menu_or_game; // 0: init, 1: game, 2: menu

// GLOBAL: GTA 0x0077060c
HSAMPLE g_Miles_samples_0077060c[10];

// GLOBAL: GTA 0x00770ff4
HSAMPLE g_Miles_samples_00770ff4[3];

// GLOBAL: GTA 0x007709ac
HSAMPLE g_Miles_sample_007709ac;

// GLOBAL: GTA 0x00771074
HSAMPLE g_Miles_sample_00771074;

// GLOBAL: GTA 0x00771008
HSAMPLE g_Miles_sample_00771008;

// GLOBAL: GTA 0x007709b0
HSAMPLE g_Miles_sample_007709b0;

// GLOBAL: GTA 0x00771079
bool g_Miles_memory_allocated;

// GLOBAL: GTA 0x0077107a
u8 g_Level_env_short_samples_1_idx;

// GLOBAL: GTA 0x0077107b
u8 g_Level_env_short_samples_2_idx;

// GLOBAL: GTA 0x004b3118
bool g_Sample_freq_mod_sign = true;

// GLOBAL: GTA 0x00770634
void *g_Miles_memory;

// GLOBAL: GTA 0x0077063c
void *g_Level_environment_audio_0077063c;

// GLOBAL: GTA 0x00770fec
void *g_Level_environment_audio_00770fec;

// GLOBAL: GTA 0x0077099c
HSAMPLE g_Miles_samples_0077099c[4];

// GLOBAL: GTA 0x007709b4
HSAMPLE g_Miles_samples_007709b4[4];

// GLOBAL: GTA 0x007709c8
tSample_info g_Level_environment_audio_index[131];

// GLOBAL: GTA 0x004ab2e0
int g_INT_004ab2e0 = 5;

// GLOBAL: GTA 0x004ab058
bool g_BOOL_004ab058 = true;

// GLOBAL: GTA 0x00770608
HSTREAM g_Cutscene_music_stream;

// GLOBAL: GTA 0x0077100c
int g_Int_0077100c;

// GLOBAL: GTA 0x00770638
int g_Cutscene_music_pos;

// FUNCTION: GTA 0x00405450
void SetMusicStreamEnabled(bool enable) {
    g_BOOL_004ab058 = enable;
}

// GLOBAL: GTA 0x005015f0
u8 g_Count_music_tracks;

// GLOBAL: GTA 0x004b3110
u8 g_Cutscene_volume = 100;

// GLOBAL: GTA 0x004ab2e8
const char g_Music_track_paths[10][264] = {
    "..\\music\\track1.wav",
    "..\\music\\track2.wav",
    "..\\music\\track3.wav",
    "..\\music\\track4.wav",
    "..\\music\\track5.wav",
    "..\\music\\track6.wav",
    "..\\music\\track7.wav",
    "..\\music\\track8.wav",
    "..\\music\\track9.wav",
    "..\\music\\track10.wav",
};

// GLOBAL: GTA 0x00501578
tStream_play_track_info g_Music_track_infos_2[9];

// GLOBAL: GTA 0x005015e4
tStream_play_track_info g_Music_track_info_005015e4;

// GLOBAL: GTA 0x004ab2e4
int g_INT_004ab2e4 = 25;

// GLOBAL: GTA 0x0077557c
HTIMER g_Audio_timer;

// GLOBAL: GTA 0x00775584
BOOL g_Speed_limiter_active;


// FUNCTION: GTA 0x00471e80
void StopMilesFromGames() {
    if (g_Miles_samples_allocated) {
        g_Miles_samples_allocated = false;
        {
            for (int i = 0; i < GTA_ASIZE(g_Miles_samples_0077060c); i++) {
                AIL_release_sample_handle(g_Miles_samples_0077060c[i]);
            }
        }
        {
            for (int i = 0; i < GTA_ASIZE(g_Miles_samples_00770ff4); i++) {
                AIL_release_sample_handle(g_Miles_samples_00770ff4[i]);
            }
        }
        AIL_release_sample_handle(g_Miles_sample_007709ac);
        AIL_release_sample_handle(g_Miles_sample_00771074);
        AIL_release_sample_handle(g_Miles_sample_00771008);
        AIL_release_sample_handle(g_Miles_sample_007709b0);
        AIL_waveOutClose(g_Miles_device);
    }
    AIL_shutdown();
    DebugPrint("AIL_shutdown called from SAMPMAN.cpp");
}

// FUNCTION: GTA 0x004280b0
void ApplySoundVolumes() {
    if (g_Save_state.sound_volume < 0 || g_Save_state.sound_volume >= GTA_ASIZE(g_Sound_volume_preset_values)
            || g_Save_state.music_volume < 0 || g_Save_state.music_volume >= GTA_ASIZE(g_Music_volume_preset_values)) {
        FatalError(eFatalError_volume_out_of_range_2, 478, NULL);
    }
    if (g_Save_state.sound_volume != 0) {
        SetSoundEnabled(true);
    } else {
        SetSoundEnabled(false);
    }
    SetMasterSoundVolumePreset(g_Save_state.sound_volume);
    if (g_Save_state.music_volume != 0) {
        SetMusicEnabled(true);
    } else {
        SetMusicEnabled(false);
    }
    SetMasterMusicVolumePreset(g_Save_state.music_volume);
}

// FUNCTION: GTA 0x00402640
void SetSoundEnabled(bool enabled) {
    g_Sound_enabled = enabled;
}

// FUNCTION: GTA 0x00402650
void SetMusicEnabled(bool enabled) {
    g_Music_enabled = enabled;
}

// FUNCTION: GTA 0x00404c30
void SetMasterSoundVolumePreset(s8 preset) {
    int index = preset;
    if (preset >= GTA_ASIZE(g_Sound_volume_preset_values)) {
        index = GTA_ASIZE(g_Sound_volume_preset_values) - 1;
    }
    g_Master_sound_volume = g_Sound_volume_preset_values[index];
    SetSoundVolume(g_Master_sound_volume);
}

// FUNCTION: GTA 0x00472280
void SetSoundVolume(u8 level) {
    if (g_Miles_samples_allocated) {
        if (g_Master_music_volume != 0 && level == 0) {
            AIL_set_digital_master_volume(g_Miles_device, 0.8f * g_Master_music_volume);
            SetSoundEnabled(false);
        } else {
            AIL_set_digital_master_volume(g_Miles_device, 108);
            g_Master_volume_level = level;
        }
    }
}

// FUNCTION: GTA 0x00404bf0
void SetMasterMusicVolumePreset(s8 preset) {
    int index = preset;
    if (preset >= GTA_ASIZE(g_Music_volume_preset_values)) {
        index = GTA_ASIZE(g_Music_volume_preset_values) - 1;
    }
    g_Master_music_volume = g_Music_volume_preset_values[index];
    SetMusicVolume(g_Master_music_volume, g_Unknown_music_bool);
}

// FUNCTION: GTA 0x0040c8b0
void SetMusicVolume(u8 volume, bool enable) {
    if (g_Music_stream != NULL) {
        AIL_set_stream_volume(g_Music_stream, volume);
    }
    char buffer[50] = {0};
    sprintf(buffer, "CD SET VOL: Volume = %d", volume);
    g_Music_stream_volume = volume;
}

// FUNCTION: GTA 0x00472180
bool StartMilesSoundForIntro() {
    AIL_startup();
    PCMWAVEFORMAT wave_format;
    wave_format.wf.wFormatTag = WAVE_FORMAT_PCM;
    wave_format.wf.nChannels = 1;
    wave_format.wf.nSamplesPerSec = 22050;
    wave_format.wf.nAvgBytesPerSec = 44100;
    wave_format.wf.nBlockAlign = 2;
    wave_format.wBitsPerSample = 16;
    AIL_set_preference(0, 1);
    AIL_set_preference(15, 0);
    if (AIL_waveOutOpen(&g_Miles_device, 0, -1, &wave_format) != 0) {
        g_Miles_samples_allocated = false;
        SetSoundEnabled(false);
        return false;
    }
    char name[80];
    AIL_digital_configuration(g_Miles_device, 0, 0, name);
    if (strnicmp(name, "Emulated", 8) == 0) {
        AIL_waveOutClose(g_Miles_device);
        AIL_set_preference(15, 1);
        if (AIL_waveOutOpen(&g_Miles_device, 0, -1, &wave_format) != 0) {
            g_Miles_samples_allocated = false;
            SetSoundEnabled(false);
            return false;
        }
    }
    return true;
}

// FUNCTION: GTA 0x00472260
void StopMilesForIntro() {
    AIL_waveOutClose(g_Miles_device);
    AIL_shutdown();
}

// FUNCTION: GTA 0x004725a0
void CloseVocalcomFile() {
    if (g_Miles_samples_allocated) {
        fclose(g_File_vocalcom);
    }
}

// FUNCTION: GTA 0x0040c490
void StopMusic() {
    if (g_Music_initialized) {
        g_Music_initialized = false;
        if (g_Music_state != 0) {
            if (g_Music_state == 2 || g_Music_state == 4) {
                int track_pos = AIL_stream_position(g_Music_stream);
                if (g_Always_music) {
                    g_Music_track_infos[g_Music_track + 1].track_position = track_pos;
                } else if (g_Music_track != 8) {
                    g_Audio_track_infos_2[g_Music_track].track_position = track_pos;
                    char buffer[128] = {0};
                    sprintf(buffer, "CD STOP - TRACK = %d, track pos = %d", g_Music_track, track_pos);
                }
                if (g_Music_state == 2 || g_Music_state == 4) {
                    if (g_Music_track < 0 || g_Music_track > 9) {
                        g_Music_track = 5;
                    }
                    AIL_pause_stream(g_Music_stream, 1);
                    AIL_close_stream(g_Music_stream);
                    g_Music_stream = NULL;
                }
            }
            g_Music_state = 1;
        }
        if (g_Music_stream != NULL) {
            AIL_close_stream(g_Music_stream);
            g_Music_stream = NULL;
        }
        g_Music_state = 0;
        if (g_BOOL_004ab2dc) {
            g_BOOL_004ab2dc = false;
        }
    }
}

// FUNCTION: GTA 0x004056c0
void StopGameAudio() {
    if (!g_Miles_running) {
        return;
    }
    g_Miles_running = false;
    CloseVocalcomFile();
    StopMusic();
    StopMilesFromGames();
}

// FUNCTION: GTA 0x00471f10
void StartMilesAudio() {
    if (!g_Miles_memory_allocated) {
        g_Miles_memory = AIL_mem_alloc_lock(1113576);
        if (g_Miles_memory == NULL) {
            FatalError(eFatalError_out_of_memory_allocating_D_bytes, 91, 1113576);
        }
        g_Miles_memory_allocated = true;
    }
    if (!g_Miles_samples_allocated) {
        g_Miles_samples_allocated = true;
        SetSoundEnabled(true);
        AIL_startup();
        PCMWAVEFORMAT format;
        format.wf.nSamplesPerSec = 44100;
        format.wf.nAvgBytesPerSec = 176400;
        format.wf.nBlockAlign = 4;
        format.wBitsPerSample = 16;
        format.wf.wFormatTag = 1;
        format.wf.nChannels = 2;
        AIL_set_preference(0, 12);
        AIL_set_preference(15, 0);
        if (AIL_waveOutOpen(&g_Miles_device, 0, -1, &format) != 0) {
            g_Miles_samples_allocated = false;
            SetSoundEnabled(false);
            return;
        }
        char name[128];
        AIL_digital_configuration(g_Miles_device, 0, 0, name);
        if (strnicmp(name, "Emulated", 8) == 0) {
            AIL_waveOutClose(g_Miles_device);
            AIL_set_preference(15, 1);
            if (AIL_waveOutOpen(&g_Miles_device, 0, -1, &format) != 0) {
                g_Miles_samples_allocated = false;
                SetSoundEnabled(false);
                return;
            }
        }
        g_Level_environment_audio_0077063c = g_Miles_memory;
        g_Level_environment_audio_00770fec = g_Miles_memory;
        {
            for (int i = 0; i < GTA_ASIZE(g_Miles_samples_0077099c); i++) {
                g_Miles_samples_0077099c[i] = AIL_allocate_sample_handle(g_Miles_device);
                AIL_init_sample(g_Miles_samples_0077099c[i]);
                AIL_set_sample_type(g_Miles_samples_0077099c[i], 3, 1);
                AIL_set_sample_loop_count(g_Miles_samples_0077099c[i], 1);
                AIL_set_sample_volume(g_Miles_samples_0077099c[i], (int) (0.79f * (float) g_Master_volume_level));
            }
        }
        {
            for (int i = 0; i < GTA_ASIZE(g_Miles_samples_007709b4); i++) {
                g_Miles_samples_007709b4[i] = AIL_allocate_sample_handle(g_Miles_device);
                AIL_init_sample(g_Miles_samples_007709b4[i]);
                AIL_set_sample_type(g_Miles_samples_007709b4[i], 1, 1);
                AIL_set_sample_loop_count(g_Miles_samples_007709b4[i], 1);
                AIL_set_sample_volume(g_Miles_samples_007709b4[i], (int) (0.79f * (float) g_Master_volume_level));
            }
        }
    }
}

// FUCNTION: GTA 0x0044b510
void FreeMilesBuffers() {
    if (g_Miles_memory_allocated) {
        AIL_mem_free_lock(g_Miles_memory);
        g_Miles_memory_allocated = false;
    }
}

// FUNCTION: GTA 0x00472300
void LoadLevelAudio(int level) {
    if (g_Miles_samples_allocated) {
        char path_no_ext[80];
        sprintf(path_no_ext, "..\\GTADATA\\AUDIO\\LEVEL%03d", level);
        char raw_path[80];
        strcpy(raw_path, path_no_ext);
        strcat(raw_path, ".RAW");
        char sdt_path[80];
        strcpy(sdt_path, path_no_ext);
        strcat(sdt_path, ".SDT");
        FILE *f = fopen(raw_path, "rb");
        if (f == NULL) {
            SetCurrentLoadFilePath(raw_path);
            FatalError(eFatalError_unable_to_open_file, 91, NULL);
        }
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        if (size > 0x100000) {
            SetCurrentLoadFilePath(raw_path);
            FatalError(eFatalError_file_too_large_S_exceeds_max_size_by_D_bytes, 91, size - 0x100000);
        }
        rewind(f);
        fread(g_Level_environment_audio_0077063c, 1, size, f);
        fclose(f);
        f = fopen(sdt_path, "rb");
        if (f == NULL) {
            SetCurrentLoadFilePath(sdt_path);
            FatalError(eFatalError_unable_to_open_file, 91, NULL);
        }
        fread(g_Level_environment_audio_index, sizeof(tStream_play_track_info), 131, f);
        fclose(f);
    }
}

// FUNCTION: GTA 0x0040c300
int InitializeMusicTrackMetadata() {
    int sample_rate = 0;
    int length = 0;
    g_Count_music_tracks = 10;
    {
        for (int i = 0; i < g_Count_music_tracks; i++) {
            if (g_Miles_device == NULL || !g_Miles_samples_allocated) {
                return eFatalError_track_D_not_present_on_cd;
            }
            g_Music_stream = AIL_open_stream(g_Miles_device, g_Music_track_paths[i], 0);
            char buffer[128] = {0};
            if (g_Music_stream != NULL) {
                AIL_stream_info(g_Music_stream, NULL, &sample_rate, &length, NULL);
            }
            sprintf(buffer, "CATALOG: Track %d, Length = %d, SampleRate = %d", i, length, sample_rate);
            if (length <= 0) {
                g_Music_initialized = false;
                g_Music_state = 0;
                AIL_close_stream(g_Music_stream);
                g_Music_stream = NULL;
                return eFatalError_track_D_not_present_on_cd;
            }
            AIL_close_stream(g_Music_stream);
            g_Music_stream = NULL;
            g_Music_track_infos[i].prev_position = 0;
            g_Music_track_infos[i].length = length;
            g_Music_track_infos[i].track_position = 0;
        }
    }
    {
        for (int i = g_Count_music_tracks; i < 99; i++) {
            memset(&g_Music_track_infos[i], 0, sizeof(g_Music_track_infos[i]));
        }
    }
    return 0;
}

// FUNCTION: GTA 0x0040c5c0
bool ActuallyInitMusic() {
    time_t t1 = time(NULL);
    time_t t2 = time(NULL);
    g_Music_track = 0;
    g_INT_004ab2e0 = 5;
    memset(g_Audio_track_infos_2, 0, sizeof(g_Audio_track_infos_2));
    AIL_startup();
    SetMusicStreamEnabled(true);
    if (InitializeMusicTrackMetadata() != 0) {
        return false;
    }
    g_Music_track_info_005015e4.prev_position = g_Music_track_infos[1].prev_position;
    g_Music_track_info_005015e4.length = g_Music_track_infos[1].length;
    g_Music_track_info_005015e4.track_position = g_Music_track_infos[1].track_position;
    for (int i = 0; i < 9; i++) {
        g_Music_track_infos_2[i].prev_position = g_Music_track_infos[i].prev_position;
        g_Music_track_infos_2[i].length = g_Music_track_infos[i].length;
        if (g_Music_track_infos[i].prev_position >= g_Music_track_infos[i].length) {
            SetMusicStreamEnabled(false);
            return false;
        }
        char buffer[128] = {0};
        int offset = (t1 * t2) % (g_Music_track_infos[i].length - g_Music_track_infos[i].prev_position);
        int pos = g_Music_track_infos[i].prev_position + offset;
        g_Music_track_infos_2[i].track_position = pos;
        sprintf(buffer, "CD INIT: Track = %d , Offset = %d, Track_pos = %d", i, offset, pos);
    }
    g_Music_track_infos_2[8].track_position = g_Music_track_infos_2[8].prev_position;
    SetMusicStreamEnabled(true);
    g_Music_state = 1;
    return true;
}

// FUNCTION: GTA 0x0040c470
void InitMusic() {
    if (!g_Music_initialized) {
        if (ActuallyInitMusic()) {
            g_Music_initialized = true;
        }
    }
}

// FUNCTION: GTA 0x00405460
void StartMenuMilesAudio() {
    if (g_Miles_running) {
        return;
    }
    g_Miles_running = true;
    g_Cutscene_music_running = false;
    StartMilesAudio();
    LoadLevelAudio(0);
    if (!g_Demo_version || g_Enable_music) {
        InitMusic();
    }
}

// FUNCTION: GTA 0x00414670
void StartMenuAudio() {
    if (g_BOOL_00502f6c) {
        if (g_Audio_menu_or_game == 1) {
            StopGameAudio();
        }
        StartMenuMilesAudio();
        g_Audio_menu_or_game = 2;
    }
}

// FUNCTION: GTA 0x0040d5c0
void StopMusicBeforeCutscene() {
    if (g_Music_state == 2 || g_Music_state == 4) {
        if (g_Music_track < 0 || g_Music_track > 9) {
            g_Music_track = 5;
        }
        AIL_pause_stream(g_Music_stream, 1);
        AIL_close_stream(g_Music_stream);
        g_Music_stream = NULL;
        g_Music_state = 3;
    }
}

// FUNCTION: GTA 0x00473430
void SetCutsceneMusicvolume(int volume) {
    g_Cutscene_volume = volume;
}

// FUNCTION: GTA 0x004732f0
void StartCutsceneMusicTrack(int mission) {
    if (g_Miles_samples_allocated) {
        char path[80];
        sprintf(path, "..\\GTADATA\\AUDIO\\%d.WAV", mission);
        g_Cutscene_music_stream = AIL_open_stream(g_Miles_device, path, 0);
        if (g_Cutscene_music_stream != NULL) {
            int arg2;
            int length;
            AIL_stream_info(g_Cutscene_music_stream, &arg2, NULL, &length, NULL);
            g_Int_0077100c = length * 500 / arg2;
            g_Cutscene_music_pos = AIL_ms_count();
            AIL_set_stream_volume(g_Cutscene_music_stream, g_Cutscene_volume);
            AIL_start_stream(g_Cutscene_music_stream);
        }
    }
}

// FUNCTION: GTA 0x00404e00
void StartMusicForCutscene(int mission) {
    if (g_Miles_running && g_Sound_enabled && mission < 6) {
        g_Cutscene_music_running = true;
        StopMusicBeforeCutscene();
        SetCutsceneMusicvolume(g_Master_sound_volume);
        StartCutsceneMusicTrack(mission);
    }
}

// FUNCTION: GTA 0x004733e0
int GetCutsceneMusicPosition(void) {
    if (!g_Miles_samples_allocated) {
        return -2;
    }
    if (AIL_stream_status(g_Cutscene_music_stream) != SMP_PLAYING) {
        return -1;
    }
    return 250 * AIL_stream_position(g_Cutscene_music_stream) / 22050;
}

// FUNCTION: GTA 0x00404e60
int GetCutsceneAudioPosition() {
    if (g_Miles_running && g_Sound_enabled && g_Cutscene_music_running) {
        return GetCutsceneMusicPosition();
    }
    return -2;
}

// FUNCTION: GTA 0x004733c0
void CloseCutsceneMusicStream() {
    if (g_Miles_samples_allocated) {
        AIL_close_stream(g_Cutscene_music_stream);
    }
}

// FUNCTION: GTA 0x00404e40
void StopCutsceneMusic() {
    if (g_Miles_running && g_Sound_enabled) {
        g_Cutscene_music_running = false;
        CloseCutsceneMusicStream();
    }
}

// FUNCTION: GTA 0x00472e00
void PlaySample(int sample) {
    int freq_mod;
    void *data = (u8*)g_Level_environment_audio_00770fec + g_Level_environment_audio_index[sample].offset;
    if (g_Level_environment_audio_index[sample].sample_rate != 0) {
        freq_mod = rand() % (g_Level_environment_audio_index[sample].sample_rate / 8);
        if (!g_Sample_freq_mod_sign) {
            freq_mod = -freq_mod;
        }
        g_Sample_freq_mod_sign = !g_Sample_freq_mod_sign;
    } else {
        freq_mod = 0;
    }
    if (sample > 2) {
        g_Level_env_short_samples_1_idx = (g_Level_env_short_samples_1_idx + 1) % GTA_ASIZE(g_Miles_samples_007709b4);
        AIL_set_sample_address(g_Miles_samples_007709b4[g_Level_env_short_samples_1_idx], data, g_Level_environment_audio_index[sample].size);
        AIL_set_sample_loop_count(g_Miles_samples_007709b4[g_Level_env_short_samples_1_idx], 1);
        AIL_set_sample_playback_rate(g_Miles_samples_007709b4[g_Level_env_short_samples_1_idx], g_Level_environment_audio_index[sample].sample_rate);
        AIL_set_sample_pan(g_Miles_samples_007709b4[g_Level_env_short_samples_1_idx], 64 + (rand() % 32));
        AIL_set_sample_volume(g_Miles_samples_007709b4[g_Level_env_short_samples_1_idx], g_Master_volume_level);
        AIL_start_sample(g_Miles_samples_007709b4[g_Level_env_short_samples_1_idx]);
    } else {
        g_Level_env_short_samples_2_idx = (g_Level_env_short_samples_2_idx + 1) % GTA_ASIZE(g_Miles_samples_0077099c);
        AIL_set_sample_address(g_Miles_samples_0077099c[g_Level_env_short_samples_2_idx], data, g_Level_environment_audio_index[sample].size);
        AIL_set_sample_loop_count(g_Miles_samples_0077099c[g_Level_env_short_samples_2_idx], 1);
        AIL_set_sample_playback_rate(g_Miles_samples_0077099c[g_Level_env_short_samples_2_idx], g_Level_environment_audio_index[sample].sample_rate + freq_mod);
        AIL_set_sample_pan(g_Miles_samples_0077099c[g_Level_env_short_samples_2_idx], 64);
        AIL_set_sample_volume(g_Miles_samples_0077099c[g_Level_env_short_samples_2_idx], g_Master_volume_level);
        AIL_start_sample(g_Miles_samples_0077099c[g_Level_env_short_samples_2_idx]);
    }
}

// FUNCTION: GTA 0x00404860
void PlayMenuSound(int index) {
    if (g_Sound_enabled && g_Miles_running) {
        PlaySample(index);
    }
}

// FUNCTION: GTA 0x0040c700
int PlayAudioTrackStream(int track, int offset) {
    if (track < 0 || track > 9) {
        track = 0;
    }
    g_Music_stream = AIL_open_stream(g_Miles_device, g_Music_track_paths[track], 0);
    if (g_Music_stream == NULL) {
        return 0;
    }
    char buffer[32] = {0};
    sprintf(buffer, "STREAMPLAY: Track = %d", g_Music_track);
    AIL_set_stream_volume(g_Music_stream, 0);
    AIL_start_stream(g_Music_stream);
    g_INT_004ab2e4 = 25;
    if (offset < g_Music_track_infos_2[track].length && offset > 0) {
        if (offset % 2 != 1) {
            if (offset + 1 >= g_Music_track_infos_2[track].length) {
                offset -= 1;
            } else {
                offset += 1;
            }
        }
        sprintf(buffer, "Offset Play: %d", offset);
        AIL_set_stream_position(g_Music_stream, offset);
    }
    sprintf(buffer, "STREAMPLAY: Volume = %d", g_Music_stream_volume);
    AIL_set_stream_loop_count(g_Music_stream, 0);
    return 1;
}

// STUB: GTA 0x00405510
void ServiceGameMusic() {
    // FIXME
    NOT_IMPLEMENTED();
}

// FUNCTION: GTA 0x004733a0
void ServiceCutsceneMusicStream() {
    if (g_Miles_samples_allocated) {
        if (g_Cutscene_music_stream != NULL) {
            AIL_service_stream(g_Cutscene_music_stream, 0);
        }
    }
}

// FUNCTION: GTA 0x0040cc80
void ServiceMenuMusicStream() {
    if (g_Music_state != 0) {
        char buffer[20] = {0};
        if (g_Miles_device != NULL && g_Music_stream != NULL) {
            sprintf(buffer, "offset: %s", AIL_stream_position(g_Music_stream));
            if (AIL_stream_status(g_Music_stream) == -1) {
                if (g_BOOL_004ab2dc) {
                    if (PlayAudioTrackStream(0, g_Music_track_infos_2[0].prev_position) == 0) {
                        g_Music_state = 0;
                        return;
                    }
                } else {
                    if (PlayAudioTrackStream(1, g_Music_track_info_005015e4.prev_position) == 0) {
                        g_Music_state = 0;
                        return;
                    }
                }
                g_Music_state = 2;
            }
        }
    }
}

// FUNCTION: GTA 0x004054f0
void ServiceMenuMusic() {
    if (g_Cutscene_music_running) {
        ServiceCutsceneMusicStream();
    } else {
        ServiceMenuMusicStream();
    }
}

// FUNCTION: GTA 0x00414620
void ServiceMusic() {
    if (g_BOOL_00502f6c) {
        switch (g_Audio_menu_or_game) {
        case 1:
            ServiceGameMusic();
            break;
        case 2:
            ServiceMenuMusic();
            break;
        }
    }
}

// FUNCTION: GTA 0x0047dcb0
void StopAndReleaseSpeedLimiter() {
    if (g_Speed_limiter_active) {
        AIL_stop_timer(g_Audio_timer);
        AIL_release_timer_handle(g_Audio_timer);
        g_Speed_limiter_active = FALSE;
    }
}

// FUNCTION: GTA 0x00472fe0
bool IsAnyMenuSoundPlaying(void) {
    {
        for (int i = 0; i < GTA_ASIZE(g_Miles_samples_007709b4); i++) {
            if (AIL_sample_status(g_Miles_samples_007709b4[i]) == 4) {
                return true;
            }
        }
    }
    {
        for (int i = 0; i < GTA_ASIZE(g_Miles_samples_0077099c); i++) {
            if (AIL_sample_status(g_Miles_samples_0077099c[i]) == 4) {
                return true;
            }
        }
    }
    return false;
}

// FUNCTION: GTA 0x00472110
void StopMilesFromMenus() {
    int start = AIL_ms_count();
    if (g_Miles_samples_allocated) {
        while (AIL_ms_count() < start + 1000) {
        }
        g_Miles_samples_allocated = false;
        {
            for (int i = 0; i < GTA_ASIZE(g_Miles_samples_0077099c); i++) {
                AIL_release_sample_handle(g_Miles_samples_0077099c[i]);
            }
        }
        {
            for (int i = 0; i < GTA_ASIZE(g_Miles_samples_007709b4); i++) {
                AIL_release_sample_handle(g_Miles_samples_007709b4[i]);
            }
        }
        AIL_waveOutClose(g_Miles_device);
    }
    AIL_shutdown();
}

// FUNCTION: GTA 0x004054a0
void StopMenuAudio(bool no_exit_sound) {
    if (!g_Miles_running) {
        return;
    }
    g_Miles_running = false;
    g_Cutscene_music_running = false;
    StopMusic();
    if (g_Music_enabled && g_BOOL_004ab058 && !no_exit_sound) {
        PlaySample(15);
        while (IsAnyMenuSoundPlaying()) {
        }
    }
    StopMilesFromMenus();
}

// FUNCTION: GTA 0x004146a0
void StopAudio() {
    if (g_BOOL_00502f6c) {
        switch (g_Audio_menu_or_game) {
        case 1:
            StopGameAudio();
            break;
        case 2:
            StopMenuAudio(true);
            break;
        }
    }
}

// FUNCTION: GTA 0x0040d620
void SetAlwaysMusic(BOOL always_music) {
    g_Always_music = !!always_music;
}

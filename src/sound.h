#ifndef SOUND_H
#define SOUND_H

#include "common.h"

#include <mss.h>

extern HDIGDRIVER g_Miles_device;

void ApplySoundVolumes();

void SetSoundEnabled(bool enabled);

void SetMusicEnabled(bool enabled);

void SetMasterSoundVolumePreset(s8 preset);

void SetMasterMusicVolumePreset(s8 preset);

void SetSoundVolume(u8 level);

void SetMusicVolume(u8 volume, bool enable);

bool StartMilesSoundForIntro();

void StopMilesForIntro();

void StartMenuAudio();

void StartMusicForCutscene(int mission);

void FreeMilesBuffers();

void FreeMilesBuffers();

int GetCutsceneAudioPosition();

void StopCutsceneMusic();

void PlayMenuSound(int index);

void ServiceMusic();

void StopAndReleaseSpeedLimiter();

void StopAudio();

void PlaySample(int sample);

void SetAlwaysMusic(BOOL always_music);

#endif // SOUND_H

#ifndef SOUND_H
#define SOUND_H

#include "common.h"

#include <mss.h>

extern HDIGDRIVER g_Miles_device;

extern void ApplySoundVolumes();

extern void SetSoundEnabled(bool enabled);

extern void SetMusicEnabled(bool enabled);

extern void SetMasterSoundVolumePreset(s8 preset);

extern void SetMasterMusicVolumePreset(s8 preset);

extern void SetSoundVolume(u8 level);

extern void SetMusicVolume(u8 volume, bool enable);

extern bool StartMilesSoundForIntro();

extern void StopMilesForIntro();

extern void StartMenuAudio();

extern void StartMusicForCutscene(int mission);

extern void FreeMilesBuffers();

extern void FreeMilesBuffers();

extern int GetCutsceneAudioPosition();

extern void StopCutsceneMusic();

extern void PlayMenuSound(int index);

extern void ServiceMusic();

extern void StopAndReleaseSpeedLimiter();

extern void StopAudio();

extern void PlaySample(int sample);

extern void SetAlwaysMusic(b32 always_music);

#endif // SOUND_H

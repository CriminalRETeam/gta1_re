#ifndef MSS_H
#define MSS_H

#include <stdlib.h>

#include <windows.h> // mmreg.h
#include <mmreg.h> // WAVEFORMAT

typedef void *HDIGDRIVER;
typedef void *HSAMPLE;
typedef void *HSTREAM;
typedef void *HTIMER;

#ifdef _WIN32
#define AIL_DECL __stdcall
#else
#define AIL_DECL
#endif

#define SMP_PLAYING 4

#ifdef __cplusplus
extern "C" {
#endif

extern int AIL_DECL AIL_startup(void);
extern void AIL_DECL AIL_shutdown(void);
extern void AIL_DECL AIL_set_digital_master_volume(HDIGDRIVER driver, int volume);
extern int AIL_DECL AIL_set_preference(unsigned int, int);
extern void AIL_DECL AIL_digital_configuration(HDIGDRIVER driver, int, int, char *name);
extern void *AIL_DECL AIL_mem_alloc_lock(size_t size);
extern void AIL_DECL AIL_mem_free_lock(void *pointer);
extern void AIL_DECL AIL_set_stream_volume(HSTREAM stream, int volume);
extern HSTREAM AIL_DECL AIL_open_stream(HDIGDRIVER driver, const char *path, int arg3);
extern int AIL_DECL AIL_start_stream(HSTREAM stream);
extern int AIL_DECL AIL_close_stream(HSTREAM stream);
extern int AIL_DECL AIL_pause_stream(HSTREAM stream, int pause);
extern int AIL_DECL AIL_service_stream(HSTREAM stream, int arg2);
extern void AIL_DECL AIL_stream_info(HSTREAM stream, void *arg2, int *sample_rate, int *length, void *arg4);
extern int AIL_DECL AIL_stream_status(HSTREAM stream);
extern int AIL_DECL AIL_stream_position(HSTREAM stream);
extern void AIL_DECL AIL_set_stream_loop_count(HSTREAM, int count);
extern HSAMPLE AIL_DECL AIL_allocate_sample_handle(HDIGDRIVER device);
extern void AIL_DECL AIL_init_sample(HSAMPLE sample);
extern void AIL_DECL AIL_start_sample(HSAMPLE sample);
extern int AIL_DECL AIL_sample_status(HSAMPLE sample);
extern void AIL_DECL AIL_set_sample_address(HSAMPLE sample, void *data, size_t size);
extern void AIL_DECL AIL_set_sample_type(HSAMPLE sample, int property, int value);
extern void AIL_DECL AIL_set_sample_loop_count(HSAMPLE sample, int count);
extern void AIL_DECL AIL_set_sample_playback_rate(HSAMPLE sample, int rate);
extern void AIL_DECL AIL_set_sample_volume(HSAMPLE sample, int volume);
extern void AIL_DECL AIL_set_sample_pan(HSAMPLE sample, int pan);
extern int AIL_DECL AIL_set_stream_position(HSAMPLE, int position);
extern int AIL_DECL AIL_release_sample_handle(HSAMPLE sample);
extern int AIL_DECL AIL_waveOutOpen(HDIGDRIVER *driver, int, int, PCMWAVEFORMAT *waveformat);
extern int AIL_DECL AIL_waveOutClose(HDIGDRIVER driver);
extern int AIL_DECL AIL_ms_count(void);
extern void AIL_DECL AIL_stop_timer(HTIMER timer);
extern void AIL_DECL AIL_release_timer_handle(HTIMER timer);

#ifdef __cplusplus
}
#endif

#endif // MSS_H

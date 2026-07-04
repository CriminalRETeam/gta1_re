#include "mss.h"

#include <stdlib.h>

#include <SDL3/SDL.h>

int AIL_DECL AIL_startup(void) {
	return 0;
}

void AIL_DECL AIL_shutdown(void) {
}

void AIL_DECL AIL_set_digital_master_volume(HDIGDRIVER driver, int volume) {
}

int AIL_DECL AIL_set_preference(unsigned int, int) {
	return 0;
}

void AIL_DECL AIL_digital_configuration(HDIGDRIVER driver, int, int, char *name) {
}

void *AIL_DECL AIL_mem_alloc_lock(size_t size) {
	return malloc(size);
}

void AIL_DECL AIL_mem_free_lock(void *pointer) {
	free(pointer);
}

void AIL_DECL AIL_set_stream_volume(HSTREAM stream, int volume) {
}

HSTREAM AIL_DECL AIL_open_stream(HDIGDRIVER driver, const char *path, int arg3) {
	return NULL;
}

int AIL_DECL AIL_start_stream(HSTREAM stream) {
	return 0;
}

int AIL_DECL AIL_close_stream(HSTREAM stream) {
	return 0;
}

int AIL_DECL AIL_pause_stream(HSTREAM stream, int pause) {
	return 0;
}

int AIL_DECL AIL_service_stream(HSTREAM stream, int arg2) {
	return 0;
}

void AIL_DECL AIL_stream_info(HSTREAM stream, void *arg2, int *sample_rate, int *length, void *arg4) {
}

int AIL_DECL AIL_stream_status(HSTREAM stream) {
	return 0;
}

int AIL_DECL AIL_stream_position(HSTREAM stream) {
	return 0;
}

void AIL_DECL AIL_set_stream_loop_count(HSTREAM, int count) {
}

HSAMPLE AIL_DECL AIL_allocate_sample_handle(HDIGDRIVER device) {
	return NULL;
}

void AIL_DECL AIL_init_sample(HSAMPLE sample) {
}

void AIL_DECL AIL_start_sample(HSAMPLE sample) {
}

int AIL_DECL AIL_sample_status(HSAMPLE sample) {
	return 0;
}

void AIL_DECL AIL_set_sample_address(HSAMPLE sample, void *data, size_t size) {
}

void AIL_DECL AIL_set_sample_type(HSAMPLE sample, int property, int value) {
}

void AIL_DECL AIL_set_sample_loop_count(HSAMPLE sample, int count) {
}

void AIL_DECL AIL_set_sample_playback_rate(HSAMPLE sample, int rate) {
}

void AIL_DECL AIL_set_sample_volume(HSAMPLE sample, int volume) {
}

void AIL_DECL AIL_set_sample_pan(HSAMPLE sample, int pan) {
}

int AIL_DECL AIL_set_stream_position(HSAMPLE, int position) {
	return 0;
}

int AIL_DECL AIL_release_sample_handle(HSAMPLE sample) {
	return 0;
}

int AIL_DECL AIL_waveOutOpen(HDIGDRIVER *driver, int, int, PCMWAVEFORMAT *waveformat) {
	return 0;
}

int AIL_DECL AIL_waveOutClose(HDIGDRIVER driver) {
	return 0;
}

int AIL_DECL AIL_ms_count(void) {
	return SDL_GetTicks();
}

void AIL_DECL AIL_stop_timer(HTIMER timer) {
}

void AIL_DECL AIL_release_timer_handle(HTIMER timer) {
}

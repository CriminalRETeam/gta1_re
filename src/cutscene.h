#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "font.h"
#include "graphics.h"

extern int g_Count_mission_cutscene_fonts[6];

extern const char *g_Cut_font_names[6][4];

extern const char *g_Cut_font_text_names[6][4];

extern tFont *g_City_cut_fonts[4];

extern const char *g_Cut_backscreen_paths[6];

extern tPixelmap g_Cutscreen_pixelmap;

extern const char g_Cut_scene_animation_data[6][5][193];

extern int g_Cut_scene_animation_positions[6][4][2];

extern const char *g_Cut_text_names[6][4];


void PlayDMAIntro();

void LoadCutscenePixelmap(const char *path);

#endif

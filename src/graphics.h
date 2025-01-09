#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"
#include "decomp.h"

#include "font.h"

#include <mgraph.h>
#include <windows.h>

struct tAvailable_mode_list {
    int mode;
    int x;
    int y;
    int bpp;
    tAvailable_mode_list *next;
};

enum {
    e15bpp = 0,
    e16bpp = 1,
    e32bpp = 2,
};

struct tSuitable_video_mode {
    undefined field_0x00[0x14];
    u8 bpp;  // 0: 15bpp, 1: 16bpp, 2: 32bpp
    undefined field_0x15[3];
    tAvailable_mode_list *mode;
};

struct tRect {
    int left;
    int top;
    int right;
    int bottom;
};

struct tPixelmap {
    int w;
    int h;
    union {
        void *pixels;
        u8 *pixels_u8;
        u16 *pixels_u16;
    };
};

extern MGLDC *g_Memory_DC;

extern MGLDC *g_MGLDC_Screen;

extern int g_Pixelformat_type;

extern int g_Mode_red_adjust;

extern int g_Mode_red_pos;

extern int g_Mode_green_adjust;

extern int g_Mode_green_pos;

extern int g_Mode_blue_adjust;

extern int g_Mode_blue_pos;

extern BOOL g_No_cutscene_video_mode;


void InitGraphDriver(HINSTANCE *hInstance);

void AddAvailableMode(tAvailable_mode_list **list, int *mode, int *x_rel, int *y_rel, int *bpp);

//LRESULT CALLBACK FilterEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void QuitGraphics();

void SelectSuitableGraphicsMode(u8 *mode);

void ConfigureDisplay2(int mode);

void SwapScreenBuffer();

void LoadPixelmap(tPixelmap *pixelmap, const char *base_path, int inverse8);

void FreePixelmap(tPixelmap *map);

u8 GetSuitableGraphicsMode();

void FreeMenuBuffers();

void ClearScreenOrCopyCutscene(int mode);

void BlitTextWithKanjiToScreen(tFont *font, int x, int y, const char *text);

void BlitTextCenterAligned(tFont *font, const char *text, int x, int y, int width);

void BlitMenuBackground(BOOL animating_upper_logo, int lower_index);

void BlitHeadTextCentered(int y, const char *text);

void BlitMultilineText(tFont *font, int x, int y, int width, const char *text);

void BlitMenuViewportClippedTextLine(tFont *font, int x, int y, const char *text);

void DrawMenuOptionWithMaybeKey_216(int row, int selected_row, const char *text);

void DrawMenuOptionWithMaybeKey_224(int row, int selected_row, const char *text);

void DrawMultiplayerConnectionNameToScreen(int row, int selected_row, const char *text);

void BlitPixelmap(int x, int y, tPixelmap *src);

void BlitTextToScreen(tFont *font, int x, int y, const char *text);

#endif // GRAPHICS_H

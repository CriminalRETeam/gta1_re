#include "graphics.h"
#include "common.h"
#include "cutscene.h"
#include "decomp.h"
#include "error.h"
#include "interface.h"
#include "menu.h"
#include "multiplayer.h"
#include "text.h"
#include "util.h"

#include <windows.h>
#include <mgraph.h>

#include <sstream>
#include <stdlib.h>

struct tScreen_description {
    int dword_count;
    void *surface;
    int pixel_count;
    int width;
    int height;
    int width_2;
    void *surface_2;
};

// GLOBAL: GTA 0x00503204
int g_Available_mode_counts[3];

// GLOBAL: GTA 0x00503210
struct tAvailable_mode_counts_u8_u32 {
    u8 val_u8[4];
    u32 val_u32;
} g_Available_mode_counts_u8;

// GLOBAL: GTA 0x00504cc8
MGLDC *g_MGLDC_Screen;

// GLOBAL: GTA 0x00504ccc
MGLDC *g_Memory_DC;

// GLOBAL: GTA 0x00504cd0
tSuitable_video_mode *g_Suitable_video_modes;

// GLOBAL: GTA 0x00504cd4
tAvailable_mode_list *g_Available_modes[3];

// GLOBAL: GTA 0x00504cbc
int g_Selected_mode;

// GLOBAL: GTA 0x00503224
u8 g_Suitable_graphics_mode;

// GLOBAL: GTA 0x00503214
int g_Active_page;

// GLOBAL: GTA 0x00503218
int g_Row_stride;

// GLOBAL: GTA 0x00504cc0
int g_Screen_width;

// GLOBAL: GTA 0x00504cc4
int g_Screen_height;

// GLOBAL: GTA 0x00503200
int g_Count_available_pages;

// GLOBAL: GTA 0x004ac06c
BOOL g_No_cutscene_video_mode = TRUE;

// GLOBAL: GTA 0x004b8850
struct tShort_window_range{
    s16 xmin;
    undefined2 field_0x2;
    s16 ymin;
    undefined2 field_0x6;
    s16 xmax;
    undefined2 field_0xa;
    s16 ymax;
    undefined2 field_0xe;
} g_Short_window_range;

// GLOBAL: GTA 0x0078e548
struct tLong_window_range{
    s32 xmin;
    s32 ymin;
    s32 xmax;
    s32 ymax;
} g_Long_window_range;

// GLOBAL: GTA 0x00775318
int g_Mode_red_adjust;

// GLOBAL: GTA 0x007750c8
int g_Mode_red_pos;

// GLOBAL: GTA 0x007752e0
int g_Mode_green_adjust;

// GLOBAL: GTA 0x00775528
int g_Mode_green_pos;

// GLOBAL: GTA 0x00775520
int g_Mode_blue_adjust;

// GLOBAL: GTA 0x007752dc
int g_Mode_blue_pos;

// GLOBAL: GTA 0x00504cb8
int g_Bits_per_pixel;

// GLOBAL: GTA 0x00511590
int g_Pixelformat_type;

// GLOBAL: GTA 0x0050321c
int g_Bytes_per_pixel;

// GLOBAL: GTA 0x00503228
void *g_Row_pointers[1660];

// GLOBAL: GTA 0x00785164
tScreen_description g_Screen_description;

// GLOBAL: GTA 0x00503220
int g_Pixels_per_stride;

// GLOBAL: GTA 0x00511904
void *g_Display_surface;

// GLOBAL: GTA 0x0051156c
u16 *g_Blit_palette;

// GLOBAL: GTA 0x00511898
u8 g_Logo_animation_flags;

// GLOBAL: GTA 0x0051189c
int g_Menu_logo_animation_next_swap;

// GLOBAL: GTA 0x00511950
int g_Menu_logo_animation_index;

// GLOBAL: GTA 0x00511584
int g_Key_frame_index;

// GLOBAL: GTA 0x00511588
int g_Font_key_animation_idx;

// GLOBAL: GTA 0x0051158c
int g_Font_key_mp_session_index;


// FUNCTION: GTA 0x00415950
long CALLBACK FilterEventProc(void *hWnd, unsigned int uMsg, unsigned int wParam, long int lParam) {
//LRESULT CALLBACK FilterEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if ((uMsg >= WM_SYSKEYDOWN && uMsg <= WM_SYSDEADCHAR) || (uMsg == WM_SYSCOMMAND && wParam == SC_SCREENSAVE)) {
        return 0;
    }
    return DefWindowProcA((HWND)hWnd, uMsg, wParam, lParam);
}

// FUNCTION: GTA 0x00415310
void InitGraphDriver(HINSTANCE *hInstance) {
    g_Available_mode_counts[0] = 0;
    g_Available_modes[0] = NULL;
    g_Available_mode_counts[1] = 0;
    g_Available_modes[1] = NULL;
    g_Available_mode_counts[2] = 0;
    g_Available_modes[2] = NULL;
    MGL_setAppInstance(*hInstance);
    MGL_registerDriver(MGL_DDRAW8NAME, DDRAW8_driver);
    MGL_registerDriver(MGL_DDRAW16NAME, DDRAW16_driver);
    MGL_registerDriver(MGL_DDRAW32NAME, DDRAW32_driver);
    MGL_registerDriver(MGL_PACKED8NAME, PACKED8_driver);
    MGL_registerDriver(MGL_PACKED16NAME, PACKED16_driver);
    MGL_registerDriver(MGL_PACKED32NAME, PACKED32_driver);
    int driver = grDETECT;
    int video_mode = -1;
    MGL_detectGraph(&driver, &video_mode);
    uchar *modes = MGL_availableModes();
    int mode_i;
    for (mode_i = 0; modes[mode_i] != 0xff; mode_i++) {
        int mode = modes[mode_i];
        int x_rel, y_rel, bpp;
        MGL_modeResolution(mode, &x_rel, &y_rel, &bpp);
        if (x_rel <= 1600) {
            if (x_rel == y_rel * 8 / 5 || x_rel == y_rel * 4 / 3) {
                int list;
                if (bpp == 15) {
                    list = 0;
                } else if (bpp == 16) {
                    list = 1;
                } else if (bpp == 32) {
                    list = 2;
                } else {
                    continue;
                }
                AddAvailableMode(&g_Available_modes[list], &mode, &x_rel, &y_rel, &bpp);
                g_Available_mode_counts[list] += 1;
            }
        }
    }
    if (mode_i == 0) {
        FatalError(eFatalError_cannot_find_a_suitable_display_mode, 473, 0);
    }
    g_Suitable_video_modes = new tSuitable_video_mode[g_Available_mode_counts[0] + g_Available_mode_counts[1] + g_Available_mode_counts[2]];

    tAvailable_mode_list *amode = g_Available_modes[0];
    for (mode_i = 0; mode_i < g_Available_mode_counts[0]; mode_i++, amode = amode->next) {
        if (g_Selected_mode == -1 && amode->x == 640 && amode->y == 480) {
            g_Selected_mode = mode_i;
        }
        g_Suitable_video_modes[mode_i].bpp = e15bpp;
        g_Suitable_video_modes[mode_i].mode = amode;
        std::stringstream sstream;
        sstream << amode->x << 'x' << amode->y << 'x' << amode->bpp << '\0';
    }

    amode = g_Available_modes[1];
    for (mode_i = g_Available_mode_counts[0]; mode_i < g_Available_mode_counts[0] + g_Available_mode_counts[1]; mode_i++, amode = amode->next) {
        if (g_Selected_mode == -1 && amode->x == 640 && amode->y == 480) {
            g_Selected_mode = mode_i;
        }
        g_Suitable_video_modes[mode_i].bpp = e16bpp;
        g_Suitable_video_modes[mode_i].mode = amode;
        std::stringstream sstream;
        sstream << amode->x << 'x' << amode->y << 'x' << amode->bpp << '\0';
    }

    amode = g_Available_modes[2];
    for (mode_i = g_Available_mode_counts[0] + g_Available_mode_counts[1]; mode_i < g_Available_mode_counts[0] + g_Available_mode_counts[1] + g_Available_mode_counts[2]; mode_i++, amode = amode->next) {
        g_Suitable_video_modes[mode_i].bpp = e32bpp;
        g_Suitable_video_modes[mode_i].mode = amode;
        std::stringstream sstream;
        sstream << amode->x << 'x' << amode->y << 'x' << amode->bpp << '\0';
    }

    if (g_Selected_mode == -1) {
        FatalError(eFatalError_cannot_find_the_default_display_mode, 473, 0);
    }
    if (!MGL_init(&driver, &video_mode, ".")) {
        FatalError(eFatalError_display_error, 502, MGL_errorMsg(MGL_result()));
    }
    MGL_registerEventProc(FilterEventProc);
}

// FUNCTION: GTA 0x00415840
void AddAvailableMode(tAvailable_mode_list **list, int *mode, int *x_rel, int *y_rel, int *bpp) {
    tAvailable_mode_list *node;
    node = *list;
    if (node == NULL || (*x_rel <= node->x && *y_rel < node->y)) {
        *list = new tAvailable_mode_list;
        (*list)->mode = *mode;
        (*list)->x = *x_rel;
        (*list)->y = *y_rel;
        (*list)->bpp = *bpp;
        (*list)->next = node;
        return;
    }
    tAvailable_mode_list *next_node = node->next;
    while (next_node != NULL) {
        if (*x_rel <= next_node->x && *y_rel < next_node->y) {
            next_node = node->next;
            tAvailable_mode_list *new_node = new tAvailable_mode_list;
            node->next = new_node;
            new_node->mode = *mode;
            node->next->x = *x_rel;
            node->next->y = *y_rel;
            node->next->bpp = *bpp;
            node->next->next = next_node;
            return;
        }
        node = next_node;
        next_node = next_node->next;
    }
    next_node = node->next;
    tAvailable_mode_list *new_node = new tAvailable_mode_list;
    node->next = new_node;
    new_node->mode = *mode;
    node->next->x = *x_rel;
    node->next->y = *y_rel;
    node->next->bpp = *bpp;
    node->next->next = next_node;
}

// FUNCTION: GTA 0x004159b0
void QuitGraphics() {
    for (int i = 0; i < GTA_ASIZE(g_Available_modes); i++) {
        tAvailable_mode_list *amode = g_Available_modes[i];
        while (amode != NULL) {
            tAvailable_mode_list *next = amode->next;
            delete amode;
            amode = next;
        }
    }
    if (g_Suitable_video_modes != NULL) {
        delete[] g_Suitable_video_modes;
        g_Suitable_video_modes = NULL;
    }
    MGL_exit();

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, TRUE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// FUNCTION: GTA 0x00414c70
void SelectSuitableGraphicsMode(u8 *mode) {
    // FIXME: this does not make sense
    if (*mode < g_Active_page + 2 * g_Row_stride + g_Available_mode_counts_u8.val_u32 + g_Available_mode_counts[2] + g_Available_mode_counts[1] + g_Available_mode_counts[0]) {
        g_Suitable_graphics_mode = grVGAX_320x200x256;
    } else {
        g_Suitable_graphics_mode = grVGA_320x200x16;
    }
}

void SetWindowRange(int xmin, int ymin, int xmax, int ymax) {
    g_Short_window_range.xmin = xmin;
    g_Long_window_range.xmin = xmin;
    g_Short_window_range.ymin = ymin;
    g_Long_window_range.ymin = ymin;
    g_Short_window_range.xmax = xmax;
    g_Long_window_range.xmax = xmax;
    g_Short_window_range.ymax = ymax;
    g_Long_window_range.ymax = ymax;
}

// FUNCTION: GTA 0x00497bfa
void UpdateRowPointers(void *surface, int row_stride, int height) {
    for (int i = 0; i < height; i++) {
        g_Row_pointers[i] = surface;
        surface = (u8 *)surface + row_stride;
    }
}

// FUNCTION: GTA 0x00486830
void SetScreenDescription(int width, int height, void *surface) {
    g_Screen_description.height = height;
    g_Screen_description.pixel_count = width * height;
    g_Screen_description.width = width;
    g_Screen_description.width_2 = width;
    g_Screen_description.dword_count = g_Screen_description.pixel_count / 4;                          2;
    g_Screen_description.surface = surface;
    g_Screen_description.surface_2 = surface;
}

// FUNCTION: GTA 0x00414db0
void ConfigureDisplay2(int mode) {
    if (g_MGLDC_Screen != NULL) {
        if (g_Memory_DC != NULL) {
            MGL_clearDevice();
            MGL_bitBltCoord(g_MGLDC_Screen, g_Memory_DC,
                0, 0, g_Screen_width, g_Screen_height, 0, 0, 0);
        }
        if (MGL_maxPage(g_MGLDC_Screen) > 0) {
            for (int i = 0; i < g_Count_available_pages; i++) {
                MGL_clearDevice();
                MGL_setVisualPage(g_MGLDC_Screen, g_Active_page, 1);
                g_Active_page += 1;
                g_Active_page %= MGL_maxPage(g_MGLDC_Screen);
                MGL_setActivePage(g_MGLDC_Screen, g_Active_page);
            }
        }
    }
    if (g_Memory_DC != NULL) {
        MGL_destroyDC(g_Memory_DC);
        g_Memory_DC = NULL;
    }
    if (g_MGLDC_Screen != NULL) {
        MGL_destroyDC(g_MGLDC_Screen);
        g_MGLDC_Screen = NULL;
    }
    if (mode == -2) {
        if (!MGL_changeDisplayMode(grSVGA_640x480x256)) {
            g_No_cutscene_video_mode = 1;
            return;
        }
    } else if (mode == -1) {
        if (!MGL_changeDisplayMode(grSVGA_640x480x64k) && !MGL_changeDisplayMode(grSVGA_640x480x32k)) {
            FatalError(eFatalError_display_error, 502, "cannot change to front-end display mode");
        }
    } else {
        if (!MGL_changeDisplayMode(mode)) {
            g_Suitable_graphics_mode = g_Selected_mode;
            mode = g_Suitable_video_modes[g_Selected_mode].mode->mode;
            if (!MGL_changeDisplayMode(mode)) {
                FatalError(eFatalError_display_error, 502, "cannot change to display mode");
            }
        }
    }
    g_Count_available_pages = MGL_availablePages(mode);
    if (g_Count_available_pages < 0 || mode == -2) {
        g_Count_available_pages = 1;
    }
    g_MGLDC_Screen = MGL_createDisplayDC(g_Count_available_pages);
    if (g_MGLDC_Screen == NULL) {
        FatalError(eFatalError_display_error, 502, MGL_errorMsg(MGL_result()));
    }
    MGL_makeCurrentDC(g_MGLDC_Screen);
    MGL_clearDevice();
    if (MGL_maxPage(g_MGLDC_Screen) > 0) {
        for (int i = 0; i < g_Count_available_pages; i++) {
            MGL_clearDevice();
            MGL_setVisualPage(g_MGLDC_Screen, g_Active_page, 1);
            g_Active_page += 1;
            g_Active_page %= MGL_maxPage(g_MGLDC_Screen);
            MGL_setActivePage(g_MGLDC_Screen, g_Active_page);
        }
    }
    g_Screen_width = MGL_maxxDC(g_MGLDC_Screen) + 1;
    g_Screen_height = MGL_maxyDC(g_MGLDC_Screen) + 1;
    MGLDC *draw_dc;
    if (mode == -2 || MGL_maxPage(g_MGLDC_Screen) == 0 || MGL_surfaceAccessType(g_MGLDC_Screen) == MGL_NO_ACCESS) {
        pixel_format_t pf;
        MGL_getPixelFormat(g_MGLDC_Screen, &pf);
        g_Memory_DC = MGL_createMemoryDC(g_Screen_width, g_Screen_height, MGL_getBitsPerPixel(g_MGLDC_Screen), &pf);
        if (g_Memory_DC == NULL) {
            FatalError(eFatalError_display_error, 502, MGL_errorMsg(MGL_result()));
        }
        MGL_makeCurrentDC(g_Memory_DC);
        draw_dc = g_Memory_DC;
    } else {
        g_Memory_DC = NULL;
        draw_dc = g_MGLDC_Screen;
    }
    if (mode == -2) {
        return;
    }
    if (MGL_maxPage(g_MGLDC_Screen) > 0) {
        g_Active_page = 0;
        MGL_setVisualPage(g_MGLDC_Screen, 0, 1);
        g_Active_page += 1;
        g_Active_page %= MGL_maxPage(g_MGLDC_Screen);
        MGL_setActivePage(g_MGLDC_Screen, g_Active_page);
        MGL_clearDevice();
        MGL_setVisualPage(g_MGLDC_Screen, g_Active_page, 1);
        g_Active_page += 1;
        g_Active_page %= MGL_maxPage(g_MGLDC_Screen);
        MGL_setActivePage(g_MGLDC_Screen, g_Active_page);
    }
    SetPlayerScreenDimension(g_Multiplayer_player_id, g_Screen_width, g_Screen_height);
    SetWindowRange(0, 0, g_Screen_width - 1, g_Screen_height - 1);

    g_Mode_red_adjust = draw_dc->pf.redAdjust;
    g_Mode_red_pos = draw_dc->pf.redPos;
    g_Mode_green_adjust = draw_dc->pf.greenAdjust;
    g_Mode_green_pos = draw_dc->pf.greenPos;
    g_Mode_blue_adjust = draw_dc->pf.blueAdjust;
    g_Mode_blue_pos = draw_dc->pf.bluePos;
    g_Bits_per_pixel = MGL_getBitsPerPixel(draw_dc);
    if (g_Bits_per_pixel == 15) {
        g_Pixelformat_type = 0x110;
        g_Bytes_per_pixel = 2;
    } else if (g_Bits_per_pixel == 16) {
        g_Pixelformat_type = 0x111;
        g_Bytes_per_pixel = 2;
    } else if (g_Bits_per_pixel == 32) {
        g_Bytes_per_pixel = 4;
    } else {
        FatalError(eFatalError_invalid_case, 501, draw_dc->mi.bitsPerPixel);
    }
    UpdateRowPointers(draw_dc->surface, draw_dc->mi.bytesPerLine, g_Screen_height);
    g_Row_stride = draw_dc->mi.bytesPerLine;
    g_Pixels_per_stride = draw_dc->mi.bytesPerLine / g_Bytes_per_pixel;
    SetScreenDescription(g_Screen_width, g_Screen_height, draw_dc->surface);
    g_Display_surface = draw_dc->surface;
}

// FUNCTION: GTA 0x00414b10
void SwapScreenBuffer() {
    if (g_Memory_DC != NULL) {
        MGL_bitBltCoord(g_MGLDC_Screen, g_Memory_DC, 0, 0, g_Screen_width, g_Screen_height, 0, 0, 0);
    }
    if (MGL_maxPage(g_MGLDC_Screen) > 0) {
        MGL_setVisualPage(g_MGLDC_Screen, g_Active_page, 1);
        g_Active_page += 1;
        g_Active_page %= MGL_maxPage(g_MGLDC_Screen) + 1;
        MGL_setActivePage(g_MGLDC_Screen, g_Active_page);
        UpdateRowPointers(g_MGLDC_Screen->surface, g_MGLDC_Screen->mi.bytesPerLine, g_Screen_height);
        g_Row_stride = g_MGLDC_Screen->mi.bytesPerLine;
        g_Pixels_per_stride = g_MGLDC_Screen->mi.bytesPerLine / (g_Bytes_per_pixel & 0xff);
        SetScreenDescription(g_Screen_width, g_Screen_height, g_MGLDC_Screen->surface);
        g_Display_surface = g_MGLDC_Screen->surface;
    }
}

// FUNCTION: GTA 0x0042d5f0
void LoadPixelmap(tPixelmap *pixelmap, const char *base_path, int inverse8) {
    if (g_Pixelformat_type == 0x101) {
        sprintf(g_Load_temporary_buffer, "%s.rat", base_path);
    } else {
        sprintf(g_Load_temporary_buffer, "%s.raw", base_path);
    }
    FILE *f = fopen(g_Load_temporary_buffer, "rb");
    if (f == NULL) {
        FatalError(eFatalError_cannot_open_S, 482, g_Load_temporary_buffer);
    }
    if (g_Pixelformat_type == 0x101) {
        int count = fread(pixelmap->pixels, 1, pixelmap->w * pixelmap->h, f);
        if (count != pixelmap->w * pixelmap->h) {
            FatalError(eFatalError_cannot_read_data_from_S, 482, g_Load_temporary_buffer);
        }
        if (inverse8) {
            for (int i = 0; i < pixelmap->w * pixelmap->h; i++) {
                pixelmap->pixels_u8[i] = 255 - pixelmap->pixels_u8[i];
            }
        }
    } else {
        for (int i = 0; i < pixelmap->w * pixelmap->h; i++) {
            int red = fgetc(f);
            if (red == EOF) {
                FatalError(eFatalError_cannot_read_data_from_S, 482, g_Load_temporary_buffer);
            }
            int green = fgetc(f);
            if (green == EOF) {
                FatalError(eFatalError_cannot_read_data_from_S, 482, g_Load_temporary_buffer);
            }
            int blue = fgetc(f);
            if (blue == EOF) {
                FatalError(eFatalError_cannot_read_data_from_S, 482, g_Load_temporary_buffer);
            }
            pixelmap->pixels_u16[i] = ((blue >> g_Mode_blue_adjust) << g_Mode_blue_pos)
                + ((green >> g_Mode_green_adjust) << g_Mode_green_pos)
                + ((red >> g_Mode_red_adjust) << g_Mode_red_pos);
        }
    }
    fclose(f);
}

// FUNCTION: GTA 0x0042d7e0
void FreePixelmap(tPixelmap *map) {
    map->w = 0;
    map->h = 0;
    free(map->pixels);
    map->pixels = NULL;
}

// FUNCTION: GTA 0x00414d30
u8 GetSuitableGraphicsMode() {
    return g_Suitable_graphics_mode;
}

// FUNCTION: GTA 0x0042dc40
void FreeMenuBuffers() {
    g_Pixelformat_type = 0;
    FreePixelmap(&g_Cutscreen_pixelmap);
    FreePixelmap(&g_Menu_upper);
    {
        for (int i = 0; i < GTA_ASIZE(g_Menu_logos); i++) {
            FreePixelmap(&g_Menu_logos[i]);
        }
    }
    {
        for (int i = 0; i < GTA_ASIZE(g_Menu_lowers); i++) {
            FreePixelmap(&g_Menu_lowers[i]);
        }
    }
}

// FUNCTION: GTA 0x0042d830
void ClearScreenOrCopyCutscene(int mode) {
    if (mode == 0) {
        for (int y = 0; y < g_Screen_description.height; y++) {
            void *dst_row_pointer =  (u8 *)g_Display_surface + y * g_Screen_description.width * g_Menu_bytes_per_pixel;
            memset(dst_row_pointer, 640 * g_Menu_bytes_per_pixel, 0);
        }
    } else {
        for (int y = 0; y < g_Screen_description.height; y++) {
            void *src_row_pointer = (u8 *)g_Cutscreen_pixelmap.pixels + y * 640 * g_Menu_bytes_per_pixel;
            void *dst_row_pointer = (u8 *)g_Display_surface + y * g_Screen_description.width * g_Menu_bytes_per_pixel;
            memcpy(dst_row_pointer, src_row_pointer, 640 * g_Menu_bytes_per_pixel);
        }
    }
}

// FUNCTION: GTA 0x00489814
void BlitToScreen640Width(u8 *src, int src_w, int src_h, u8 *dst) {
    // assembly code?
    for (int i = 0; i < src_h; i++) {
        for (int j = 0; j < src_w; j++) {
            if (*src != 0) {
                *dst = *src;
            }
            dst++;
            src++;
        }
        dst += 640 - src_w;
    }
}

// FUNCTION: GTA 0x004898ac
void BlitPalettizedToScreen640Width(u8 *src, int src_w, int src_h, u16 *dst) {
    // assembly code?
    for (int i = 0; i < src_h; i++) {
        for (int j = 0; j < src_w; j++) {
            if (*src != 0) {
                *dst = g_Blit_palette[*src];
            }
            dst++;
            src++;
        }
        dst += 640 - src_w;
    }
}

// FUNCTION: GTA 0x0042bad0
void BlitTextWithKanjiToScreen(tFont *font, int x, int y, const char *text) {
    if (g_Pixelformat_type == 0x101) {
        g_Blit_palette = font->header.palette;
        int display_offset = x + y * g_Screen_description.width;
        SetCurrentFontAndExtra(font);
        int dx = 0;
        int height = GetFontHeight();
        while (text[0] != '\0') {
            u16 character = DecodeCharacterPointer(&text);
            int width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    void *dst = (u8 *)g_Display_surface + display_offset + dx;
                    BlitToScreen640Width((u8 *)GetGlyphData(character), width, height, (u8 *)dst);
                } else {
                    // FIXME: never reached
                    void *dst = (u8 *)g_Display_surface + display_offset + 2 * dx;
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), width, height, (u16 *)dst);
                }
            }
            dx += width;
        }
    } else {
        g_Blit_palette = font->header.palette;
        int display_offset = x + y * g_Screen_description.width;
        SetCurrentFontAndExtra(font);
        int dx = 0;
        int height = GetFontHeight();
        while (text[0] != '\0') {
            u16 character = DecodeCharacterPointer(&text);
            int width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    // FIXME: never reached
                    void *dst = (u8 *)g_Display_surface + dx + 2 * display_offset;
                    BlitToScreen640Width((u8 *)GetGlyphData(character), width, height, (u8 *)dst);
                } else {
                    void *dst = (u8 *)g_Display_surface + 2 * dx + 2 * display_offset;
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), width, height, (u16 *)dst);
                }
            }
            dx += width;
        }
    }
}

// FUNCTION: GTA 0x0042c160
void BlitTextCenterAligned(tFont *font, const char *text, int x, int y, int width) {
    SetCurrentFontAndExtra(font);
    int text_width = 0;
    const char *s = text;
    while (*s != '\0') {
        text_width += GetCharacterWidth(DecodeCharacterPointer(&s));
    }
    int x_center = (width - text_width) / 2;
    if (x_center < 0) {
        x_center = 0;
    }
    if (g_Pixelformat_type == 0x101) {
        g_Blit_palette = font->header.palette;
        int offset = x_center + x + y * g_Screen_description.width;
        SetCurrentFontAndExtra(font);
        s = text;
        int dx = 0;
        int font_height = GetFontHeight();
        while (*s != '\0') {
            u16 glyph = DecodeCharacterPointer(&s);
            int width = GetCharacterWidth(glyph);
            if (glyph != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u8 *)g_Display_surface + offset + dx);
                } else {
                    // FIXME: not reachable
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u16 *)((u8 *)g_Display_surface + offset) + dx);
                }
            }
            dx += width;
        }
    } else {
        g_Blit_palette = font->header.palette;
        int offset = x_center + x + y * g_Screen_description.width;
        SetCurrentFontAndExtra(font);
        s = text;
        int dx = 0;
        int font_height = GetFontHeight();
        while (*s != '\0') {
            u16 glyph = DecodeCharacterPointer(&s);
            int width = GetCharacterWidth(glyph);
            if (glyph != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    // FIXME: not reachable
                    BlitToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u8 *)g_Display_surface + 2 * offset + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u16 *)g_Display_surface + offset + dx);
                }
            }
            dx += width;
        }
    }
}

// FUNCTION: GTA 0x0042d8e0
void BlitMenuBackground(BOOL animating_upper_logo, int lower_index) {
    long lVar1;
    uint uVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    undefined4 *puVar6;
    undefined4 *puVar7;

    if (!(g_Logo_animation_flags & 1)) {
        g_Logo_animation_flags = g_Logo_animation_flags | 1;
        g_Menu_logo_animation_next_swap = GetMillisecondsSinceStartup() + 83;
    }
    if (animating_upper_logo) {
        for (int i = 0; i < 168; i++) {
            memcpy((u8 *)g_Display_surface + i * g_Screen_description.width * g_Menu_bytes_per_pixel,
                (u8 *)g_Menu_logos[g_Menu_logo_animation_index].pixels + i * 640 * g_Menu_bytes_per_pixel,
                640 * g_Menu_bytes_per_pixel);
        }
        if (GetMillisecondsSinceStartup() > g_Menu_logo_animation_next_swap) {
            g_Menu_logo_animation_index += 1;
            if (g_Menu_logo_animation_index > 7) {
                g_Menu_logo_animation_index = 0;
            }
            g_Menu_logo_animation_next_swap = GetMillisecondsSinceStartup() + 83;
        }
    } else {
        for (int i = 0; i < 168; i++) {
            memcpy((u8 *)g_Display_surface + i * g_Screen_description.width * g_Menu_bytes_per_pixel,
               (u8 *)g_Menu_upper.pixels + i * 640 * g_Menu_bytes_per_pixel,
               640 * g_Menu_bytes_per_pixel);
        }
    }
    for (int i = 168; i < 480; i++) {
        memcpy((u8 *)g_Display_surface + i * g_Screen_description.width * g_Menu_bytes_per_pixel,
            (u8 *)g_Menu_lowers[lower_index].pixels + (i - 168) * 640 * g_Menu_bytes_per_pixel,
            640 * g_Menu_bytes_per_pixel);
    }
}

// FUNCTION: GTA 0x0042cd90
void BlitHeadTextCentered(int y, const char *text) {
    SetCurrentFontAndExtra(g_Font_mhead);
    int text_width = 0;
    const char *s = text;
    while (*s != '\0') {
        text_width += GetCharacterWidth(DecodeCharacterPointer(&s));
    }
    int x = 640 / 2 - text_width / 2;
    if (g_Pixelformat_type == 0x101) {
        int offset = x + y * g_Screen_description.width;
        g_Blit_palette = g_Font_mhead->header.palette;
        SetCurrentFontAndExtra(g_Font_mhead);
        s = text;
        int dx = 0;
        int font_height = GetFontHeight();
        while (*s != '\0') {
            u16 glyph = DecodeCharacterPointer(&s);
            int width = GetCharacterWidth(glyph);
            if (glyph != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u8 *)g_Display_surface + offset + dx);
                } else {
                    // FIXME: not reachable
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u16 *)((u8 *)g_Display_surface + offset) + dx);
                }
            }
            dx += width;
        }
    } else {
        g_Blit_palette = g_Font_mhead->header.palette;
        int offset = x + y * g_Screen_description.width;
        SetCurrentFontAndExtra(g_Font_mhead);
        s = text;
        int dx = 0;
        int font_height = GetFontHeight();
        while (*s != '\0') {
            u16 glyph = DecodeCharacterPointer(&s);
            int width = GetCharacterWidth(glyph);
            if (glyph != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    // FIXME: not reachable
                    BlitToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u8 *)g_Display_surface + 2 * offset + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(glyph), width, font_height, (u16 *)g_Display_surface + offset + dx);
                }
            }
            dx += width;
        }
    }
}

// FUNCTION: GTA 0x0042d1c0
void BlitMultilineTextRaw(const char *text, tFont *font, void *dest, int width) {
    g_Blit_palette = font->header.palette;
    SetCurrentFontAndExtra(font);
    int height = GetFontHeight();
    int count_lines;
    const char *multi_text = AdaptMultilineTextForWidth(text, width, &count_lines);
    int dx = 0;
    int dy = 0;
    while (multi_text[0] != '\0') {
        u16 glyph = DecodeCharacterPointer(&multi_text);
        int glyph_width = GetCharacterWidth(glyph);
        if (glyph == '\n') {
            dx = 0;
            dy += g_Screen_description.width * height;
        } else if (glyph == ' ') {
            dx += GetCharacterWidth(0x6e);
        } else {
            if (g_Pixelformat_type == 0x101) {
                BlitToScreen640Width((u8 *)GetGlyphData(glyph), glyph_width, height, (u8 *)dest + dx + dy);
            } else {
                BlitPalettizedToScreen640Width((u8 *)GetGlyphData(glyph), glyph_width, height, (u16 *)dest + dx + dy);
            }
        }
    }
}

// FUNCTION: GTA 0x00428680
void BlitMultilineText(tFont *font, int x, int y, int width, const char *text) {
    if (g_Pixelformat_type == 0x101) {
        BlitMultilineTextRaw(text, font, (u8 *)g_Display_surface + x + y * g_Screen_description.width, width);
    } else {
        BlitMultilineTextRaw(text, font, (u16 *)g_Display_surface + x + y * g_Screen_description.width, width);
    }
}

// FUNCTION: GTA 0x00489851
void BlitPartialToScreen640Width(u8 *src, int src_width, int src_height, u8 *dst, int src_x_left, int src_x_cutoff_right) {
    int draw_width = src_width - (src_x_left + src_x_cutoff_right);
    if (draw_width <= 0) {
        return;
    }
    u8 *src_ptr = src + src_x_left;
    for (int y = 0; y < src_height; y++) {
        for (int x = 0; x < draw_width; x++) {
            if (*src_ptr != 0) {
                *dst = *src_ptr;
            }
            dst += 1;
            src_ptr += 1;
        }
        dst += 640 - draw_width;
        src_ptr += src_x_cutoff_right + src_x_left;
    }
}

// FUNCTION: GTA 0x00489904
void BlitPartialPalettizedToScreen640Width(u8 *src, int src_width, int src_height, u16 *dst, int src_x_left, int src_x_cutoff_right) {
    u16 *palette = g_Blit_palette;
    int draw_width = src_width - (src_x_left + src_x_cutoff_right);
    if (draw_width <= 0) {
        return;
    }
    u8 *src_ptr = src + src_x_left;
    for (int y = 0; y < src_height; y++) {
        for (int x = 0; x < draw_width; x++) {
            if (*src_ptr != 0) {
                *dst = palette[*src_ptr];
            }
            dst += 1;
            src_ptr += 1;
        }
        dst += 640 - draw_width;
        src_ptr += src_x_cutoff_right + src_x_left;
    }
}

// FUNCTION: GTA 0x0042be10
void BlitPartialTextLine(const char *text, void *dst, int src_x, int dst_stride, int src_height, int src_y) {
    if (text[0] == '\0') {
        return;
    }
    int pre_text_width = 0;
    while (pre_text_width <= src_x) {
        if (text[0] == '\0') {
            break;
        }
        pre_text_width += GetCharacterWidth(DecodeCharacterPointer(&text));
    }
    PreviousCharPointer(&text);
    u16 character = DecodeCharacter(text);
    int character_width = GetCharacterWidth(character);
    int character_x = src_x - (pre_text_width - character_width);
    if (character_x < character_width) {
        int dst_x = 0;
        if (character_x != 0) {
            // blit partial first character
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitPartialToScreen640Width(
                        (u8 *)GetGlyphData(character) + src_y * character_width,
                        character_width,
                        src_height,
                        (u8 *)dst,
                        character_x,
                        dst_x);
                } else {
                    BlitPartialPalettizedToScreen640Width(
                        (u8 *)GetGlyphData(character) + src_y * character_width,
                        character_width,
                        src_height,
                        (u16 *)dst,
                        character_x,
                        dst_x);
                }
            }
            dst_x += character_width - character_x;
            NextCharacterPointer(&text);
        }
        if (text[0] == '\0') {
            return;
        }
        for (;;) {
            character = DecodeCharacterPointer(&text);
            character_width = GetCharacterWidth(character);
            int next_dst_x = dst_x + character_width;
            if (next_dst_x > dst_stride) {
                break;
            }
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width(
                        (u8 *)GetGlyphData(character) + src_y * character_width,
                        character_width,
                        src_height,
                        (u8 *)dst + dst_x);
                } else {
                    BlitPalettizedToScreen640Width(
                        (u8 *)GetGlyphData(character) + src_y * character_width,
                        character_width,
                        src_height,
                        (u16 *)dst + dst_x);
                }
            }
            dst_x = next_dst_x;
            if (text[0] == '\0') {
                return;
            }
        }
        int partial_width = (character_width - dst_stride) + dst_x;
        // blit partial last character
        if (partial_width != character_width && character != ' ') {
            if (g_Pixelformat_type == 0x101) {
                BlitPartialToScreen640Width(
                    (u8 *)GetGlyphData(character) + src_y * character_width,
                    character_width,
                    src_height,
                    (u8 *)dst + dst_x,
                    0,
                    partial_width);
            } else {
                BlitPartialPalettizedToScreen640Width(
                    (u8 *)GetGlyphData(character) + src_y * character_width,
                    character_width,
                    src_height,
                    (u16 *)dst + dst_x,
                    0,
                    partial_width);
            }
        }
    }
}


// FUNCTION: GTA 0x0042c030
void BlitMenuViewportClippedTextLine(tFont *font, int x, int y, const char *text) {
    int font_drawn_height_offset;

    SetCurrentFont(font);
    g_Blit_palette = font->header.palette;
    int src_height = GetFontHeight();
    if (y + src_height > g_Menu_viewport.top && y <= g_Menu_viewport.bottom) {
        if (src_height + y > g_Menu_viewport.bottom + 1) {
            src_height = g_Menu_viewport.bottom - y + 1;
        }
        int src_y_start = 0;
        if (y < g_Menu_viewport.top) {
            src_y_start = g_Menu_viewport.top - y;
            src_height -= src_y_start;
            y = g_Menu_viewport.top;
        }
        if (g_Pixelformat_type == 0x101) {
            BlitPartialTextLine(text, (u8 *)g_Display_surface + x + y * g_Screen_description.width,
                         0, g_Screen_description.width, src_height, src_y_start);
        } else {
            BlitPartialTextLine(text, (u16 *)g_Display_surface + g_Screen_description.width * y + x,
                         0, g_Screen_description.width, src_height, src_y_start);
        }
    }
}

// FUNCTION: GTA 0x0042c340
void DrawMenuOptionWithMaybeKey_216(int row, int selected_row, const char *text) {
    int y = 216 + g_Font_mhead->header.height * (row - 1);
    if (g_Pixelformat_type == 0x101) {
        int x_start = y * g_Screen_description.width + 196;
        g_Blit_palette = g_Font_mhead->header.palette;
        SetCurrentFontAndExtra(g_Font_mhead);
        int dx = 0;
        int font_height = GetFontHeight();
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                }
            }
            dx += char_width;
        }
    } else {
        int x_start = sizeof(u16) * (y * g_Screen_description.width + 196);
        g_Blit_palette = g_Font_mhead->header.palette;
        SetCurrentFontAndExtra(g_Font_mhead);
        int dx = 0;
        int font_height = GetFontHeight();
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                }
            }
            dx += char_width;
        }
    }
    if (row == selected_row) {
        g_Key_frame_index += 1;
        if (g_Key_frame_index > 12) {
            g_Key_frame_index = 0;
        }
        char key_text[2];
        key_text[0] = g_Key_frame_index + 1;
        key_text[1] = '\0';
        text = key_text;
        if (g_Pixelformat_type == 0x101) {
            int x_start = (y - 16) * g_Screen_description.width + 116;
            g_Blit_palette = g_Font_key->header.palette;
            SetCurrentFontAndExtra(g_Font_key);
            int dx = 0;
            int font_height = GetFontHeight();
            while (*text != '\0') {
                int character = DecodeCharacterPointer(&text);
                int char_width = GetCharacterWidth(character);
                if (character != ' ') {
                    if (g_Pixelformat_type == 0x101) {
                        BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                    } else {
                        BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                    }
                }
                dx += char_width;
            }
        } else {
            int x_start = sizeof(u16) * ((y - 16) * g_Screen_description.width + 116);
            g_Blit_palette = g_Font_key->header.palette;
            SetCurrentFontAndExtra(g_Font_key);
            int dx = 0;
            int font_height = GetFontHeight();
            while (*text != '\0') {
                int character = DecodeCharacterPointer(&text);
                int char_width = GetCharacterWidth(character);
                if (character != ' ') {
                    if (g_Pixelformat_type == 0x101) {
                        BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                    } else {
                        BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                    }
                }
                dx += char_width;
            }
        }
    }
}

// FUNCTION: GTA 0x0042c340
void DrawMenuOptionWithMaybeKey_224(int row, int selected_row, const char *text) {
    int y = 224 + g_Font_mtext->header.height * (row - 1);
    if (g_Pixelformat_type == 0x101) {
        int x_start = y * g_Screen_description.width + 196;
        g_Blit_palette = g_Font_mtext->header.palette;
        SetCurrentFontAndExtra(g_Font_mtext);
        int dx = 0;
        int font_height = GetFontHeight();
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                }
            }
            dx += char_width;
        }
    } else {
        int x_start = sizeof(u16) * (y * g_Screen_description.width + 196);
        g_Blit_palette = g_Font_mtext->header.palette;
        SetCurrentFontAndExtra(g_Font_mtext);
        int dx = 0;
        int font_height = GetFontHeight();
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                }
            }
            dx += char_width;
        }
    }
    if (row == selected_row) {
        g_Font_key_animation_idx += 1;
        if (g_Font_key_animation_idx > 12) {
            g_Font_key_animation_idx = 0;
        }
        char key_text[2];
        key_text[0] = g_Key_frame_index + 1;
        key_text[1] = '\0';
        text = key_text;
        if (g_Pixelformat_type == 0x101) {
            int x_start = (y - 16) * g_Screen_description.width + 116;
            g_Blit_palette = g_Font_key->header.palette;
            SetCurrentFontAndExtra(g_Font_key);
            int dx = 0;
            int font_height = GetFontHeight();
            while (*text != '\0') {
                int character = DecodeCharacterPointer(&text);
                int char_width = GetCharacterWidth(character);
                if (character != ' ') {
                    if (g_Pixelformat_type == 0x101) {
                        BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                    } else {
                        BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                    }
                }
                dx += char_width;
            }
        } else {
            int x_start = sizeof(u16) * ((y - 16) * g_Screen_description.width + 116);
            g_Blit_palette = g_Font_key->header.palette;
            SetCurrentFontAndExtra(g_Font_key);
            int dx = 0;
            int font_height = GetFontHeight();
            while (*text != '\0') {
                int character = DecodeCharacterPointer(&text);
                int char_width = GetCharacterWidth(character);
                if (character != ' ') {
                    if (g_Pixelformat_type == 0x101) {
                        BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                    } else {
                        BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                    }
                }
                dx += char_width;
            }
        }
    }
}

// FUNCTION: GTA 0x0042ca20
void DrawMultiplayerConnectionNameToScreen(int row, int selected_row, const char *text) {
    int y = 224 + g_Font_mtext->header.height * (row - 1);
    if (g_Pixelformat_type == 0x101) {
        int x_start = y * g_Screen_description.width + 196;
        g_Blit_palette = g_Font_mtext->header.palette;
        SetCurrentFont(g_Font_mtext);
        int dx = 0;
        int font_height = GetFontHeight();
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                }
            }
            dx += char_width;
        }
    } else {
        int x_start = sizeof(u16) * (y * g_Screen_description.width + 196);
        g_Blit_palette = g_Font_mtext->header.palette;
        SetCurrentFont(g_Font_mtext);
        int dx = 0;
        int font_height = GetFontHeight();
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                }
            }
            dx += char_width;
        }
    }
    if (row == selected_row) {
        g_Font_key_mp_session_index += 1;
        if (g_Font_key_mp_session_index > 12) {
            g_Font_key_mp_session_index = 0;
        }
        char key_text[2];
        key_text[0] = g_Font_key_mp_session_index + 1;
        key_text[1] = '\0';
        text = key_text;
        y = 200 + g_Font_mtext->header.height * (row - 1);
        if (g_Pixelformat_type == 0x101) {
            int x_start = y * g_Screen_description.width + 116;
            g_Blit_palette = g_Font_key->header.palette;
            SetCurrentFontAndExtra(g_Font_key);
            int dx = 0;
            int font_height = GetFontHeight();
            while (*text != '\0') {
                int character = DecodeCharacterPointer(&text);
                int char_width = GetCharacterWidth(character);
                if (character != ' ') {
                    if (g_Pixelformat_type == 0x101) {
                        BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                    } else {
                        BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                    }
                }
                dx += char_width;
            }
        } else {
            int x_start = sizeof(u16) * (y * g_Screen_description.width + 116);
            g_Blit_palette = g_Font_key->header.palette;
            SetCurrentFontAndExtra(g_Font_key);
            int dx = 0;
            int font_height = GetFontHeight();
            while (*text != '\0') {
                int character = DecodeCharacterPointer(&text);
                int char_width = GetCharacterWidth(character);
                if (character != ' ') {
                    if (g_Pixelformat_type == 0x101) {
                        BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + x_start + dx);
                    } else {
                        BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + x_start) + dx);
                    }
                }
                dx += char_width;
            }
        }
    }
}

// FUNCTION: GTA 0x0042d490
void BlitPixelmap(int x, int y, tPixelmap *src) {
    int dst_stride = g_Screen_description.width * g_Menu_bytes_per_pixel;
    int src_stride = src->w * g_Menu_bytes_per_pixel;
    int dst_offset;
    int src_offset;
    int w_bytes;
    if (x < g_Menu_viewport.right && x + src->w >= g_Menu_viewport.left) {
        if (x < g_Menu_viewport.left) {
            dst_offset = y * dst_stride;
            src_offset = (g_Menu_viewport.left - x) * g_Menu_bytes_per_pixel;
            w_bytes = (src->w - (g_Menu_viewport.left - x)) * g_Menu_bytes_per_pixel;
        } else {
            dst_offset = (g_Screen_description.width * y + x) * g_Menu_bytes_per_pixel;
            src_offset = 0;
            w_bytes = src_stride;
            if (x + src->w >= g_Menu_viewport.right) {
                w_bytes = (g_Menu_viewport.right - x) * g_Menu_bytes_per_pixel;
            }
        }
        int original_y = y;
        for (y = original_y; y < original_y + src->h; y++) {
            if (y >= g_Menu_viewport.top && y < g_Menu_viewport.bottom) {
                memcpy((u8 *)g_Display_surface + dst_offset, src->pixels_u8 + src_offset, w_bytes);
            }
            dst_offset += dst_stride;
            src_offset += src_stride;
        }
    }
}

// FUNCTION: GTA 0x0042bc70
void BlitTextToScreen(tFont *font, int x, int y, const char *text) {
    if (g_Pixelformat_type == 0x101) {
        g_Blit_palette = font->header.palette;
        int start_offset = x + y * g_Screen_description.width;
        SetCurrentFont(font);
        int font_height = GetFontHeight();
        int dx = 0;
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + start_offset + dx);
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)((u8 *)g_Display_surface + start_offset) + dx);
                }
            }
            dx += char_width;
        }
    } else {
        g_Blit_palette = font->header.palette;
        int start_offset = x + y * g_Screen_description.width;
        SetCurrentFont(font);
        int font_height = GetFontHeight();
        int dx = 0;
        while (*text != '\0') {
            int character = DecodeCharacterPointer(&text);
            int char_width = GetCharacterWidth(character);
            if (character != ' ') {
                if (g_Pixelformat_type == 0x101) {
                    BlitToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u8 *)g_Display_surface + start_offset + dx * sizeof(u16));
                } else {
                    BlitPalettizedToScreen640Width((u8 *)GetGlyphData(character), char_width, font_height, (u16 *)g_Display_surface + start_offset + dx);
                }
            }
            dx += char_width;
        }
    }
}

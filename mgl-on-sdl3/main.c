#include "mgraph.h"

#include <SDL3/SDL.h>

#define WIDTH 640
#define HEIGHT 480

#define FOREACH_MGL_ERROR(X) \
    X(grOK) \
    X(grNoInit) \
    X(grNotDetected) \
    X(grDriverNotFound) \
    X(grBadDriver) \
    X(grLoadMem) \
    X(grInvalidMode) \
    X(grError) \
    X(grInvalidName) \
    X(grNoMem) \
    X(grNoModeSupport) \
    X(grInvalidFont) \
    X(grBadFontFile) \
    X(grFontNotFound) \
    X(grOldDriver) \
    X(grInvalidDevice) \
    X(grInvalidDC) \
    X(grInvalidCursor) \
    X(grCursorNotFound) \
    X(grInvalidIcon) \
    X(grIconNotFound) \
    X(grInvalidBitmap) \
    X(grBitmapNotFound) \
    X(grNewFontFile) \
    X(grNoDoubleBuff) \
    X(grNoHardwareBlt) \
    X(grNoOffscreenMem) \
    X(grInvalidPF) \
    X(grInvalidBuffer) \
    X(grNoDisplayDC) \
    X(grFailLoadRef2d) \
    X(grErrorBPD) \
    X(grInvalidLicense) \
    X(grNeedsFullScreen)

#define X_CASE_RET_STR(V) case V: return #V;

struct mgldc_private_data {
    enum {
        MGLDC_TYPE_MEMORY = 0,
        MGLDC_TYPE_DISPLAY = 1,
    } type;
    SDL_Surface *surface_sdl;
    SDL_Palette *palette_sdl;
    SDL_Color palette_colors[256];
};

#define NOT_IMPLEMENTED() do {SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Not implemented %s %s:%d\n", __func__, __FILE__, __LINE__); SDL_TriggerBreakpoint(); abort(); } while (0)

static struct {
    bool ddraw8;
    bool ddraw16;
    bool ddraw32;
    bool packed8;
    bool packed16;
    bool packed32;
} registered_drivers;


static bool global_sdl_inited = false;
static MGLDC *global_current_mgldc;
static MGL_HINSTANCE global_hinstance;
static MGL_WNDPROC global_userWndProc;
static int global_result = grOK;
static int global_driver;
static int global_mode;
static SDL_Window *global_window;
static SDL_Renderer *global_renderer;

static int bios_scancodes[SDL_SCANCODE_COUNT];

static void init_bios_scancodes() {
    bios_scancodes[SDL_SCANCODE_ESCAPE] = 0x01;
    bios_scancodes[SDL_SCANCODE_1] = 0x02;
    bios_scancodes[SDL_SCANCODE_2] = 0x03;
    bios_scancodes[SDL_SCANCODE_3] = 0x04;
    bios_scancodes[SDL_SCANCODE_4] = 0x05;
    bios_scancodes[SDL_SCANCODE_5] = 0x06;
    bios_scancodes[SDL_SCANCODE_6] = 0x07;
    bios_scancodes[SDL_SCANCODE_7] = 0x08;
    bios_scancodes[SDL_SCANCODE_8] = 0x09;
    bios_scancodes[SDL_SCANCODE_9] = 0x0a;
    bios_scancodes[SDL_SCANCODE_0] = 0x0b;
    bios_scancodes[SDL_SCANCODE_MINUS] = 0x0c;
    bios_scancodes[SDL_SCANCODE_EQUALS] = 0x0d;
    bios_scancodes[SDL_SCANCODE_BACKSPACE] = 0x0e;
    bios_scancodes[SDL_SCANCODE_TAB] = 0x0f;
    bios_scancodes[SDL_SCANCODE_Q] = 0x10;
    bios_scancodes[SDL_SCANCODE_W] = 0x11;
    bios_scancodes[SDL_SCANCODE_E] = 0x12;
    bios_scancodes[SDL_SCANCODE_R] = 0x13;
    bios_scancodes[SDL_SCANCODE_T] = 0x14;
    bios_scancodes[SDL_SCANCODE_Y] = 0x15;
    bios_scancodes[SDL_SCANCODE_U] = 0x16;
    bios_scancodes[SDL_SCANCODE_I] = 0x17;
    bios_scancodes[SDL_SCANCODE_O] = 0x18;
    bios_scancodes[SDL_SCANCODE_P] = 0x19;
    bios_scancodes[SDL_SCANCODE_LEFTBRACKET] = 0x1a;
    bios_scancodes[SDL_SCANCODE_RIGHTBRACKET] = 0x1b;
    bios_scancodes[SDL_SCANCODE_BACKSLASH] = 0x2b;
    bios_scancodes[SDL_SCANCODE_CAPSLOCK] = 0x3a;
    bios_scancodes[SDL_SCANCODE_A] = 0x1e;
    bios_scancodes[SDL_SCANCODE_S] = 0x1f;
    bios_scancodes[SDL_SCANCODE_D] = 0x20;
    bios_scancodes[SDL_SCANCODE_F] = 0x21;
    bios_scancodes[SDL_SCANCODE_G] = 0x22;
    bios_scancodes[SDL_SCANCODE_H] = 0x23;
    bios_scancodes[SDL_SCANCODE_J] = 0x24;
    bios_scancodes[SDL_SCANCODE_K] = 0x25;
    bios_scancodes[SDL_SCANCODE_L] = 0x26;
    bios_scancodes[SDL_SCANCODE_SEMICOLON] = 0x27;
    bios_scancodes[SDL_SCANCODE_APOSTROPHE] = 0x28;
    bios_scancodes[SDL_SCANCODE_RETURN] = 0x1c;
    bios_scancodes[SDL_SCANCODE_LSHIFT] = 0x2a;
    bios_scancodes[SDL_SCANCODE_Z] = 0x2c;
    bios_scancodes[SDL_SCANCODE_X] = 0x2d;
    bios_scancodes[SDL_SCANCODE_C] = 0x2e;
    bios_scancodes[SDL_SCANCODE_V] = 0x2f;
    bios_scancodes[SDL_SCANCODE_B] = 0x30;
    bios_scancodes[SDL_SCANCODE_N] = 0x31;
    bios_scancodes[SDL_SCANCODE_M] = 0x32;
    bios_scancodes[SDL_SCANCODE_COMMA] = 0x33;
    bios_scancodes[SDL_SCANCODE_PERIOD] = 0x34;
    bios_scancodes[SDL_SCANCODE_SLASH] = 0x35;
    bios_scancodes[SDL_SCANCODE_RSHIFT] = 0x36;
    bios_scancodes[SDL_SCANCODE_LCTRL] = 0x58;
    bios_scancodes[SDL_SCANCODE_LALT] = 0x38;
    bios_scancodes[SDL_SCANCODE_SPACE] = 0x39;
    bios_scancodes[SDL_SCANCODE_RALT] = 0x38;
    bios_scancodes[SDL_SCANCODE_RCTRL] = 0x1d;
    bios_scancodes[SDL_SCANCODE_INSERT] = 0x52;
    bios_scancodes[SDL_SCANCODE_HOME] = 0x47;
    bios_scancodes[SDL_SCANCODE_PAGEUP] = 0x49;
    bios_scancodes[SDL_SCANCODE_DELETE] = 0x53;
    bios_scancodes[SDL_SCANCODE_END] = 0x4f;
    bios_scancodes[SDL_SCANCODE_PAGEDOWN] = 0x51;
    bios_scancodes[SDL_SCANCODE_UP] = 0x48;
    bios_scancodes[SDL_SCANCODE_DOWN] = 0x50;
    bios_scancodes[SDL_SCANCODE_LEFT] = 0x4b;
    bios_scancodes[SDL_SCANCODE_RIGHT] = 0x4d;
    bios_scancodes[SDL_SCANCODE_GRAVE] = 0x29;
    bios_scancodes[SDL_SCANCODE_F1] = 0x3b;
    bios_scancodes[SDL_SCANCODE_F2] = 0x3c;
    bios_scancodes[SDL_SCANCODE_F3] = 0x3d;
    bios_scancodes[SDL_SCANCODE_F4] = 0x3e;
    bios_scancodes[SDL_SCANCODE_F5] = 0x3f;
    bios_scancodes[SDL_SCANCODE_F6] = 0x40;
    bios_scancodes[SDL_SCANCODE_F7] = 0x41;
    bios_scancodes[SDL_SCANCODE_F8] = 0x42;
    bios_scancodes[SDL_SCANCODE_F9] = 0x43;
    bios_scancodes[SDL_SCANCODE_F10] = 0x44;
    bios_scancodes[SDL_SCANCODE_F11] = 0x57;
    bios_scancodes[SDL_SCANCODE_F12] = 0x58;
    bios_scancodes[SDL_SCANCODE_NUMLOCKCLEAR] = 0x45;
    bios_scancodes[SDL_SCANCODE_KP_DIVIDE] = 0x35;
    bios_scancodes[SDL_SCANCODE_KP_MULTIPLY] = 0x37;
    bios_scancodes[SDL_SCANCODE_KP_7] = 0x47;
    bios_scancodes[SDL_SCANCODE_KP_8] = 0x48;
    bios_scancodes[SDL_SCANCODE_KP_9] = 0x49;
    bios_scancodes[SDL_SCANCODE_KP_MINUS] = 0x4A;
    bios_scancodes[SDL_SCANCODE_KP_4] = 0x4b;
    bios_scancodes[SDL_SCANCODE_KP_5] = 0x4c;
    bios_scancodes[SDL_SCANCODE_KP_6] = 0x4d;
    bios_scancodes[SDL_SCANCODE_KP_PLUS] = 0x4e;
    bios_scancodes[SDL_SCANCODE_KP_1] = 0x4f;
    bios_scancodes[SDL_SCANCODE_KP_2] = 0x50;
    bios_scancodes[SDL_SCANCODE_KP_3] = 0x51;
}

static void inline ensure_sdl_init() {
    if (!global_sdl_inited) {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
        }
        init_bios_scancodes();
        global_sdl_inited = true;
    }
}

void MGL_setAppInstance(MGL_HINSTANCE hInstApp) {
    global_hinstance = hInstApp;
}

void MGL_registerEventProc(MGL_WNDPROC userWndProc) {
    global_userWndProc = userWndProc;
}

int MGL_registerDriver(const char *name, void *driver) {
    if (SDL_strcmp(name, MGL_DDRAW8NAME) == 0) {
        registered_drivers.ddraw8 = true;
        return grOK;
    } else if (SDL_strcmp(name, MGL_DDRAW16NAME) == 0) {
        registered_drivers.ddraw16 = true;
        return grOK;
    } else if (SDL_strcmp(name, MGL_DDRAW32NAME) == 0) {
        registered_drivers.ddraw32 = true;
        return grOK;
    } else if (SDL_strcmp(name, MGL_PACKED8NAME) == 0) {
        registered_drivers.packed8 = true;
        return grOK;
    } else if (SDL_strcmp(name, MGL_PACKED16NAME) == 0) {
        registered_drivers.packed16 = true;
        return grOK;
    } else if (SDL_strcmp(name, MGL_PACKED32NAME) == 0) {
        registered_drivers.packed32 = true;
        return grOK;
    }
    return grDriverNotFound;
}

void MGL_detectGraph(int *driver, int *mode) {
    ensure_sdl_init();
    *driver = grVGA;
    *mode = grVGA_640x480x16;
}

ibool MGL_init(int *driver, int *mode, const char *mglpath) {
    if (*driver == grDETECT) {
        MGL_detectGraph(driver, mode);
    }
    global_driver = *driver;
    global_mode = *mode;
    global_result = grOK;
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
    {
        global_window = SDL_CreateWindow("regta", WIDTH, HEIGHT, 0);
        if (global_window == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow(NULL, %d, %d, 0) failed: %s", WIDTH, HEIGHT, SDL_GetError());
            global_result = grInvalidMode;
        }
        global_renderer = SDL_CreateRenderer(global_window, NULL);
        SDL_SetRenderVSync(global_renderer, false);
        if (global_window == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed: %s", SDL_GetError());
            SDL_DestroyWindow(global_window);
            global_window = NULL;
            global_result = grInvalidMode;
        }
    }
    return true;
}

void MGL_exit(void) {
    SDL_Quit();
    global_sdl_inited = false;
}

static uchar available_modes[] = {
    grVGA_320x200x16,
    grVGA_640x480x16,
    grSVGA_320x200x256,
    grSVGA_320x240x256,
    grSVGA_640x480x256,
    grSVGA_320x200x32k,
    grSVGA_320x240x32k,
    grSVGA_640x480x32k,
    grSVGA_320x200x4G,
    grSVGA_320x240x4G,
    grSVGA_640x480x4G,
    0xff,
};

uchar *MGL_availableModes(void) {
    return available_modes;
}

int MGL_modeResolution(int mode, int *xRes, int *yRes, int *bitsPerPixel) {
    switch (mode) {
    case grVGA_320x200x16:
        *xRes = 320; *yRes = 200; *bitsPerPixel = 16; break;
    case grVGA_640x480x16:
        *xRes = 640; *yRes = 480; *bitsPerPixel = 16; break;
    case grSVGA_320x200x256:
        *xRes = 320; *yRes = 200; *bitsPerPixel = 8; break;
    case grSVGA_320x240x256:
        *xRes = 320; *yRes = 240; *bitsPerPixel = 8; break;
    case grSVGA_640x480x256:
        *xRes = 640; *yRes = 480; *bitsPerPixel = 8; break;
    case grSVGA_320x200x32k:
        *xRes = 320; *yRes = 200; *bitsPerPixel = 15; break;
    case grSVGA_320x240x32k:
        *xRes = 320; *yRes = 240; *bitsPerPixel = 15; break;
    case grSVGA_640x480x32k:
        *xRes = 640; *yRes = 480; *bitsPerPixel = 15; break;
    case grSVGA_320x200x4G:
        *xRes = 320; *yRes = 200; *bitsPerPixel = 32; break;
    case grSVGA_320x240x4G:
        *xRes = 320; *yRes = 240; *bitsPerPixel = 32; break;
    case grSVGA_640x480x4G:
        *xRes = 640; *yRes = 480; *bitsPerPixel = 32; break;
    default:
        return grInvalidMode;
        NOT_IMPLEMENTED();
    }
    return grOK;
}

void MGL_fatalError(const char *msg) {
    NOT_IMPLEMENTED();
}

int MGL_result(void) {
    int r = global_result;
    global_result = grOK;
    return r;
}

const char *MGL_errorMsg(int err) {
    switch (err) {
    FOREACH_MGL_ERROR(X_CASE_RET_STR)
    default:
        return "Unknown error";
    }
}

static void sdl_pixelformat_fill_in_MGLDC(MGLDC *dc, SDL_PixelFormat pf_sdl) {
    const SDL_PixelFormatDetails *pixelformat_details_sdl = SDL_GetPixelFormatDetails(pf_sdl);
    dc->pf.redMask = pixelformat_details_sdl->Rmask >> pixelformat_details_sdl->Rshift;
    dc->pf.greenMask = pixelformat_details_sdl->Gmask >> pixelformat_details_sdl->Gshift;
    dc->pf.blueMask = pixelformat_details_sdl->Bmask >> pixelformat_details_sdl->Bshift;
    dc->pf.redPos = pixelformat_details_sdl->Rshift;
    dc->pf.greenPos = pixelformat_details_sdl->Gshift;
    dc->pf.bluePos = pixelformat_details_sdl->Bshift;
    dc->pf.redAdjust = 8 - pixelformat_details_sdl->Rbits;
    dc->pf.greenAdjust = 8 - pixelformat_details_sdl->Gbits;
    dc->pf.blueAdjust = 8 - pixelformat_details_sdl->Bbits;

    dc->mi.bitsPerPixel = pixelformat_details_sdl->bits_per_pixel;
}

MGLDC *MGL_createDisplayDC(int numBuffers) {
    int width, height, bits;
    SDL_PixelFormat pixelformat_sdl;
    MGLDC *dc;
    SDL_Surface *surface_sdl;

    if (numBuffers != 1) {
        global_result = grError;
        return NULL;
    }
    if (MGL_modeResolution(global_mode, &width, &height, &bits) != grOK) {
        return NULL;
    }
    switch (bits) {
    case 8:
        pixelformat_sdl = SDL_PIXELFORMAT_INDEX8;
        break;
    case 15:
        pixelformat_sdl = SDL_PIXELFORMAT_XBGR1555;
        break;
    case 16:
        pixelformat_sdl = SDL_PIXELFORMAT_BGR565;
        break;
    default:
        global_result = grError;
        return NULL;
    }
    dc = SDL_calloc(1, sizeof(MGLDC));
    dc->private_data = SDL_calloc(1, sizeof(struct mgldc_private_data));
    SDL_Log("Creating DISPLAY DC with pixelformat %s", SDL_GetPixelFormatName(pixelformat_sdl));
    surface_sdl = SDL_CreateSurface(width, height, pixelformat_sdl);
    if (pixelformat_sdl == SDL_PIXELFORMAT_INDEX8) {
        dc->private_data->palette_sdl = SDL_CreatePalette(256);
        SDL_SetSurfacePalette(surface_sdl, dc->private_data->palette_sdl);
    }
    dc->private_data->surface_sdl = surface_sdl;
    dc->private_data->type = MGLDC_TYPE_DISPLAY;
    sdl_pixelformat_fill_in_MGLDC(dc, pixelformat_sdl);

    dc->mi.xRes = width;
    dc->mi.yRes = height;
    dc->mi.bytesPerLine = surface_sdl->pitch;
    dc->surface = surface_sdl->pixels;
    return dc;
}

MGLDC *MGL_createMemoryDC(int xSize, int ySize, int bitsPerPixel, pixel_format_t *pf) {
    SDL_PixelFormat pixelformat_sdl;
    MGLDC *dc;
    SDL_Surface *surface_sdl;

    switch (bitsPerPixel) {
    case 8:
        pixelformat_sdl = SDL_PIXELFORMAT_INDEX8;
        break;
    case 15:
        pixelformat_sdl = SDL_PIXELFORMAT_XBGR1555;
        if (pf->redMask != 0x1f || pf->greenMask != 0x1f || pf->blueMask != 0x1f) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MGL_createMemoryDC: XRGB1555: unknown masks: redMask=0x%x greenMask=0x%x blueMask=0x%x", pf->redMask, pf->greenMask, pf->blueMask);
            global_result = grError;
            return NULL;
        }
        break;
    case 16:
        pixelformat_sdl = SDL_PIXELFORMAT_BGR565;
        if (pf->redMask != 0x1f || pf->greenMask != 0x3f || pf->blueMask != 0x1f) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MGL_createMemoryDC: RGB565: unknown masks: redMask=0x%x greenMask=0x%x blueMask=0x%x", pf->redMask, pf->greenMask, pf->blueMask);
            global_result = grError;
            return NULL;
        }
        break;
    default:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unknown bitsPerPixel");
        NOT_IMPLEMENTED();
        break;
    }
    dc = SDL_calloc(1, sizeof(MGLDC));
    dc->private_data = SDL_calloc(1, sizeof(struct mgldc_private_data));
    surface_sdl = SDL_CreateSurface(xSize, ySize, pixelformat_sdl);
    SDL_Log("Creating MEMORY DC with pixelformat %s", SDL_GetPixelFormatName(pixelformat_sdl));
    if (pixelformat_sdl == SDL_PIXELFORMAT_INDEX8) {
        dc->private_data->palette_sdl = SDL_CreatePalette(256);
        SDL_SetSurfacePalette(surface_sdl, dc->private_data->palette_sdl);
    }
    dc->private_data->surface_sdl = surface_sdl;
    dc->private_data->type = MGLDC_TYPE_MEMORY;
    sdl_pixelformat_fill_in_MGLDC(dc, pixelformat_sdl);

    dc->mi.xRes = xSize;
    dc->mi.yRes = ySize;
    dc->mi.bytesPerLine = surface_sdl->pitch;
    dc->surface = surface_sdl->pixels;
    return dc;
}

MGLDC *MGL_makeCurrentDC(MGLDC *dc) {
    MGLDC *original = global_current_mgldc;
    global_current_mgldc = dc;
    return original;
}

ibool MGL_destroyDC(MGLDC *dc) {
    if (dc->private_data->surface_sdl) {
        SDL_DestroySurface(dc->private_data->surface_sdl);
    }
    if (dc->private_data->palette_sdl) {
        SDL_DestroyPalette(dc->private_data->palette_sdl);
    }
    return true;
}

int MGL_surfaceAccessType(MGLDC *dc) {
    return MGL_VIRTUAL_ACCESS;
}

int MGL_maxPage(MGLDC *dc) {
    return 0;
}

void MGL_setVisualPage(MGLDC *dc, int page, int waitVRT) {
    if (page != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MGL_setVisualPage(page=%d) UNSUPPORTED");
    }
}

void MGL_setActivePage(MGLDC *dc, int page) {
    if (page != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MGL_setActivePage(page=%d) UNSUPPORTED");
        SDL_TriggerBreakpoint();
    }
}

int MGL_availablePages(int mode) {
    return 1;
}

ibool MGL_changeDisplayMode(int mode) {
    int width;
    int height;
    int bits;
    if (MGL_modeResolution(mode, &width, &height, &bits) != grOK) {
        return false;
    }
    global_mode = mode;
    return true;
}

void MGL_clearDevice(void) {
    SDL_memset(global_current_mgldc->private_data->surface_sdl->pixels, 0, global_current_mgldc->private_data->surface_sdl->h * global_current_mgldc->private_data->surface_sdl->pitch);
}

static void maybe_render_to_screen(MGLDC *dc) {
    if (dc->private_data->type == MGLDC_TYPE_DISPLAY) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(global_renderer, dc->private_data->surface_sdl);
        if (texture == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_bitBltCoord: SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        }
        if (!SDL_RenderTexture(global_renderer, texture, NULL, NULL)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_bitBltCoord: SDL_RenderTexture failed: %s", SDL_GetError());
        }
        SDL_RenderPresent(global_renderer);
        SDL_DestroyTexture(texture);
    }
}

void MGL_setPalette(MGLDC *dc, palette_t *pal, int numColors, int startIndex) {
    int endIndex = SDL_min(startIndex + numColors, SDL_arraysize(dc->private_data->palette_colors));
    {
        int i;
        for (i = startIndex; i < endIndex; i++) {
            dc->private_data->palette_colors[i].r = pal[i].red;
            dc->private_data->palette_colors[i].g = pal[i].green;
            dc->private_data->palette_colors[i].b = pal[i].blue;
            dc->private_data->palette_colors[i].a = SDL_ALPHA_OPAQUE;
        }
    }
}

void MGL_realizePalette(MGLDC *dc, int numColors, int startIndex, int waitVRT) {
    if (!SDL_SetPaletteColors(dc->private_data->palette_sdl, dc->private_data->palette_colors, startIndex, numColors)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MGL_realizePalette: SDL_SetPaletteColors failed: %s", SDL_GetError());
        global_result = grError;
    }
}

void MGL_bitBltCoord(MGLDC *dst, MGLDC *src, int left, int top, int right, int bottom, int dstLeft, int dstTop, int op) {
    SDL_Rect dst_rect;
    dst_rect.x = left;
    dst_rect.y = top;
    dst_rect.w = right - left;
    dst_rect.h = bottom - top;
    SDL_BlitSurface(src->private_data->surface_sdl, NULL, dst->private_data->surface_sdl, &dst_rect);
    maybe_render_to_screen(dst);
}

void MGL_stretchBltCoord(MGLDC *dst, MGLDC *src, int left, int top, int right, int bottom, int dstLeft, int dstTop, int dstRight, int dstBottom) {
    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    src_rect.x = left;
    src_rect.y = top;
    src_rect.w = bottom - top;
    src_rect.h = right - left;
    dst_rect.x = dstLeft;
    dst_rect.y = dstTop;
    dst_rect.w = dstBottom - dstTop;
    dst_rect.h = dstRight - dstLeft;
    if (!SDL_BlitSurfaceScaled(src->private_data->surface_sdl, &src_rect, dst->private_data->surface_sdl, &dst_rect, SDL_SCALEMODE_LINEAR)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "MGL_stretchBltCoord: SDL_BlitSurfaceScaled failes: %s", SDL_GetError());
    }
    maybe_render_to_screen(dst);
}

int MGL_maxxDC(MGLDC *dc) {
    return dc->private_data->surface_sdl->w - 1;
}

int MGL_maxyDC(MGLDC *dc) {
    return dc->private_data->surface_sdl->h - 1;
}

void MGL_getPixelFormat(MGLDC *dc, pixel_format_t *pf) {
    SDL_memcpy(pf, &dc->pf, sizeof(*pf));
}

int MGL_getBitsPerPixel(MGLDC *dc) {
    return dc->mi.bitsPerPixel;
}

void EVT_flush(uint mask) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
    }
}

ibool EVT_getNext(event_t *evt, uint mask) {
    int bios_scancode;
    int key_char;
    SDL_Event event;
    SDL_zerop(evt);
    evt->what = EVT_NULLEVT;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            if (event.type == SDL_EVENT_KEY_DOWN) {
                evt->what = EVT_KEYDOWN;
            } else {
                evt->what = EVT_KEYUP;
            }
            bios_scancode = bios_scancodes[event.key.scancode];
            if (bios_scancode == 0) {
                bios_scancode = 0xff;
            }
            evt->when = (ulong) event.key.timestamp;
            key_char = event.key.key;
            if (key_char >= 0x100) {
                key_char = '\0';
            }
            evt->message = (key_char & 0xff) | ((bios_scancode & 0xff)<< 8);
            if (event.key.mod & SDL_KMOD_LSHIFT) {
                evt->modifiers |= EVT_LEFTSHIFT;
            }
            if (event.key.mod & SDL_KMOD_RSHIFT) {
                evt->modifiers |= EVT_RIGHTSHIFT;
            }
            if (event.key.mod & SDL_KMOD_LCTRL) {
                evt->modifiers |= EVT_LEFTCTRL;
            }
            if (event.key.mod & SDL_KMOD_RCTRL) {
                evt->modifiers |= EVT_CTRLSTATE;
            }
            if (event.key.mod & SDL_KMOD_LALT) {
                evt->modifiers |= EVT_LEFTALT;
            }
            if (event.key.mod & SDL_KMOD_RALT) {
                evt->modifiers |= EVT_ALTSTATE;
            }
            return evt->what;
        }
    }
    return evt->what;
}

void *DDRAW8_driver;
void *DDRAW16_driver;
void *DDRAW32_driver;
void *PACKED8_driver;
void *PACKED16_driver;
void *PACKED32_driver;

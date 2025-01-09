#ifndef MGRAPH_H
#define MGRAPH_H

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef int             ibool;

#ifdef _WIN32
#include "mglwin.h"
#endif

#define MGL_DDRAW8NAME      "DDRAW8.DRV"
#define MGL_DDRAW16NAME     "DDRAW16.DRV"
#define MGL_DDRAW24NAME     "DDRAW24.DRV"
#define MGL_DDRAW32NAME     "DDRAW32.DRV"
#define MGL_PACKED8NAME     "PACK8.DRV"
#define MGL_PACKED16NAME    "PACK16.DRV"
#define MGL_PACKED32NAME    "PACK32.DRV"
typedef enum {
    grOK                = 0,
    grNoInit            = -1,
    grNotDetected       = -2,
    grDriverNotFound    = -3,
    grBadDriver         = -4,
    grLoadMem           = -5,
    grInvalidMode       = -6,
    grError             = -8,
    grInvalidName       = -9,
    grNoMem             = -10,
    grNoModeSupport     = -11,
    grInvalidFont       = -12,
    grBadFontFile       = -13,
    grFontNotFound      = -14,
    grOldDriver         = -15,
    grInvalidDevice     = -16,
    grInvalidDC         = -17,
    grInvalidCursor     = -18,
    grCursorNotFound    = -19,
    grInvalidIcon       = -20,
    grIconNotFound      = -21,
    grInvalidBitmap     = -22,
    grBitmapNotFound    = -23,
    grNewFontFile       = -25,
    grNoDoubleBuff      = -26,
    grNoHardwareBlt     = -28,
    grNoOffscreenMem    = -29,
    grInvalidPF         = -30,
    grInvalidBuffer     = -31,
    grNoDisplayDC       = -32,
    grFailLoadRef2d     = -33,
    grErrorBPD          = -34,
    grInvalidLicense    = -35,
    grNeedsFullScreen   = -36
} MGL_errorType;
typedef enum {
    grDETECT        = -1,   /* Auto detect the graphics subsystem */
    grNONE          = 0,    /* No graphics hardware detected */
    grVGA,                  /* Standard VGA */
    grVESA,                 /* VESA VBE compliant SuperVGA */
    grSVGA,                 /* Unaccelerated SuperVGA */
    grACCEL,                /* Accelerated SuperVGA */
    grDDRAW,                /* Unaccelerated DirectDraw */
    grDDRAWACCEL,           /* Accelerated DirectDraw */
    grDDRAW3D,              /* 3D Accelerated DirectDraw */
    grMAXDRIVER,            /* Maximum driver number */
} MGL_driverType;

typedef enum {
    MGL_NO_ACCESS       = 0x0,
    MGL_VIRTUAL_ACCESS  = 0x1,
    MGL_LINEAR_ACCESS   = 0x2,
    MGL_SURFACE_FLAGS   = 0x3,
} MGL_surfaceAccessFlagsType;

/* Graphics modes supported - the only video modes supported by this
 * graphics library are those that support at least 16 colors per pixel.
 */

typedef enum {
    /* 16 color VGA video modes */

    grVGA_320x200x16,
    grVGA_640x200x16,
    grVGA_640x350x16,
    grVGA_640x400x16,
    grVGA_640x480x16,
    grSVGA_800x600x16,

    /* 256 color VGA ModeX video modes */

    grVGAX_320x200x256,
    grVGAX_320x240x256,
    grVGAX_320x400x256,
    grVGAX_320x480x256,

    /* 256 color VGA video modes */

    grVGA_320x200x256,

    /* 256 color VGA/SuperVGA video modes */

    grSVGA_320x200x256,
    grSVGA_320x240x256,
    grSVGA_320x400x256,
    grSVGA_320x480x256,
    grSVGA_400x300x256,
    grSVGA_512x384x256,
    grSVGA_640x350x256,
    grSVGA_640x400x256,
    grSVGA_640x480x256,
    grSVGA_800x600x256,
    grSVGA_1024x768x256,
    grSVGA_1152x864x256,
    grSVGA_1280x960x256,
    grSVGA_1280x1024x256,
    grSVGA_1600x1200x256,

    /* 32,768 color Super VGA video modes */

    grSVGA_320x200x32k,
    grSVGA_320x240x32k,
    grSVGA_320x400x32k,
    grSVGA_320x480x32k,
    grSVGA_400x300x32k,
    grSVGA_512x384x32k,
    grSVGA_640x350x32k,
    grSVGA_640x400x32k,
    grSVGA_640x480x32k,
    grSVGA_800x600x32k,
    grSVGA_1024x768x32k,
    grSVGA_1152x864x32k,
    grSVGA_1280x960x32k,
    grSVGA_1280x1024x32k,
    grSVGA_1600x1200x32k,

    /* 65,536 color Super VGA video modes */

    grSVGA_320x200x64k,
    grSVGA_320x240x64k,
    grSVGA_320x400x64k,
    grSVGA_320x480x64k,
    grSVGA_400x300x64k,
    grSVGA_512x384x64k,
    grSVGA_640x350x64k,
    grSVGA_640x400x64k,
    grSVGA_640x480x64k,
    grSVGA_800x600x64k,
    grSVGA_1024x768x64k,
    grSVGA_1152x864x64k,
    grSVGA_1280x960x64k,
    grSVGA_1280x1024x64k,
    grSVGA_1600x1200x64k,

    /* 16 million color, 24 bits per pixel Super VGA video modes */

    grSVGA_320x200x16m,
    grSVGA_320x240x16m,
    grSVGA_320x400x16m,
    grSVGA_320x480x16m,
    grSVGA_400x300x16m,
    grSVGA_512x384x16m,
    grSVGA_640x350x16m,
    grSVGA_640x400x16m,
    grSVGA_640x480x16m,
    grSVGA_800x600x16m,
    grSVGA_1024x768x16m,
    grSVGA_1152x864x16m,
    grSVGA_1280x960x16m,
    grSVGA_1280x1024x16m,
    grSVGA_1600x1200x16m,

    /* 16 million color, 32 bits per pixel Super VGA video modes */

    grSVGA_320x200x4G,
    grSVGA_320x240x4G,
    grSVGA_320x400x4G,
    grSVGA_320x480x4G,
    grSVGA_400x300x4G,
    grSVGA_512x384x4G,
    grSVGA_640x350x4G,
    grSVGA_640x400x4G,
    grSVGA_640x480x4G,
    grSVGA_800x600x4G,
    grSVGA_1024x768x4G,
    grSVGA_1152x864x4G,
    grSVGA_1280x960x4G,
    grSVGA_1280x1024x4G,
    grSVGA_1600x1200x4G,

    /* Render into Windowing System DC (Windows, OS/2 PM, X11) */

    grWINDOWED,

    grMAXMODE,  /* Maximum mode number */
} MGL_modeType;

typedef struct {
    uint    redMask, greenMask;
    uint    blueMask;
#if 0
    uint    rsvdMask;
#endif
    int     redPos, redAdjust;
    int     greenPos, greenAdjust;
    int     bluePos, blueAdjust;
#if 0
    int     rsvdPos, rsvdAdjust;
#endif
} pixel_format_t;

typedef struct {
    int __unused_remove_me;
#if 0
    color_t         color;
    color_t         backColor;
    int             colorMode;
    int             markerSize;
    int             markerStyle;
    color_t         markerColor;
    color_t         bdrBright;
    color_t         bdrDark;
    point_t         CP;
    int             writeMode;
    int             penStyle;
    int             penHeight;
    int             penWidth;
    pattern_t       penPat;
    pixpattern_t    penPixPat;
    int             lineStyle;
    uint            lineStipple;
    uint            stippleCount;
    rect_t          viewPort;
    point_t         viewPortOrg;
    rect_t          clipRect;
    int             clip;
    int             polyType;
    text_settings_t ts;
#endif
} attributes_t;

typedef struct {
    int     xRes;
    int     yRes;
    int     bitsPerPixel;
#if 0
    int     numberOfPlanes;
    color_t maxColor;
    int     maxPage;
#endif
    int     bytesPerLine;
#if 0
    int     aspectRatio;
    long    pageSize;
    int     scratch1;
    int     scratch2;
    int     redMaskSize;
    int     redFieldPosition;
    int     greenMaskSize;
    int     greenFieldPosition;
    int     blueMaskSize;
    int     blueFieldPosition;
    int     rsvdMaskSize;
    int     rsvdFieldPosition;
    ulong   modeFlags;
#endif
} gmode_t;

typedef struct MGLDC {
#if 0
    attributes_t a;
#endif
    void *surface;
#if 0
    void *zbuffer;
    int zbits;
    int zwidth;
#endif
    gmode_t mi;
    pixel_format_t pf;
#if 0
    color_t *colorTab;
    color_t *shadeTab;
    int bankOffset;
#endif
    struct mgldc_private_data *private_data;
} MGLDC;

typedef enum {
    EVT_NULLEVT     = 0x0000,
    EVT_KEYDOWN     = 0x0001,
    EVT_KEYREPEAT   = 0x0002,
    EVT_KEYUP       = 0x0004,
    EVT_MOUSEDOWN   = 0x0008,
    EVT_MOUSEUP     = 0x0010,
    EVT_MOUSEMOVE   = 0x0020,
    EVT_TIMERTICK   = 0x0040,
    EVT_USEREVT     = 0x0080,
} MGL_eventType;

#define	EVT_LEFTBUT		0x0001
#define	EVT_RIGHTBUT	0x0002
#define	EVT_RIGHTSHIFT	0x0008
#define	EVT_LEFTSHIFT	0x0010
#define	EVT_CTRLSTATE	0x0020
#define	EVT_ALTSTATE	0x0040
#define	EVT_LEFTCTRL	0x0080
#define	EVT_LEFTALT		0x0100
#define	EVT_SHIFTKEY	0x0018

typedef enum {
    EVT_KEYEVT      = (EVT_KEYDOWN | EVT_KEYREPEAT | EVT_KEYUP),
    EVT_MOUSEEVT    = (EVT_MOUSEDOWN | EVT_MOUSEUP | EVT_MOUSEMOVE),
    EVT_MOUSECLICK  = (EVT_MOUSEDOWN | EVT_MOUSEUP),
    EVT_EVERYEVT    = 0xffff,
} MGL_eventMaskType;

#define EVT_ASCIIMASK   0x00FF      /* Ascii code of key pressed */
#define EVT_SCANMASK    0xFF00      /* Scan code of key pressed */
#define EVT_COUNTMASK   0x7FFF0000L /* Count for KEYREPEAT's */
#define EVT_asciiCode(m)    ( (uchar) (m & EVT_ASCIIMASK) )
#define EVT_scanCode(m)     ( (uchar) ( (m & EVT_SCANMASK) >> 8 ) )
#define EVT_repeatCount(m)  ( (short) ( (m & EVT_COUNTMASK) >> 16 ) )

typedef struct {
    uchar   blue;
    uchar   green;
    uchar   red;
    uchar   alpha;
} palette_t;

typedef struct {
    ulong       which;
    uint        what;
    ulong       when;
    int         where_x;
    int         where_y;
    ulong       message;
    ulong       modifiers;
    int         next;
    int         prev;
} event_t;

#ifdef __cplusplus
extern "C" {
#endif


extern int MGL_registerDriver(const char *name, void *driver);
extern void MGL_detectGraph(int *driver, int *mode);
extern ibool MGL_init(int *driver, int *mode, const char *mglpath);
extern void MGL_exit(void);
extern uchar *MGL_availableModes(void);
extern int MGL_modeResolution(int mode, int *xRes, int *yRes, int *bitsPerPixel);

extern void MGL_fatalError(const char *msg);
extern int MGL_result(void);
extern const char *MGL_errorMsg(int err);

extern MGLDC *MGL_createDisplayDC(int numBuffers);
extern MGLDC *MGL_createMemoryDC(int xSize, int ySize, int bitsPerPixel, pixel_format_t *pf);
extern MGLDC *MGL_makeCurrentDC(MGLDC *dc);
extern ibool MGL_destroyDC(MGLDC *dc);
extern int MGL_surfaceAccessType(MGLDC *dc);

extern int MGL_maxPage(MGLDC *dc);

extern void MGL_setVisualPage(MGLDC *dc, int page, int waitVRT);
extern void MGL_setActivePage(MGLDC *dc, int page);
extern int MGL_availablePages(int mode);
extern ibool MGL_changeDisplayMode(int mode);

extern void MGL_clearDevice(void);

extern void MGL_setPalette(MGLDC *dc, palette_t *pal, int numColors, int startIndex);
extern void MGL_realizePalette(MGLDC *dc, int numColors, int startIndex, int waitVRT);

extern void MGL_bitBltCoord(MGLDC *dst, MGLDC *src, int left, int top, int right, int bottom, int dstLeft, int dstTop, int op);
extern void MGL_stretchBltCoord(MGLDC *dst, MGLDC *src, int left, int top, int right, int bottom, int dstLeft, int dstTop, int dstRight, int dstBottom);

extern int MGL_maxxDC(MGLDC *dc);
extern int MGL_maxyDC(MGLDC *dc);
extern void MGL_getPixelFormat(MGLDC *dc, pixel_format_t *pf);
extern int MGL_getBitsPerPixel(MGLDC *dc);

extern void EVT_flush(uint mask);
extern ibool EVT_getNext(event_t *evt, uint mask);

extern void *DDRAW8_driver;
extern void *DDRAW16_driver;
extern void *DDRAW32_driver;
extern void *PACKED8_driver;
extern void *PACKED16_driver;
extern void *PACKED32_driver;

#ifdef __cplusplus
};
#endif

#endif // MGRAPH_H

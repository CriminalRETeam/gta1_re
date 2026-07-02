#ifndef FONT_H
#define FONT_H

#include "common.h"
#include "decomp.h"

struct tFont_glyph {
    u8 width;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    void *data;
};

struct tFont_header {
    u8 count_glyphs;
    u8 height;
    undefined2 field_0x2;
    undefined4 kanji;
    u16 *palette;
};

struct tFont {
    tFont_header header;
    tFont_glyph glyphs[256];
};

struct tKanji_bit {
    u8 bits[32];
};

extern tFont *g_Font_cuttext;

extern tFont *g_Font_mhead;

extern tFont *g_Font_mtext;

extern tFont *g_Font_key;

extern tFont *g_Font_mmis;

extern tFont *g_Font_cities[4];


extern void LoadKanjiFont(void);

extern void LoadKanjiFont2(tFont **font);

extern void LoadFont(tFont **font, const char *path, undefined2 arg3, bool palette);

extern void UnloadKanjiFont();

extern void LoadMenuFonts();

extern void FreeMenuFonts();

extern void FreeFont(tFont **font);

extern void SetCurrentFont(tFont *font);

extern void SetCurrentFontAndExtra(tFont *font);

extern int GetFontHeight();

extern int GetCharacterWidth(u16 character);

extern void *GetGlyphData(u16 glyph);

extern int GetTextWidthWithExtra(tFont *font, const char *text);

extern int GetTextWidth(tFont *font, const char *text);

extern char *AdaptMultilineTextForWidth(const char *text, int width, int *count_lines);

extern void PreviousCharPointer(const char **ptext);

extern u16 DecodeCharacter(const char *text);

extern void NextCharacterPointer(const char **text);

extern void FreeGameFonts();

#endif // FONT_H

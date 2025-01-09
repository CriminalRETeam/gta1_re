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


void LoadKanjiFont(void);

void LoadKanjiFont2(tFont **font);

void LoadFont(tFont **font, const char *path, undefined2 arg3, bool palette);

void UnloadKanjiFont();

void LoadMenuFonts();

void FreeMenuFonts();

void FreeFont(tFont **font);

void SetCurrentFont(tFont *font);

void SetCurrentFontAndExtra(tFont *font);

int GetFontHeight();

int GetCharacterWidth(u16 character);

void *GetGlyphData(u16 glyph);

int GetTextWidthWithExtra(tFont *font, const char *text);

int GetTextWidth(tFont *font, const char *text);

char *AdaptMultilineTextForWidth(const char *text, int width, int *count_lines);

void PreviousCharPointer(const char **ptext);

u16 DecodeCharacter(const char *text);

void NextCharacterPointer(const char **text);

void FreeGameFonts();

#endif // FONT_H

#include "font.h"
#include "error.h"
#include "graphics.h"
#include "text.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLOBAL: GTA 0x00511580
bool g_Dummy_kanji_font_initialized;

// GLOBAL: GTA 0x0051370c
bool g_Kanji_loaded;

// GLOBAL: GTA 0x005136f8
u16 *g_Kanji_idx;

// GLOBAL: GTA 0x005136fc
tKanji_bit *g_Kanji_bit;

// GLOBAL: GTA 0x00513705
u8 g_Kanji_mask;

// GLOBAL: DATA 0x00513708
undefined *g_Double_kanji_glyph_buffer;

// GLOBAL: DATA 0x00513708;
undefined *g_Single_kanji_glyph_buffer;

// GLOBAL: GTA 0x00511150
tFont *g_Dummy_kanji_font;

// GLOBAL: GTA 0x00511578
tFont *g_Font_cuttext;

// GLOBAL: GTA 0x00511570
tFont *g_Font_mhead;

// GLOBAL: GTA 0x0051157c
tFont *g_Font_mmis;

// GLOBAL: GTA 0x00511568
tFont *g_Font_mtext;

// GLOBAL: GTA 0x00511574
tFont *g_Font_key;

// GLOBAL: GTA 0x00511154
tFont *g_Font_cities[4];

// GLOBAL: GTA 0x00513228
tFont *g_Current_font;

// GLOBAL: GTA 0x00785130
tFont *g_Font_pager;

// GLOBAL: GTA 0x00785158
tFont *g_Font_big;

// GLOBAL: GTA 0x00513704
bool g_Wide_double_kanji;

// GLOBAL: GTA 0x004b0a80
const u16 g_Special_char_widths[128] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    128, 129, 130, 0,   131, 0,   132, 133,
    134, 135, 136, 137, 138, 139, 140, 141,
    0,   0,   142, 143, 144, 0,   145, 0,
    0,   146, 147, 148, 149, 0,   0,   150,
    151, 152, 153, 0,   154, 0,   155, 156,
    157, 158, 159, 160, 161, 162, 163, 164,
    0,   0,   165, 166, 167, 0,   168, 0,
    0,   169, 170, 171, 172, 0,   0,   0,
};

// GLOBAL: GTA 0x00511168
char g_Multi_line_text_buffer[1024];

// GLOBAL: GTA 0x00785150
tFont *g_Font_sub;

// GLOBAL: GTA 0x00785128
tFont *g_Font_street;

// GLOBAL: GTA 0x00785138
tFont *g_Font_expscor;

// GLOBAL: GTA 0x00785140
tFont *g_Font_score;

// GLOBAL: GTA 0x00785148
tFont *g_Font_missmul;

// GLOBAL: GTA 0x0078515c;
BOOL g_Font_big_loaded;

// GLBOAL: GTA 0x00785154
BOOL g_Font_sub_loaded;

// GLOBAL: GTA 0x0078514c
BOOL g_Font_missmul_loaded;

// GLOBAL: GTA 0x0078512c
BOOL g_Font_street_loaded;

// GLOBAL: GTA 0x00785134
BOOL g_Font_pager_loaded;

// GLOBAL: GTA 0x0078513c
BOOL g_Font_expscor_loaded;

// GLOBAL: GTA 0x00785144
BOOL g_Font_score_loaded;


// FUNCTION: GTA 0x0042d2c0
void LoadKanjiFont(void) {
    if (g_Language_is_japanese) {
        LoadKanjiFont2(&g_Dummy_kanji_font);
        g_Dummy_kanji_font_initialized = true;
    }
}

// FUNCTION: GTA 0x004304d0
void LoadFont(tFont **font, const char *path, undefined2 arg3, bool palette) {
    *font = (tFont *)CheckedMalloc(sizeof(tFont));
    u8 *palette_rgb888 = NULL;
    if (palette) {
        (*font)->header.palette = (u16 *)CheckedMalloc(2 * 256);
        palette_rgb888 = (u8 *)CheckedMalloc(3 * 256);
    } else {
        (*font)->header.palette = NULL;
    }
    (*font)->header.kanji = 0;
    (*font)->header.field_0x2 = arg3;
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        FatalError(eFatalError_cannot_open_S, 482, path);
    }
    if (fread(&(*font)->header.count_glyphs, 1, 1, f) != 1) {
        FatalError(eFatalError_cannot_read_data_from_S, 482, path);
    }
    if (fread(&(*font)->header.height, 1, 1, f) != 1) {
        FatalError(eFatalError_cannot_read_data_from_S, 482, path);
    }
    for (int i = 0; i < (*font)->header.count_glyphs; i++) {
        if (fread(&(*font)->glyphs[i].width, 1, 1, f) != 1) {
            FatalError(eFatalError_cannot_read_data_from_S, 482, path);
        }
        int count = (*font)->glyphs[i].width * (*font)->header.height;
        (*font)->glyphs[i].data = CheckedMalloc(count);
        if (fread((*font)->glyphs[i].data, 1, count, f) != count) {
            FatalError(eFatalError_cannot_read_data_from_S, 482, path);
        }
    }
    if (palette && palette_rgb888 != NULL) {
        if (fread(palette_rgb888, 1, 3 * 256, f) != 3 * 256) {
            FatalError(eFatalError_cannot_read_data_from_S, 482, path);
        }
    }
    fclose(f);
    if (palette && palette_rgb888 != NULL) {
        if (g_Pixelformat_type == 0x101) {
            (*font)->header.palette[(*font)->header.count_glyphs] = palette_rgb888[(*font)->header.count_glyphs] / 4;
        } else {
            for (int i = 0; i < 256; i++) {
                int red = (palette_rgb888[3 * i + 0] >> g_Mode_red_adjust) << g_Mode_red_pos;
                int green = (palette_rgb888[3 * i + 1] >> g_Mode_green_adjust) << g_Mode_green_pos;
                int blue = (palette_rgb888[3 * i + 2] >> g_Mode_blue_adjust) << g_Mode_blue_pos;
                (*font)->header.palette[i] = red + green + blue;
            }
        }
        free(palette_rgb888);
    }
}

// FUNCTION: GTA 0x00433d50
void LoadRealKanjiFont() {
    if (!g_Kanji_loaded) {
        size_t size_idx;
        g_Kanji_idx = (u16*)ReadFileToBuffer("..\\gtadata\\kanji.idx", &size_idx);
        size_t size_bit;
        g_Kanji_bit = (tKanji_bit*)ReadFileToBuffer("..\\gtadata\\kanji.bit", &size_bit);
        g_Kanji_loaded = true;
        g_Kanji_mask = 0xff;
        g_Double_kanji_glyph_buffer = (undefined*)CheckedMalloc(0x440);
        g_Single_kanji_glyph_buffer = (undefined*)CheckedMalloc(0x110);
        memset(g_Double_kanji_glyph_buffer, 0, 0x440);
        memset(g_Single_kanji_glyph_buffer, 0, 0x110);
    }
}

// FUNCTION: GTA 0x004304a
void LoadKanjiFont2(tFont **font) {

    *font = (tFont*)CheckedMalloc(sizeof(tFont_header));
    (*font)->header.kanji = 1;
    (*font)->header.height = 16;
    (*font)->header.palette = NULL;
    LoadRealKanjiFont();
}

// FUNCTION: GTA 0x00433de0
void UnloadKanjiFont() {
    if (g_Kanji_loaded) {
        g_Kanji_loaded = false;
        free(g_Kanji_bit);
        free(g_Kanji_idx);
        free(g_Double_kanji_glyph_buffer);
        free(g_Single_kanji_glyph_buffer);
    }
}

// FUNCTION: GTA 0x0042d2e0
void LoadMenuFonts() {
    if (g_Language_is_japanese) {
        LoadKanjiFont2(&g_Dummy_kanji_font);
        g_Dummy_kanji_font_initialized = true;
    }
    LoadFont(&g_Font_cuttext, "..\\gtadata\\cuttext.fon", 0x21, true);
    LoadFont(&g_Font_mhead, "..\\gtadata\\f_mhead.fon", 0x21, true);
    LoadFont(&g_Font_mmis, "..\\gtadata\\f_mmiss.fon", 0x21, true);
    LoadFont(&g_Font_mtext, "..\\gtadata\\f_mtext.fon", 0x21, true);
    LoadFont(&g_Font_key, "..\\gtadata\\f_key.fon", 0x1, true);
    if (g_Pixelformat_type == 0x101) {
        LoadFont(&g_Font_cities[0], "..\\gtadata\\f8_city1.fon", 0x1, true);
        LoadFont(&g_Font_cities[1], "..\\gtadata\\f8_city2.fon", 0x1, true);
        LoadFont(&g_Font_cities[2], "..\\gtadata\\f8_city3.fon", 0x1, true);
        LoadFont(&g_Font_cities[3], "..\\gtadata\\f8_city4.fon", 0x1, true);
    } else {
        LoadFont(&g_Font_cities[0], "..\\gtadata\\f_city1.fon", 0x1, true);
        LoadFont(&g_Font_cities[1], "..\\gtadata\\f_city2.fon", 0x1, true);
        LoadFont(&g_Font_cities[2], "..\\gtadata\\f_city3.fon", 0x1, true);
        LoadFont(&g_Font_cities[3], "..\\gtadata\\f_city4.fon", 0x1, true);
    }
}

// FUNCTION: GTA 0x0042d410
void FreeMenuFonts() {
    FreeFont(&g_Font_cuttext);
    FreeFont(&g_Font_mhead);
    FreeFont(&g_Font_mmis);
    FreeFont(&g_Font_mtext);
    FreeFont(&g_Font_key);
    FreeFont(&g_Font_cities[0]);
    FreeFont(&g_Font_cities[1]);
    FreeFont(&g_Font_cities[2]);
    FreeFont(&g_Font_cities[3]);
}

// FUNCTION: GTA 0x00430750
void FreeFont(tFont **font) {
    if ((*font)->header.kanji == 1) {
        free(*font);
        UnloadKanjiFont();
    } else {
        for (int i = 0; i < (*font)->header.count_glyphs; i++) {
            free((*font)->glyphs[i].data);
        }
        if ((*font)->header.palette != NULL) {
            free((*font)->header.palette);
        }
        free(*font);
        *font = NULL;
    }
}

// FUNCTION: GTA 0x00430840
void SetCurrentFont(tFont *font) {
    g_Current_font = font;
}

// FUNCTION: GTA 0x004307c0
void SetCurrentFontAndExtra(tFont *font) {
    if (!g_Language_is_japanese || font->header.field_0x2 != 0x21) {
        g_Current_font = font;
        return;
    }
    g_Current_font = g_Dummy_kanji_font;
    if (font == g_Font_mhead) {
        g_Wide_double_kanji = true;
        g_Kanji_mask = 0xff;
        return;
    }
    if (font == g_Font_pager) {
        g_Wide_double_kanji = false;
        g_Kanji_mask = 0x68;
        return;
    }
    if (font == g_Font_big) {
        g_Wide_double_kanji = true;
        g_Kanji_mask = 0x37;
        return;
    }
    g_Wide_double_kanji = false;
    g_Kanji_mask = 0xff;
}

// FUNCTION: GTA 0x00430930
int GetFontHeight() {
    if (g_Current_font->header.kanji == 1) {
        return g_Wide_double_kanji ? 34 : 17;
    }
    return g_Current_font->header.height;
}

// FUNCTION: GTA 0x00430850
int GetCharacterWidth(u16 character) {
    if (g_Current_font->header.kanji == 1) {
        return g_Wide_double_kanji ? 32 : 16;
    }
    u16 start = g_Current_font->header.field_0x2;
    u16 end;
    if (character < 0x80) {
        if (g_Current_font->header.field_0x2 == 1 | character != ' ') {
            end = character;
        } else {
            end = 110;
        }
    } else {
        end = g_Special_char_widths[character];
    }
    int glyph_index = end - start;
    if (glyph_index < 0 || glyph_index >= g_Current_font->header.count_glyphs) {
        glyph_index = 0;
    }
    return g_Current_font->glyphs[glyph_index].width;
}

// FUNCTION: GTA 0x00433e30
void *GetKanjiSingleGlyphData(tKanji_bit *kanji_bit) {
    u8 *single_buffer = g_Single_kanji_glyph_buffer;
    for (int i = 0; i < 32; i++) {
        u8 bits = kanji_bit->bits[i];

        single_buffer[8 * i + 0] = (bits & 0x80) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 1] = (bits & 0x40) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 2] = (bits & 0x20) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 3] = (bits & 0x10) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 4] = (bits & 0x08) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 5] = (bits & 0x04) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 6] = (bits & 0x02) ? g_Kanji_mask : 0;
        single_buffer[8 * i + 7] = (bits & 0x01) ? g_Kanji_mask : 0;
    }
    return g_Single_kanji_glyph_buffer;
}

// FUNCTION: GTA 0x00433e30
void *GetKanjiGlyphData(u16 glyph) {
    if (glyph < 256) {
        glyph = glyph << 8;
    }
    u16 index = g_Kanji_idx[glyph];
    if (index == 0xffff) {
        index = g_Kanji_idx[0x2100];
    }
    if (!g_Wide_double_kanji) {
        return GetKanjiSingleGlyphData(&g_Kanji_bit[index]);
    } else {
        u8 *bits = g_Kanji_bit[index].bits;
        u8 *double_buffer = g_Double_kanji_glyph_buffer;
        for (int i = 0; i < 16; i++) {
            u8 v;

            v = (bits[2 * i + 0] & 0x80) ? g_Kanji_mask : 0;
            double_buffer[ 0 +  0] = v;
            double_buffer[ 1 +  0] = v;
            double_buffer[32 +  0] = v;
            double_buffer[33 +  0] = v;

            v = (bits[2 * i + 0] & 0x40) ? g_Kanji_mask : 0;
            double_buffer[ 0 +  2]  = v;
            double_buffer[ 1 +  2] = v;
            double_buffer[32 +  2] = v;
            double_buffer[33 +  2] = v;

            v = (bits[2 * i + 0] & 0x20) ? g_Kanji_mask : 0;
            double_buffer[ 0 +  4] = v;
            double_buffer[ 1 +  4] = v;
            double_buffer[32 +  4] = v;
            double_buffer[33 +  4] = v;

            v = (bits[2 * i + 0] & 0x10) ? g_Kanji_mask : 0;
            double_buffer[ 0 +  6] = v;
            double_buffer[ 1 +  6] = v;
            double_buffer[32 +  6] = v;
            double_buffer[33 +  6] = v;

            v = (bits[2 * i + 0] & 0x08) ? g_Kanji_mask : 0;
            double_buffer[ 0 +  8] = v;
            double_buffer[ 1 +  8] = v;
            double_buffer[32 +  8] = v;
            double_buffer[33 +  8] = v;

            v = (bits[2 * i + 0] & 0x04) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 10] = v;
            double_buffer[ 1 + 10] = v;
            double_buffer[32 + 10] = v;
            double_buffer[33 + 10] = v;

            v = (bits[2 * i + 0] & 0x02) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 12] = v;
            double_buffer[ 1 + 12] = v;
            double_buffer[32 + 12] = v;
            double_buffer[33 + 12] = v;

            v = (bits[2 * i + 0] & 0x01) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 14] = v;
            double_buffer[ 1 + 14] = v;
            double_buffer[32 + 14] = v;
            double_buffer[33 + 14] = v;

            v = (bits[2 * i + 1] & 0x80) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 16] = v;
            double_buffer[ 1 + 16] = v;
            double_buffer[32 + 16] = v;
            double_buffer[33 + 16] = v;

            v = (bits[2 * i + 1] & 0x40) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 18]  = v;
            double_buffer[ 1 + 18] = v;
            double_buffer[32 + 18] = v;
            double_buffer[33 + 18] = v;

            v = (bits[2 * i + 1] & 0x20) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 20] = v;
            double_buffer[ 1 + 20] = v;
            double_buffer[32 + 20] = v;
            double_buffer[33 + 20] = v;

            v = (bits[2 * i + 1] & 0x10) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 22] = v;
            double_buffer[ 1 + 22] = v;
            double_buffer[32 + 22] = v;
            double_buffer[33 + 22] = v;

            v = (bits[2 * i + 1] & 0x08) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 24] = v;
            double_buffer[ 1 + 24] = v;
            double_buffer[32 + 24] = v;
            double_buffer[33 + 24] = v;

            v = (bits[2 * i + 1] & 0x04) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 26] = v;
            double_buffer[ 1 + 26] = v;
            double_buffer[32 + 26] = v;
            double_buffer[33 + 26] = v;

            v = (bits[2 * i + 1] & 0x02) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 28] = v;
            double_buffer[ 1 + 28] = v;
            double_buffer[32 + 28] = v;
            double_buffer[33 + 28] = v;

            v = (bits[2 * i + 1] & 0x01) ? g_Kanji_mask : 0;
            double_buffer[ 0 + 30] = v;
            double_buffer[ 1 + 30] = v;
            double_buffer[32 + 30] = v;
            double_buffer[33 + 30] = v;
        }
        return g_Double_kanji_glyph_buffer;
    }
}

// FUNCTION: GTA 0x004308d0
void *GetGlyphData(u16 glyph) {
    if (g_Current_font->header.kanji == 1) {
        return GetKanjiGlyphData(glyph);
    }
    u16 start = g_Current_font->header.field_0x2;
    u16 end;
    if (glyph & 0x80) {
        end = g_Special_char_widths[glyph];
    } else {
        end = glyph;
    }
    int glyph_index = end - start;
    if (glyph_index < 0 || glyph_index >= g_Current_font->header.count_glyphs) {
        glyph_index = 0;
    }
    return g_Current_font->glyphs[glyph_index].data;
}

// FUNCTION: GTA 0x0042c0e0
int GetTextWidthWithExtra(tFont *font, const char *text) {
    SetCurrentFontAndExtra(font);
    int width = 0;
    while (text[0] != '\0') {
        width += GetCharacterWidth(DecodeCharacterPointer(&text));
    }
    return width;
}

// FUNCTION: GTA 0x0042c120
int GetTextWidth(tFont *font, const char *text) {
    SetCurrentFont(font);
    int width = 0;
    while (text[0] != '\0') {
        width += GetCharacterWidth(DecodeCharacterPointer(&text));
    }
    return width;
}

// FUNCTION: GTA 0x0042b800
char *AdaptMultilineTextForWidth(const char *text, int width, int *count_lines) {
    const char *src_ptr = text;
    int line_width = 0;
    *count_lines = 1;
    char *put_ptr = g_Multi_line_text_buffer;
    const char *prev_space_src = NULL;
    char *prev_space_dst = NULL;
    int len = 0;
    for (;;) {
        u16 character;
        if (!(src_ptr[0] & 0x80)) {
            character = src_ptr[0];
        } else if (!(src_ptr[0] & 0x20)) {
            character = (src_ptr[0] & 0x1f) << 6 | (src_ptr[1] & 0x3f);
        } else {
            character = (src_ptr[0] & 0x1f) << 12 | ((src_ptr[1] & 0x3f)) << 6 | (src_ptr[2] & 0x3f);
        }
        if (character == 0) {
            *put_ptr = '\0';
            return g_Multi_line_text_buffer;
        }
        if (src_ptr[0] & 0x80) {
            if (src_ptr[0] & 0x20) {
                put_ptr[2] = src_ptr[2];
            }
            put_ptr[1] = src_ptr[1];
        }
        put_ptr[0] = src_ptr[0];
        if (!(src_ptr[0] & 0x80)) {
            character = src_ptr[0];
        } else if (!(src_ptr[0] & 0x20)) {
            character = (src_ptr[0] & 0x1f) << 6 | (src_ptr[1] & 0x3f);
        } else {
            character = (src_ptr[0] & 0x1f) << 12 | ((src_ptr[1] & 0x3f)) << 6 | (src_ptr[2] & 0x3f);
        }
        if (character == '\n') {
            prev_space_src = NULL;
            prev_space_dst = NULL;
            line_width = 0;
            *count_lines += 1;
        } else if (character == ' ') {
            line_width += GetCharacterWidth(0x6e);
            prev_space_src = src_ptr;
            prev_space_dst = put_ptr;
        } else if (character == '@') {
            *put_ptr = '\n';
            prev_space_src = NULL;
            prev_space_dst = NULL;
            line_width = 0;
            *count_lines += 1;
        } else {
            if (!(src_ptr[0] & 0x80)) {
                character = src_ptr[0];
            } else if (!(src_ptr[0] & 0x20)) {
                character = (src_ptr[0] & 0x1f) << 6 | (src_ptr[1] & 0x3f);
            } else {
                character = (src_ptr[0] & 0x1f) << 12 | ((src_ptr[1] & 0x3f)) << 6 | (src_ptr[2] & 0x3f);
            }
            line_width += GetCharacterWidth(character);
        }
        if (line_width > width) {
            line_width = 0;
            if (prev_space_src != NULL && prev_space_dst != NULL) {
                *prev_space_dst = '\n';
                src_ptr = prev_space_src;
                put_ptr = prev_space_dst;
                prev_space_src = NULL;
                prev_space_dst = NULL;
            } else {
                *put_ptr = '\n';
                len += 1;
                if (!(src_ptr[0] & 0x80)) {
                    character = src_ptr[0];
                } else if (!(src_ptr[0] & 0x20)) {
                    character = (src_ptr[0] & 0x1f) << 6 | (src_ptr[1] & 0x3f);
                } else {
                    character = (src_ptr[0] & 0x1f) << 12 | ((src_ptr[1] & 0x3f)) << 6 | (src_ptr[2] & 0x3f);
                }
                line_width += GetCharacterWidth(character);
                if (src_ptr[0] & 0x80) {
                    if (src_ptr[0] & 0x20) {
                        put_ptr[3] = src_ptr[2];
                    }
                    put_ptr[2] = src_ptr[1];
                }
                put_ptr[1] = src_ptr[0];
                put_ptr += 1;
            }
            *count_lines += 1;
        }
        if (!(src_ptr[0] & 0x80)) {
            src_ptr += 1;
        } else if (!(src_ptr[0] & 0x20)) {
            src_ptr += 2;
        } else {
            src_ptr += 3;
        }
        if (!(put_ptr[0] & 0x80)) {
            put_ptr += 1;
            len += 1;
        } else if (!(put_ptr[0] & 0x20)) {
            put_ptr += 2;
            len += 2;
        } else {
            put_ptr += 3;
            len += 3;
        }
        if (len > sizeof(g_Multi_line_text_buffer) - 1) {
            FatalError(eFatalError_string_is_too_long_for_text_wrapper, 481, NULL);
        }
    }
}

// FUNCTION: GTA 0x00486730
void PreviousCharPointer(const char **ptext) {
    if (!((*ptext)[-1] & 0x80)) {
        *ptext -= 1;
    } else if ((*ptext)[-2] & 0x40) {
        *ptext -= 2;
    } else {
        *ptext -= 3;
    }
}

// FUNCTION: GTA 0x004866c0
u16 DecodeCharacter(const char *text) {
    if (!(text[0] & 0x80)) {
        return text[0];
    }
    if (!(text[0] & 0x20)) {
        return (text[0] * 0x1f) << 6 | (text[1] & 0x3f);
    }
    return (text[0] & 0x3f) << 12 | (text[1] & 0x3f) << 6 | text[2] & 0x3f;
}

// FUNCTION: GTA 0x00486700
void NextCharacterPointer(const char **text) {
    if (!((*text)[0] & 0x80)) {
        *text = *text + 1;
        return;
    }
    if (!((*text)[0] & 0x20)) {
        *text = *text + 2;
        return;
    }
    *text = *text + 3;
}

// FUNCTION: GTA 0x004832c0
void FreeGameFonts() {
    if (g_Font_big_loaded) {
        g_Font_big_loaded = FALSE;
        FreeFont(&g_Font_big);
    }
    if (g_Font_sub_loaded) {
        g_Font_sub_loaded = FALSE;
        FreeFont(&g_Font_sub);
    }
    if (g_Font_street_loaded) {
        g_Font_street_loaded = FALSE;
        FreeFont(&g_Font_street);
    }
    if (g_Font_pager_loaded) {
        g_Font_pager_loaded = FALSE;
        FreeFont(&g_Font_pager);
    }
    if (g_Font_expscor_loaded) {
        g_Font_expscor_loaded = FALSE;
        FreeFont(&g_Font_expscor);
    }
    if (g_Font_score_loaded) {
        g_Font_score_loaded = FALSE;
        FreeFont(&g_Font_score);
    }
    if (g_Font_missmul_loaded) {
        g_Font_missmul_loaded = FALSE;
        FreeFont(&g_Font_missmul);
    }
}

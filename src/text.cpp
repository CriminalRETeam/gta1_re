#include "text.h"
#include "error.h"
#include "input.h"
#include "util.h"

#include <stdio.h>

#include <windows.h>

// GLOBAL: GTA 0x007537ac
tLanguage g_Language_id_007537ac;

// GLOBAL: GTA 0x00775554
tLanguage g_Language_id_00775554;

// GLOBAL: GTA 0x00775550
BOOL g_Translated_strings_loaded;

// GLOBAL: GTA 0x00775558
bool g_Language_is_japanese;

// GLOBAL: GTA 0x0077555c
char g_Language_data_path[28];

// GLOBAL: GTA 0x00775578
char *g_Translated_strings;

// GLOBAL: GTA 0x006b3ece
const char *g_Direction_texts_keys[9] = {
    "s", "n", "e", "w", "c", "se", "ne", "sw", "nw"
};

// GLOBAL: GTA 0x006b3ece
char g_Direction_texts[9][20];

// GLOBAL: GTA 0x006b3f94
bool g_Translated_directions_initialized;


// FUNCTION: GTA 0x0047d7f0
void SetLanguageDataPath() {
    tLanguage language;
    g_Translated_strings_loaded = FALSE;
    const char *lang_suffix = GetLanguageSuffix();
    if (lang_suffix == NULL) {
        language = eLanguage_english;
    } else {
        language = (tLanguage)(lang_suffix[0] - '0');
    }
    g_Language_id_00775554 = language;
    switch (language) {
    case eLanguage_english:
        strcpy(g_Language_data_path, "..\\gtadata\\english.fxt");
        g_Language_is_japanese = false;
        break;
    case eLanguage_french:
        strcpy(g_Language_data_path, "..\\gtadata\\french.fxt");
        g_Language_is_japanese = false;
        break;
    case eLanguage_german:
        strcpy(g_Language_data_path, "..\\gtadata\\german.fxt");
        g_Language_is_japanese = false;
        break;
    case eLanguage_italian:
        strcpy(g_Language_data_path, "..\\gtadata\\italian.fxt");
        g_Language_is_japanese = false;
        break;
    case eLanguage_japanese:
        strcpy(g_Language_data_path, "..\\gtadata\\japanese.fxt");
        g_Language_is_japanese = true;
        break;
    case eLanguage_special:
        strcpy(g_Language_data_path, "..\\gtadata\\special.fxt");
        g_Language_is_japanese = false;
        break;
    default:
        FatalError(eFatalError_invalid_case, 95, language);
        if (language == eLanguage_japanese) {
            g_Language_is_japanese = true;
        } else {
            g_Language_is_japanese = false;
        }
    }
}

//FUNCTION: GTA 0x0047d9a0
const char *GetTranslatedString(const char *uid) {

    if (!g_Translated_strings_loaded) {
        size_t size;
        char *buffer = static_cast<char *>(ReadFileToBuffer(g_Language_data_path, &size));
        char mod1 = 100;
        char mod2 = 99;
        g_Translated_strings = buffer;
        for (size_t i = 0; i < size; i++) {
            buffer[i] -= mod1;
            mod1 += mod2;
            mod2 *= 2;
        }
        g_Translated_strings_loaded = TRUE;
        if (g_Translated_strings[size - 2] != '[' && g_Translated_strings[size - 1] != ']') {
            FatalError(eFatalError_invalid_format_in_text_file_S, 97, NULL);
        }
    }
    const char *cursor = g_Translated_strings;
    for (;;) {
        while (*cursor != '[') {
            cursor++;
        }
        if (cursor[1] == ']') {
            FatalError(eFatalError_text_identifier_S_not_found_in_language_file, 96, uid);
            return NULL;
        }
        cursor++;
        const char *uid_cursor = uid;
        while (*cursor == *uid_cursor) {
            cursor++;
            uid_cursor++;
            if (*cursor == ']') {
                break;
            }
        }
        if (*uid_cursor == '\0' && *cursor == ']') {
            if (cursor + 1 == NULL) {
                FatalError(eFatalError_text_identifier_S_not_found_in_language_file, 96, uid);
                return NULL;
            }
            return cursor + 1;
        }
    }
}

// FUNCTION: GTA 0x0047d680
void InitLanguageFilePath(tLanguage language) {
    g_Language_id_00775554 = language;
    switch (language) {
    case eLanguage_english:
        strcpy(g_Language_data_path, "..\\gtadata\\english.fxt");
        break;
    case eLanguage_french:
        strcpy(g_Language_data_path, "..\\gtadata\\french.fxt");
        break;
    case eLanguage_german:
        strcpy(g_Language_data_path, "..\\gtadata\\german.fxt");
        break;
    case eLanguage_italian:
        strcpy(g_Language_data_path, "..\\gtadata\\italian.fxt");
        break;
    case eLanguage_japanese:
        strcpy(g_Language_data_path, "..\\gtadata\\japanese.fxt");
        break;
    case eLanguage_special:
        strcpy(g_Language_data_path, "..\\gtadata\\special.fxt");
        break;
    default:
        FatalError(eFatalError_invalid_case, 95, language);
        break;
    }
    g_Language_is_japanese = language == eLanguage_japanese;
}

// FUNCTION: GTA 0x0044b510
void InitDirectionTexts() {
    if (!g_Translated_directions_initialized) {
        for (int i = 0; i < GTA_ASIZE(g_Direction_texts_keys); i++) {
            const char *direction = GetTranslatedString(g_Direction_texts_keys[i]);
            if (strlen(direction) > sizeof(g_Direction_texts[i])) {
                FatalError(eFatalError_text_is_too_long_S, 143, direction);
            }
            sprintf(g_Direction_texts[i], "%s ", direction);
        }
        g_Translated_directions_initialized = true;
    }
}

// FUNCTION: GTA 0x0047d970
void FreeLanguageData() {
    if (g_Translated_strings_loaded) {
        free(g_Translated_strings);
        g_Translated_strings_loaded = FALSE;
    }
}

// FUNCTION: GTA 0x00486670
u16 DecodeCharacterPointer(const char **text) {
    u8 c0 = (*text)[0];
    if (!(c0 & 0x80)) {
        *text += 1;
        return c0;
    }
    u8 c1 = (*text)[1];
    if (!(c0 & 0x20)) {
        (*text) += 2;
        return ((c0 & 0x1f) << 6) | (c1 & 0x3f);
    }
    u8 c2 = (*text)[2];
    (*text) += 3;
    return ((c0 & 0x3f) << 12) | ((c1 & 0x3f) << 6) | ((c2 & 0x3f) << 0);
}

// FUNCTION: GTA 0x0047da90
bool IsTranslated() {
    return g_Language_id_00775554 != eLanguage_english && g_Language_id_00775554 != eLanguage_special;
}

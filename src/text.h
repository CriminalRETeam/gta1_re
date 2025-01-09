#ifndef TEXT_H
#define TEXT_H

#include "common.h"

enum tLanguage {
    eLanguage_english = 0,
    eLanguage_french = 1,
    eLanguage_german = 2,
    eLanguage_italian = 3,
    eLanguage_japanese = 4,
    eLanguage_special = 99,
};

extern tLanguage g_Language_id_007537ac;

extern bool g_Language_is_japanese;


void SetLanguageDataPath();

const char *GetTranslatedString(const char *uid);

void InitLanguageFilePath(tLanguage language);

void InitDirectionTexts();

void FreeLanguageData();

u16 DecodeCharacterPointer(const char **text);

bool IsTranslated();

#endif // TEXT_H

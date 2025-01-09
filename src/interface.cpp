#include "interface.h"
#include "city.h"
#include "text.h"

#include <stdio.h>

// GLOBAL: GTA 0x00502f78
char g_Load_temporary_buffer[256];

// GLOBAL: GTA 0x00511124
bool g_Interface_strings_loaded;

// GLOBAL: GTA 0x00510750
const char *g_Message_sscolon;

// GLOBAL: GTA 0x00510274
const char *g_Message_sdcolon;

// GLOBAL: GTA 0x00510868
const char *g_Message_dscolon;

// GLOBAL: GTA 0x005106ec
const char *g_Message_cdMessage_1;

// GLOBAL: GTA 0x00511098
const char *g_Message_highScores;

// GLOBAL: GTA 0x0051024c
const char *g_Message_winScore;

// GLOBAL: GTA 0x00511088
const char *g_Message_winRace;

// GLOBAL: GTA 0x00510c70
const char *g_Message_winKills;

// GLOBAL: GTA 0x00510754
const char *g_Message_on;

// GLOBAL: GTA 0x00510250
const char *g_Message_off;

// GLOBAL: GTA 0x00511094
const char *g_Message_chapter;

// GLOBAL: GTA 0x005110a4
const char *g_Message_spaceKey;

// GLOBAL: GTA 0x0051109c
const char *g_Message_returnKey;

// GLOBAL: GTA 0x005106d0
const char *g_Message_escKey;

// GLOBAL: GTA 0x005106d0
const char *g_Message_status;

// GLOBAL: GTA 0x00510240
const char *g_Message_quit;

// GLOBAL: GTA 0x005101c8
const char *g_Message_play;

// GLOBAL: GTA 0x005101d4
const char *g_Message_cancel;

// GLOBAL: GTA 0x005110c4
const char *g_Message_commsfail;

// GLOBAL: GTA 0x00510230
const char *g_Message_commsversionerror;

// GLOBAL: GTA 0x00511090
const char *g_Message_svgaerror;

// GLOBAL: GTA 0x00510864
const char *g_Message_loading;

// GLOBAL: GTA 0x00510864
const char *g_Message_loadingDemo;

// GLOBAL: GTA 0x00510210
const char *g_Message_gatherNetwork;

// GLOBAL: GTA 0x00510258
const char *g_Message_joinNetwork;

// GLOBAL: GTA 0x005110c0
const char *g_Message_options;

// GLOBAL: GTA 0x0051107c
const char *g_Message_sound;

// GLOBAL: GTA 0x0051028c
const char *g_Message_music;

// GLOBAL: GTA 0x00510238
const char *g_Message_text;

// GLOBAL: GTA 0x00510244
const char *g_Message_slow;

// GLOBAL: GTA 0x005101ec
const char *g_Message_normal;

// GLOBAL: GTA 0x00510248
const char *g_Message_fast;

// GLOBAL: GTA 0x005110cc
const char *g_Message_musicMode;

// GLOBAL: GTA 0x005110cc
const char *g_Message_transEffects;

// GLOBAL: GTA 0x0051086c
const char *g_Message_constant;

// GLOBAL: GTA 0x00510224
const char *g_Message_radio;

// GLOBAL: GTA 0x005101e8
const char *g_Message_save;

// GLOBAL: GTA 0x005101c0
const char *g_Message_multiOptions;

// GLOBAL: GTA 0x00510200
const char *g_Message_endGame;

// GLOBAL: GTA 0x00510758
const char *g_Message_endScore;

// GLOBAL: GTA 0x005110c8
const char *g_Message_score;

// GLOBAL: GTA 0x005101cc
const char *g_Message_kills;

// GLOBAL: GTA 0x005110d4
const char *g_Message_endKills;

// GLOBAL: GTA 0x00510228
const char *g_Message_scores;

// GLOBAL: GTA 0x005106b8
const char *g_Message_delKey;

// GLOBAL: GTA 0x0051025c
const char *g_Message_rename;

// GLOBAL: GTA 0x0051020c
const char *g_Message_rKey;

// GLOBAL: GTA 0x0051020c
const char *g_Message_reset;

// GLOBAL: GTA 0x005110d0
const char *g_Message_finish;

// GLOBAL: GTA 0x005101e0
const char *g_Message_crimeRTA;

// GLOBAL: GTA 0x005110f4
const char *g_Message_crimeHAR;

// GLOBAL: GTA 0x005110d8
const char *g_Message_crimeHIJ;

// GLOBAL: GTA 0x005106d4
const char *g_Message_crimeCAR;

// GLOBAL: GTA 0x005101dc
const char *g_Message_crimeSHO;

// GLOBAL: GTA 0x005110a8
const char *g_Message_crimeMUR;

// GLOBAL: GTA 0x005106cc
const char *g_Message_crimeBAN;

// GLOBAL: GTA 0x005101b4
const char *g_Message_didNotFinish;

// GLOBAL: GTA 0x005101bc
const char *g_Message_shighest;

// GLOBAL: GTA 0x00510288
const char *g_Message_crimes;

// GLOBAL: GTA 0x00510208
const char *g_Message_missionsComp;

// GLOBAL: GTA 0x005106d8
const char *g_Message_secretsFound;

// GLOBAL: GTA 0x0051023c
const char *g_Message_raceResults;

// GLOBAL: GTA 0x00510278
const char *g_Message_finalScores;

// GLOBAL: GTA 0x00510254
const char *g_Message_abandon;

// GLOBAL: GTA 0x00510204
const char *g_Message_winner;

// GLOBAL: GTA 0x005110bc
const char *g_Message_loser;

// GLOBAL: GTA 0x005101d8
const char *g_Message_noWin;

// GLOBAL: GTA 0x005110b0
const char *g_Message_menu;

// GLOBAL: GTA 0x005101b0
const char *g_Message_story;

// GLOBAL: GTA 0x005110f0
const char *g_Message_colour;

// GLOBAL: GTA 0x00511080
const char *g_Message_cdTitle;

// GLOBAL: GTA 0x005101c4
const char *g_Message_cdText;

// GLOBAL: GTA 0x0051022c
const char *g_Message_cdMessage_2;

// GLOBAL: GTA 0x00510860
const char *g_Message_demoMessage;

// GLOBAL: GTA 0x00510260
const char *g_Message_fix1;

// GLOBAL: GTA 0x0051026c
const char *g_Message_fix2;

// GLOBAL: GTA 0x00510270
const char *g_Message_fix3;

// GLOBAL: GTA 0x00510264
const char *g_Message_fix4;

// GLOBAL: GTA 0x00510268
const char *g_Message_fix5;


// FUNCTION: GTA 0x00426320
void InitInterfaceStrings() {

    if (!g_Interface_strings_loaded) {
        g_Interface_strings_loaded = true;

        {
            for (int i = 0; i < GTA_ASIZE(g_Cities_singleplayer); i++) {
                sprintf(g_Load_temporary_buffer, "city%d", i);
                const char *name = GetTranslatedString(g_Load_temporary_buffer);
                g_Cities_singleplayer[i].name = name;
                g_Cities_multiplayer[i].name = name;
            }
        }

        {
            for (int i = 0; i < GTA_ASIZE(g_Cities_singleplayer); i++) {
                for (int j = 0; j < g_Cities_singleplayer[i].count_missions; j++) {
                    sprintf(g_Load_temporary_buffer, "mission%d", g_Cities_singleplayer[i].missions[j]);
                    const char *name = GetTranslatedString(g_Load_temporary_buffer);
                    g_Cities_singleplayer[i].mission_names[j] = name;
                }
            }
        }

        {
            for (int i = 0; i < GTA_ASIZE(g_Cities_multiplayer); i++) {
                for (int j = 0; j < g_Cities_multiplayer[i].count_missions; j++) {
                    sprintf(g_Load_temporary_buffer, "mission%d", g_Cities_multiplayer[i].missions[j]);
                    const char *name = GetTranslatedString(g_Load_temporary_buffer);
                    g_Cities_multiplayer[i].mission_names[j] = name;
                }
            }
        }
        g_Message_sscolon = GetTranslatedString("sscolon");
        g_Message_sdcolon = GetTranslatedString("sdcolon");
        g_Message_dscolon = GetTranslatedString("dscolon");
        g_Message_cdMessage_1 = GetTranslatedString("cd-message");
        g_Message_highScores = GetTranslatedString("high-scores");
        g_Message_winScore = GetTranslatedString("win-score");
        g_Message_winRace = GetTranslatedString("win-race");
        g_Message_winKills = GetTranslatedString("win-kills");
        g_Message_on = GetTranslatedString("On");
        g_Message_off = GetTranslatedString("Off");
        g_Message_chapter = GetTranslatedString("chapter:");
        g_Message_spaceKey = GetTranslatedString("spc-key");
        g_Message_returnKey = GetTranslatedString("rtn-key");
        g_Message_escKey = GetTranslatedString("esc-key");
        g_Message_status = GetTranslatedString("status");
        g_Message_quit = GetTranslatedString("Quit");
        g_Message_play = GetTranslatedString("Play");
        g_Message_cancel = GetTranslatedString("Cancel");
        g_Message_commsfail = GetTranslatedString("commsfail");
        g_Message_commsversionerror = GetTranslatedString("commsversionerror");
        g_Message_svgaerror = GetTranslatedString("svgaerror");
        g_Message_loading = GetTranslatedString("Loading");
        g_Message_loadingDemo = GetTranslatedString("LoadingDemo");
        g_Message_gatherNetwork = GetTranslatedString("GatherNetwork");
        g_Message_joinNetwork = GetTranslatedString("JoinNetwork");
        g_Message_options = GetTranslatedString("Options");
        g_Message_sound = GetTranslatedString("Sound");
        g_Message_music = GetTranslatedString("Music");
        g_Message_text = GetTranslatedString("Text");
        g_Message_slow = GetTranslatedString("Slow");
        g_Message_normal = GetTranslatedString("Normal");
        g_Message_fast = GetTranslatedString("Fast");
        g_Message_musicMode = GetTranslatedString("music-mode");
        g_Message_transEffects = GetTranslatedString("trans-effects");
        g_Message_constant = GetTranslatedString("constant");
        g_Message_radio = GetTranslatedString("radio");
        g_Message_save = GetTranslatedString("save");
        g_Message_multiOptions = GetTranslatedString("multi-options");
        g_Message_endGame = GetTranslatedString("end-game");
        g_Message_endScore = GetTranslatedString("end-score");
        g_Message_score = GetTranslatedString("Score");
        g_Message_kills = GetTranslatedString("kills");
        g_Message_endKills = GetTranslatedString("end-kills");
        g_Message_scores = GetTranslatedString("scores");
        g_Message_delKey = GetTranslatedString("del-key");
        g_Message_rename = GetTranslatedString("rename");
        g_Message_rKey = GetTranslatedString("r-key");
        g_Message_reset = GetTranslatedString("Reset");
        g_Message_finish = GetTranslatedString("finish");
        g_Message_crimeRTA = GetTranslatedString("crimeRTA");
        g_Message_crimeHAR = GetTranslatedString("crimeHAR");
        g_Message_crimeHIJ = GetTranslatedString("crimeHIJ");
        g_Message_crimeCAR = GetTranslatedString("crimeCAR");
        g_Message_crimeSHO = GetTranslatedString("crimeSHO");
        g_Message_crimeMUR = GetTranslatedString("crimeMUR");
        g_Message_crimeBAN = GetTranslatedString("crimeBAN");
        g_Message_didNotFinish = GetTranslatedString("did-not-finish");
        g_Message_shighest = GetTranslatedString("shighest");
        g_Message_crimes = GetTranslatedString("crimes");
        g_Message_missionsComp = GetTranslatedString("missions-comp");
        g_Message_secretsFound = GetTranslatedString("secrets_found");
        g_Message_raceResults = GetTranslatedString("race-results");
        g_Message_finalScores = GetTranslatedString("final-scores");
        g_Message_abandon = GetTranslatedString("abandon");
        g_Message_winner = GetTranslatedString("winner");
        g_Message_loser = GetTranslatedString("loser");
        g_Message_noWin = GetTranslatedString("no-win");
        g_Message_menu = GetTranslatedString("menu");
        g_Message_story = GetTranslatedString("story");
        g_Message_colour = GetTranslatedString("colour");
        g_Message_cdTitle = GetTranslatedString("cd-title");
        g_Message_cdText = GetTranslatedString("cd-text");
        g_Message_cdMessage_2 = GetTranslatedString("cd-message");
        g_Message_demoMessage = GetTranslatedString("demo-message");
        g_Message_fix1 = GetTranslatedString("fix1");
        g_Message_fix2 = GetTranslatedString("fix2");
        g_Message_fix3 = GetTranslatedString("fix3");
        g_Message_fix4 = GetTranslatedString("fix4");
        g_Message_fix5 = GetTranslatedString("fix5");
    }
}

#include "menu.h"
#include "city.h"
#include "cutscene.h"
#include "error.h"
#include "font.h"
#include "game.h"
#include "graphics.h"
#include "interface.h"
#include "mission.h"
#include "multiplayer.h"
#include "save.h"
#include "sound.h"
#include "text.h"
#include "util.h"

#include <stdio.h>


// GLOBAL: GTA 0x00511104
tMenu_HostOrClient g_Menu_host_or_client;

// GLOBAL: GTA 0x00511900
int g_Menu_bytes_per_pixel;

// GLOBAL: GTA 0x00511920
tRect g_Menu_viewport;

// GLOBAL: GTA 0x0051193c
tPixelmap g_Menu_upper;

// GLOBAL: GTA 0x005118a0
tPixelmap g_Menu_logos[10];

// GLOBAL: GTA 0x00511908
tPixelmap g_Menu_lowers[2];

// GLOBAL: GTA 0x005110f8
bool g_Menu_logos_loaded;

// GLOBAL: GTA 0x005106f0
tPixelmap g_Profile_pixelmaps[8];

// GLOBAL: GTA 0x00510218
tPixelmap g_Playn;

// GLOBAL: GTA 0x0051027c
tPixelmap g_Logo_sw;

// GLOBAL: GTA 0x005106ac
tPixelmap g_Logo_se;

// GLOBAL: GTA 0x004af410
int g_Cutscene_loaded_index = -1;

// GLOBAL: GTA 0x005106c8
tPost_cutscene_menu g_Post_cutscene_menu;

// GLOBAL: GTA 0x005110b4
tMenu_result g_Menu_return_code;

// GLOBAL: GTA 0x00429180
u8 g_Exit_font_height_initialized;


// FUNCTION: GTA 0x0042b4a0
void InitPlayerData() {
    InitializeSaveState();
    InitInterfaceStrings();
    UpdateAvailableCitiesCount();
    SelectSuitableGraphicsMode(&g_Save_state.field_0x4);
    ApplySoundVolumes();
}

// FUNCTION: GTA 0x0042da60
void LoadMenuLogos() {
    g_Menu_bytes_per_pixel = 2;
    ConfigureDisplay2(-1);
    g_Menu_viewport.left = 0;
    g_Menu_viewport.top = 0;
    g_Menu_viewport.right = 639;
    g_Menu_viewport.bottom = 479;
    g_Cutscreen_pixelmap.w = 640;
    g_Cutscreen_pixelmap.h = 480;
    g_Cutscreen_pixelmap.pixels = CheckedMalloc(g_Menu_bytes_per_pixel * g_Cutscreen_pixelmap.w * g_Cutscreen_pixelmap.h);
    g_Menu_upper.w = 640;
    g_Menu_upper.h = 168;
    g_Menu_upper.pixels = CheckedMalloc(g_Menu_bytes_per_pixel * g_Menu_upper.w * g_Menu_upper.h);
    {
        for (int i = 0; i < GTA_ASIZE(g_Menu_logos); i++) {
            g_Menu_logos[i].w = 640;
            g_Menu_logos[i].h = 168;
            g_Menu_logos[i].pixels = CheckedMalloc(g_Menu_bytes_per_pixel * g_Menu_logos[i].w * g_Menu_logos[i].h);
        }
    }
    {
        for (int i = 0; i < GTA_ASIZE(g_Menu_lowers); i++) {
            g_Menu_lowers[i].w = 640;
            g_Menu_lowers[i].h = 312;
            g_Menu_lowers[i].pixels = CheckedMalloc(g_Menu_bytes_per_pixel * g_Menu_lowers[i].w * g_Menu_lowers[i].h);
        }
    }
    LoadPixelmap(&g_Menu_upper, "..\\gtadata\\f_upper", 0);
    LoadPixelmap(&g_Menu_logos[0], "..\\gtadata\\f_logo0", 0);
    LoadPixelmap(&g_Menu_logos[1], "..\\gtadata\\f_logo1", 0);
    LoadPixelmap(&g_Menu_logos[2], "..\\gtadata\\f_logo2", 0);
    LoadPixelmap(&g_Menu_logos[3], "..\\gtadata\\f_logo3", 0);
    LoadPixelmap(&g_Menu_logos[4], "..\\gtadata\\f_logo4", 0);
    LoadPixelmap(&g_Menu_logos[5], "..\\gtadata\\f_logo5", 0);
    LoadPixelmap(&g_Menu_logos[6], "..\\gtadata\\f_logo6", 0);
    LoadPixelmap(&g_Menu_logos[7], "..\\gtadata\\f_logo7", 0);
    LoadPixelmap(&g_Menu_lowers[0], "..\\gtadata\\f_lower0", 0);
    LoadPixelmap(&g_Menu_lowers[1], "..\\gtadata\\f_lower1", 0);
}

// FUNCTION: GTA 0x0042d5c0
void InitializePixelmap(tPixelmap *pixelmap, int w, int h) {
    pixelmap->w = w;
    pixelmap->h = h;
    pixelmap->pixels = CheckedMalloc(g_Menu_bytes_per_pixel * w * h);
}

// GLOBAL: GTA 0x00511118
int g_Cutscene_index;

// GLOBAL: GTA 0x005110fc
BOOL g_Menu_is_multiplayer;

// GLOBAL: GTA 0x00513230
tEndOfRace_result g_End_of_game_result;

// GLOBAL: GTA 0x00511114
int g_Menu_current_mission_index;

// GLOBAL: GTA 0x00511108
bool g_GTA_cd_present;

// GLOBAL: GTA 0x00511100
bool g_Scores_updated;

// GLOBAL: GTA 0x005101d0
tMenu_index g_Current_menu;

// GLOBAL: GTA 0x00510290
bool g_Bool_array_00510290[6];;

// GLOBAL: GTA 0x005110ac
bool g_Special_high_score[3];

// GLOBAL: GTA 0x005101f0
char g_Buffer_profile_name[16];

// GLOBAL: GTA 0x00510870
char g_End_of_multiplayer_result_text[1024];

// GLOBAL: GTA 0x005110b8
bool g_Menu_player_has_won[4];

// GLOBAL: GTA 0x005110e0
int g_Post_game_summary_game_result;

// GLOBAL: GTA 0x00511078
BOOL g_Current_menu_updated;

// GLOBAL: GTA 0x005110a0
BOOL g_Selected_menu_option;

// GLOBAL: GTA 0x00510760
char g_Menu_error_buffer[256];

// GLOBAL: GTA 0x005110e8
int g_Exit_font_height;

// GLOBAL: GTA 0x00511144
int g_Exit_credits_time;

// GLOBAL: GTA 0x004af130
const char *g_Text_exit_credits[180] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "Grand Theft Auto",
    "",
    "Copyright *2002 Take-Two Interactive, Inc.",
    "Rockstar, the Rockstar logo, Rockstar North",
    "and the Rockstar North logo are trademarks",
    "of Take-Two Interactive, Inc.",
    "All other trademarks and trade names",
    "are properties of their respective owners.",
    "All rights reserved.",
    "",
    "Portions copyright *1993-1997, SciTech Software",
    "Uses Miles Sound System from RAD Software.",
    "Copyright *1994-1996 by Miles Design, Inc.",
    "Uses Smacker Video Technology.",
    "Copyright *1994-1996 by Invisible,Inc. d/b/a RAD Software",
    "",
    "Team Leader and Lead Programmer",
    "@Keith Hamilton",
    "",
    "Lead Artist",
    "@Ian McQue",
    "",
    "Programming",
    "@Alan Campbell",
    "@Toby Atkin",
    "@Brian Baird",
    "@Iain Ross",
    "@Ian Johnson",
    "@Cameron Rattray",
    "@James Watson",
    "@David Kivlin",
    "",
    "Additional Programming",
    "@Sandeep Singh Bisla",
    "",
    "Artwork",
    "@Donald Robertson",
    "@Craig Moore",
    "@Stewart Waterson",
    "",
    "Map Design and Mission Programming:",
    "",
    "San Andreas",
    "@Steve Banks",
    "",
    "Liberty City",
    "@Paul Farley",
    "",
    "Vice City",
    "@Billy Thomson",
    "",
    "Writers",
    "@Brian Baglow",
    "@Brian Lawson",
    "",
    "Music Written, Produced and Performed by",
    "@Craig Conner",
    "@Colin Anderson",
    "@Grant Middleton",
    "",
    "Audio Programming",
    "@Raymond Usher",
    "@Grant Middleton",
    "",
    "Sound Effects",
    "@Allan Walker",
    "@Raymond Usher",
    "@Colin Anderson",
    "@Craig Conner",
    "@Grant Middleton",
    "",
    "Additional Musicians",
    "@Brian Baglow",
    "@Robert De Negro",
    "@John Gurney",
    "@Eugene Henderson",
    "@Michael James",
    "@Drew Larg",
    "@Paul Mackie",
    "@Chris Marra",
    "@Gennine Peirce",
    "@Stephen Porter",
    "@Melissa Ripley",
    "@Ciaran Rooney",
    "@Stuart Ross",
    "@Brian Socha",
    "@Heather Sowards",
    "@Karen Taylor",
    "@Allan Walker",
    "@Julie Wemyss",
    "",
    "Graphics Engine",
    "@Mike Dailly",
    "@Aaron Puzey",
    "",
    "Physics Programming",
    "@Pat Kerr",
    "",
    "Tools Programming",
    "@Iain Macdonald",
    "@Andrew Patterson",
    "@Robert Parsons",
    "",
    "Additional Artwork",
    "@Darren Baines",
    "@Jamie Bryan",
    "@Jeff Cairns",
    "@Ken Fee",
    "@Aaron Garbut",
    "@Martin Good",
    "@Andy Hay",
    "@Stacey Jamieson",
    "@Rob Jeffrey",
    "@Lenny Lane",
    "@Pat McGovern",
    "@Liam O'Neill",
    "@Paul Reeves",
    "@Andrew Strachan",
    "",
    "Quality Assurance",
    "@Craig Arbuthnott",
    "@William Mills",
    "@Michael Stirling",
    "@John Ree",
    "@Sean Taylor",
    "@Graeme Laws",
    "@Fiona Robertson",
    "@Kevin Finnigan",
    "@Alan Dobson",
    "@Gordon Ross",
    "@Lee Bekier",
    "",
    "Additional Design",
    "@Lesley Firth",
    "",
    "BMG Chief Whip",
    "@Sam Houser",
    "",
    "BMG Senior Technical Manager",
    "@Gary J. Foreman",
    "",
    "BMG Technical Manager",
    "@Mark Crane",
    "",
    "BMG Production Co-ordinator",
    "@Claire Cuthill",
    "",
    "BMG QA Manager",
    "@Steve Mann",
    "",
    "BMG QA Department",
    "@Darren Lloyd",
    "@Mike Wenn",
    "@Adam Rabin",
    "@Ken Jordan",
    "@Paul Cuthill",
    "@Ronnie Jackson",
    "@Sean James",
    "@Jeremy Goldstein",
    "",
    "",
    "Project Blokes",
    "@Gary Penn",
    "@Brian Lawson",
    "",
    "Capo Di Tutte Cappa",
    "@David Jones",
    "",
    NULL
};

// GLOBAL: GTA 0x00426fd0
char g_Temp_menu_buffer[1024];

// GLOBAL: GTA 0x005110ec
tNetwork_waiting_room_state g_Multiplayer_waiting_room_state;

// GLBOAL: GTA 0x00501d70
tNetwork_waiting_room_state g_Multiplayer_room_status;

// GLOBAL: GTA 0x004af414
BOOL g_Multiplayer_BOOL_004af414 = TRUE;

// GLOBAL: GTA 0x0051110c
int g_Current_multiplayer_session_index;

// GLOBAL: GTA 0x0051113c
int g_Profile_caret_counter;

// GLOBAL: GTA 0x0051111d
u8 g_Score_blink_counter;

// GLOBAL: GTA 0x004af660
const char *g_Cheat_nineinarow = "nhienreei naarrtohwe";

// GLOBAL: GTA 0x004af664
const char *g_Cheat_super_well = "scuhpeeart  wceoldle";

// GLOBAL: GTA 0x004af668
const char *g_Cheat_itsgallus = "ist s.g aflilnuds ";

// GLOBAL: GTA 0x004af66c
const char *g_Cheat_6031769 = "6t0h3e1m7 6i9f";

// GLOBAL: GTA 0x004af670
const char *g_Cheat_itstantrum = "iytosut acnatnr u.m ";

// GLOBAL: GTA 0x004af674
const char *g_Cheat_iamthelaw = "isaommteh eolfa wt";

// GLOBAL: GTA 0x004af678
const char *g_Cheat_stevesmates = "shteemv easrmea tneost";

// GLOBAL: GTA 0x004af67c
const char *g_Cheat_callmenigel = "csaalflem etnoi guesle";

// GLOBAL: GTA 0x004af680
const char *g_Cheat_buckfast = "bsuoc kpflaesats";

// GLOBAL: GTA 0x004af684
const char *g_Cheat_porkcharsui = "peo rbkec hcaarrseufiu";

// GLOBAL: GTA 0x004af688
const char *g_Cheat_Heart_of_gold = "Hle awrith tohfe mG o.l dy";

// GLOBAL: GTA 0x004af68c
const char *g_Cheat_iamgarypenn = "ioanmegvaerry pkennonw";

// GLOBAL: GTA 0x004af690
const char *g_Cheat_iamnotgarypenn = "i aqmunioet gwahraytp einsn ";

// GLOBAL: GTA 0x004af694
const char *g_Cheat_hate_machine = "hgaotien gm atcoh ihnaep";

// GLOBAL: GTA 0x004af698
const char *g_Cheat_itcouldbeyou = "iptecno u!l dbbee yfoour";

// GLOBAL: GTA 0x004af69c
const char *g_Cheat_suckmyrocket = "snuocwk m.y rkoecikteht.";

// GLOBAL: GTA 0x0051111c
u8 g_Mission_highscore_blink_counter;

// GLOBAL: GTA 0x00510234
tMenu_result g_Loading_menu_return_code;

// GLOBAL: GTA 0x0051112c
int g_City_logo_animation_indices[4];

// GLOBAL: GTA 0x00511128
int g_City_selection_mission_key_animation_key_index;

// GLOBAL: GTA 0x00511110
int g_Selected_multiplayer_session;

// GLOBAL: GTA 0x00511140
int g_Multiplayer_session_name_caret_index;

// GLOBAL: GTA 0x00501d1c
char g_Join_gather_status_string[40];


void UpdateNextMenu(tMenu_index menu) {
    if (menu == eMenu_cutscene) {
        g_Cutscene_index = 0;
        if (!g_Menu_is_multiplayer && g_End_of_game_result == eEndOfRace_success) {
            g_Save_state.active_profile_index;
            g_Menu_current_mission_index = g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[0]].mission_save_indexes[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[0]];;
            g_Save_state.profiles[g_Save_state.active_profile_index].missions_finished[g_Menu_current_mission_index] = 1;
        } else {
            menu = eMenu_post_game_summary;
        }
    }

    switch (menu) {
    case eMenu_cutscene:
        {
            for (int i = 0; i < g_Count_mission_cutscene_fonts[g_Menu_current_mission_index]; i++) {
                const char *font_filename;
                char font_path[32];
                if (g_Pixelformat_type == 0x101) {
                    font_filename = g_Cut_font_text_names[g_Menu_current_mission_index][i];
                } else {
                    font_filename = g_Cut_font_names[g_Menu_current_mission_index][i];
                }
                sprintf(font_path, "..\\gtadata\\%s.fon", font_filename);
                LoadFont(&g_City_cut_fonts[i], font_path, 1, true);
            }
        }
        if (g_Cutscene_loaded_index != g_Menu_current_mission_index) {
            LoadCutscenePixelmap(g_Cut_backscreen_paths[g_Menu_current_mission_index]);
            g_Cutscene_loaded_index = g_Menu_current_mission_index;
        }
        StartMusicForCutscene(g_Menu_current_mission_index);
        break;
    case eMenu_cd_check:
        g_GTA_cd_present = true;
        break;
    case eMenu_client_wait_for_players:
    case eMenu_loading:
    case eMenu_main_menu:
        g_Scores_updated = false;
        break;
    case eMenu_city_select:
        if (g_Current_menu == eMenu_cutscene) {
            for (int i = 0; i < g_Count_mission_cutscene_fonts[g_Menu_current_mission_index]; i++) {
                FreeFont(&g_City_cut_fonts[i]);
            }
        }
        g_Special_high_score[0] = false;
        g_Special_high_score[1] = false;
        g_Special_high_score[2] = false;
        g_Bool_array_00510290[0] = false;
        g_Bool_array_00510290[1] = false;
        g_Bool_array_00510290[2] = false;
        g_Bool_array_00510290[3] = false;
        g_Bool_array_00510290[4] = false;
        g_Bool_array_00510290[5] = false;
        UpdateAvailableCitiesCount();
        break;
    case eMenu_profile_select:
        UpdateAvailableCitiesCount();
        g_Special_high_score[0] = false;
        g_Special_high_score[1] = false;
        g_Special_high_score[2] = false;
        g_Bool_array_00510290[0] = false;
        g_Bool_array_00510290[1] = false;
        g_Bool_array_00510290[2] = false;
        g_Bool_array_00510290[3] = false;
        g_Bool_array_00510290[4] = false;
        g_Bool_array_00510290[5] = false;
        break;
    case eMenu_profile_name_edit:
        g_Buffer_profile_name[0] = '\0';
        break;
    case eMenu_post_game_summary:
        if (g_Current_menu == eMenu_cutscene) {
            for (int i = 0; i < g_Count_mission_cutscene_fonts[g_Menu_current_mission_index]; i++) {
                FreeFont(&g_City_cut_fonts[i]);
            }
        }
        g_End_of_multiplayer_result_text[0] = '\0';
        int score = GetEndOfMultiplayerGameResult(g_End_of_multiplayer_result_text);
        int profile_index = g_Save_state.active_profile_index;
        int selected_city = g_Save_state.profiles[profile_index].selected_city[g_Menu_is_multiplayer];
        int selected_mission = g_Save_state.profiles[profile_index].selected_mission[g_Menu_is_multiplayer];
        int mission_save_index = g_Current_cities[selected_city].mission_save_indexes[selected_mission];
        g_Special_high_score[0] = false;
        g_Special_high_score[1] = false;
        g_Special_high_score[2] = false;
        g_Bool_array_00510290[0] = false;
        g_Bool_array_00510290[1] = false;
        g_Bool_array_00510290[2] = false;
        g_Bool_array_00510290[3] = false;
        g_Bool_array_00510290[4] = false;
        g_Bool_array_00510290[5] = false;
        if (g_Menu_host_or_client == eMenuMP_singleplayer && !g_Scores_updated) {
            if (score > g_Save_state.profiles[profile_index].scores[mission_save_index]) {
                g_Save_state.profiles[profile_index].scores[mission_save_index] = score;
                g_Bool_array_00510290[mission_save_index] = true;
            }
            {
                for (int i = 2; i > 0; i--) {
                    if (score > g_Save_state.highscores[mission_save_index][i].score) {
                        for (int j = 0; j < i; j++) {
                            g_Save_state.highscores[mission_save_index][j] = g_Save_state.highscores[mission_save_index][j + 1];
                        }
                        g_Save_state.highscores[mission_save_index][i].score = score;
                        strcpy(g_Save_state.highscores[mission_save_index][i].name, g_Save_state.profiles[g_Save_state.active_profile_index].name);
                        g_Special_high_score[i] = true;
                        break;
                    }
                }
            }
            profile_index = g_Save_state.active_profile_index;
            mission_save_index = (int)(char)g_Current_cities[g_Save_state.profiles[profile_index].selected_city[g_Menu_is_multiplayer]].mission_save_indexes[g_Save_state.profiles[profile_index].selected_mission[g_Menu_is_multiplayer]];
            if (g_End_of_game_result == eEndOfRace_success && mission_save_index < 5 && g_Save_state.profiles[profile_index].scores[mission_save_index + 1] == -1) {
                g_Save_state.profiles[profile_index].scores[mission_save_index + 1] = 0;
            }
            g_Scores_updated = true;
        } else {
            int max_score = 1;
            g_Menu_player_has_won[0] = false;
            g_Menu_player_has_won[1] = false;
            g_Menu_player_has_won[2] = false;
            g_Menu_player_has_won[3] = false;
            int count_winners = 0;
            if (g_Current_cities[profile_index].field_0x12[selected_mission] == 0) {
                if (g_Save_state.multiplayer_target == eMultiplayerGameType_money) {
                    {
                        for (int i = 0; i < 4; i++) {
                            int score = GetMultiplayerScore_004643b0(i);
                            if (score >= max_score) {
                                max_score = score;
                            }
                        }
                    }
                    {
                        for (int i = 0; i < 4; i++) {
                            int score = GetMultiplayerScore_004643b0(i);
                            if (score == max_score) {
                                g_Menu_player_has_won[i] = true;
                                count_winners += 1;
                            }
                        }
                    }
                } else if (g_Save_state.multiplayer_target == eMultiplayerGameType_money) {
                    int max_frags = 1;
                    {
                        for (int i = 0; i < 4; i++) {
                            int score = GetMultiplayerScore_004643b0(i);
                            if (score != -1) {
                                int frags = GetMultiplayerFrags(i);
                                if (frags >= max_frags) {
                                    max_frags = frags;
                                }
                            }
                        }
                    }
                    {
                        for (int i = 0; i < 4; i++) {
                            int score = GetMultiplayerScore_004643b0(i);
                            if (score != -1 && GetMultiplayerFrags(i) == max_frags) {
                                g_Menu_player_has_won[i] = true;
                                count_winners += 1;
                            }
                        }
                    }
                }
                if (g_End_of_game_result == eEndOfRace_7) {
                    g_Post_game_summary_game_result = 3;
                } else if (count_winners == 0) {
                    /* 2: everybody left (no scores available) */
                    g_Post_game_summary_game_result = 2;
                } else {
                    /* 0: I won
                     * 1: Somebody else won */
                    g_Post_game_summary_game_result = g_Menu_player_has_won[g_Multiplayer_player_id] == false ? 1 : 0;
                }
            } else {
                int fastest_player = -1;
                for (int i = 0; i < 4; i++) {
                    int score = GetMultiplayerScore_004643b0(i);
                    if (score != -1) {
                        if (GetMultiplayerRacePosition(i) == 0) {
                            fastest_player = i;
                        }
                    }
                }
                if (g_End_of_game_result == eEndOfRace_7) {
                    g_Post_game_summary_game_result = 3;
                } else if (fastest_player == g_Multiplayer_player_id) {
                    /* 0: I won */
                    g_Post_game_summary_game_result = 0;
                } else {
                    /* 1: Somebody else won
                     * 2: everybody left (no scores available) */
                    g_Post_game_summary_game_result = (fastest_player != -1) ? 1 : 2;
                }
            }
        }
        UpdateAvailableCitiesCount();
        g_Save_state.field_0x4 = GetSuitableGraphicsMode();
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == (FILE *)0x0) {
            FatalError(eFatalError_cannot_open_S, 478,"..\\gtadata\\player_a.dat");
        }
        // assert(sizeof(g_Save_state) == 0x414);
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
        }
        fclose(f);
        break;
    }
    g_Current_menu_updated = 1;
    g_Current_menu = menu;
}

// FUNCTION: GTA 0x0042b690
void InitMenus(tMenu_index menu) {
    StartMenuAudio();
    SetMasterSoundVolumePreset(g_Save_state.sound_volume);
    if (!g_Demo_version || g_Enable_music) {
        SetMasterMusicVolumePreset(g_Save_state.music_volume);
    }
    LoadMenuLogos();
    if (!g_Menu_logos_loaded) {
        for (int i = 0; i < GTA_ASIZE(g_Default_profiles); i++) {
            InitializePixelmap(&g_Profile_pixelmaps[i], 102, 141);
            LoadPixelmap(&g_Profile_pixelmaps[i], g_Default_profiles[i].profile_pic_path, 0);
        }
        InitializePixelmap(&g_Playn, 180, 50);
        LoadPixelmap(&g_Playn, "..\\gtadata\\f_playn", 0);
        InitializePixelmap(&g_Logo_sw, 64, 59);
        LoadPixelmap(&g_Logo_sw, "..\\gtadata\\f_rstar", 0);
        InitializePixelmap(&g_Logo_se, 64, 59);
        LoadPixelmap(&g_Logo_se, "..\\gtadata\\f_rstarn", 0);
        g_Menu_logos_loaded = true;
    }
    g_Cutscene_loaded_index = -1;
    g_Post_cutscene_menu = ePostCutscene_game_summary;
    g_Menu_return_code = eMenuRC_continue;
    LoadMenuFonts();
    UpdateNextMenu(menu);
}

// FUNCTION: GTA 0x0042b7a0
void UnloadMenuLogos() {
    FreeMenuFonts();
    if (g_Menu_logos_loaded) {
        for (int i = 0; i < GTA_ASIZE(g_Profile_pixelmaps); i++) {
            FreePixelmap(&g_Profile_pixelmaps[i]);
        }
        FreePixelmap(&g_Playn);
        FreePixelmap(&g_Logo_sw);
        FreePixelmap(&g_Logo_se);
        g_Menu_logos_loaded = false;
    }
    FreeMenuBuffers();
}

// FUNCTION: GTA 0x00429f60
void BlitProfilePictureAndText(int x, int y, int profile, const char *name, BOOL draw_nav) {
    if (profile < 0) {
        profile = 8 - (8 - profile) & 0x7;
    }
    g_Profile_caret_counter += 1;
    if (g_Profile_caret_counter >= 8) {
        g_Profile_caret_counter = 0;
    }
    BlitPixelmap(((x - g_Profile_pixelmaps[profile].w / 2) / 2) * 2, y, &g_Profile_pixelmaps[profile]);
    if (name == NULL) {
        BlitPixelmap(((x - g_Playn.w / 2) / 2) * 2, y + 150, &g_Playn);
        if (draw_nav && g_Profile_caret_counter < 4) {
            BlitTextToScreen(g_Font_mtext, x - g_Playn.w / 2 - 2 * GetTextWidth(g_Font_mtext, "<"), y + 164, "<");
            BlitTextToScreen(g_Font_mtext, x + g_Playn.w / 2 + GetTextWidth(g_Font_mtext, ">"), y + 164, ">");
        }
        BlitTextToScreen(g_Font_mtext, x - GetTextWidth(g_Font_mtext, g_Save_state.profiles[profile].name) / 2,y + 164, g_Save_state.profiles[profile].name);
    } else {
        BlitTextToScreen(g_Font_mhead, x -  GetTextWidth(g_Font_mhead, name) / 2, y + 164, name);
        if (draw_nav && g_Profile_caret_counter < 4) {
            BlitTextToScreen(g_Font_mhead, x + GetTextWidth(g_Font_mhead, name) / 2, y + 164, "<");
        }
    }
}

// FUNCTION: GTA 0x00429df0
void DisplayProfileScores(int x, int y, int profile, const char *title) {
    int score_i = 0;
    BlitTextCenterAligned(g_Font_mtext, title, x - 26, y, 224);
    for (int i = 0; i < 3; i++) {  // FIXME: GTA_ASIZE(g_Cities_singleplayer)
        tCity *city = &g_Current_cities[i];
        int text_y = y + 12 * score_i + 12;
        for (int j = 0; j < city->count_missions; j++) {
            int score = g_Save_state.profiles[profile].scores[city->mission_save_indexes[j]];
            if (score != -1) {
                text_y += 12;
                score_i += 1;
                if (g_Bool_array_00510290[city->mission_save_indexes[i]]) {
                    g_Score_blink_counter += 1;
                    if (g_Score_blink_counter == 6) {
                        g_Score_blink_counter = 0;
                    }
                    if (g_Score_blink_counter >= 3) {
                        continue;
                    }
                }
                BlitTextToScreen(g_Font_mmis, x, text_y, city->mission_names[j]);
                sprintf(g_Load_temporary_buffer, "%d", score);
                int text_dx = 72 - GetTextWidth(g_Font_mmis, g_Load_temporary_buffer);
                if (text_dx < 0) {
                    text_dx = 0;
                }
                BlitTextToScreen(g_Font_mmis, x + text_dx + 128, text_y, g_Load_temporary_buffer);
            }
        }
    }
}

void DisplayMissionHighScores(int x, int y, int mission) {
    BlitTextCenterAligned(g_Font_mtext, g_Message_highScores, x, y, 184);
    for (int i = 0; i < GTA_ASIZE(g_Save_state.highscores[mission]); i++) {
        int line_y = y + 12 * (GTA_ASIZE(g_Save_state.highscores[mission]) + 1 - i);
        if (g_Save_state.highscores[mission][i].score == 0) {
            continue;
        }
        if (g_Special_high_score[i]) {
            g_Mission_highscore_blink_counter += 1;
            if (g_Mission_highscore_blink_counter == 6) {
                g_Mission_highscore_blink_counter = 0;
            }
            if (g_Mission_highscore_blink_counter > 2) {
                continue;
            }
        }
        BlitTextToScreen(g_Font_mmis, x, line_y, g_Save_state.highscores[mission][i].name);
        sprintf(g_Load_temporary_buffer, "%d", g_Save_state.highscores[mission][i].score);
        int score_dx = 72 - GetTextWidth(g_Font_mmis, g_Load_temporary_buffer);
        if (score_dx < 0) {
            score_dx = 0;
        }
        BlitTextToScreen(g_Font_mmis, x + 112 + score_dx, line_y, g_Load_temporary_buffer);
    }
}

// FUNCTION: GTA 0x00426fd0
void BlitReturnKeyText(const char *text) {
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_returnKey, text);
    BlitTextWithKanjiToScreen(g_Font_mtext,
        640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer),
        480 - g_Font_mtext->header.height,
        g_Temp_menu_buffer);
}

// FUNCTION: GTA 0x004616b0
const char *GetMultiplayerPlayerColorString() {
    switch (g_Multiplayer_player_id) {
    case 0:
        return GetTranslatedString("yellow");
    case 1:
        return GetTranslatedString("green");
    case 2:
        return GetTranslatedString("red");
    case 3:
        return GetTranslatedString("blue");
    default:
        return NULL;
    }
}

// FUNCTION: GTA 0x00428150
void DoChooseAConnectionMenu(tMenu_action action) {
    BlitMenuBackground(TRUE, 0);
    if (GetCountMultiplayerProviders() >= 1) {
        BlitHeadTextCentered(176, g_Message_fix1);
    } else {
        BlitHeadTextCentered(176, g_Message_fix2);
    }
    int count_displayed_providers = GetCountMultiplayerProviders();
    if (count_displayed_providers > 8) {
        count_displayed_providers = 8;
    }
    for (int i = 1; i <= count_displayed_providers; i++) {
        DrawMultiplayerConnectionNameToScreen(i, g_Current_multiplayer_session_index + 1,
            GetMultiplayerProviderName(GetCountMultiplayerProviders() - i));
    }
    SwapScreenBuffer();
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_profile_select);
        PlayMenuSound(4);
    }
    if ((action & eMenuAction_Enter) || (action & eMenuAction_Space)) {
        PlayMenuSound(2);
        if (GetCountMultiplayerProviders() > 0) {
            if (ConnectMultiplayerProvider(GetCountMultiplayerProviders() - g_Current_multiplayer_session_index - 1)) {
                if (g_Menu_host_or_client == eMenuMP_multiplayer_host) {
                    UpdateNextMenu(eMenu_multiplayer_new_host_session);
                } else if (g_Menu_host_or_client == eMenuMP_multiplayer_client) {
                    ClearScreenOrCopyCutscene(0);
                    SwapScreenBuffer();
                    if (FindMultiplayerSessions()) {
                        if (g_Selected_multiplayer_session >= GetCountMultiplayerSessions()) {
                            g_Selected_multiplayer_session = 0;
                        }
                        UpdateNextMenu(eMenu_multiplayer_client);
                    }
                }
            }
        }
    }
    if (action & eMenuAction_Up) {
        g_Current_multiplayer_session_index -= 1;
        if (g_Current_multiplayer_session_index < 0) {
            g_Current_multiplayer_session_index = GetCountMultiplayerProviders() - 1;
        }
        PlayMenuSound(7);
    }
    if (action & eMenuAction_Down) {
        g_Current_multiplayer_session_index += 1;
        if (g_Current_multiplayer_session_index >= GetCountMultiplayerProviders()) {
            g_Current_multiplayer_session_index = 0;
        }
        PlayMenuSound(8);
    }
}

// STUB: GTA 0x00428680
void DoHostWaitOnPlayersMenu(tMenu_action action) {
    // FIXME
    NOT_IMPLEMENTED();
}

// STUB: GTA 0x00426fd0
void DoClientWaitForPlayersMenu(tMenu_action action) {
    // FIXME
    NOT_IMPLEMENTED();
}

// FUNCTION: GTA 0x00428bf0
void DoLoadMenu(tMenu_action) {
    g_Menu_return_code = g_Loading_menu_return_code;
    SetTextSpeed(g_Save_state.text_speed);
    g_Transparency_effects = g_Save_state.transparency_effects;
    if (!g_Demo_version || g_Enable_music) {
        SetAlwaysMusic(g_Save_state.music_mode != eMusicMode_radio);
    }
    BlitMenuBackground(TRUE, 0);
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        DisplayMissionHighScores(440, 130,
            g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].mission_save_indexes[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]]);
    }
    int y_text;
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        y_text = 330;
    } else {
        y_text = 300;
    }
    if (g_Menu_host_or_client != eMenuMP_singleplayer) {
        tMultiplayer_game_type game_type;
        if (g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].field_0x12[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]] == 0) {
            game_type = (tMultiplayer_game_type) g_Save_state.multiplayer_target;
        } else {
            game_type = eMultiplayerGameType_race;
        }
        switch (game_type) {
        case eMultiplayerGameType_money:
            sprintf(g_Load_temporary_buffer, g_Message_winScore, g_Save_state.multiplayer_money_target);
            break;
        case eMultiplayerGameType_frags:
            sprintf(g_Load_temporary_buffer, g_Message_winKills, g_Save_state.multiplayer_kill_target);
            break;
        default:
            sprintf(g_Load_temporary_buffer, g_Message_winRace);
            break;
        }
        BlitTextWithKanjiToScreen(g_Font_mtext, 320 - GetTextWidthWithExtra(g_Font_mtext, g_Load_temporary_buffer) / 2, 360, g_Load_temporary_buffer);
        sprintf(g_Load_temporary_buffer, g_Message_colour, GetMultiplayerPlayerColorString());
        BlitTextWithKanjiToScreen(g_Font_mtext, 320 - GetTextWidthWithExtra(g_Font_mtext, g_Load_temporary_buffer) / 2, 390, g_Load_temporary_buffer);
    }
    int profile_index = g_Save_state.active_profile_index;
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        if (g_Language_is_japanese) {
            sprintf(g_Load_temporary_buffer, g_Message_chapter,
                g_Save_state.profiles[profile_index].selected_mission[g_Menu_is_multiplayer] + 1,
                g_Current_cities[g_Save_state.profiles[profile_index].selected_city[g_Menu_is_multiplayer]].name);
        } else {
            sprintf(g_Load_temporary_buffer, g_Message_chapter,
                g_Save_state.profiles[profile_index].selected_mission[g_Menu_is_multiplayer] + 1,
                g_Current_cities[g_Save_state.profiles[profile_index].selected_city[g_Menu_is_multiplayer]].name);
        }
    } else {
        strcpy(g_Load_temporary_buffer, g_Current_cities[g_Save_state.profiles[profile_index].selected_city[g_Menu_is_multiplayer]].name);
    }
    BlitTextWithKanjiToScreen(g_Font_mtext, 320 - GetTextWidthWithExtra(g_Font_mtext, g_Load_temporary_buffer) / 2, y_text, g_Load_temporary_buffer);
    y_text += 30;
    const char *mission_name = g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].mission_names[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]];
    BlitTextToScreen(g_Font_mtext, 320 - GetTextWidth(g_Font_mtext,mission_name) / 2, y_text, mission_name);
    BlitHeadTextCentered(408, g_Message_loading);
    SwapScreenBuffer();
}

// STUB: GTA 0x00428ee0
void DoMainMenu(tMenu_action action) {
    // FIXME
    BOOL multiplayer_available = IsMultiplayerAvailable();
    if (g_Demo_version) {
        multiplayer_available = FALSE;
    }
    if (action & eMenuAction_Up) {
        PlayMenuSound(7);
        g_Selected_menu_option -= 1;
        if (g_Selected_menu_option <= 0) {
            g_Selected_menu_option = multiplayer_available ? 4 : 2;
        }
    }
    if (action & eMenuAction_Down) {
        PlayMenuSound(8);
        g_Selected_menu_option += 1;
        if (g_Selected_menu_option > (multiplayer_available ? 4 : 2)) {
            g_Selected_menu_option = 1;
        }
    }
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_exit_credits);
        PlayMenuSound(4);
    }
    if (action & eMenuAction_Enter) {
        PlayMenuSound(2);
        switch (g_Selected_menu_option) {
        case 1:
            if (g_GTA_cd_present) {
                g_Menu_host_or_client = eMenuMP_singleplayer;
                g_Menu_is_multiplayer = FALSE;
                UpdateNextMenu(eMenu_profile_select);
            }
            break;
        case 2:
            if (!multiplayer_available) {
                UpdateNextMenu(eMenu_options);
            } else if (g_GTA_cd_present) {
                g_Menu_host_or_client = eMenuMP_multiplayer_host;
                g_Menu_is_multiplayer = TRUE;
                UpdateNextMenu(eMenu_profile_select);
            }
            break;
        case 3:
            if (multiplayer_available) {
                g_Menu_host_or_client = eMenuMP_multiplayer_client;
                g_Menu_is_multiplayer = TRUE;
                UpdateNextMenu(eMenu_profile_select);
            }
            break;
        case 4:
            if (multiplayer_available) {
                UpdateNextMenu(eMenu_options);
            }
            break;
        }
    }
    BlitMenuBackground(true, 0);
    DrawMenuOptionWithMaybeKey_216(1, g_Selected_menu_option, g_Message_play);
    if (multiplayer_available) {
        DrawMenuOptionWithMaybeKey_216(2, g_Selected_menu_option, g_Message_gatherNetwork);
        DrawMenuOptionWithMaybeKey_216(3, g_Selected_menu_option, g_Message_joinNetwork);
        DrawMenuOptionWithMaybeKey_216(4, g_Selected_menu_option, g_Message_options);
    } else {
        DrawMenuOptionWithMaybeKey_216(2, g_Selected_menu_option, g_Message_options);
    }
    BlitPixelmap(20, 390, &g_Logo_sw);
    BlitPixelmap(556, 390, &g_Logo_se);
    if (!g_GTA_cd_present || g_Demo_version) {
        const char *msg = !g_GTA_cd_present ? g_Message_cdMessage_2 : g_Message_demoMessage;
        BlitTextWithKanjiToScreen(g_Font_mtext, (640 - GetTextWidthWithExtra(g_Font_mtext, msg)) / 2, 480 - 2 * g_Font_mtext->header.height, msg);
    }
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_quit);
    BlitTextWithKanjiToScreen(g_Font_mtext, (640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer)) / 2, 480 - 1 * g_Font_mtext->header.height, g_Temp_menu_buffer);
    SwapScreenBuffer();
}

// FUNCTION: GTA 0x004277c0
void DoCitySelectMenu(tMenu_action action) {
    if (action & eMenuAction_Left) {
        PlayMenuSound(0);
        g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] = 0;
        g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer] -= 1;
        if (g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer] < 0) {
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer] = g_Current_cities_max_index;
        }
    }
    if (action & eMenuAction_Right) {
        PlayMenuSound(1);
        g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] = 0;
        g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer] += 1;
        if (g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer] > g_Current_cities_max_index) {
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer] = 0;
        }
    }
    if (action & eMenuAction_Up) {
        PlayMenuSound(7);
        g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] -= 1;
        if (g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] < 0) {
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] = g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].index_last_mission;
        }
    }
    if (action & eMenuAction_Down) {
        PlayMenuSound(8);
        g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] += 1;
        if (g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] > g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].index_last_mission) {
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer] = 0;
        }
    }
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_profile_select);
        PlayMenuSound(4);
    }
    if (action & eMenuAction_Enter) {
        PlayMenuSound(2);
        if (g_Menu_host_or_client == eMenuMP_singleplayer) {
            SetCurrentMission(g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].missions[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]]);
            LoadMission();
            g_Loading_menu_return_code = eMenuRC_game_singleplayer;
            UpdateNextMenu(eMenu_loading);
            ApplySoundVolumes();
        } else {
            SetCurrentMission(g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].missions[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]]);
            LoadMission();
            if (g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].field_0x12[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]] == 0) {
                UpdateNextMenu(eMenu_multiplayer_options);
            } else {
                Multiplayer_FUN_0044b8a0(0);
                g_Multiplayer_waiting_room_state = eWaiting_room_NoLetsGoSent;
                g_Multiplayer_BOOL_004af414 = TRUE;
                if (EnumerateMultiplayerProviders()) {
                    if (g_Current_multiplayer_session_index >= GetCountMultiplayerProviders()) {
                        g_Current_multiplayer_session_index = 0;
                    }
                    UpdateNextMenu(eMenu_connection_select);
                }
                FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
                if (f == NULL) {
                    FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
                }
                if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
                    FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
                }
                fclose(f);
            }
        }
    }
    if ((action & eMenuAction_Space)
            && !g_Menu_is_multiplayer
            && g_Save_state.profiles[g_Save_state.active_profile_index].missions_finished[g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[0]].mission_save_indexes[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[0]]] == 1) {
        g_Post_cutscene_menu = ePostCutscene_city_select;
        g_End_of_game_result = eEndOfRace_success;
        UpdateNextMenu(eMenu_cutscene);
        PlayMenuSound(4);
    }
    BlitMenuBackground(TRUE, 1);

    int mission_key_x = 0;
    int mission_Key_y = 0;
    int cities_x[3] = { 470, 80, 368 };
    int cities_y[3] = { 242, 230, 338 + ((g_Menu_host_or_client != eMenuMP_singleplayer) ? -28 : 0) };
    int dx_logo_missions[3] = { 20, 8, 0 };
    int font_index[3] = { 3, 0, 2 };
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        DisplayMissionHighScores(440, 130, g_Current_cities[g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer]].mission_save_indexes[g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]]);
    }
    for (int i = 0; i <= g_Current_cities_max_index; i++) {
        int selected_city = g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[g_Menu_is_multiplayer];
        int font_i = font_index[i];
        if (i == selected_city || g_City_logo_animation_indices[font_i] != 0) {
            g_City_logo_animation_indices[font_i] += 1;
            if (g_City_logo_animation_indices[font_i] == g_Font_cities[font_i]->header.count_glyphs) {
                g_City_logo_animation_indices[font_i] = 0;
            }
        }
        int y = cities_y[i];
        int x = cities_x[i];
        char city_logo_text[2];
        city_logo_text[0] = g_City_logo_animation_indices[font_i] + 1;
        city_logo_text[1] = '\0';
        BlitTextWithKanjiToScreen(g_Font_cities[font_i], x, y, city_logo_text);
        y += g_Font_cities[font_i]->header.height;
        BlitTextToScreen(g_Font_mtext, x, y, g_Current_cities[i].name);
        y += g_Font_mtext->header.height;
        x += dx_logo_missions[i];
        if (i == selected_city) {
            for (int j = 0; j <= g_Current_cities[i].index_last_mission; j++) {
                if (j == g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[g_Menu_is_multiplayer]) {
                    mission_key_x = x - 69;
                    mission_Key_y = y - 32;
                }
                sprintf(g_Load_temporary_buffer, g_Message_dscolon, j + 1, g_Current_cities[i].mission_names[j]);
                BlitTextToScreen(g_Font_mmis, x, y, g_Load_temporary_buffer);
                y += g_Font_mmis->header.height;
            }
        }
    }
    g_City_selection_mission_key_animation_key_index += 1;
    if (g_City_selection_mission_key_animation_key_index > 12) {
        g_City_selection_mission_key_animation_key_index = 0;
    }
    char mission_key_text[2];
    mission_key_text[0] = g_City_selection_mission_key_animation_key_index + 1;
    mission_key_text[1] = '\0';
    BlitTextWithKanjiToScreen(g_Font_key, mission_key_x, mission_Key_y, mission_key_text);
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_cancel);
    BlitTextWithKanjiToScreen(g_Font_mtext, 0, 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_returnKey, g_Message_play);
    BlitTextWithKanjiToScreen(g_Font_mtext, 640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer), 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    if (!g_Menu_is_multiplayer) {
        int profile_index = g_Save_state.active_profile_index;
        if (g_Save_state.profiles[profile_index].missions_finished[g_Current_cities[g_Save_state.profiles[profile_index].selected_city[0]].mission_save_indexes[g_Save_state.profiles[profile_index].selected_mission[0]]] == 1) {
            sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_spaceKey, g_Message_story);
            BlitTextWithKanjiToScreen(g_Font_mtext, (640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer)) / 2, 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
        }
    }
    SwapScreenBuffer();
}

// FUNCTION: GTA 0x00429180
void DoOptionsMenu(tMenu_action action) {
    if (action & eMenuAction_Up) {
        PlayMenuSound(7);
        g_Selected_menu_option -= 1;
        if (g_Selected_menu_option <= 0) {
            g_Selected_menu_option = 5;
        }
    }
    if (action & eMenuAction_Down) {
        PlayMenuSound(8);
        g_Selected_menu_option += 1;
        if (g_Selected_menu_option > 5) {
            g_Selected_menu_option = 1;
        }
    }
    int selection_dx = 0;
    if (action & eMenuAction_Right) {
        selection_dx = 1;
    } else if (action & eMenuAction_Left) {
        selection_dx = -1;
    }
    if (action & eMenuAction_Enter) {
        PlayMenuSound(2);
        UpdateNextMenu(eMenu_main_menu);
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == NULL) {
            FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
        }
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state),f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
        }
        fclose(f);
    } else if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_main_menu);
        FILE *f = fopen("..\\gtadata\\player_a.dat", "rb");
        if (f != NULL) {
            fread(&g_Save_state, 1, sizeof(g_Save_state), f);
            fclose(f);
        }
        ApplySoundVolumes();
        PlayMenuSound(4);
    }

    if (selection_dx > 0) {
        switch (g_Selected_menu_option) {
        case 1:
            if (g_Save_state.sound_volume >= 7) {
                break;
            }
            g_Save_state.sound_volume += 1;
            ApplySoundVolumes();
            PlayMenuSound(14);
            break;
        case 2:
            if (g_Save_state.music_volume >= 7) {
                break;
            }
            g_Save_state.music_volume += 1;
            ApplySoundVolumes();
            break;
        case 3:
            g_Save_state.text_speed += 1;
            if (g_Save_state.text_speed > 3) {
                g_Save_state.text_speed = 1;
            }
            PlayMenuSound(8 + g_Save_state.text_speed);
            break;
        case 4:
            if (!g_Demo_version) {
                if (g_Save_state.music_mode == eMusicMode_radio) {
                    PlayMenuSound(5);
                    g_Save_state.music_mode = eMusicMode_constant;
                } else {
                    PlayMenuSound(6);
                    g_Save_state.music_mode = eMusicMode_radio;
                }
            }
            break;
        case 5:
            g_Save_state.transparency_effects = !g_Save_state.transparency_effects;
            break;
        }
    } else if (selection_dx < 0) {
        switch (g_Selected_menu_option) {
        case 1:
            if (g_Save_state.sound_volume <= 0) {
                break;
            }
            g_Save_state.sound_volume -= 1;
            ApplySoundVolumes();
            PlayMenuSound(14);
            break;
        case 2:
            if (g_Save_state.music_volume <= 0) {
                break;
            }
            g_Save_state.music_volume -= 1;
            ApplySoundVolumes();
            break;
        case 3:
            g_Save_state.text_speed -= 1;
            if (g_Save_state.text_speed <= 0) {
                g_Save_state.text_speed = eTextSpeed_fast;
            }
            PlayMenuSound(8 + g_Save_state.text_speed);
            break;
        case 4:
            if (!g_Demo_version) {
                if (g_Save_state.music_mode == eMusicMode_radio) {
                    PlayMenuSound(5);
                    g_Save_state.music_mode = eMusicMode_constant;
                } else {
                    PlayMenuSound(6);
                    g_Save_state.music_mode = eMusicMode_radio;
                }
            }
            break;
        case 5:
            g_Save_state.transparency_effects = !g_Save_state.transparency_effects;
            break;
        }
    }
    BlitMenuBackground(TRUE, 0);
    BlitHeadTextCentered(176, g_Message_options);

    if (g_Save_state.sound_volume > 0) {
        sprintf(g_Temp_menu_buffer, g_Message_sdcolon, g_Message_sound, g_Save_state.sound_volume);
    } else {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_sound, g_Message_off);
    }
    DrawMenuOptionWithMaybeKey_224(1, g_Selected_menu_option, g_Temp_menu_buffer);

    if (g_Save_state.music_volume > 0) {
        sprintf(g_Temp_menu_buffer,g_Message_sdcolon,g_Message_music, g_Save_state.music_volume);
    } else {
        sprintf(g_Temp_menu_buffer,g_Message_sscolon, g_Message_music, g_Message_off);
    }
    DrawMenuOptionWithMaybeKey_224(2, g_Selected_menu_option, g_Temp_menu_buffer);

    switch (g_Save_state.text_speed) {
    case eTextSpeed_slow:
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_text, g_Message_slow);
        break;
    case eTextSpeed_normal:
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_text, g_Message_normal);
        break;
    case eTextSpeed_fast:
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_text, g_Message_fast);
        break;
    }
    DrawMenuOptionWithMaybeKey_224(3, g_Selected_menu_option, g_Temp_menu_buffer);

    if (g_Save_state.music_mode == eMusicMode_radio) {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_musicMode, g_Message_radio);
    } else {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_musicMode, g_Message_constant);
    }
    DrawMenuOptionWithMaybeKey_224(4, g_Selected_menu_option, g_Temp_menu_buffer);

    if (g_Save_state.transparency_effects) {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_transEffects, g_Message_on);
    } else {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_transEffects, g_Message_off);
    }
    DrawMenuOptionWithMaybeKey_224(5, g_Selected_menu_option, g_Temp_menu_buffer);

    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_cancel);
    BlitTextWithKanjiToScreen(g_Font_mtext, 0, 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);

    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_returnKey, g_Message_save);
    BlitTextWithKanjiToScreen(g_Font_mtext,
        640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer),
        480 - g_Font_mtext->header.height,
        g_Temp_menu_buffer);

    SwapScreenBuffer();
}

// FUNCTION: GTA 0x00429a20
void DoProfileSelectionMenu(tMenu_action action) {
    if (action & eMenuAction_Left) {
        PlayMenuSound(3);
        g_Save_state.active_profile_index -= 1;
        if (g_Save_state.active_profile_index < 0) {
            g_Save_state.active_profile_index = 7;
        }
        if (IsTranslated()) {
            if (g_Save_state.active_profile_index == 0) {
                g_Save_state.active_profile_index = 7;
            } else if (g_Save_state.active_profile_index > 1 && g_Save_state.active_profile_index < 5) {
                g_Save_state.active_profile_index = 1;
            }
        }
    }
    if (action & eMenuAction_Right) {
        PlayMenuSound(3);
        g_Save_state.active_profile_index += 1;
        if (g_Save_state.active_profile_index > 7) {
            g_Save_state.active_profile_index = 0;
        }
        if (IsTranslated()) {
            if (g_Save_state.active_profile_index == 0) {
                g_Save_state.active_profile_index = 1;
            }
            else if (g_Save_state.active_profile_index > 1 && g_Save_state.active_profile_index < 5) {
                g_Save_state.active_profile_index = 5;
            }
        }
    }
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_main_menu);
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == NULL) {
            FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
        }
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
        }
        fclose(f);
        PlayMenuSound(4);
    }
    if (action & eMenuAction_Enter) {
        PlayMenuSound(2);
        SetMultiplayerProfileName(0,
            g_Save_state.profiles[g_Save_state.active_profile_index].name,
            g_Save_state.active_profile_index);
        if (g_Menu_host_or_client == eMenuMP_multiplayer_client) {
            Multiplayer_FUN_0044b8a0(1);
            g_Multiplayer_waiting_room_state = eWaiting_room_WaitingOnLetsGo;
            g_Multiplayer_BOOL_004af414 = TRUE;
            if (EnumerateMultiplayerProviders()) {
                if (g_Current_multiplayer_session_index >= GetCountMultiplayerProviders()) {
                    g_Current_multiplayer_session_index = 0;
                }
                UpdateNextMenu(eMenu_connection_select);
            }
        }
        else {
            UpdateNextMenu(eMenu_city_select);
        }
    }
    if (action & eMenuAction_Backspace) {
        PlayMenuSound(12);
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == NULL) {
            FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
        }
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478,"..\\gtadata\\player_a.dat");
        }
        fclose(f);
        UpdateNextMenu(eMenu_profile_name_edit);
    }
    if ((action & eMenuAction_Char) && (action & 0xff0000) == ('r' << 16) && g_Menu_host_or_client == eMenuMP_singleplayer) {
        PlayMenuSound(13);
        UpdateNextMenu(eMenu_profile_reset);
    }
    BlitMenuBackground(TRUE, 0);
    BlitProfilePictureAndText(319, 192, g_Save_state.active_profile_index, NULL, TRUE);
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        DisplayProfileScores(414, 202, g_Save_state.active_profile_index, g_Message_scores);
    }
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_delKey, g_Message_rename);
    BlitTextWithKanjiToScreen(g_Font_mmis, 440, 400, g_Temp_menu_buffer);
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_rKey, g_Message_reset);
        BlitTextWithKanjiToScreen(g_Font_mmis, 440, 400 + g_Font_mmis->header.height, g_Temp_menu_buffer);
    }
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_cancel);
    BlitTextWithKanjiToScreen(g_Font_mtext, 0, 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_returnKey, g_Message_play);
    BlitTextWithKanjiToScreen(g_Font_mtext, 640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer),480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    SwapScreenBuffer();
}

static bool inline check_cheat(const char *name, const char *cheat) {
    while (*name != '\0') {
        if (*name != *cheat) {
            return false;
        }
        name += 1;
        cheat += 2;
    }
    return *cheat == '\0';
}

// FUNCTION: GTA 0x00412c50
const char *GetMultiplayerJoinGatherString() {
    sprintf(g_Join_gather_status_string, "");
    const char *id;
    switch (g_Multiplayer_room_status) {
    case eWaiting_room_WaitingOnLetsGo:
        id = "join";
        break;
    case eWaiting_room_NoLetsGoSent:
        id = "gather";
        break;
    default:
        FatalError(eFatalError_invalid_case, 235, g_Multiplayer_room_status);
    }
    strcat(g_Join_gather_status_string, GetTranslatedString(id));
    return g_Join_gather_status_string;
}

// FUNCTION: GTA 0x0042a160
void DoProfileNameEditMenu(tMenu_action action) {
    int len_profile_name = strlen(g_Buffer_profile_name);
    if ((action & eMenuAction_Backspace) && len_profile_name != 0) {
        PlayMenuSound(13);
        g_Buffer_profile_name[len_profile_name - 1] = '\0';
    }
    if (((action & eMenuAction_Char) || (action & eMenuAction_Space)) && len_profile_name < GTA_ASIZE(g_Buffer_profile_name) - 2) {
        PlayMenuSound(12);
        char c;
        if (action & eMenuAction_Space) {
            c = ' ';
        } else {
            c = action >> 16;
            if ((action & eMenuAction_Shift) && islower(c)) {
                c += 'A' - 'a';
            }
        }
        g_Buffer_profile_name[len_profile_name] = c;
        g_Buffer_profile_name[len_profile_name + 1] = '\0';
    }
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_profile_select);
        FILE *f = fopen("..\\gtadata\\player_a.dat","rb");
        if (f != NULL) {
            fread(&g_Save_state, 1, sizeof(g_Save_state), f);
            fclose(f);
        }
        PlayMenuSound(4);
    } else if (action & eMenuAction_Enter) {
        UpdateNextMenu(eMenu_profile_select);
        if (len_profile_name == 0) {
            strcpy(g_Save_state.profiles[g_Save_state.active_profile_index].name,
                g_Default_profiles[g_Save_state.active_profile_index].name);
        } else {
            strcpy(g_Save_state.profiles[g_Save_state.active_profile_index].name,
                g_Buffer_profile_name);
        }
        bool is_cheat;
        if (g_Demo_version == false) {
            is_cheat = true;
            if (check_cheat(g_Cheat_nineinarow, g_Save_state.profiles[g_Save_state.active_profile_index].name)
                    || check_cheat(g_Cheat_super_well, g_Save_state.profiles[g_Save_state.active_profile_index].name)
                    || (check_cheat(g_Cheat_itsgallus, g_Save_state.profiles[g_Save_state.active_profile_index].name))) {
                // nineinarow: all levels
                // super well: all levels
                // itsgallus: all levels
                for (int i = 0; i < 3; i++) {
                    tCity *city = &g_Current_cities[i];
                    for (int j = 0; j < city->count_missions; j++) {
                        if (g_Save_state.profiles[g_Save_state.active_profile_index].scores[city->mission_save_indexes[j]] == -1) {
                            g_Save_state.profiles[g_Save_state.active_profile_index].scores[city->mission_save_indexes[j]] = 0;
                        }
                    }
                }
            } else if (check_cheat(g_Cheat_6031769, g_Save_state.profiles[g_Save_state.active_profile_index].name)
                    || check_cheat(g_Cheat_itstantrum, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // 6031769: unlimited lives
                // itstantrum: unlimited lives
                g_Unlimited_lives = true;
            } else if (check_cheat(g_Cheat_iamthelaw, g_Save_state.profiles[g_Save_state.active_profile_index].name)
                    || check_cheat(g_Cheat_stevesmates, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // iamthelaw: no police
                // stevesmates: no police
                g_No_police = true;
            } else if (check_cheat(g_Cheat_callmenigel, g_Save_state.profiles[g_Save_state.active_profile_index].name)
                    || check_cheat(g_Cheat_buckfast, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // callmenigel: all weapons
                // buckfast: all weapons
                g_All_weapons = true;
            } else if (check_cheat(g_Cheat_porkcharsui, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // porkcharsui: debug mode
                g_Debug_mode = TRUE;
            } else if (check_cheat(g_Cheat_Heart_of_gold, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // Heart of Gold: all missions finished
                for (int i = 0; i < GTA_ASIZE(g_Save_state.profiles[g_Save_state.active_profile_index].missions_finished); i++) {
                    g_Save_state.profiles[g_Save_state.active_profile_index].missions_finished[i] = 1;
                }
            } else if (check_cheat(g_Cheat_iamgarypenn, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // iamgarypenn: massive cursing
                g_Save_state.language_id = eLanguage_special;
            } else if (check_cheat(g_Cheat_iamnotgarypenn, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // iamnotgarypenn: massive cursing
                g_Save_state.language_id = -1;
            } else if (check_cheat(g_Cheat_hate_machine, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // hate machine: +10 Score Multipliers from the start
                g_BOOL_00502f74 = true;
            } else if (check_cheat(g_Cheat_itcouldbeyou, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // itcouldbeyou: 9,999,999 points
                g_BOOL_0050318c = true;
            } else if (check_cheat(g_Cheat_suckmyrocket, g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                // suckmyrocket: start with all weapons and power-ups
                g_BOOL_005031e5 = true;
            } else {
                is_cheat = false;
            }
        } else {
            is_cheat = false;
        }
        if (is_cheat) {
            PlayMenuSound(7);
        } else {
            PlayMenuSound(2);
        }
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == NULL) {
            FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
        }
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
        }
        fclose(f);
    }
    BlitMenuBackground(TRUE, 0);
    BlitProfilePictureAndText(319, 192, g_Save_state.active_profile_index, g_Buffer_profile_name, 1);
    if (g_Menu_host_or_client == eMenuMP_singleplayer) {
        DisplayProfileScores(414, 202, g_Save_state.active_profile_index, g_Message_scores);
    }
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_cancel);
    BlitTextWithKanjiToScreen(g_Font_mtext, 0, 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_returnKey, len_profile_name == 0 ? g_Message_reset : g_Message_finish);
    BlitTextWithKanjiToScreen(g_Font_mtext,
        640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer),
        480 - g_Font_mtext->header.height,
        g_Temp_menu_buffer);
    SwapScreenBuffer();
}

// STUB: GTA 0x0042a750
void DoPostGameSummaryMenu(tMenu_action) {
    // FIXME
    NOT_IMPLEMENTED();
}

// FUNCTION: GTA 0x0042b260
void DoResetProfileMenu(tMenu_action action) {
    if (action & eMenuAction_Up) {
        PlayMenuSound(7);
        g_Selected_menu_option -= 1;
        if (g_Selected_menu_option <= 0) {
            g_Selected_menu_option = 2;
        }
    }
    if (action & eMenuAction_Down) {
        PlayMenuSound(8);
        g_Selected_menu_option += 1;
        if (g_Selected_menu_option > 2) {
            g_Selected_menu_option = 1;
        }
    }
    if (action & eMenuAction_Escape) {
        PlayMenuSound(4);
        action |= eMenuAction_Enter;
        g_Selected_menu_option = 1;
    }
    if ((action & eMenuAction_Enter) || (action & eMenuAction_Space)) {
        PlayMenuSound(2);
        if (g_Selected_menu_option == 1) {
            UpdateNextMenu(eMenu_profile_select);
        } else if (g_Selected_menu_option == 2) {
            strcpy(g_Save_state.profiles[g_Save_state.active_profile_index].name, g_Default_profiles[g_Save_state.active_profile_index].name);
            g_Save_state.profiles[g_Save_state.active_profile_index].scores[0] = 0;
            {
                int i;
                for (i = 1; i < GTA_ASIZE(g_Save_state.profiles[g_Save_state.active_profile_index].scores); i++) {
                    g_Save_state.profiles[g_Save_state.active_profile_index].scores[i] = -1;
                }
            }
            {
                int i;
                for (i = 0; i < GTA_ASIZE(g_Save_state.profiles[g_Save_state.active_profile_index].missions_finished); i++) {
                    g_Save_state.profiles[g_Save_state.active_profile_index].missions_finished[i] = 0;
                }
                }
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[0] = 0;
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[0] = 0;
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_city[1] = 0;
            g_Save_state.profiles[g_Save_state.active_profile_index].selected_mission[1] = 0;
            UpdateNextMenu(eMenu_profile_select);
            FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
            if (f == NULL) {
                FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
            }
            if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
                FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
            }
            fclose(f);
        }
    }
    BlitMenuBackground(TRUE, 0);
    DrawMenuOptionWithMaybeKey_216(1, g_Selected_menu_option, g_Message_cancel);
    DrawMenuOptionWithMaybeKey_216(2, g_Selected_menu_option, g_Message_reset);
    BlitProfilePictureAndText(447, 192, g_Save_state.active_profile_index, NULL, FALSE);
    SwapScreenBuffer();
}

// STUB: GTA 0x004282c0
void DoClientMultiplayerSessionSelectMenu(tMenu_action) {
    // FIXME
    NOT_IMPLEMENTED();
}

// FUNCTION: GTA 0x00428480
void DoCreateNewHostSessionMenu(tMenu_action action) {
    int len_session_name = strlen(g_Save_state.session_name);
    g_Multiplayer_session_name_caret_index += 1;
    if (g_Multiplayer_session_name_caret_index > 7) {
        g_Multiplayer_session_name_caret_index = 0;
    }
    if (action & eMenuAction_Backspace) {
        PlayMenuSound(14);
        if (len_session_name != 0) {
            g_Save_state.session_name[len_session_name - 1] = '\0';
            len_session_name -= 1;
        }
    }
    if ((action & eMenuAction_Char) || (action & eMenuAction_Space) && len_session_name < 15) {
        PlayMenuSound(12);
        char c;
        if (action & eMenuAction_Space) {
            c = ' ';
        } else {
            c = action >> 16;
            if (action & eMenuAction_Shift) {
                if (islower(c)) {
                    c += 'A' - 'a';
                }
            }
        }
        g_Save_state.session_name[len_session_name] = c;
        g_Save_state.session_name[len_session_name + 1] = '\0';
        len_session_name += 1;
    }
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_connection_select);
        PlaySample(4);
    }
    if (action & eMenuAction_Enter) {
        PlayMenuSound(2);
        if (len_session_name != 0) {
            ClearScreenOrCopyCutscene(0);
            SwapScreenBuffer();
            if (CreateMultiplayerSession(g_Save_state.session_name) && RegisterMultiplayerPlayer(g_Save_state.profiles[g_Save_state.active_profile_index].name)) {
                g_Multiplayer_waiting_room_state = eWaiting_room_NoLetsGoSent;
                g_Multiplayer_BOOL_004af414 = 1;
                UpdateNextMenu(eMenu_host_wait_for_players);
            }
        }
    }
    BlitMenuBackground(TRUE, 0);
    BlitTextWithKanjiToScreen(g_Font_mtext, 320 - GetTextWidthWithExtra(g_Font_mtext, g_Message_fix5) / 2, 240, g_Message_fix5);
    BlitTextWithKanjiToScreen(g_Font_mtext, 320 - GetTextWidthWithExtra(g_Font_mtext, g_Save_state.session_name) / 2, g_Font_mtext->header.height + 240, g_Save_state.session_name);
    if (g_Multiplayer_session_name_caret_index < 4) {
        BlitTextWithKanjiToScreen(g_Font_mtext, GetTextWidthWithExtra(g_Font_mtext, g_Save_state.session_name) / 2 + 320, g_Font_mtext->header.height + 240, "<");
    }
    SwapScreenBuffer();
}

// FUNCTION: GTA 0x004276e0
void DoConnectionFailureMenu(tMenu_action action, int error_code) {
    if ((action & eMenuAction_Enter) || (action & eMenuAction_Escape) || (action & eMenuAction_Space)) {
        UpdateNextMenu(eMenu_main_menu);
    }
    BlitMenuBackground(TRUE, 0);
    BlitHeadTextCentered(360, GetMultiplayerJoinGatherString());
    const char *msg;
    if (error_code == 19) {
        msg = g_Message_commsversionerror;
    } else {
        msg = g_Message_commsfail;
    }
    BlitTextWithKanjiToScreen(g_Font_mtext, 320 - GetTextWidthWithExtra(g_Font_mtext, msg) / 2, 428, msg);
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_quit);
    BlitTextWithKanjiToScreen(g_Font_mtext, 640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer), 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    SwapScreenBuffer();
}

// FUNCTION: GTA 0x00429650
void DoMultiplayerOptionsMenu(tMenu_action action) {
    if (action & eMenuAction_Up) {
        g_Selected_menu_option -= 1;
        if (g_Selected_menu_option <= 0) {
            g_Selected_menu_option = 2;
        }
    } else if (action & eMenuAction_Down) {
        g_Selected_menu_option += 1;
        if (g_Selected_menu_option > 2) {
            g_Selected_menu_option = 1;
        }
    }
    int selection_dx = 0;
    if (action & eMenuAction_Right) {
        PlayMenuSound(1);
        selection_dx = 1;
    } else if (action & eMenuAction_Left) {
        PlayMenuSound(0);
        selection_dx = -1;
    }
    if (action & eMenuAction_Escape) {
        UpdateNextMenu(eMenu_city_select);
        FILE *f = fopen("..\\gtadata\\player_a.dat", "rb");
        if (f != NULL) {
            fread(&g_Save_state, 1, sizeof(g_Save_state), f);
            fclose(f);
        }
        PlayMenuSound(4);
    } else if (action & eMenuAction_Enter) {
        PlayMenuSound(2);
        Multiplayer_FUN_0044b8a0(0);
        g_Multiplayer_waiting_room_state = eWaiting_room_NoLetsGoSent;
        g_Multiplayer_BOOL_004af414 = 1;
        if (EnumerateMultiplayerProviders()) {
            if (g_Current_multiplayer_session_index >= GetCountMultiplayerProviders()) {
                g_Current_multiplayer_session_index = 0;
            }
            UpdateNextMenu(eMenu_connection_select);
        }
        FILE *f = fopen("..\\gtadata\\player_a.dat", "wb");
        if (f == NULL) {
            FatalError(eFatalError_cannot_open_S, 478, "..\\gtadata\\player_a.dat");
        }
        if (fwrite(&g_Save_state, 1, sizeof(g_Save_state), f) != sizeof(g_Save_state)) {
            FatalError(eFatalError_cannot_write_data_to_S, 478, "..\\gtadata\\player_a.dat");
        }
        fclose(f);
    }
    if (selection_dx != 0) {
        if (g_Selected_menu_option == 1) {
            if (g_Save_state.multiplayer_target == eMultiplayerGameType_money) {
                g_Save_state.multiplayer_target = eMultiplayerGameType_frags;
            } else {
                g_Save_state.multiplayer_target = eMultiplayerGameType_money;
            }
        } else if (g_Selected_menu_option == 2) {
            if (g_Save_state.multiplayer_target == eMultiplayerGameType_money) {
                g_Save_state.multiplayer_money_target = g_Save_state.multiplayer_money_target + 10000 * selection_dx;
                if (g_Save_state.multiplayer_money_target < 10000) {
                    g_Save_state.multiplayer_money_target = 10000;
                } else if (g_Save_state.multiplayer_money_target > 999999999) {
                    g_Save_state.multiplayer_money_target = 999999999;
                }
            } else if (g_Save_state.multiplayer_target == eMultiplayerGameType_frags) {
                g_Save_state.multiplayer_kill_target += selection_dx;
                if (g_Save_state.multiplayer_kill_target < 1) {
                    g_Save_state.multiplayer_kill_target = 1;
                } else if (g_Save_state.multiplayer_kill_target > 1000) {
                    g_Save_state.multiplayer_kill_target = 1000;
                }
            }
        }
    }
    BlitMenuBackground(TRUE, 0);
    BlitHeadTextCentered(176, g_Message_multiOptions);
    if (g_Save_state.multiplayer_target == eMultiplayerGameType_money) {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_endGame, g_Message_score);
        DrawMenuOptionWithMaybeKey_224(1, g_Selected_menu_option, g_Temp_menu_buffer);
        sprintf(g_Temp_menu_buffer, g_Message_sdcolon, g_Message_endScore, g_Save_state.multiplayer_money_target);
        DrawMenuOptionWithMaybeKey_224(2, g_Selected_menu_option, g_Temp_menu_buffer);
    } else if (g_Save_state.multiplayer_target == eMultiplayerGameType_frags) {
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_endGame,g_Message_kills);
        DrawMenuOptionWithMaybeKey_224(1, g_Selected_menu_option, g_Temp_menu_buffer);
        sprintf(g_Temp_menu_buffer, g_Message_sdcolon, g_Message_endKills, g_Save_state.multiplayer_kill_target);
        DrawMenuOptionWithMaybeKey_224(2, g_Selected_menu_option, g_Temp_menu_buffer);
    }
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_cancel);
    BlitTextWithKanjiToScreen(g_Font_mtext, 0, 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_returnKey, g_Message_play);
    BlitTextWithKanjiToScreen(g_Font_mtext, 640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer), 480 - g_Font_mtext->header.height, g_Temp_menu_buffer);
    SwapScreenBuffer();
}

// FUNCTION: GTA 0x0042d590
void SetMenuViewport(int left, int top, int right, int bottom) {
    g_Menu_viewport.left = left;
    g_Menu_viewport.top = top;
    g_Menu_viewport.right = right;
    g_Menu_viewport.bottom = bottom;
}

// FUNCTION: GTA 0x00426a50
tMenu_result DoMenuStep(tMenu_action action) {
    g_Current_menu_updated = FALSE;
    switch (g_Current_menu) {
    case eMenu_connection_select:
        DoChooseAConnectionMenu(action);
        break;
    case eMenu_cutscene:
        {
            int cutscene_time = GetCutsceneAudioPosition();
            if (cutscene_time != -1) {
                ClearScreenOrCopyCutscene(2);
                int frame = g_Cutscene_index;
                if (cutscene_time == -2) {
                    g_Cutscene_index += 1;
                    if (g_Cutscene_index == 192) {
                        g_Cutscene_index = 0;
                    }
                } else {
                    frame = cutscene_time / 100;
                }
                for (int font = 0; font < g_Count_mission_cutscene_fonts[g_Menu_current_mission_index]; font++) {
                    char animation_char = g_Cut_scene_animation_data[g_Menu_current_mission_index][font][frame];
                    if (animation_char != '0') {
                        char text[2];
                        text[0] = animation_char - '0';
                        text[1] = '\0';
                        BlitTextWithKanjiToScreen(g_City_cut_fonts[font],
                            g_Cut_scene_animation_positions[g_Menu_current_mission_index][font][0],
                            g_Cut_scene_animation_positions[g_Menu_current_mission_index][font][1],
                            text);
                    }
                }
                u8 cut_text_index = g_Cut_scene_animation_data[g_Menu_current_mission_index][4][frame] - '0';
                if (cut_text_index != 0) {
                    const char *text = GetTranslatedString(g_Cut_text_names[g_Menu_current_mission_index][cut_text_index - 1]);
                    BlitTextCenterAligned(g_Font_cuttext, text, 0, 480 - 2 * g_Font_cuttext->header.height, 640);
                }
                SwapScreenBuffer();
                if (!(action & (eMenuAction_Enter | eMenuAction_Escape)) && !(action & eMenuAction_Space)) {
                    break;
                }
            }
            StopCutsceneMusic();
            PlayMenuSound(2);
            if (g_Post_cutscene_menu == ePostCutscene_city_select) {
                UpdateNextMenu(eMenu_city_select);
            } else if (g_Post_cutscene_menu == ePostCutscene_game_summary) {
                UpdateNextMenu(eMenu_post_game_summary);
            }
            break;
        }
    case eMenu_error:
        if ((action & (eMenuAction_Enter | eMenuAction_Escape)) || (action & eMenuAction_Space)) {
            UpdateNextMenu(eMenu_main_menu);
        }
        BlitMenuBackground(TRUE, 0);
        BlitHeadTextCentered(408, "Error");
        BlitTextWithKanjiToScreen(g_Font_mtext, 640 / 2 - GetTextWidthWithExtra(g_Font_mtext, g_Menu_error_buffer) / 2, 240, g_Menu_error_buffer);
        SwapScreenBuffer();
        break;
    case eMenu_host_wait_for_players:
        DoHostWaitOnPlayersMenu(action);
        break;
    case eMenu_cd_check:
        if (g_GTA_cd_present) {
            UpdateNextMenu(eMenu_main_menu);
            ClearScreenOrCopyCutscene(0);
            SwapScreenBuffer();
        } else {
            if ((action & (eMenuAction_Enter | eMenuAction_Escape)) || (action & eMenuAction_Space)) {
                UpdateNextMenu(eMenu_main_menu);
            }
            BlitMenuBackground(TRUE, 0);
            BlitHeadTextCentered(176, g_Message_cdTitle);
            BlitMultilineText(g_Font_mtext, 120, 224, 400, g_Message_cdText);
            BlitReturnKeyText(g_Message_play);
            SwapScreenBuffer();
        }
        break;
    case eMenu_client_wait_for_players:
        DoClientWaitForPlayersMenu(action);
        break;
    case eMenu_loading:
        DoLoadMenu(action);
        break;
    case eMenu_main_menu:
        DoMainMenu(action);
        break;
    case eMenu_city_select:
        DoCitySelectMenu(action);
        break;
    case eMenu_options:
        DoOptionsMenu(action);
        break;
    case eMenu_exit_credits:
        {
            /* DoCreditsOnExitMenu */
            if (!(g_Exit_font_height_initialized & 0x1)) {
                g_Exit_font_height_initialized |= 0x1;
                g_Exit_font_height = g_Font_mtext->header.height;
            }
            SetMenuViewport(0, 192, 640 - 1, 480 - 1);
            g_Exit_credits_time += 2;
            int credit_index = g_Exit_credits_time / g_Exit_font_height;
            int start_y = g_Exit_credits_time % g_Exit_font_height;
            int end_credit_index = credit_index + 12;
            if (g_Text_exit_credits[credit_index] == NULL || (action & (eMenuAction_Enter | eMenuAction_Escape)) ||
                (action & eMenuAction_Space)) {
                g_Menu_return_code = eMenuRC_exit;
            }
            BlitMenuBackground(TRUE, 0);
            for (int y = 0; credit_index < end_credit_index; credit_index += 1) {
                const char *text = g_Text_exit_credits[credit_index];
                if (text == NULL) {
                    break;
                }
                if (text[0] != '\0') {
                    int x;
                    if (text[0] == '@') {
                        text += 1;
                        x = 128;
                    } else {
                        x = 64;
                    }
                    BlitMenuViewportClippedTextLine(g_Font_mtext, x, 192 - start_y + y, text);
                }
                y += g_Font_mtext->header.height;
            }
            SwapScreenBuffer();
            break;
        }
    case eMenu_profile_select:
        DoProfileSelectionMenu(action);
        break;
    case eMenu_profile_name_edit:
        DoProfileNameEditMenu(action);
        break;
    case eMenu_post_game_summary:
        DoPostGameSummaryMenu(action);
        break;
    case eMenu_profile_reset:
        DoResetProfileMenu(action);
        break;
    case eMenu_multiplayer_client:
        DoClientMultiplayerSessionSelectMenu(action);
        break;
    case eMenu_multiplayer_new_host_session:
        DoCreateNewHostSessionMenu(action);
        break;
    case eMenu_connection_failure:
        DoConnectionFailureMenu(action, 17);
        break;
    case eMenu_multiplayer_options:
        DoMultiplayerOptionsMenu(action);
        break;
    case eMenu_connection_version_mismatch_failure:
        DoConnectionFailureMenu(action, 19);
        break;
    case eMenu_svga_error:
        if ((action & (eMenuAction_Enter | eMenuAction_Escape)) || (action & eMenuAction_Space)) {
            UpdateNextMenu(eMenu_main_menu);
        }
        BlitMenuBackground(TRUE, 0);
        BlitTextWithKanjiToScreen(g_Font_mtext, 640 / 2 - GetTextWidthWithExtra(g_Font_mtext, g_Message_svgaerror) / 2, 428, g_Message_svgaerror);
        sprintf(g_Temp_menu_buffer, g_Message_sscolon, g_Message_escKey, g_Message_quit);
        BlitTextWithKanjiToScreen(g_Font_mtext,
            640 - GetTextWidthWithExtra(g_Font_mtext, g_Temp_menu_buffer),
            480 - (uint)g_Font_mtext->header.height,
            g_Temp_menu_buffer);
        SwapScreenBuffer();
        break;
    default:
        ServiceMusic();
        return g_Menu_return_code;
    }
    if (g_Current_menu_updated == TRUE) {
        g_Selected_menu_option = 1;
    }
    ServiceMusic();
    return g_Menu_return_code;
}

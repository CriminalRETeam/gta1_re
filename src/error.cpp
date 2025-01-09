#include "error.h"

#include "decomp.h"

#include <mgraph.h>

#include <sstream>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


// GLOBAL: GTA 0x0050f5b8
int g_Error_state;

// GLOBAL: GTA 0x0050f3b8
char g_Current_file_path[256];

// GLOBAL: GTA 0x0050f4b8
char g_Error_temp_path[256];

// FUNCTION: GTA 0x004228b0
void InitError() {
    g_Error_state = 0;
    g_Current_file_path[0] = '\0';
}

// FUNCTION: GTA 0x004228e0
void QuitError() {
    if (g_Error_state == 0) {
        g_Error_state = 1;
    }
}

// FUNCTION: GTA 0x004228c0
void SetCurrentLoadFilePath(const char *path) {
    strcpy(g_Current_file_path, path);
}

#include <SDL3/SDL.h>

// FUNCTION: GTA 0x00422900
void FatalError(tError error, int location, ...) {
    SDL_TriggerBreakpoint();

    if (g_Error_state == 0 || g_Error_state == 1) {
        strcpy(g_Error_temp_path, g_Current_file_path);
        g_Error_state = 2;
        FreeOnError();
        strcpy(g_Current_file_path, g_Error_temp_path);
    }
    va_list ap;
    va_start(ap, location);
    switch (error) {
        case eFatalError_no_rail_found_around_DDD:
        case eFatalError_invalid_tile_D_at_DDD:
        case eFatalError_unable_to_place_car_type_D_at_DDD:
        case eFatalError_unknown_error_neg140:
        case eFatalError_invalid_map_access_at_DDD:
        case eFatalError_invalid_tl_junction_DDD:
            ShowErrorMessage(error, location, va_arg(ap, uintptr_t), va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
            break;
        default:
            ShowErrorMessage(error, location, va_arg(ap, uintptr_t));
            break;
    }
    va_end(ap);
    exit(-1);
}

// FUNCTION: GTA 0x004229e0
void ShowErrorMessage(tError error, int location, ...) {
    char message_buffer[80];
    BOOL read_error;
    BOOL network_error;
    va_list ap;
    int extra;
    uintptr_t arg;

    va_start(ap, location);
    arg = va_arg(ap, uintptr_t);
    switch (error) {
    case eFatalError_no_rail_found_around_DDD:
    case eFatalError_invalid_tile_D_at_DDD:
    case eFatalError_unable_to_place_car_type_D_at_DDD:
    case eFatalError_unknown_error_neg140:
    case eFatalError_invalid_map_access_at_DDD:
    case eFatalError_invalid_tl_junction_DDD:
        extra = va_arg(ap, int);
        extra = va_arg(ap, int);
        extra = va_arg(ap, int);
        FormatErrorMessage(error, message_buffer, &read_error, &network_error, arg, extra, extra, extra);
        break;
    default:
        FormatErrorMessage(error, message_buffer, &read_error, &network_error, arg);
        break;
    }
    va_end(ap);

    std::stringstream sstream;
    sstream << "Error " << -error << '.' << location << '\n' << message_buffer << '\0';
    std::string s = sstream.str();
    MGL_fatalError(s.c_str());
}

// STUB: GTA 0x00422b90
void FormatErrorMessage(tError error, char *buffer, BOOL *read_error, BOOL *network_error, ...) {
    *read_error = FALSE;
    *network_error = FALSE;
    NOT_IMPLEMENTED();
    // TODO
}

// STUB: GTA 0x004371e0
void FreeOnError() {
    // TODO
    NOT_IMPLEMENTED();
}

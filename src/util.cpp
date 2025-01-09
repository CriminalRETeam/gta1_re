#include "util.h"
#include "common.h"
#include "error.h"

#include <stdio.h>

#include <windows.h>

// GLOBAL: GTA 0x00511954
FILE *g_File;

// GLOBAL: GTA 0x00511958
BOOL g_File_opened;


// FUNCTION: GTA 0x0043cc00
void *CheckedMalloc(size_t size) {
    if (size == 0) {
        FatalError(eFatalError_attempting_to_allocate_zero_bytes, 5, NULL);
    }
    void *mem = malloc(size);
    if (mem == NULL) {
        FatalError(eFatalError_out_of_memory_allocating_D_bytes, 5, size);
    }
    return mem;
}

// FUNCTION: GTA 0x0042dce0
void *ReadFileToBuffer(const char *path, size_t *size) {
    SetCurrentLoadFilePath(path);
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        FatalError(eFatalError_unable_to_open_file, 6, NULL);
    }
    if (ftell(f) == -1) {
        FatalError(eFatalError_ftell_error_in_file_S, 7, NULL);
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        FatalError(eFatalError_fseek_error_in_file_S, 7, NULL);
    }
    long file_size = ftell(f);
    if (file_size == -1) {
        FatalError(eFatalError_ftell_error_in_file_S, 7, NULL);
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        FatalError(eFatalError_fseek_error_in_file_S, 7, NULL);
    }
    *size = file_size;
    if (fclose(f)) {
        FatalError(eFatalError_unable_to_close_file_S, 6, NULL);
    }
    void *buffer = CheckedMalloc(*size);
    f = fopen(path, "rb");
    if (f == NULL) {
        free(buffer);
        FatalError(eFatalError_unable_to_open_file, 6, NULL);
    }
    if (fread(buffer, *size, 1, f) != 1) {
        free(buffer);
        fclose(f);
        FatalError(eFatalError_read_failure_in_file, 6, NULL);
    }
    if (fclose(f)) {
        free(buffer);
        FatalError(eFatalError_unable_to_close_file_S, 6, NULL);
    }
    return buffer;
}

static s64 GetCurrentSystemTimeIn100NS() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return (((s64)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

// GLOBAL: GTA 0x00786780
s64 g_System_time_at_startup_in_100ns = GetCurrentSystemTimeIn100NS();


// FUNCTION: GTA 0x0049ccb8
int GetMillisecondsSinceStartup() {
    s64 current_time = GetCurrentSystemTimeIn100NS();
    return (int)((current_time - g_System_time_at_startup_in_100ns) / 10000);
}

// FUNCTION: GTA 0x0042e2e0
void CloseTextFile() {
    if (g_File_opened) {
        int result = fclose(g_File);
        g_File_opened = FALSE;
        if (result != 0) {
            FatalError(eFatalError_unable_to_close_file_S, 22, NULL);
        }
    }
}

// FUNCTION: GTA 0x0042e270
void OpenTextFile(const char *path) {
    CloseTextFile();
    SetCurrentLoadFilePath(path);
    g_File = fopen(path, "rb");
    if (g_File == NULL) {
        FatalError(eFatalError_unable_to_open_file, 21, NULL);
    }
    g_File_opened = TRUE;
}

// FUNCTION: GTA 0x0042e400
void SkipUntilStringDelim(const char *delim) {
    for (;;) {
        size_t delim_i = 0;
        for (;;) {
            int c = fgetc(g_File);
            if (c == EOF) {
                FatalError(eFatalError_cannot_find_S_in_file_S, 98, delim, g_Current_file_path);
            }
            if (c != delim[delim_i]) {
                break;
            }
            delim_i += 1;
            if (delim[delim_i] == '\0') {
                return;
            }
        }
    }
}

// FUNCTION: GTA 0x0042e540
void ReadCopyUntilDelim(char *buffer, size_t capacity, char delim) {
    int state = 0;
    int last_text_pos = 0;
    int len = 0;
    for (;;) {
        int c = fgetc(g_File);
        if (c == delim) {
            buffer[last_text_pos + 1] = '\0';
            return;
        }
        if (c == -1) {
            buffer[last_text_pos + 1] = '\0';
            return;
        }
        switch (state) {
        case 0:
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                continue;
            }
            state = 1;
            // FALLTHROUGH
        case 1:
            if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
                last_text_pos = len;
                if (len >= capacity) {
                    FatalError(eFatalError_text_string_too_long_in_file_S, 101, NULL);
                }
            }
            if (len >= capacity) {
                continue;
            }
            buffer[len] = c;
            len += 1;
            break;
        default:
            FatalError(eFatalError_invalid_case, 101, state);
            break;
        }
    }
}

// GLOBAL: GTA 0x0042e440
int ReadInteger() {
    int number = 0;
    int state = 0;
    for (;;) {
        int ch = fgetc(g_File);
        if (ch == ',') {
            return number;
        }
        if (ch == EOF) {
            return number;
        }
        if (ch == '[') {
            int res = ungetc('[', g_File);
            if (res != '[') {
                FatalError(eFatalError_ungetc_failure_in_file_S, 100, NULL);
            }
            return number;
        }
        switch (state) {
        case 0:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
                continue;
            }
            state = 1;
            // FALLTHROUGH
        case 1:
            if (ch >= '0' && ch <= '9') {
                number = 10 * number + (ch - '0');
            } else if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
                state = 2;
            } else {
                FatalError(eFatalError_invalid_integer_encountered_in_file_S, 100, NULL);
            }
            break;
        case 2:
            if (!(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')) {
                FatalError(eFatalError_invalid_integer_encountered_in_file_S, 100, NULL);
            }
            break;
        }
    }
}

// FUNCTION: GTA 0x0042e0e0
void WriteBufferToFile(const char *path, const void *buffer, size_t size) {
    SetCurrentLoadFilePath(path);
    if (size == 0) {
        FatalError(eFatalError_writing_zero_bytes_to_file_S, 55, NULL);
    }
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        FatalError(eFatalError_unable_to_open_file, 55, NULL);
    }
    if (fwrite(buffer, size, 1, f) != 1) {
        fclose(f);
        FatalError(eFatalError_write_failure_on_file_S, 55, NULL);
    }
    if (fclose(f) != 0) {
        FatalError(eFatalError_unable_to_close_file_S, 55, NULL);
    }
}

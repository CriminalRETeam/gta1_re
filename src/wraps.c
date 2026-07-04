#include <SDL3/SDL.h>

#include <stdio.h>
#include <string.h>
extern FILE *__real_fopen(const char *path, const char *mode);

static int sys_initialized;
static char *working_dir;

char *strrpbrk(char *haystack, char *accept)
{
	char *last = NULL;

	for (; *haystack; ++haystack) {
		for (const char *a = accept; *a; ++a) {
			if (*haystack == *a) {
				last = haystack;
				break;
			}
		}
	}
	return last;
}

static void ensure_working_dir(void) {
	if (sys_initialized) {
		return;
	}
	working_dir = SDL_GetCurrentDirectory();
	char *working_dir_lastsep = strrpbrk(working_dir, "/\\");
	if (working_dir_lastsep != NULL) {
		*working_dir_lastsep = '\0';
	}
	working_dir_lastsep = strrpbrk(working_dir, "/\\");
	if (working_dir_lastsep != NULL) {
		*working_dir_lastsep = '\0';
	}
	sys_initialized = 1;
}

typedef struct {
	char disk_filename[1024];
	const char *filename;
	bool found;
} enum_context;

static SDL_EnumerationResult SDLCALL enum_callback(void *userdata, const char *dirname, const char *fname) {
	enum_context *context = userdata;
	if (SDL_strcasecmp(fname, context->filename) == 0) {
		SDL_strlcpy(context->disk_filename, fname, sizeof(context->disk_filename));
		context->found = true;
		return SDL_ENUM_SUCCESS;
	}
	return SDL_ENUM_CONTINUE;
}

FILE *__wrap_fopen(const char *const path, const char *const mode)
{
	FILE *f = __real_fopen(path, mode);
	if (f != NULL) {
		return f;
	}
	ensure_working_dir();
	const char *path_first_sep = strpbrk(path, "/\\");
	if (!(path_first_sep - path == 2 && strncasecmp(path, "..", 2) == 0)) {
		return NULL;
	}
	const char *ptr_orig = path_first_sep + 1;
	char real_path[1024];
	SDL_strlcpy(real_path, working_dir, sizeof(real_path));
	const char* ptr_sep;
	while (1) {
		ptr_sep = SDL_strpbrk(ptr_orig, "/\\");
		char newpart[1024];
		enum_context context = { 0 };
		if (ptr_sep == NULL) {
			SDL_strlcpy(newpart, ptr_orig, sizeof(newpart));
			context.filename = newpart;
		} else if (ptr_sep != ptr_orig + 1) {
			SDL_strlcpy(newpart, ptr_orig, ptr_sep - ptr_orig + 1);
			context.filename = newpart;
		}
		if (context.filename != NULL) {
			SDL_EnumerateDirectory(real_path, enum_callback, &context);
			if (!context.found && !(ptr_sep == NULL && SDL_strchr(mode, 'w') == NULL)) {
				return NULL;
			}
			SDL_strlcat(real_path, "/", sizeof(real_path));
			SDL_strlcat(real_path, context.disk_filename, sizeof(real_path));
		}
		if (ptr_sep == NULL) {
			break;
		}
		ptr_orig = ptr_sep + 1;
	}
	SDL_Log("'%s' -> '%s'", path, real_path);
	return __real_fopen(real_path, mode);;
}

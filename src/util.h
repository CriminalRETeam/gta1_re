#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

extern void *CheckedMalloc(size_t size);

extern void *ReadFileToBuffer(const char *path, size_t *size);

extern int GetMillisecondsSinceStartup();

extern void CloseTextFile();

extern void OpenTextFile(const char *path);

extern void SkipUntilStringDelim(const char *delim);

extern void ReadCopyUntilDelim(char *buffer, size_t capacity, char delim);

extern int ReadInteger();

extern void WriteBufferToFile(const char *path, const void *buffer, size_t size);

#endif // UTIL_H

#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

void *CheckedMalloc(size_t size);

void *ReadFileToBuffer(const char *path, size_t *size);

int GetMillisecondsSinceStartup();

void CloseTextFile();

void OpenTextFile(const char *path);

void SkipUntilStringDelim(const char *delim);

void ReadCopyUntilDelim(char *buffer, size_t capacity, char delim);

int ReadInteger();

void WriteBufferToFile(const char *path, const void *buffer, size_t size);

#endif // UTIL_H

#ifndef WIN_COMPAT_H
#define WIN_COMPAT_H

#ifdef _WIN32
#include <windows.h>

#else

#define CALLBACK

typedef int WINBOOL;
typedef char *LPSTR;
typedef wchar_t *LPWSTR;
typedef unsigned int ULONG;
typedef void *LPVOID;
typedef unsigned int HRESULT;
typedef unsigned int DWORD;
typedef DWORD *LPDWORD;
typedef void *HANDLE;

#define WAVE_FORMAT_PCM 1

typedef struct {
	unsigned short wFormatTag;
	unsigned short nChannels;
	unsigned int nSamplesPerSec;
	unsigned int nAvgBytesPerSec;
	unsigned short nBlockAlign;
} WAVEFORMAT;

typedef struct PCMWAVEFORMAT{
	WAVEFORMAT wf;
	unsigned short wBitsPerSample;
} PCMWAVEFORMAT;

typedef struct {
	unsigned int Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
} GUID, *LPGUID;

typedef GUID &REFIID;

class IUnknown {
public:
	virtual HRESULT CALLBACK QueryInterface(REFIID riid, void **ppvObject) = 0;
	virtual ULONG CALLBACK AddRef() = 0;
	virtual ULONG CALLBACK Release() = 0;
};

#endif

#endif // WIN_COMPAT_H

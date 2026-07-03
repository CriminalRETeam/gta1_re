#include "dplay.h"

GUID IID_IDirectPlay2A = {0x9d460580, 0xa822, 0x11cf, 0x96, 0xc, 0x0, 0x80, 0xc7, 0x53, 0x4e, 0x82};

HRESULT CALLBACK DirectPlayEnumerateA( LPDPENUMDPCALLBACKA, LPVOID ) {
	return 0;
}

HRESULT CALLBACK DirectPlayCreate( LPGUID lpGUID, LPDIRECTPLAY *lplpDP, IUnknown *pUnk) {
	return 0;
}

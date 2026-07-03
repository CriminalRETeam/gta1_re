#include "smack.h"

#include <stddef.h>

void SMACK_DECL SmackClose(HSMACK smack) {
}

unsigned int SMACK_DECL SmackDoFrame(HSMACK smack) {
	return 0;
}

void SMACK_DECL SmackNextFrame(HSMACK smack) {
}

HSMACK SMACK_DECL SmackOpen(const char*, unsigned int uFlags, unsigned int uExtraBuffers) {
	return NULL;
}

int SMACK_DECL SmackSoundUseMSS(SMACK_HDIGDRIVER hDrv) {
	return 0;
}

void SMACK_DECL SmackToBuffer(HSMACK smack, unsigned int uX, unsigned int uY, unsigned int uPitch, unsigned int uHeight, void *pBuffer, unsigned int uFlags) {
}

unsigned int SMACK_DECL SmackToBufferRect(HSMACK smack, unsigned int uSmackSurface) {
	return 0;
}

unsigned int SMACK_DECL SmackWait(HSMACK smack) {
	return 0;
}


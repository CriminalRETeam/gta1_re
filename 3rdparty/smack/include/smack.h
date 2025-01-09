#ifndef SMACK_H
#define SMACK_H

#ifdef _WIN32
#define SMACK_DECL __stdcall
#else
#define SMACK_DECL
#endif
#define SMACKTRACK1 0x02000
#define SMACKTRACK2 0x04000
#define SMACKTRACK3 0x08000
#define SMACKTRACK4 0x10000
#define SMACKTRACK5 0x20000
#define SMACKTRACK6 0x40000
#define SMACKTRACK7 0x80000
#define SMACKTRACKS (SMACKTRACK1|SMACKTRACK2|SMACKTRACK3|SMACKTRACK4|SMACKTRACK5|SMACKTRACK6|SMACKTRACK7)

#ifdef __cplusplus
extern "C" {
#endif

struct _SMACK {
    unsigned int Version;           // SMK2 only right now
    unsigned int Width;             // Width (1 based, 640 for example)
    unsigned int Height;            // Height (1 based, 480 for example)
    unsigned int Frames;            // Number of frames (1 based, 100 = 100 frames)
    unsigned int MSPerFrame;        // Frame Rate
    unsigned int SmackerType;       // bit 0 set=ring frame
    unsigned int LargestInTrack[7]; // Largest single size for each track
    unsigned int tablesize;         // Size of the init tables
    unsigned int codesize;          // Compression info
    unsigned int absize;            // ditto
    unsigned int detailsize;        // ditto
    unsigned int typesize;          // ditto
    unsigned int TrackType[7];      // high byte=0x80-Comp,0x40-PCM data,0x20-16 bit,0x10-stereo
    unsigned int extra;             // extra value (should be zero)
    unsigned int NewPalette;        // set to one if the palette changed
    unsigned char Palette[772];     // palette data
    unsigned int PalType;           // type of palette
    unsigned int FrameNum;          // 0374 Frame Number to be displayed
    unsigned int FrameSize;         // The current frame's size in bytes
    unsigned int SndSize;           // The current frame sound tracks' size in bytes
    int LastRectx;                  // 0380 Rect set in from SmackToBufferRect (X coord)
    int LastRecty;                  // Rect set in from SmackToBufferRect (Y coord)
    int LastRectw;                  // Rect set in from SmackToBufferRect (Width)
    int LastRecth;                  // 038C Rect set in from SmackToBufferRect (Height)
    unsigned int OpenFlags;         // flags used on open
    unsigned int LeftOfs;           // Left Offset used in SmackTo
    unsigned int TopOfs;            // Top Offset used in SmackTo
    unsigned int LargestFrameSize;  // Largest frame size
    unsigned int Highest1SecRate;   // Highest 1 sec data rate
    unsigned int Highest1SecFrame;  // Highest 1 sec data rate starting frame
    unsigned int ReadError;         // Set to non-zero if a read error has ocurred
    unsigned int addr32;            // translated address for 16 bit interface
};

typedef struct _SMACK *HSMACK;
typedef void *SMACK_HDIGDRIVER;

extern void SMACK_DECL SmackClose(HSMACK smack);
extern unsigned int SMACK_DECL SmackDoFrame(HSMACK smack);
extern void SMACK_DECL SmackNextFrame(HSMACK smack);
extern HSMACK SMACK_DECL SmackOpen(const char*, unsigned int uFlags, unsigned int uExtraBuffers);
extern int SMACK_DECL SmackSoundUseMSS(SMACK_HDIGDRIVER hDrv);
extern void SMACK_DECL SmackToBuffer(HSMACK smack, unsigned int uX, unsigned int uY, unsigned int uPitch, unsigned int uHeight, void *pBuffer, unsigned int uFlags);
extern unsigned int SMACK_DECL SmackToBufferRect(HSMACK smack, unsigned int uSmackSurface);
extern unsigned int SMACK_DECL SmackWait(HSMACK smack);

#ifdef __cplusplus
}
#endif

#endif // SMACK_H

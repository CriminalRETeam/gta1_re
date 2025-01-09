#ifndef MGLWIN_H
#define MGLWIN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void *MGL_HINSTANCE;
typedef void *MGL_HWND;
typedef long (__stdcall *MGL_WNDPROC)(MGL_HWND, uint, uint, long);

extern void MGL_setAppInstance(MGL_HINSTANCE hInstApp);
extern void MGL_registerEventProc(MGL_WNDPROC userWndProc);

#ifdef __cplusplus
};
#endif

#endif // MGLWIN_H

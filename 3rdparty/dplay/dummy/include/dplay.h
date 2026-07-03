#ifndef DUMMY_DPLAY_H
#define DUMMY_DPLAY_H

#include "win_compat.h"

typedef unsigned int DPID;
typedef DPID *LPDPID;

extern GUID IID_IDirectPlay2A;

typedef class IDirectPlay *LPDIRECTPLAY;
typedef class IDirectPlay2 *LPDIRECTPLAY2;

#define DP_OK 0
#define DPERR_USERCANCEL (0x80200000 + 280)
#define DPERR_NOSESSIONS (0x80200000 + 220)
#define DPERR_TIMEOUT (0x80200000 + 240)

typedef struct
{
	DWORD   dwSize;
	DWORD   dwFlags;
	GUID    guidInstance;
	GUID    guidApplication;
	DWORD   dwMaxPlayers;
	DWORD   dwCurrentPlayers;

	union
	{
		LPWSTR  lpszSessionName;
		LPSTR   lpszSessionNameA;
	};

	union
	{
		LPWSTR  lpszPassword;
		LPSTR   lpszPasswordA;
	};

	DWORD   dwReserved1;
	DWORD   dwReserved2;

	DWORD   dwUser1;
	DWORD   dwUser2;
	DWORD   dwUser3;
	DWORD   dwUser4;
} DPSESSIONDESC2, *LPDPSESSIONDESC2;
typedef const DPSESSIONDESC2* LPCDPSESSIONDESC2;

#define DPOPEN_CREATE                   0x00000002

typedef struct
{
	DWORD   dwSize;
	DWORD   dwFlags;            /* Not used must be 0 */

	union /*playerShortName */      /* Player's Handle? */
	{
		LPWSTR  lpszShortName;
		LPSTR   lpszShortNameA;
	};

	union /*playerLongName */       /* Player's formal/real name */
	{
		LPWSTR  lpszLongName;
		LPSTR   lpszLongNameA;
	};

} DPNAME, *LPDPNAME;


typedef WINBOOL (CALLBACK *LPDPENUMDPCALLBACKA)(
	LPGUID      lpguidSP,
	LPSTR       lpSPName,
	DWORD       dwMajorVersion,
	DWORD       dwMinorVersion,
	LPVOID      lpContext);

typedef WINBOOL (CALLBACK *LPDPENUMSESSIONSCALLBACK2)(
	LPCDPSESSIONDESC2   lpThisSD,
	LPDWORD             lpdwTimeOut,
	DWORD               dwFlags,
	LPVOID              lpContext );

#define DPESC_TIMEDOUT          0x00000001

class IDirectPlay : public IUnknown {
public:
	virtual HRESULT DestroyPlayer(DPID idPlayer) = 0;
};

class IDirectPlay2 : public IUnknown {
public:
	virtual HRESULT EnumSessions(LPDPSESSIONDESC2 lpsd, DWORD dwTimeout, LPDPENUMSESSIONSCALLBACK2 lpEnumSessionsCallback2, LPVOID lpContext, DWORD dwFlags) = 0;
	virtual HRESULT CreatePlayer(LPDPID lpidPlayer, LPDPNAME lpPlayerName, HANDLE hEvent, LPVOID lpData, DWORD dwDataSize, DWORD dwFlags) = 0;

	virtual HRESULT DestroyPlayer(DPID idPlayer) = 0;
	virtual HRESULT Close() = 0;
    virtual HRESULT Open(LPDPSESSIONDESC2 lpsd, DWORD dwFlags);
};

extern HRESULT CALLBACK DirectPlayEnumerateA( LPDPENUMDPCALLBACKA, LPVOID );
extern HRESULT CALLBACK DirectPlayCreate( LPGUID lpGUID, LPDIRECTPLAY *lplpDP, IUnknown *pUnk );

#endif // DUMMY_DPLAY_H

// npbx3d.h : main header file for the NPbx3d DLL
//

#ifndef _NPBX3D_H_
#define _NPBX3D_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define DllExport    __declspec( dllexport )

#include "npbx3d_res.h"		// main symbols

// netscape
// hg changed to exports 
#ifndef _NPAPI_H_
#include "npapi.h"
#endif
#ifndef _NPUPP_H_
#include "npupp.h"
#endif

#include "blaxxuncc3d.h"		// passport control
#include "javactrl.h"		

// messages send from the passport control to the plugin
#define WM_BX_STATUS_MSG	WM_USER + 500
#define WM_BX_LOAD_URL   	WM_USER + 501
#define WM_BX_LOAD_FILE		WM_USER + 502
#define WM_BX_KILL_STREAM   WM_USER + 503

typedef enum StreamType 
{ 	
    StreamNew       = 0,
    StreamNewStream = 1,
    StreamRequest   = 2,
    StreamRequestStream = 3,
    StreamStream    = 4,
    StreamFile      = 5,
    StreamErr       = 6,
    StreamDeleted   = 7,
    StreamLoaded    = 8,
    StreamDone      = 9
} StreamType_t;

class StreamEntry : public CObject
{
public:
    StreamType_t    type;
    NPStream        *stream;
	CString		    url;
	CString		    target;
	CString		    cache;
	CString		    mimeType;
    long            modified;
    long            size;
    StreamEntry(const char * urlp, const char * targetp);
    StreamEntry(StreamEntry * pStream);
    StreamEntry();
    ~StreamEntry();
    void copy(StreamEntry * stream );
};

/////////////////////////////////////////////////////////////////////////////
// CNpbx3dApp
// See npbx3d.cpp for the implementation of this class
//

class CNpbx3d : public CWnd
{
public:
    // class constants
	typedef enum Reason { 	
        LoadOK          = 0,
        NetworkError    = 1,
        UserBreak       = 2,
        LoadStream      = 3
    } Reason_t;

public:
    // construction
	CNpbx3d( NPP instance, int16 argc, char* argn[], char* argv[], bool useJava );
	~CNpbx3d();

public:
    // operations
    void    CheckSize(HWND hWnd);
    void    setWindow(HWND window, uint32 x, uint32 y, uint32 width, uint32 height);
    const char * getParam(const char * name);
    void    setNoDownloads(int no) { noOfDownloads = no; };

    uint16  newStream(NPStream    *stream, NPMIMEType mimeType);
    void    destroyStream(NPStream    *stream, NPError reason);
    void    writeStream(NPStream    *stream, void *buffer, int32 len);
    void    killStreams();
    void    checkStreams();
    void    loadUrl(const char * urls, const char * target, bool loadAnchor) ;
    void    notifyURL(NPStream *stream, const char * url, const char * file, Reason_t reason);
    bool    isValidStream( StreamEntry * stream );
    StreamEntry *   findStream( CString url );
    CJavaCtrl * getJavaCtrl() { return m_javaCtrl->getControl(); };

#ifdef STRICT
	WNDPROC		GetWndProc() const			{return _pfnDefault;}
#else
	FARPROC		GetWndProc() const			{return (FARPROC)_pfnDefault;}
#endif
		operator	HWND() const				{return _hPluginWnd;}

protected:
	bool		 InitControl();

private:
		// used to register this object instance in the window property
	static LPSTR _ThisLookUp;

public:
    static LRESULT  CALLBACK PluginWndProc(HWND window, UINT Msg, WPARAM WParam, LPARAM lParam);
	static void CALLBACK CNpbx3d::PluginTimer(HWND  hwnd, UINT  uMsg, UINT  idEvent, DWORD dwTime);

private:
    NPP     m_instance;
    int16   m_argc;
    CStringArray  m_argn;
    CStringArray  m_argv;
    RECT    m_rect;
    RECT	marginRect;
    StreamEntry m_world;
    CJavaCtrl *m_javaCtrl;
    CblaxxunCC3D*  m_cc3d;
	HWND		_hPluginWnd;	// plugin window handle coming from the navigator
	WNDPROC		_pfnDefault;	// old window proc (before subclassing)
    
    bool        isInitialized;
    bool        m_activated;
    int         noOfDownloads;
    int         curDownloads;
    CObArray    m_streams;
    bool        m_streamsChecked;
	CMapStringToOb  cacheIndex;
 	UINT			m_TimerID;

};


//-----------------------------------------------------------------------------
//---   Inline functions for class Cn                              ---

//-----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////

#endif // _NPBX3D_

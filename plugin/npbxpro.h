#ifndef _NPBXPRO_H_
#define _NPBXPRO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "npbxpro_res.h"		// main symbols

// netscape
#ifndef _NPAPI_H_
#include "npapi.h"
#endif
#ifndef _NPUPP_H_
#include "npupp.h"
#endif

// passport
#include "passport.h"		

// messages send from the passport control to the plugin
#define WM_PASSPORT_NOTIFY		WM_USER + 500
#define WM_PASSPORT_LOAD_URL	WM_USER + 501
#define WM_PASSPORT_LOAD_DOC	WM_USER + 502

class CNpbxpro : public CWinApp
{
public:
	// attributes
	typedef enum Reason 
	{ 	
      LoadOK          = 0,
      NetworkError    = 1,
      UserBreak       = 2
    } Reason_t;

	CWnd * mainWnd;

public:
    // construction
	CNpbxpro( NPP instance, int16 argc, char* argn[], char* argv[] );
	~CNpbxpro();

public:
    // operations
    void    setWindow(HWND window, uint32 x, uint32 y, uint32 width, uint32 height);
    void    notifyURL(const char * url, const char * file, Reason_t reason);

protected:
	// operations
    const char * getParam(const char * name);
    void  loadURL(const char * url);

private:
	// attributes
    NPP     instance;
    int16   argc;
    char**  argn;
    char**  argv;
    HWND    window;
    RECT    rect;
    CPassport*  passport;

	// plugin message handler functions
	afx_msg LRESULT OnNotify ( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLoadUrl( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnLoadDoc( WPARAM wParam, LPARAM lParam );
	
	//{{AFX_MSG(CNpbxpro)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // _NPBXPRO_
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "passport.h"

/////////////////////////////////////////////////////////////////////////////
// CPassport

IMPLEMENT_DYNCREATE(CPassport, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CPassport properties

/////////////////////////////////////////////////////////////////////////////
// CPassport operations

void CPassport::SetNetscapePlugin(long* plugin)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plugin);
}

void CPassport::SetNetscapeServer(LPCTSTR server)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 server);
}

void CPassport::SetNetscapePort(LPCTSTR port)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 port);
}

void CPassport::SetNetscapeScene(LPCTSTR scene)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 scene);
}

void CPassport::SetNetscapeOpenSession()
{
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CPassport::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

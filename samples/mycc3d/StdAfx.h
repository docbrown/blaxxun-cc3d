// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F44EA527_9D59_11D1_AF19_444553540000__INCLUDED_)
#define AFX_STDAFX_H__F44EA527_9D59_11D1_AF19_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


// CC3D insert ATL support

// see ATL1.1 examples MFCATL
//

#include <atlbase.h>

// We can implement the MFC/ATL lock count interaction in two different ways
// (you may comment/uncomment the one you want to try)

// ATL can blindly delegate all the ATL Lock()/Unlock() calls to MFC
class CAtlGlobalModule : public CComModule
{
public:
	LONG Lock()
	{
		AfxOleLockApp();
		return 0;
	}
	LONG Unlock()
	{
		AfxOleUnlockApp();
		return 0;
	}
};
// ATL can increment MFC's lock count the first time and decrement MFC's lock
// count when its lock count reaches zero.
/*
class CAtlGlobalModule : public CComModule
{
public:
	LONG Lock()
	{
		if (GetLockCount()==0)
			AfxOleLockApp();
		return CComModule::Lock();
	}
	LONG Unlock()
	{
		LONG l = CComModule::Unlock();
		if (GetLockCount() == 0)
			AfxOleUnlockApp();
		return l;
	}
};
*/

extern CAtlGlobalModule _Module;

#include <atlcom.h>



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F44EA527_9D59_11D1_AF19_444553540000__INCLUDED_)

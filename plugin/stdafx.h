#if !defined(AFX_STDAFX_H__F20CCB87_EE55_11D0_81D6_444553540000__INCLUDED_)
#define AFX_STDAFX_H__F20CCB87_EE55_11D0_81D6_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes
#include <afxtempl.h>
#include <atlbase.h>

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

extern CAtlGlobalModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F20CCB87_EE55_11D0_81D6_444553540000__INCLUDED_)

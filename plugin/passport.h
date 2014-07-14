#if !defined(AFX_PASSPORT_H__33D759E6_122B_11D1_81D6_444553540000__INCLUDED_)
#define AFX_PASSPORT_H__33D759E6_122B_11D1_81D6_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CPassport wrapper class

class CPassport : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPassport)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xf20ccb6d, 0xee55, 0x11d0, { 0x81, 0xd6, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void SetNetscapePlugin(long* plugin);
	void SetNetscapeServer(LPCTSTR server);
	void SetNetscapePort(LPCTSTR port);
	void SetNetscapeScene(LPCTSTR scene);
	void SetNetscapeOpenSession();
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSPORT_H__33D759E6_122B_11D1_81D6_444553540000__INCLUDED_)

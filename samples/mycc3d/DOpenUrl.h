#if !defined(AFX_DOPENURL_H__72B351E1_9D64_11D1_AF19_444553540000__INCLUDED_)
#define AFX_DOPENURL_H__72B351E1_9D64_11D1_AF19_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DOpenUrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DOpenUrl dialog

class DOpenUrl : public CDialog
{
// Construction
public:
	DOpenUrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DOpenUrl)
	enum { IDD = IDD_OPEN_URL };
	CString	m_url;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DOpenUrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DOpenUrl)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOPENURL_H__72B351E1_9D64_11D1_AF19_444553540000__INCLUDED_)

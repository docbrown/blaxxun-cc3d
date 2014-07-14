// MyCC3DView.h : interface of the CMyCC3DView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCC3DVIEW_H__F44EA52D_9D59_11D1_AF19_444553540000__INCLUDED_)
#define AFX_MYCC3DVIEW_H__F44EA52D_9D59_11D1_AF19_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//CC3D: added includes
#include "blaxxuncc3d.h"
#include "blaxxunVRML.h"

//


class CMyCC3DView : public CView
{
protected: // create from serialization only
	CMyCC3DView();
	DECLARE_DYNCREATE(CMyCC3DView)

// Attributes
public:
	CMyCC3DDoc* GetDocument();

    CblaxxunCC3D *  m_cc3d; // pointer to embedded CC3D control


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCC3DView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SaveFile(LPCTSTR pathName);
	void OpenFile(LPCTSTR pathName);
	virtual ~CMyCC3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyCC3DView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFileOpenUrl();
	afx_msg void OnUpdateFileOpenUrl(CCmdUI* pCmdUI);
	afx_msg void OnAddGeometry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyCC3DView.cpp
inline CMyCC3DDoc* CMyCC3DView::GetDocument()
   { return (CMyCC3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCC3DVIEW_H__F44EA52D_9D59_11D1_AF19_444553540000__INCLUDED_)

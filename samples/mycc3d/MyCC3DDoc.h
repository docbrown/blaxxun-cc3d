// MyCC3DDoc.h : interface of the CMyCC3DDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCC3DDOC_H__F44EA52B_9D59_11D1_AF19_444553540000__INCLUDED_)
#define AFX_MYCC3DDOC_H__F44EA52B_9D59_11D1_AF19_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMyCC3DDoc : public CDocument
{
protected: // create from serialization only
	CMyCC3DDoc();
	DECLARE_DYNCREATE(CMyCC3DDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCC3DDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual void OnChangedViewList();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyCC3DDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyCC3DDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCC3DDOC_H__F44EA52B_9D59_11D1_AF19_444553540000__INCLUDED_)

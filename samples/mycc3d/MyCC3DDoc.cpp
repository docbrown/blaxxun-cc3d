// MyCC3DDoc.cpp : implementation of the CMyCC3DDoc class
//

#include "stdafx.h"
#include "MyCC3D.h"

#include "MyCC3DDoc.h"

// CC3D
#include "MyCC3DView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DDoc

IMPLEMENT_DYNCREATE(CMyCC3DDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyCC3DDoc, CDocument)
	//{{AFX_MSG_MAP(CMyCC3DDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DDoc construction/destruction

CMyCC3DDoc::CMyCC3DDoc()
{
	// TODO: add one-time construction code here

}

CMyCC3DDoc::~CMyCC3DDoc()
{
}

BOOL CMyCC3DDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyCC3DDoc serialization

void CMyCC3DDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DDoc diagnostics

#ifdef _DEBUG
void CMyCC3DDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyCC3DDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyCC3DDoc commands

BOOL CMyCC3DDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
#if 0	
	// CC3D: walk all views and pass OpenFile to control
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CMyCC3DView* pView = (CMyCC3DView*) GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		pView->OpenFile(lpszPathName);
	}
#endif

	
	return TRUE;
}

BOOL CMyCC3DDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL ret = FALSE;
	
	// CC3D: walk a view and pass SaveFile to control
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CMyCC3DView* pView = (CMyCC3DView*) GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		ret= pView->SaveFile(lpszPathName);
		break;
	}
	return ret;

	
	return CDocument::OnSaveDocument(lpszPathName);
}

void CMyCC3DDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::SetTitle(lpszTitle);
}

void CMyCC3DDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::SetPathName(lpszPathName, bAddToMRU);
}

void CMyCC3DDoc::OnChangedViewList() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::OnChangedViewList();
}

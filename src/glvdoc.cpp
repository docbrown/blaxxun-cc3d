/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/
/******************************************************************************
@doc

@module GLVDoc.cpp - GLView MFC document class	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

Todo :

******************************************************************************/


#include "stdafx.h"
#include "glviewhdr.h"


//#include "glview.h"
//#include "glvdoc.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyglDoc

IMPLEMENT_DYNCREATE(CMyglDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyglDoc, CDocument)
	//{{AFX_MSG_MAP(CMyglDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyglDoc construction/destruction

CMyglDoc::CMyglDoc()
{
	// TODO: add one-time construction code here
}

CMyglDoc::~CMyglDoc()
{
}

BOOL CMyglDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

BOOL CMyglDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	 CMyglView *view = ((CMyglView*)m_viewList.GetHead());
	 if (!view) return(FALSE);
     int ret = view->ReadModel(lpszPathName);
	 return(ret>=0);
}

BOOL CMyglDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	 CMyglView *view = ((CMyglView*)m_viewList.GetHead());
	 if (!view) return(FALSE);
     int ret = view->WriteModel(lpszPathName);
	 return(ret>=0);
}

void CMyglDoc::OnCloseDocument() 
{
  CDocument::OnCloseDocument(); // ???
}


/////////////////////////////////////////////////////////////////////////////
// CMyglDoc serialization

void CMyglDoc::Serialize(CArchive& ar)
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
// CMyglDoc diagnostics

#ifdef _DEBUG
void CMyglDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyglDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyglDoc commands

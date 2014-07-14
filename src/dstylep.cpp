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
// dstylep.cpp : implementation file
// Style page


#include "stdafx.h"
#include "glviewhdr.h"

// drawstyle
#include "ddrawsty.h"

#include "dstylep.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DStylePage property page

IMPLEMENT_DYNCREATE(DStylePage, CPropertyPage)

DStylePage::DStylePage(UINT nIDCaption, 
				CMyglView *pView /* NULL */, 
				DTarget Target) : CPropertyPage(DStylePage::IDD,nIDCaption)
{
	applied = 0;
	m_target = Target;
	m_pView = pView;

	//{{AFX_DATA_INIT(DStylePage)
	m_visible = FALSE;
	m_textured = FALSE;
	m_smooth = FALSE;
	m_matcolor = FALSE;
	m_lighted = FALSE;
	m_colored = FALSE;
	m_color = _T("");
	//}}AFX_DATA_INIT
}

DStylePage::~DStylePage()
{
}

void DStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	if (!pDX->m_bSaveAndValidate) {
	  m_pView->GetStyle(*this);
	}

	//{{AFX_DATA_MAP(DStylePage)
	DDX_Check(pDX, IDC_VISIBLE, m_visible);
	DDX_Check(pDX, IDC_TEXTURED, m_textured);
	DDX_Check(pDX, IDC_SMOOTH, m_smooth);
	DDX_Check(pDX, IDC_MATCOLOR, m_matcolor);
	DDX_Check(pDX, IDC_LIGHTED, m_lighted);
	DDX_Check(pDX, IDC_COLORED, m_colored);
	DDX_Text(pDX, IDC_COLOR, m_color);
	//}}AFX_DATA_MAP
}

void DStylePage::OnButtonClicked(UINT /*nCmdID*/)
{

  SetModified(); // enable Apply Now button

#if 0	
	CPropertySheet* pPropertySheet = (CPropertySheet*)GetParent();

	if (pPropertySheet->IsKindOf(RUNTIME_CLASS(CModalShapePropSheet)))
	{
		UpdateData();
		((CModalShapePropSheet*)pPropertySheet)->UpdateShapePreview();
		SetModified(); // enable Apply Now button
	}

	if (pPropertySheet->IsKindOf(RUNTIME_CLASS(CModelessShapePropSheet)))
	{
		CMDIFrameWnd* pMDIFrameWnd = (CMDIFrameWnd*)AfxGetMainWnd();
		ASSERT(pMDIFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
		CView* pView = pMDIFrameWnd->MDIGetActive()->GetActiveView();
		UpdateData();
		pView->SendMessage(WM_USER_CHANGE_OBJECT_PROPERTIES, 0, 0);
	}
#endif
}


BEGIN_MESSAGE_MAP(DStylePage, CPropertyPage)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_VISIBLE, IDC_MATCOLOR, OnButtonClicked)
	//{{AFX_MSG_MAP(DStylePage)
	ON_BN_CLICKED(IDC_COLOR_EDIT, OnColorEdit)
	ON_BN_CLICKED(IDC_DRAWSTYLE, OnDrawstyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DStylePage message handlers

void DStylePage::OnColorEdit() 
{
	CString cs;
	Point c;
	int len=180;
	int focus =IDC_COLOR;

	GetDlgItemText(focus,cs.GetBuffer(len+1),len);

	CstringToPoint(cs,c);
	if (m_pView->EditColor(m_strCaption + "Color",c)) {
		CstringFromPoint(cs,c);
		SetDlgItemText(focus,cs);
		SetModified(TRUE);
	}

}

void DStylePage::OnDrawstyle() 
{
	// TODO: Add your control notification handler code here
	DDrawStyle d(NULL,m_pView,m_target);
	d.DoModal();
	
}


BOOL DStylePage::OnKillActive()	// Update data !
{
   if (CPropertyPage::OnKillActive()) {		// Will do UpdateData(1)
	if (m_pView) m_pView->ApplyStyle(*this);
		CancelToClose();	//  cancel no longer possible
		return(TRUE);
	}
	else return(FALSE);
}
//SetModified(TRUE); // Enable Apply Now Button

#if 0
CPropertySheet* DStylePage::GetSheet()
{
	CPropertySheet* pSheet = (CPropertySheet*)m_pParentWnd;
	ASSERT(pSheet != NULL);
	ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CPropertySheet)));
  	return(pSheet);
}
#endif

void DStylePage::OnOK()
{
  CPropertyPage::OnOK();
}

void DStylePage::OnCancel()
{
  CPropertyPage::OnCancel();
}

BOOL DStylePage::OnSetActive()
{
  return CPropertyPage::OnSetActive();
}

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
// GLViewCtrlPpg.cpp : Implementation of the CGLViewCtrlPropPage property page class.

#include "stdafx.h" 
#include "blaxxunCC3d.h"
#include "blaxxunCC3dPpg.h"

#include "GVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGLViewCtrlPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CGLViewCtrlPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CGLViewCtrlPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CGLViewCtrlPropPage, _PROGRAM ".PropPage.1",
	0x4b6e3014, 0x6e45, 0x11d0, 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8)

//IMPLEMENT_OLECREATE_EX(CGLViewCtrlPropPage, "GLVIEWCTRL.GLViewCtrlPropPage.1",
//0x4b6e3014, 0x6e45, 0x11d0, 0x93, 0x9, 0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8)


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlPropPage::CGLViewCtrlPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CGLViewCtrlPropPage

BOOL CGLViewCtrlPropPage::CGLViewCtrlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_GLVIEWCTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlPropPage::CGLViewCtrlPropPage - Constructor

CGLViewCtrlPropPage::CGLViewCtrlPropPage() :
	COlePropertyPage(IDD, IDS_GLVIEWCTRL_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CGLViewCtrlPropPage)
	m_src = _T("");
	m_panel = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlPropPage::DoDataExchange - Moves data between page and properties

void CGLViewCtrlPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CGLViewCtrlPropPage)
	DDP_Text(pDX, IDC_SRC, m_src, _T("URL"));
	DDX_Text(pDX, IDC_SRC, m_src);
	DDX_Check(pDX, IDC_PANEL, m_panel);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlPropPage message handlers

#if 0
/////////////////////////////////////////////////////////////////////////////
// CRenderingPropPage dialog

IMPLEMENT_DYNCREATE(CRenderingPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRenderingPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRenderingPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {E708E6E1-DB3C-11D0-93DF-0020AFE05CC8}
IMPLEMENT_OLECREATE_EX(CRenderingPropPage, "GLViewCtrl.CRenderingPropPage",
	0xe708e6e1, 0xdb3c, 0x11d0, 0x93, 0xdf, 0x0, 0x20, 0xaf, 0xe0, 0x5c, 0xc8)


/////////////////////////////////////////////////////////////////////////////
// CRenderingPropPage::CRenderingPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CRenderingPropPage

BOOL CRenderingPropPage::CRenderingPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, 0);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRenderingPropPage::CRenderingPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CRenderingPropPage::CRenderingPropPage() :
	COlePropertyPage(IDD, 0)
{
	//{{AFX_DATA_INIT(CRenderingPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRenderingPropPage::DoDataExchange - Moves data between page and properties

void CRenderingPropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CRenderingPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRenderingPropPage message handlers

/////////////////////////////////////////////////////////////////////////////
// CNavigationPropPage property page

IMPLEMENT_DYNCREATE(CNavigationPropPage, CPropertyPage)

CNavigationPropPage::CNavigationPropPage() : 
	COlePropertyPage(IDD, 0)
{
	//{{AFX_DATA_INIT(CNavigationPropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

CNavigationPropPage::~CNavigationPropPage()
{
}

void CNavigationPropPage::DoDataExchange(CDataExchange* pDX)
{
	//CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNavigationPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}

/*
BEGIN_MESSAGE_MAP(CNavigationPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNavigationPropPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CNavigationPropPage message handlers
#endif


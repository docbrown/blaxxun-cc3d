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
// DTransform.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"
#include "DTransform.h"

#include "grender.h"

#include <gv2group.h>
#include <grayselect.h>


#include "gmodel.h"

#include "gevent.h"
#include "gedit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DTransformPosition property page

IMPLEMENT_DYNCREATE(DTransformPosition, CPropertyPage)


GView* DTransformPosition::GetView()
{
	return(m_pView->view);

}

//BOOL CDialog::PreTranslateMessage(MSG* pMsg)

DTransformPosition::DTransformPosition(CMyglView *pView, UINT nIDTemplate, UINT nIDCaption) : CPropertyPage(nIDTemplate,nIDCaption)
{
	m_handler = NULL;
	m_pView = pView;
	//{{AFX_DATA_INIT(DTransformPosition)
	m_value = _T("");
	m_type = 0;
	m_x = TRUE;
	m_y = TRUE;
	m_z = TRUE;
	//}}AFX_DATA_INIT
}

DTransformPosition::~DTransformPosition()
{
}

void DTransformPosition::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTransformPosition)
	DDX_Control(pDX, IDC_VALUE, m_valueCtrl);
	DDX_Text(pDX, IDC_VALUE, m_value);
	DDX_Radio(pDX, IDC_TYPE_SCREEN, m_type);
	DDX_Check(pDX, IDC_AXIS_X, m_x);
	DDX_Check(pDX, IDC_AXIS_Y, m_y);
	DDX_Check(pDX, IDC_AXIS_Z, m_z);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTransformPosition, CPropertyPage)
	//{{AFX_MSG_MAP(DTransformPosition)
	ON_BN_CLICKED(IDC_TYPE_SCREEN, OnTypeScreen)
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_EN_KILLFOCUS(IDC_VALUE, OnKillfocusValue)
	ON_EN_CHANGE(IDC_VALUE, OnChangeValue)
	//}}AFX_MSG_MAP

	//ON_CONTROL(BN_CLICKED, IDC_TYPE_SCREEN, OnTypeScreen)
	// tab is in between
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TYPE_SCREEN, IDC_TYPE_LOCAL, OnControlClicked)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_AXIS_X,IDC_AXIS_Z, OnControlClicked)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformPosition message handlers


void DTransformPosition::OnControlClicked(UINT ctrl) 
{
	UpdateData(1);

	if (!m_handler) return;

	m_handler->SetMapMode(m_type);
	m_handler->SetAxes(GetAxes());
	
}


void DTransformPosition::OnTypeScreen() 
{
	UpdateData(1);

	if (!m_handler) return;

	m_handler->SetMapMode(m_type);
	m_handler->SetAxes(GetAxes());
	
}

// update value in dialog 
void DTransformPosition::SetValue(const char *value)
{
	CDataExchange DX(this,FALSE);
	
	m_value = value;
	DDX_Text(&DX, IDC_VALUE, m_value);

}

// send value in edit field to handler 
void DTransformPosition::ApplyValue()
{
	if (!UpdateData(TRUE)) return;

	if (!m_handler) return;

	m_handler->SetValue(m_value);

	m_handler->SetMapMode(m_type);
	m_handler->SetAxes(GetAxes());


	//CDataExchange DX(this,TRUE);
	//DDX_Text(&DX, IDC_VALUE, m_value);

}

BOOL DTransformPosition::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
		//return CPropertyPage::PreTranslateMessage(pMsg);
		ApplyValue();
		return TRUE;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}



// update value in dialog 
void GTransformPathHandler::UpdateDialogValue(const char *value)
{
	CDataExchange DX(dialog,FALSE);
	
	dialog->m_value = value;
	DDX_Text(&DX, IDC_VALUE, dialog->m_value);

}


BOOL DTransformPosition::OnSetActive() 
{
	GView *view = GetView();

	// make translation event handler active 
	GMoveTransformHandler *h = new GMoveTransformHandler(view);
	h->SetMapMode(m_type);
	h->SetTransformMode(GMoveTransformHandler::MODE_TRANSLATE);
	h->SetDialog(this);
	h->SetAxes(GetAxes());
	h->scaleUniform = FALSE;
	h->ref();

	m_pView->Message("Select and drag translates object"); 

	m_handler = h;
    view->PushEventHandler(h);
	
	return CPropertyPage::OnSetActive();
}

BOOL DTransformPosition::OnKillActive() 
{
	GView *view = GetView();
	GEventHandler *h = view->PopEventHandler();
	h->unref();
	m_handler = NULL;
	m_pView->Message(""); 
	return CPropertyPage::OnKillActive();
}


/////////////////////////////////////////////////////////////////////////////
// DTransformScale

IMPLEMENT_DYNAMIC(DTransformScale, DTransformPosition)
DTransformScale::DTransformScale(CMyglView *pView) : DTransformPosition(pView,DTransformScale::IDD)
{
	//{{AFX_DATA_INIT(DTransformScale)
	m_uniform = TRUE;
	//}}AFX_DATA_INIT

}


DTransformScale::~DTransformScale()
{
}

void DTransformScale::DoDataExchange(CDataExchange* pDX) 
{
//	CPropertyPage::DoDataExchange(pDX);
	DTransformPosition::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTransformScale)
	DDX_Check(pDX, IDC_UNIFORM, m_uniform);
	//}}AFX_DATA_MAP

}




BEGIN_MESSAGE_MAP(DTransformScale, CPropertyPage)
	//{{AFX_MSG_MAP(DTransformScale)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TYPE_SCREEN, IDC_TYPE_LOCAL, OnControlClicked)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_AXIS_X,IDC_AXIS_Z, OnControlClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformScale message handlers

BOOL DTransformScale::OnSetActive() 
{

//	return DTransformScale::OnSetActive();
	GView *view = GetView();

	// make translation event handler active 
	GMoveTransformHandler *h = new GMoveTransformHandler(view);
	h->SetMapMode(m_type);
	h->SetTransformMode(GMoveTransformHandler::MODE_SCALE);
	h->SetDialog(this);
	h->SetAxes(GetAxes());
	h->scaleUniform = m_uniform;
	h->ref();

	m_pView->Message("Select and drag scales object"); 

	m_handler = h;
    view->PushEventHandler(h);

	
	return CPropertyPage::OnSetActive();

}

BOOL DTransformScale::OnKillActive() 
{
	return DTransformPosition::OnKillActive();
}

/////////////////////////////////////////////////////////////////////////////
// DTransformDialog dialog


DTransformDialog::DTransformDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DTransformDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DTransformDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DTransformDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTransformDialog)
	DDX_Control(pDX, IDC_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTransformDialog, CDialog)
	//{{AFX_MSG_MAP(DTransformDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TAB, OnTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformDialog message handlers

BOOL DTransformDialog::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

int DTransformDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void DTransformDialog::OnTab() 
{
	// TODO: Add your control notification handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// DTransformSheet

IMPLEMENT_DYNAMIC(DTransformSheet, CPropertySheet)

DTransformSheet::DTransformSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

DTransformSheet::DTransformSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

DTransformSheet::~DTransformSheet()
{
}


BEGIN_MESSAGE_MAP(DTransformSheet, CPropertySheet)
	//{{AFX_MSG_MAP(DTransformSheet)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformSheet message handlers

void DTransformSheet::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::DoDataExchange(pDX);
}

BOOL DTransformSheet::OnInitDialog() 
{
	// TODO: Add your specialized code here and/or call the base class
	

	BOOL bResult = CPropertySheet::OnInitDialog();
#if 0
	// add the preview window to the property sheet.
	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width() - 100,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	//CRect rectPreview(rectWnd.Width() + 25, 25,
	//		rectWnd.Width()+75, 75);
	//m_wndPreview.Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
	//	rectPreview, this, 0x1000);

	//CenterWindow();
#endif 
	return bResult;

}


void DTransformSheet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//SetActivePage(-1);

//		m_pParentWnd->SendMessage(WM_DIALOG_CLOSED, 0, (long)this);


	CPropertySheet::OnClose();
	if (m_pParentWnd) 
	   m_pParentWnd->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}


BOOL DTransformPosition::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
//		CPropertySheet* pSheet = STATIC_DOWNCAST(CPropertySheet, GetParent());

	
	return CPropertyPage::OnApply();
}
/////////////////////////////////////////////////////////////////////////////
// DTransformRotation property page

IMPLEMENT_DYNCREATE(DTransformRotation, DTransformPosition)

DTransformRotation::DTransformRotation(CMyglView *pView): DTransformPosition(pView,DTransformRotation::IDD)
{
	m_x=m_y = FALSE;
	m_z=TRUE;

	//{{AFX_DATA_INIT(DTransformRotation)
	m_trackball = FALSE;
	//}}AFX_DATA_INIT
}

DTransformRotation::~DTransformRotation()
{
}

void DTransformRotation::DoDataExchange(CDataExchange* pDX)
{
	//CPropertyPage::DoDataExchange(pDX);
	DTransformPosition::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(DTransformRotation)
	DDX_Check(pDX, IDC_TRACKBALL, m_trackball);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTransformRotation, CPropertyPage)
	//{{AFX_MSG_MAP(DTransformRotation)
	ON_BN_CLICKED(IDC_TRACKBALL, OnTrackball)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TYPE_SCREEN, IDC_TYPE_LOCAL, OnControlClicked)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_AXIS_X,IDC_AXIS_Z, OnControlClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformRotation message handlers

BOOL DTransformRotation::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
//	return DTransformScale::OnSetActive();
	GView *view = GetView();

	// make translation event handler active 
	GMoveTransformHandler *h = new GMoveTransformHandler(view);
	h->SetMapMode(m_type);
	h->SetTransformMode(GMoveTransformHandler::MODE_ROTATE);
	h->SetDialog(this);
	h->SetAxes(GetAxes());
	h->rotateTrackball = m_trackball;
	h->ref();

	m_pView->Message("Select and drag scales object"); 

	m_handler = h;
    view->PushEventHandler(h);

	
	return CPropertyPage::OnSetActive();

}

void DTransformRotation::OnTrackball() 
{
	UpdateData(1);

	if (!m_handler) return;

	m_handler->rotateTrackball = m_trackball;
	
}

void DTransformPosition::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_handler) {
		GView *view = GetView();
		GEventHandler *h = view->PopEventHandler();
		h->unref();
		m_handler = NULL;
		m_pView->Message(""); 
	}

	
	CPropertyPage::PostNcDestroy();
}

BOOL DTransformPosition::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::DestroyWindow();
}

void DTransformPosition::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnClose();
}

void DTransformPosition::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnChar(nChar, nRepCnt, nFlags);
}


void DTransformPosition::OnKillfocusValue() 
{
	// TODO: Add your control notification handler code here
	
}

void DTransformPosition::OnChangeValue() 
{
	// TODO: Add your control notification handler code here
	
}


/////////////////////////////////////////////////////////////////////////////
// DTransformNewObject property page

IMPLEMENT_DYNCREATE(DTransformNewObject, DTransformPosition)

DTransformNewObject::DTransformNewObject(CMyglView *pView) : DTransformPosition(pView,DTransformNewObject::IDD) // CPropertyPage(DTransformNewObject::IDD)
{
	//{{AFX_DATA_INIT(DTransformNewObject)
	m_share = TRUE;
	m_object_type = _T("Box");
	//}}AFX_DATA_INIT
}

DTransformNewObject::~DTransformNewObject()
{
}

void DTransformNewObject::DoDataExchange(CDataExchange* pDX)
{
	//CPropertyPage::DoDataExchange(pDX);
	DTransformPosition::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTransformNewObject)
	DDX_Control(pDX, IDC_OBJECT_TYPE, m_object_type_ctrl);
	DDX_Check(pDX, IDC_SHARE, m_share);
	DDX_CBString(pDX, IDC_OBJECT_TYPE, m_object_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTransformNewObject, DTransformPosition)
	//{{AFX_MSG_MAP(DTransformNewObject)
	ON_CBN_SELCHANGE(IDC_OBJECT_TYPE, OnSelchangeObjectType)
	ON_BN_CLICKED(IDC_SHARE, OnShare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformNewObject message handlers

BOOL DTransformNewObject::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class

	GView *view = GetView();

	// make translation event handler active 
	GNewObjectHandler *h = new GNewObjectHandler(view);
	h->SetMapMode(m_type);
	h->SetTransformMode(GMoveTransformHandler::MODE_TRANSLATE);
	h->SetDialog(this);
	h->SetAxes(GetAxes());
	h->copyObject = !m_share;
	h->group = (GvNode*) view->GetScene(); 
	h->SetObjectType(m_object_type);
	h->ref();

	m_pView->Message("Click to insert new objects"); 

	m_handler = h;
    view->PushEventHandler(h);

	
	return CPropertyPage::OnSetActive();
}


// get the object, inserted on each mouse click 
void DTransformNewObject::OnSelchangeObjectType() 
{
	GNewObjectHandler *h= (GNewObjectHandler *) m_handler;
	if (!h) return;

	int i = m_object_type_ctrl.GetCurSel();

	if (i >=0) {
	    DWORD w = m_object_type_ctrl.GetItemData(i);
		if (w>0) {
			GvMFNode *g = NULL;

			g= h->view->GetLibraryObjects();
			if (!g) return;

			GvNode *n=g->get1(w);
			h->SetObject(n);
			return;
		} 
	    m_object_type_ctrl.GetLBText(i,m_object_type);
	}	

	//CDataExchange DX(this,TRUE);
	//DDX_CBString(&DX, IDC_OBJECT_TYPE, m_object_type);

	h->SetObjectType(m_object_type);
	
}


// add object types from library to combo box
void DTransformNewObject::FillObjectTypes() 
{
	GNewObjectHandler *h= (GNewObjectHandler *) m_handler;
	if (!h) return;

	GvMFNode *g = NULL;

	g= h->view->GetLibraryObjects();
	if (!g) return;


	if (!g) return;
	CString buf;
	for (int i=0; i< g->getNum();i++) {
		GvNode *n=g->get1(i);
		buf = n->getName().getString();
		buf += " ";
		buf+= n->GetType();
		int ofs = m_object_type_ctrl.AddString(buf);
		m_object_type_ctrl.SetItemData(ofs,i+1);
	}

}


void DTransformNewObject::OnShare() 
{
	CDataExchange DX(this,TRUE);
	DDX_Check(&DX, IDC_SHARE, m_share);
	GNewObjectHandler *h= (GNewObjectHandler *) m_handler;
	if (!h) return;

	h->copyObject = !m_share;

}



BOOL DTransformNewObject::OnInitDialog() 
{
	DTransformPosition::OnInitDialog();
	
	// TODO: Add extra initialization here
	FillObjectTypes();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL DTransformNewObject::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return DTransformPosition::PreTranslateMessage(pMsg);

	if ((pMsg->message != WM_KEYDOWN) || (pMsg->wParam != VK_RETURN))
		return DTransformPosition::PreTranslateMessage(pMsg);

	
	return DTransformPosition::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////
// DTransformMode property page

IMPLEMENT_DYNCREATE(DTransformMode, CPropertyPage)

GView* DTransformMode::GetView()
{
	return(m_pView->view);

}

const IS selectionModes[] = {
	{PATH_TOP,"top level"},
	{PATH_TOP_DEF,"top level DEF"},
	{PATH_BOTTOM_DEF,"bottom level DEF"},
	{PATH_BOTTOM,"bottom level"},
	{PATH_BOTTOM_SHAPE,"bottom shape"},
	{PATH_BOTTOM_SHAPE_SINGLE,"bottom shape single"},

	{PATH_MATERIAL,"material"},
	{PATH_TEXTURE,"texture"},
	{PATH_APPEARANCE,"appearance"},
	{PATH_GEOMETRY,"geometry"},
	{PATH_FACE,"face"},
	{PATH_EDGE,"edge"},
	{PATH_VERTEX,"vertex"},
	{0,NULL},
};


DTransformMode::DTransformMode(CMyglView *pView, UINT nIDTemplate, UINT nIDCaption) : CPropertyPage(nIDTemplate,nIDCaption)
{
	m_pView = pView;

	//{{AFX_DATA_INIT(DTransformMode)
	m_value = _T("");
	m_deep = FALSE;
	//}}AFX_DATA_INIT
}

DTransformMode::~DTransformMode()
{
}

void DTransformMode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
		m_value = Lookup(selectionModes,GTransformPathHandler::defaultPathMode);
		m_deep= GTransformPathHandler::defaultPathDeep;
	}

	//{{AFX_DATA_MAP(DTransformMode)
	DDX_CBString(pDX, IDC_VALUE, m_value);
	DDX_Check(pDX, IDC_DEEP, m_deep);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		int value = Lookup(selectionModes,m_value);
		if (value >=0) GTransformPathHandler::defaultPathMode = (GPathMode) value;
		GTransformPathHandler::defaultPathDeep = !!m_deep;
	}
}


BEGIN_MESSAGE_MAP(DTransformMode, CPropertyPage)
	//{{AFX_MSG_MAP(DTransformMode)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DTransformMode message handlers

BOOL DTransformMode::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}


/////////////////////////////////////////////////////////////////////////////
// DTransformSelect property page

IMPLEMENT_DYNCREATE(DTransformSelect, CPropertyPage)

DTransformSelect::DTransformSelect(CMyglView *pView, UINT nIDTemplate, UINT nIDCaption) : CPropertyPage(nIDTemplate,nIDCaption)
{
	//{{AFX_DATA_INIT(DTransformSelect)
	m_deep = FALSE;
	//}}AFX_DATA_INIT
}

DTransformSelect::~DTransformSelect()
{
}

GView* DTransformSelect::GetView()
{
	return(m_pView->view);

}

void DTransformSelect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTransformSelect)
	DDX_Control(pDX, IDC_INFO, m_infoC);
	DDX_Control(pDX, IDC_LIST, m_listC);
	DDX_Control(pDX, IDC_VALUE, m_valueC);
	DDX_Check(pDX, IDC_DEEP, m_deep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTransformSelect, CPropertyPage)
	//{{AFX_MSG_MAP(DTransformSelect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTransformSelect message handlers

BOOL DTransformSelect::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

BOOL DTransformSelect::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void DTransformSelect::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::PostNcDestroy();
}

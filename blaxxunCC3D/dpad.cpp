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
// dpad.cpp : implementation file
//

#include "stdafx.h"
#include "blaxxunCC3d.h"
#include "dpad.h"
#include "GViewControl.h"
#include "blaxxunCC3dCtl.h"
#include "GRender.h"
#include "GModel.h"
#include "GvNodes.h"
#include "GvTraverse.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DPadAvatarView property page

IMPLEMENT_DYNCREATE(DPadAvatarView, CPropertyPage)

DPadAvatarView::DPadAvatarView() : CPropertyPage(DPadAvatarView::IDD)
{
	//{{AFX_DATA_INIT(DPadAvatarView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DPadAvatarView::~DPadAvatarView()
{
TRACE("DPadAvatarView::~DPadAvatarView()\n");
}

void DPadAvatarView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPadAvatarView)
	DDX_Control(pDX, IDC_MODE_LIST, m_modeList);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate) {
		int index,sel=-1;
		m_modeList.ResetContent();
		index = m_modeList.AddString("Off");
		index = m_modeList.AddString("Follow");
		index = m_modeList.AddString("Fixed Orientation");
		index = m_modeList.AddString("Fixed Position");
#ifdef _DEBUG
		index = m_modeList.AddString("Overlay view");
#endif


		if (view->thirdPersonView)
				sel = view->thirdPersonMode;
		else sel = 0;

		if (sel>=0) m_modeList.SetCurSel(sel);

	}




}


BEGIN_MESSAGE_MAP(DPadAvatarView, CPropertyPage)
	//{{AFX_MSG_MAP(DPadAvatarView)
	ON_LBN_SELCHANGE(IDC_MODE_LIST, OnSelchangeModeList)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_LBN_DBLCLK(IDC_MODE_LIST, OnDblclkModeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPadAvatarView message handlers



BOOL DPadAvatarView::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

BOOL DPadAvatarView::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}
/////////////////////////////////////////////////////////////////////////////
// DPadViewpoints dialog


DPadViewpoints::DPadViewpoints()
	: CPropertyPage(DPadViewpoints::IDD)
{
	//{{AFX_DATA_INIT(DPadViewpoints)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DPadViewpoints::DoDataExchange(CDataExchange* pDX)
{
	TRACE("DPadViewpoints::DoDataExchange() \n");

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPadViewpoints)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {
			// Append viewpoints to list 
			{	
				GvSceneInfo *sceneInfo = view->GetSceneInfo();
				m_list.ResetContent();
				int sel=-1;

				if (sceneInfo) {
					int n = sceneInfo->cameras.Length();
				
  
					for (int i=0; i<n; i++)  {
						GvNode *n = sceneInfo->cameras[i];
						CString s = n->getName().getString();

						if (RTISA(n,GvViewpoint)) {	 
							s = ((GvViewpoint *) n)->description.get();
							// ignore internal viewpoints in display
							if (s.GetLength() == 0) continue;
                        } 
						

						int index = m_list.AddString(s);
						if (index>=0) {
							m_list.SetItemData(index,i );
							if (i == view->viewpointIndex)
								sel = index;
						}
					}
				}


				if (sel>=0) m_list.SetCurSel(sel);
		}
	}

}


BEGIN_MESSAGE_MAP(DPadViewpoints, CPropertyPage)
	//{{AFX_MSG_MAP(DPadViewpoints)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnDeltaposSpin)
	ON_BN_CLICKED(IDC_TOUR, OnTour)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPadViewpoints message handlers

void DPadAvatarView::OnSelchangeModeList() 
{
	// TODO: Add your control notification handler code here
	int index = m_modeList.GetCurSel();
	if (index <0) return;
	int i = m_modeList.GetItemData(index);

	if (index == 0) {
		view->ShowAvatar(FALSE);
		view->SetThirdPersonView(FALSE);
	}
	else {

		int oldMode = view->getThirdPersonMode();

		if (oldMode == 4)
			view-> BindAvatar(NULL);

		view->thirdPersonMode.set(index);

		view->ShowAvatar(TRUE);
		view->SetThirdPersonView(TRUE);
/*
		if (!view->showAvatar)
			view->ShowAvatar(TRUE);
		if (!view->thirdPersonView)
			view->SetThirdPersonView(TRUE);
*/
		
		view->UpdatePhysicalCamera(TRUE);
	}
	view->Redraw();
	
}

void DPadAvatarView::OnReset() 
{
	view->followDolly.set(0,0.5,-3);
	view->followPan.set(0, -0.2,0);
	view->followOrbit.set(0,0,0);
	view->UpdatePhysicalCamera(TRUE);
	view->Redraw();

}

void DPadAvatarView::OnDblclkModeList() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL DPadViewpoints::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("DPadViewpoints::OnKillActive() \n");
	return CPropertyPage::OnKillActive();
}

BOOL DPadViewpoints::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("DPadViewpoints::OnSetActive() \n");
	
	return CPropertyPage::OnSetActive();
}

void DPadViewpoints::OnDblclkList() 
{
	// TODO: Add your control notification handler code here
	
}

void DPadViewpoints::OnSelchangeList() 
{
	int index = m_list.GetCurSel();
	if (index <0) return;
	int i = m_list.GetItemData(index );
	view->SetViewpoint(i,view->cameraAnimate);
	view->Redraw();

}

void DPadViewpoints::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	TRACE("Delta %d \n",pNMUpDown->iDelta);

	if (pNMUpDown->iDelta <0) {
		view->SetNextViewpoint(view->cameraAnimate);
	}
	else if (pNMUpDown->iDelta >0) {
		view->SetPrevViewpoint(view->cameraAnimate);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void DPadViewpoints::OnTour() 
{
	view->AnimateAllCameras();
}
/////////////////////////////////////////////////////////////////////////////
// DPadNavigationMode property page

IMPLEMENT_DYNCREATE(DPadNavigationMode, CPropertyPage)

DPadNavigationMode::DPadNavigationMode() : CPropertyPage(DPadNavigationMode::IDD)
{
	//{{AFX_DATA_INIT(DPadNavigationMode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DPadNavigationMode::~DPadNavigationMode()
{
}

void DPadNavigationMode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPadNavigationMode)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate) 
	{	
		int index;				
		int sel = -1;

		index = m_list.AddString("Walk"); m_list.SetItemData(index,WALK_MODE);
		index = m_list.AddString("Rotate"); m_list.SetItemData(index,ROTATE_MODE);
	
		if (sel>=0) m_list.SetCurSel(sel);
	}

}


BEGIN_MESSAGE_MAP(DPadNavigationMode, CPropertyPage)
	//{{AFX_MSG_MAP(DPadNavigationMode)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPadNavigationMode message handlers

void DPadNavigationMode::OnSelchangeList() 
{
	// TODO: Add your control notification handler code here
	
}

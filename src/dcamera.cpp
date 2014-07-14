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
// dcamera.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "resource.h"
#include "dcamera.h"

#include "gcamera.h"
#include "gclass.h"
#include "grender.h"
#include "gmodel.h"

#include "gmodel.h"

#include <gvnode.h>
#include <gvtraverse.h>
#include <gvperspectivecamera.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DViewpoint dialog


DViewpoint::DViewpoint(CWnd* pParent /*=NULL*/,CMyglView *pView)
	: CDialog(DViewpoint::IDD, pParent)
{
	m_pView = pView;
	
	m_pCamera = NULL;
	
	m_pCameras = NULL;

	m_selectedCamera = NULL;
	m_selectedCameraTransform = NULL;

	//{{AFX_DATA_INIT(DViewpoint)
	m_viewpoint_listi = -1;
	m_znear = 0.0f;
	m_zfar = 0.0f;
	m_fovy = 0.0f;
	m_position = _T("");
	m_target = _T("");
	m_up = _T("");
	m_speed = 1.0f;
	m_distance = 0.0f;
	m_world_up = _T("");
	m_animate = TRUE;
	//}}AFX_DATA_INIT
}


void DViewpoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	if (!pDX->m_bSaveAndValidate && m_pCamera) {
	   
	   Point direction;
	   direction= m_pCamera->target-m_pCamera->position;
	   direction.Normalize();

	   CstringFromPoint(m_position,m_pCamera->position);
	   CstringFromPoint(m_target,direction);
	   CstringFromPoint(m_up,m_pCamera->up);
	   CstringFromPoint(m_world_up,m_pCamera->worldUp);

	   m_znear = m_pCamera->znear;
	   m_zfar = m_pCamera->zfar;
	   
	   m_distance = m_pCamera->TargetDistance();
	   
	   m_fovy = RAD2DEG(m_pCamera->GetFovY()); 
	   if (m_pView) { 
           m_speed = m_pView->m_cameraSpeed;
           m_animate= m_pView->view->cameraAnimate;
           m_pView->view->GetViewpointIndex(m_viewpoint_listi);
       }

	}

	//{{AFX_DATA_MAP(DViewpoint)
	DDX_Control(pDX, IDC_VIEWPOINT_LIST, m_viewpoint_list);
	DDX_LBIndex(pDX, IDC_VIEWPOINT_LIST, m_viewpoint_listi);
	DDX_Text(pDX, IDC_ZNEAR, m_znear);
	DDX_Text(pDX, IDC_ZFAR, m_zfar);
	DDX_Text(pDX, IDC_FOVY, m_fovy);
	DDX_Text(pDX, IDC_POSITION, m_position);
	DDX_Text(pDX, IDC_TARGET, m_target);
	DDX_Text(pDX, IDC_UP, m_up);
	DDX_Text(pDX, IDC_SPEED, m_speed);
	DDX_Text(pDX, IDC_DISTANCE, m_distance);
	DDX_Text(pDX, IDC_WORLD_UP, m_world_up);
	DDX_Check(pDX, IDC_ANIMATE, m_animate);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {
        m_viewpoint_list.SetCurSel(m_viewpoint_listi);
		//OnSelchangeViewpointList(); 
    } 
	
	if (pDX->m_bSaveAndValidate && m_pCamera) {
       
       // set camera from parameter values 
	   Point direction;
	   CstringToPoint(m_position,m_pCamera->position);
	   CstringToPoint(m_target,direction);
	   CstringToPoint(m_up,m_pCamera->up);
	   CstringToPoint(m_world_up,m_pCamera->worldUp);
	   
	   if (m_distance == 0.0) m_distance = m_pCamera->width * 2.0;
	   
	   m_pCamera->znear = max(0.00000001f,m_znear);
	   m_pCamera->zfar = max(m_distance,m_zfar);
 
	   m_pCamera->up.Normalize();
	   m_pCamera->target = m_pCamera->position + direction * m_distance;

	   float fovy = DEG2RAD(m_fovy);
	   {
			float aspectRatio = m_pCamera->width / m_pCamera->height;

			m_pCamera->height = 2.0 * tan(fovy*0.5)* m_pCamera->TargetDistance();
			m_pCamera->width = m_pCamera->height *  aspectRatio;

	   }
	   m_pCamera->OnChanged();
	   if (m_pView) { 
           m_pView->m_cameraSpeed = m_speed;
           m_pView->view->cameraAnimate = m_animate;
       }

	 }
}


BEGIN_MESSAGE_MAP(DViewpoint, CDialog)
	//{{AFX_MSG_MAP(DViewpoint)
	ON_LBN_SELCANCEL(IDC_VIEWPOINT_LIST, OnSelcancelViewpointList)
	ON_LBN_DBLCLK(IDC_VIEWPOINT_LIST, OnDblclkViewpointList)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_LBN_SELCHANGE(IDC_VIEWPOINT_LIST, OnSelchangeViewpointList)
	ON_BN_CLICKED(IDC_VIEWPOINT_SAVE, OnViewpointSave)
	ON_BN_CLICKED(IDC_VIEWPOINT_SAVE_AS, OnViewpointSaveAs)
	ON_BN_CLICKED(IDC_COMPUTE_DISTANCE, OnComputeDistance)
	ON_BN_CLICKED(IDC_WORLD_UP_SET, OnWorldUpSet)
	ON_BN_CLICKED(IDC_ANIMATE, OnAnimate)
	ON_BN_CLICKED(IDC_VIEWPOINT_NEXT, OnViewpointNext)
	ON_BN_CLICKED(IDC_VIEWPOINT_PREV, OnViewpointPrev)
	ON_BN_CLICKED(IDC_ANIMATE_ALL, OnAnimateAll)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DViewpoint message handlers

void DViewpoint::OnOK() 
{
	CDialog::OnOK();
	if (m_selectedCamera != NULL) {
		m_pView->view->SetCamera(m_selectedCamera);
		UpdateData(FALSE);
		m_selectedCamera = NULL;
	}
	m_pView->Redraw();
	if (m_pView) 
	    m_pView->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}

void DViewpoint::OnSelcancelViewpointList() 
{
}

void DViewpoint::OnDblclkViewpointList() 
{
	OnSelchangeViewpointList();

	OnApply();
}

void DViewpoint::OnApply() 
{   int animate = 1;

	UpdateData(TRUE);
	if (m_selectedCamera != NULL) {
		m_pView->view->SetCamera(m_selectedCamera,m_selectedCameraTransform,m_animate);
        m_pView->view->SetViewpointIndex(m_viewpoint_listi);
		UpdateData(FALSE);
		m_selectedCamera = NULL;
	}
	m_pView->Redraw();
}

void DViewpoint::OnSelchangeViewpointList() 
{
	GvSceneInfo *sceneInfo = m_pView->view->sceneInfo;

    m_viewpoint_listi = m_viewpoint_list.GetCurSel();
	if (m_viewpoint_listi >=0 && sceneInfo && m_viewpoint_listi <sceneInfo->cameras.Length()) {
        int index = m_viewpoint_list.GetItemData(m_viewpoint_listi);
      	m_selectedCamera =  sceneInfo->cameras[index];
      	m_selectedCameraTransform =  &sceneInfo->cameraTransforms[index];

	//	m_selectedCamera = (GvNode*) m_viewpoint_list.GetItemData(m_viewpoint_listi);
	} else {
        m_selectedCamera = NULL;
        m_selectedCameraTransform = NULL;
    }
}



// overwrite a selected viewpoint 
void DViewpoint::OnViewpointSave() 
{
	UpdateData(TRUE);

	GvSceneInfo *sceneInfo = m_pView->view->sceneInfo;

	GvNode *m_selectedCamera;

    m_viewpoint_listi = m_viewpoint_list.GetCurSel();
	if (m_viewpoint_listi >=0 && sceneInfo && m_viewpoint_listi <sceneInfo->cameras.Length()) {
        int index = m_viewpoint_list.GetItemData(m_viewpoint_listi);
      	m_selectedCamera =  sceneInfo->cameras[index];
	} else {
        m_selectedCamera = NULL;
    }

	if (m_selectedCamera != NULL) {
		m_pView->view->SaveCamera(m_selectedCamera);
	}
	else OnViewpointSaveAs();
}

// save a new viewpoint 
void DViewpoint::OnViewpointSaveAs() 
{
	UpdateData(TRUE);

    m_pView->view->SaveCamera((const char *)NULL);

	SetCameraList(m_pView->view->sceneInfo);
    m_pView->view->GetViewpointIndex(m_viewpoint_listi);
    m_viewpoint_list.SetCurSel(m_viewpoint_listi);
	//OnSelchangeViewpointList(); 
}



// fill camera list box 
int DViewpoint::SetCameraList(GvSceneInfo *sceneInfo)
{
	m_viewpoint_list.ResetContent();
	m_viewpoint_listi = -1;
	
	if (sceneInfo) {
	for (int i = 0; i< sceneInfo->cameras.Length(); i++) {
		GvNode *n = sceneInfo->cameras[i];
		char buf[60];
		sprintf(buf,"#%d",i);
		int item;
        CString s;


		if (n->objName && (n->objName->getLength()>0))
            s += n->objName->getString();
		else 
  			s += buf;

        if (RTISA(n,GvViewpoint)) {
            GvViewpoint *v = (GvViewpoint *) n;
            s += " - ";
            s += + v->description.value.getString();
        }

        item=m_viewpoint_list.AddString(s);

		if (item>=0) 
			m_viewpoint_list.SetItemData(item,i); // (DWORD)n);
    }
   }

   m_pView->view->GetViewpointIndex(m_viewpoint_listi);
   m_viewpoint_list.SetCurSel(m_viewpoint_listi);

   return(1);
}

void DViewpoint::OnComputeDistance() 
{

	if (!m_pView) return;

	// recompute znear  distance zfar based on bounding box
	GView *view = m_pView->view;

	GBBoxTraversal bboxTraversal;

   	bboxTraversal.InitializeAttributes();
  	InitializeGvAttributes(bboxTraversal);

	view->Traverse(bboxTraversal);

	  	  
	if (!bboxTraversal.bbox.IsEmpty()) {
		Point size = bboxTraversal.bbox.Size();
		float field = max(max(fabs(size.x),fabs(size.y)),fabs(size.z));

		m_distance = field*2.0f;
  		m_zfar = max(field * 4.0f,field*2.0f);
//    	m_znear = min(field *0.001f,1.0f);
        m_znear = m_zfar * GCamera::znearFactor;

		CDataExchange dxr(this, FALSE);

		
		DDX_Text(&dxr, IDC_ZNEAR, m_znear);
		DDX_Text(&dxr, IDC_ZFAR, m_zfar);
		DDX_Text(&dxr, IDC_DISTANCE, m_distance);

		view->bbox = bboxTraversal.bbox;

	 }
}


// compute world up from current up vector

void DViewpoint::OnWorldUpSet() 
{
    if (!m_pCamera) return;
    m_pCamera->ComputeWorldUpFromUp();

    CDataExchange dxr(this, FALSE);
    CstringFromPoint(m_world_up,m_pCamera->worldUp);
	DDX_Text(&dxr, IDC_WORLD_UP, m_world_up);
	
}

void DViewpoint::OnAnimate() 
{
	// TODO: Add your control notification handler code here
	
}

void DViewpoint::OnViewpointNext() 
{
    m_pView->view->SetNextViewpoint(m_animate);
    m_pView->view->GetViewpointIndex(m_viewpoint_listi);
    m_viewpoint_list.SetCurSel(m_viewpoint_listi);
    UpdateData(FALSE);
    m_pView->Redraw();
	
}

void DViewpoint::OnViewpointPrev() 
{
    m_pView->view->SetPrevViewpoint(m_animate);
    m_pView->view->GetViewpointIndex(m_viewpoint_listi);
    m_viewpoint_list.SetCurSel(m_viewpoint_listi);
    UpdateData(FALSE);
    m_pView->Redraw();

	
}

void DViewpoint::OnAnimateAll() 
{
    m_pView->view->AnimateAllCameras();
    m_pView->view->GetViewpointIndex(m_viewpoint_listi);
    m_pView->Redraw();
	
}

void DViewpoint::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void DViewpoint::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();


}

void DViewpoint::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	if (m_pView) 
	    m_pView->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}

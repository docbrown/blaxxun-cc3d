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
// ddefsph.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "ddefsph.h"
// ??
#include "resource.h"
#include "gdeform.h"

// for deform
#include "gclass.h"
#include <gvmorph.h>

#include "grender.h"
#include "gmodel.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DDeformSphere dialog

DDeformSphere::DDeformSphere(CWnd* pParent /*=NULL*/,CMyglView *pView,GSphereDeform *Deform)
	: CDialog(DDeformSphere::IDD, pParent)

{
  	m_pView = pView;
   m_deform = Deform;

	//{{AFX_DATA_INIT(DDeformSphere)
	m_radius = 0;
	m_scale = 0;
	m_enable = TRUE;
	m_drag = TRUE;
	m_display = TRUE;
	//}}AFX_DATA_INIT

}


void DDeformSphere::SetDeform(GSphereDeform *deform)
{
	m_deform = deform;

}


void DDeformSphere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   if (!pDX->m_bSaveAndValidate) {
	  m_deform->GetRadius(m_radius);
	  m_deform->GetScale(m_scale);
	  m_display = m_deform->display;
	}

	//{{AFX_DATA_MAP(DDeformSphere)
	DDX_Text(pDX, IDC_RADIUS, m_radius);
	DDX_Text(pDX, IDC_SCALE, m_scale);
	DDX_Check(pDX, IDC_ENABLE, m_enable);
	DDX_Check(pDX, IDC_DRAGIT, m_drag);
	DDX_Check(pDX, IDC_DISPLAY, m_display);
	//}}AFX_DATA_MAP

   if (pDX->m_bSaveAndValidate) {
	  m_deform->SetRadius(m_radius);
	  m_deform->SetScale(m_scale);
	  m_deform->display = m_display;
	}

}

BEGIN_MESSAGE_MAP(DDeformSphere, CDialog)
	//{{AFX_MSG_MAP(DDeformSphere)
	ON_BN_CLICKED(IDC_UNSET_TRANSFORM, OnClickedUnsetTransform)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_DRAGIT, OnDragit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DDeformSphere message handlers

void DDeformSphere::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DDeformSphere::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DDeformSphere::OnClickedUnsetTransform()
{
	// TODO: Add your control notification handler code here
	m_deform->UnSetMatrix();
}


void DDeformSphere::OnApply() 
{
	if (m_pView) {	 
		if (UpdateData(TRUE)) 
		{
	   	   m_pView->Apply(*this);
	    }
	}
}

void DDeformSphere::OnEnable() 
{
	// TODO: Add your control notification handler code here
   OnApply();

	
}

void DDeformSphere::OnDragit() 
{
	// TODO: Add your control notification handler code here
   OnApply();
	
}

/////////////////////////////////////////////////////////////////////////////
// DMorph dialog


DMorph::DMorph(CWnd* pParent /*=NULL*/,CMyglView *pView)
	: CDialog(DMorph::IDD, pParent)
{
  	m_pView = pView;
	m_morph = NULL;

 	nscale_min= -2,nscale_max = 2;

	//{{AFX_DATA_INIT(DMorph)
	m_nscale1 = 1.0f;
	m_nscale2 = 1.0f;
	m_nwaves = 1.0f;
	m_rscale = 1.0f;
	m_wave_offset = 0.0f;
	m_wave_radius = 1.0f;
	m_type = _T("NONE");
	m_inormals = FALSE;
	m_t1 = 0.0f;
	m_t2 = 1.0f;
	m_cycle = FALSE;
	m_rotation = _T("");
	//}}AFX_DATA_INIT
}


void DMorph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
//	  m_nscale1_slider.SetRange(Map(nscale_min),Map(nscale_max));
//	  m_nscale1_slider.SetPos(Map(m_nscale1));
	  if (m_morph) {
	   m_type = m_morph->type.Get();
	   m_inormals = m_morph->hermite.get();	  
	   m_morph->nscale1.get(m_nscale1);	  
	   m_morph->nscale2.get(m_nscale2);	  
	   m_morph->rscale.get(m_rscale);	  
	   m_morph->wave_nwaves.get(m_nwaves);	  
	   m_morph->wave_offset.get(m_wave_offset);	  
	   m_morph->wave_radius.get(m_wave_radius);	  
	   m_morph->t0.get(m_t1);	  
	   m_morph->t1.get(m_t2);	  
	   
       m_cycle= m_morph->cycle.get();	  
       GvString rot;
       Rotation r=m_morph->rotation.value;
       r.angle = RAD2DEG(r.angle);
       r.get(rot);
       m_rotation = (const char *) rot;

	  //m_morph->swirl_nwaves.Get(m_nwaves);	  
	  }
	}
	
	//{{AFX_DATA_MAP(DMorph)
	DDX_Control(pDX, IDC_T2, m_t2_slider);
	DDX_Control(pDX, IDC_T1, m_t1_slider);
	DDX_Control(pDX, IDC_WAVE_SCALE, m_wave_scale_slider);
	DDX_Control(pDX, IDC_RSCALE, m_rscale_slider);
	DDX_Control(pDX, IDC_NWAVES, m_nwaves_slider);
	DDX_Control(pDX, IDC_NSCALE2, m_nscale2_slider);
	DDX_Control(pDX, IDC_NSCALE1, m_nscale1_slider);
	DDX_Text(pDX, IDC_NSCALE1_EDIT, m_nscale1);
	DDX_Text(pDX, IDC_NSCALE2_EDIT, m_nscale2);
	DDX_Text(pDX, IDC_NWAVES_EDIT, m_nwaves);
	DDX_Text(pDX, IDC_RSCALE_EDIT, m_rscale);
	DDX_Text(pDX, IDC_WAVE_OFFSET_EDIT, m_wave_offset);
	DDX_Text(pDX, IDC_WAVE_SCALE_EDIT, m_wave_radius);
	DDX_CBString(pDX, IDC_MORPH_TYPE, m_type);
	DDX_Check(pDX, IDC_INORMALS, m_inormals);
	DDX_Text(pDX, IDC_T1_EDIT, m_t1);
	DDX_Text(pDX, IDC_T2_EDIT, m_t2);
	DDX_Check(pDX, IDC_CYCLE, m_cycle);
	DDX_Text(pDX, IDC_ROTATION, m_rotation);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
	  if (m_morph) {
	  m_morph->type.Set(m_type);
	  m_morph->hermite.set(m_inormals);	  
	  m_morph->nscale1.set(m_nscale1);	  
	  m_morph->nscale2.set(m_nscale2);	  
	  m_morph->rscale.set(m_rscale);	  
	  m_morph->wave_nwaves.set(m_nwaves);	  
	  m_morph->wave_offset.set(m_wave_offset);	  
	  m_morph->wave_radius.set(m_wave_radius);	  
	  m_morph->swirl_nwaves.set(m_nwaves);	  
	  
      m_morph->t0.set(m_t1);	  
	  m_morph->t1.set(m_t2);	  
	  m_morph->cycle.set(m_cycle);	  

      GvString rot;
      Rotation r;
      r.set(m_rotation);
      Point axis = r.Axis(); 
      
      axis.Normalize();
      r.set(axis.x,axis.y,axis.z, DEG2RAD(r.angle));

      m_morph->rotation.value = r;

      if (fabs(m_morph->rotation.value.angle) <= 0.00001)       
          m_morph->rotation.setDefault(TRUE);
      else m_morph->rotation.setDefault(FALSE);

      m_morph->rotation.OnChanged();


      TRACE("Dform exchange  nscale %f %f  t %f %f \n",m_morph->nscale1.value,m_morph->nscale2.value,
        m_morph->t0.value,m_morph->t1.value);
	  }
	}

}


BEGIN_MESSAGE_MAP(DMorph, CDialog)
	//{{AFX_MSG_MAP(DMorph)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DEFAULTS, OnDefaults)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_NSCALE1_EDIT, OnChangeNscale1Edit)
	ON_EN_CHANGE(IDC_NSCALE2_EDIT, OnChangeNscale2Edit)
	ON_EN_CHANGE(IDC_NWAVES_EDIT, OnChangeNwavesEdit)
	ON_EN_CHANGE(IDC_RSCALE_EDIT, OnChangeRscaleEdit)
	ON_EN_CHANGE(IDC_WAVE_OFFSET_EDIT, OnChangeWaveOffsetEdit)
	ON_EN_CHANGE(IDC_WAVE_SCALE_EDIT, OnChangeWaveScaleEdit)
	ON_EN_CHANGE(IDC_T1_EDIT, OnChangeT1Edit)
	ON_EN_CHANGE(IDC_T2_EDIT, OnChangeT2Edit)
	ON_CBN_SELCHANGE(IDC_MORPH_TYPE, OnSelchangeMorphType)
	ON_BN_CLICKED(IDC_INORMALS, OnInormals)
	ON_CBN_SELENDOK(IDC_MORPH_TYPE, OnSelendokMorphType)
	ON_EN_KILLFOCUS(IDC_ROTATION, OnKillfocusRotation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DMorph message handlers

void DMorph::OnApply() 
{
	if (m_pView) {	 
		if (UpdateData(TRUE)) 
		{
	   	   //m_pView->Apply(*this);
	   	   m_pView->Redraw();
	    }
	}
	
}

void DMorph::OnDefaults() 
{
	  if (m_morph) {   
	  	m_morph->ResetParams();
		UpdateData(FALSE);
	  }
}



BOOL DMorph::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nscale1_slider.Init(IDC_NSCALE1_EDIT, &m_nscale1,nscale_min,nscale_max);
	m_nscale2_slider.Init(IDC_NSCALE2_EDIT, &m_nscale2,nscale_min,nscale_max);
	m_wave_scale_slider.Init(IDC_WAVE_SCALE_EDIT, &m_wave_radius,-2,2);
	m_rscale_slider.Init(IDC_RSCALE_EDIT, &m_rscale,-2,2);
	m_nwaves_slider.Init(IDC_NWAVES_EDIT, &m_nwaves,0,10);

  	m_t1_slider.Init(IDC_T1_EDIT, &m_t1,0.0,1.0);
  	m_t2_slider.Init(IDC_T2_EDIT, &m_t2,0.0,1.0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMorph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_nscale1_slider.OnScroll(this);
	m_nscale2_slider.OnScroll(this);
	m_wave_scale_slider.OnScroll(this);
	m_rscale_slider.OnScroll(this);
	m_nwaves_slider.OnScroll(this);
	m_t1_slider.OnScroll(this);
	m_t2_slider.OnScroll(this);

   CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	if (nSBCode == SB_THUMBPOSITION) OnApply();
	 
}

void DMorph::OnChangeNscale1Edit() 
{
	m_nscale1_slider.SetFromEdit(this);
}

void DMorph::OnChangeNscale2Edit() 
{
	m_nscale2_slider.SetFromEdit(this);
}

void DMorph::OnChangeNwavesEdit() 
{
	m_nwaves_slider.SetFromEdit(this);
	
}

void DMorph::OnChangeRscaleEdit() 
{
	m_rscale_slider.SetFromEdit(this);
}

void DMorph::OnChangeWaveOffsetEdit() 
{
	// TODO: Add your control notification handler code here
	
}

void DMorph::OnChangeWaveScaleEdit() 
{
	m_wave_scale_slider.SetFromEdit(this);

}

void DMorph::OnOK() 
{
	// enable animation if not enabled 
	if (m_pView) {
	  if (!m_pView->view->time.Enabled()) {
	  	if (AfxMessageBox("Enable animation now ?", MB_YESNO) == IDYES) {
			m_pView->view->time.Enable();
		}
	  }
	}
	
	CDialog::OnOK();
}



void DMorph::OnChangeT1Edit() 
{
	m_t1_slider.SetFromEdit(this);
	
}

void DMorph::OnChangeT2Edit() 
{
	m_t2_slider.SetFromEdit(this);
	
}

void DMorph::OnSelchangeMorphType() 
{
   OnApply();
}

// clicked on INormals 
void DMorph::OnInormals() 
{
    OnApply();
	
}

void DMorph::OnSelendokMorphType() 
{
	// TODO: Add your control notification handler code here
	
}

void DMorph::OnKillfocusRotation() 
{
	// TODO: Add your control notification handler code here
	
}

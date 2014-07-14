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
// dtime.cpp : implementation file
//

#include "stdafx.h"
#include "glviewhdr.h"

#include "dtime.h"
#include "gclass.h"
#include "grender.h"
#include "gmodel.h"

#include "resource.h"




#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DTime dialog


DTime::DTime(CWnd* pParent /*=NULL*/,CMyglView *pView /* = NULL */)
	: CDialog(DTime::IDD, pParent)
{

	m_pView = pView;
	m_enable = 0;

	//{{AFX_DATA_INIT(DTime)
	m_t = 0.0f;
	m_t0 = 0.0f;
	m_t1 = 0.0f;
	m_dt = 0.0f;
	m_wrap = FALSE;
	m_realtime = FALSE;
	m_pause = FALSE;
	//}}AFX_DATA_INIT
}

#define MAPT(T) (1000.0 * (T - m_t0)  / (m_t1-m_t0))
#define UNMAPT(T) (m_t0 + (T)/1000.0 * (m_t1-m_t0))

BOOL DTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_slider_t.SetRange(0, 1000);
	m_slider_t.SetTicFreq(50);
	m_slider_t.SetPos(MAPT(m_t));

    m_dt_slider.Init(IDC_DT, &m_dt,0,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
	   
	   GTime &t = m_pView->view->time;
	   m_t= t.t;
	   t.GetRange(m_t0,m_t1);
	   m_dt= t.dt;
	   
	   
	   m_enable = t.enabled;
	   m_pause = t.pause;
	   m_wrap = t.wrap;
	   m_realtime = m_pView->view->playRealtime;

    
	}

	//{{AFX_DATA_MAP(DTime)
	DDX_Control(pDX, IDC_DT_SLIDER, m_dt_slider);
	DDX_Control(pDX, IDC_SCROLLBAR_T, m_slider_t);
	DDX_Control(pDX, IDC_START, m_start);
	DDX_Text(pDX, IDC_T, m_t);
	DDX_Text(pDX, IDC_T0, m_t0);
	DDX_Text(pDX, IDC_T1, m_t1);
	DDX_Text(pDX, IDC_DT, m_dt);
	DDX_Check(pDX, IDC_WRAP, m_wrap);
	DDX_Check(pDX, IDC_REALTIME, m_realtime);
	DDX_Check(pDX, IDC_PAUSE, m_pause);
	//}}AFX_DATA_MAP

	// sliders are bound now
	if (!pDX->m_bSaveAndValidate) {
		   m_slider_t.SetPos(MAPT(m_t));
         
         // flickers to much
         SetDlgItemText(IDC_START,m_enable ? "Stop" : "Start");

   }

	if (pDX->m_bSaveAndValidate) {
	   
	   GTime &t = m_pView->view->time;
	   t.t = m_t;
   	   t.SetRange(m_t0,m_t1);

	   t.dt = m_dt;
	   
	   //m_t_sb = 100.0 * m_t / (m_t1-m_t0);
	   
	   t.enabled = m_enable;
	   t.pause = m_pause;
	   t.wrap = m_wrap;
	   m_pView->view->playRealtime = m_realtime;

       SetDlgItemText(IDC_START,m_enable ? "Stop" : "Start");
	}

}

void DTime::UpdateT()
{	   
   GTime &t = m_pView->view->time;
   m_t= t.t;
   DDX_Text(&CDataExchange(this, FALSE),IDC_T, m_t);
   m_slider_t.SetPos(MAPT(m_t));

}

void DTime::UpdatePlay()
{
	   GTime &t = m_pView->view->time;
	   
	   m_enable = t.enabled;
	   m_pause = t.pause;

       SetDlgItemText(IDC_START,m_enable ? "Stop" : "Start");
	   DDX_Check(&CDataExchange(this, FALSE), IDC_PAUSE, m_pause);

}




BEGIN_MESSAGE_MAP(DTime, CDialog)
	//{{AFX_MSG_MAP(DTime)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_T, OnChangeT)
	ON_EN_CHANGE(IDC_T0, OnChangeT0)
	ON_EN_CHANGE(IDC_T1, OnChangeT1)
	ON_EN_CHANGE(IDC_DT, OnChangeDt)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DTime message handlers

void DTime::OnApply() 
{
	if (m_pView) {	 
		if (UpdateData(TRUE)) 
		{
	   	  m_pView->view->TriggerTimeSensors(); // if !playRealTime, evaluate time sensors
		  m_pView->Redraw();
	    }
	}
}


void DTime::OnStart() 
{

 m_enable = ! m_enable;
 SetDlgItemText(IDC_START,m_enable ? "Stop" : "Start");
 UpdateData(TRUE);

 if (m_pView) {	 
  	   m_pView->Redraw();
 }

//  CDialog::OnOK();
	
}

void DTime::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	if (m_pView) 
	    m_pView->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}

void DTime::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	if (m_pView) 
	    m_pView->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}


void DTime::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	 float old_t = m_t;
	 // update t with slider value
	 int t = m_slider_t.GetPos();
	 m_t = UNMAPT(t);
 	 DDX_Text(&CDataExchange(this, FALSE),IDC_T, m_t);

	 m_dt_slider.OnScroll(this);

	 
	 if (t != old_t) 
	    OnApply(); // <<<<<<<<<<<<<<<Update
	 
//	 TRACE("slider code %d t = %d\n",nSBCode,m_t);

	 CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void DTime::OnChangeT() 
{
	// TODO: Add your control notification handler code here
	
}

void DTime::OnChangeT0() 
{
	// TODO: Add your control notification handler code here
	
}

void DTime::OnChangeT1() 
{
	// TODO: Add your control notification handler code here
	
}

void DTime::OnChangeDt() 
{
	// TODO: Add your control notification handler code here
	
}



void DTime::OnPause() 
{
   DDX_Check(&CDataExchange(this, TRUE), IDC_PAUSE, m_pause);
   GTime &t = m_pView->view->time;

   t.pause = m_pause;
}


void DTime::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();

// OnCancel will be called automatically 
//	if (m_pView)  
//	    m_pView->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}

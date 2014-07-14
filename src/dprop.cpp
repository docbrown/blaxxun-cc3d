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
// DProp.cpp : implementation file
//

#include "stdafx.h"
// #include "blaxxunCC3d.h"
#include "glviewhdr.h"

#include "resource.h"
#include "DProp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #include "blaxxunCC3dCtl.h"
#include "grender.h"
#include "gmodel.h"

#include "gurlcache.h"

/////////////////////////////////////////////////////////////////////////////
// DPropSheet

IMPLEMENT_DYNAMIC(DPropSheet, CPropertySheet)

DPropSheet::DPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

DPropSheet::DPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

DPropSheet::~DPropSheet()
{
}


BEGIN_MESSAGE_MAP(DPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(DPropSheet)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropSheet message handlers

void DPropSheet::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::DoDataExchange(pDX);
}

BOOL DPropSheet::OnInitDialog() 
{
	// TODO: Add your specialized code here and/or call the base class
	

	BOOL bResult = CPropertySheet::OnInitDialog();
	return bResult;

}


void DPropSheet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//SetActivePage(-1);


	CPropertySheet::OnClose();
//	if (m_pParentWnd) 
//	   m_pParentWnd->PostMessage(WM_DIALOG_CLOSED, 0, (long)this);

}

/////////////////////////////////////////////////////////////////////////////
// DPropD3D property page

IMPLEMENT_DYNCREATE(DPropD3D, CPropertyPage)

DPropD3D::DPropD3D(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropD3D::IDD)
{
	m_driverIndex = 0;
	m_modeIndex = -1;

	//{{AFX_DATA_INIT(DPropD3D)
	m_dither = FALSE;
	m_tfilter = FALSE;
	m_info = _T("");
	m_use_videomem = FALSE;
	m_antiAlias = FALSE;
	m_tmipmap = FALSE;
	//}}AFX_DATA_INIT
}

DPropD3D::~DPropD3D()
{
}


BOOL DPropD3D::OnInitDialog() 
{
	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDC_TEXT_D3D,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_D3D)->SetWindowText(value);
	
	_itoa(IDC_DITHER,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_DITHER)->SetWindowText(value);

	_itoa(IDC_TFILTER,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TFILTER)->SetWindowText(value);

	_itoa(IDC_VIDEO_MEM,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_VIDEO_MEM)->SetWindowText(value);
	
	_itoa(IDC_TEXT_FULLSCREEN,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_FULLSCREEN)->SetWindowText(value);

	_itoa(IDC_ANTI_ALIAS,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_ANTI_ALIAS)->SetWindowText(value);

	_itoa(IDC_TMIPMAP,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TMIPMAP)->SetWindowText(value);

	return CPropertyPage::OnInitDialog();

}

void DPropD3D::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropD3D)
	DDX_Control(pDX, IDC_COMBO_MODE, m_modeC);
	DDX_Control(pDX, IDC_COMBO_DRIVER, m_driverC);
	DDX_Check(pDX, IDC_DITHER, m_dither);
	DDX_Check(pDX, IDC_TFILTER, m_tfilter);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Check(pDX, IDC_VIDEO_MEM, m_use_videomem);
	DDX_Check(pDX, IDC_ANTI_ALIAS, m_antiAlias);
	DDX_Check(pDX, IDC_TMIPMAP, m_tmipmap);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {

		// FILL listboxes 
#ifdef _D3D
		int i;
		GD3Device *device = m_ctrl->view->device;
		if (device && device->bRenderingIsOK) {
		
		DWORD mem = device->TotalVideoMemory() / 1024;
		DWORD free = device->FreeVideoMemory() / 1024;
		BOOL vidBuffer = device->bBackBufferInVideo;
		BOOL soft = m_ctrl->view->renderState.doOwnLighting;
		
		int dx=3;
		if (device->dx6) dx=6;
		else if (device->dx5) dx=5;
		m_info.Format("Video Mem:%dk free:%dk, DX%d %s%s",mem,free,dx,(vidBuffer ? "using video mem" : "using system mem"),(soft ? ", software lighting" : ""));
		
		DDX_Text(pDX, IDC_INFO, m_info);

		m_driverC.ResetContent();
		for (i=0; i<device->NumDrivers; i++) {
			int o = m_driverC.AddString(device->Driver[i].NameUi + " - "+ device->Driver[i].About);
		}
		m_driverC.SetCurSel(m_driverIndex);

		m_modeC.ResetContent();
		for (i=0; i<device->NumModes; i++) {
			CString s;
			D3DAppMode &mode = device->Mode[i];
			s.Format("%d x %d  %d %s",mode.w,mode.h,mode.bpp,mode.bThisDriverCanDo ? "": "(not available)");
			int o = m_modeC.AddString(s);;
		}
		m_modeC.SetCurSel(m_modeIndex);
		}
#endif

	}

}


BEGIN_MESSAGE_MAP(DPropD3D, CPropertyPage)
	//{{AFX_MSG_MAP(DPropD3D)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVER, OnSelchangeComboDriver)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropD3D message handlers

void DPropD3D::OnSelchangeComboDriver() 
{
	int nIndex = m_driverC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_driverIndex = nIndex;
}

void DPropD3D::OnSelchangeComboMode() 
{
	int nIndex = m_modeC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_modeIndex = nIndex;
}


/////////////////////////////////////////////////////////////////////////////
// DPropGeneral property page

IMPLEMENT_DYNCREATE(DPropGeneral, CPropertyPage)

DPropGeneral::DPropGeneral(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropGeneral::IDD)
{
	//{{AFX_DATA_INIT(DPropGeneral)
	m_menueMode = _T("");
	m_verbose = FALSE;
	m_fullSpeed = FALSE;
	m_navPanelOn = FALSE;
	m_exactCollision = FALSE;
	//}}AFX_DATA_INIT
}

DPropGeneral::~DPropGeneral()
{
}

BOOL DPropGeneral::OnInitDialog() 
{
	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDT_USER_INTERFACE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDT_USER_INTERFACE)->SetWindowText(value);

	_itoa(IDC_NAV_PANEL,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_NAV_PANEL)->SetWindowText(value);

	_itoa(IDC_FULL_SPEED,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_FULL_SPEED)->SetWindowText(value);
	
	_itoa(IDC_EXACT_COLLISION,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_EXACT_COLLISION)->SetWindowText(value);

	_itoa(IDC_VERBOSE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_VERBOSE)->SetWindowText(value);

	_itoa(IDC_REGISTER_VRML,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_REGISTER_VRML)->SetWindowText(value);

	return CPropertyPage::OnInitDialog();

}


void DPropGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropGeneral)
	DDX_Control(pDX, IDC_MENUE_MODE, m_menueModeC);
	DDX_CBString(pDX, IDC_MENUE_MODE, m_menueMode);
	DDX_Check(pDX, IDC_VERBOSE, m_verbose);
	DDX_Check(pDX, IDC_FULL_SPEED, m_fullSpeed);
	DDX_Check(pDX, IDC_NAV_PANEL, m_navPanelOn);
	DDX_Check(pDX, IDC_EXACT_COLLISION, m_exactCollision);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {
/*
		// FILL listboxes 
		int i;
		m_menueModeC.ResetContent();
		for (i=0; i< 0; i++) {
			//int o = m_menueModeC.AddString();
		}
*/
		m_menueModeC.SetCurSel(m_uiModeIndex);

	}

}


BEGIN_MESSAGE_MAP(DPropGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(DPropGeneral)
	ON_BN_CLICKED(IDC_REGISTER_VRML, OnRegisterVrml)
	ON_CBN_SELCHANGE(IDC_MENUE_MODE, OnSelchangeMenueMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropGeneral message handlers


void DPropGeneral::OnRegisterVrml() 
{
	// TODO: Add your control notification handler code here
	//BOOL ret = CGLViewCtrlCtrl::factory.UpdateRegistry(TRUE);
	//xxxx BOOL ret = m_ctrl->UpdateRegistry();
}

void DPropGeneral::OnSelchangeMenueMode() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_menueModeC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_uiModeIndex = nIndex;
}


/////////////////////////////////////////////////////////////////////////////
// DAskHardware dialog


DAskHardware::DAskHardware(CWnd* pParent /*=NULL*/)
	: CDialog(DAskHardware::IDD, pParent)
{
	m_ctrl = (CGLViewCtrlCtrl*) pParent;
	m_useHardware  = FALSE;
	//{{AFX_DATA_INIT(DAskHardware)
	m_dontAskAgain = FALSE;
	m_info = _T("");
	//}}AFX_DATA_INIT
}


BOOL DAskHardware::OnInitDialog() 
{
	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDC_ASK1,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_ASK1)->SetWindowText(value);

	_itoa(IDC_ASK2,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_ASK2)->SetWindowText(value);

	_itoa(IDC_ASK3,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_ASK3)->SetWindowText(value);

	return CDialog::OnInitDialog();
}


void DAskHardware::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DAskHardware)
//	DDX_Check(pDX, IDC_ASK, m_dontAskAgain);
	DDX_Text(pDX, IDC_INFO, m_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DAskHardware, CDialog)
	//{{AFX_MSG_MAP(DAskHardware)
	ON_BN_CLICKED(IDNO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DAskHardware message handlers

void DAskHardware::OnOK() 
{
	// TODO: Add extra validation here
	m_useHardware = TRUE;
	CDialog::OnOK();
	// send notification to control window
	m_pParentWnd->PostMessage(WM_HARDWARECHECK,NULL,NULL);
}

void DAskHardware::OnNo() 
{
	UpdateData(TRUE);

	m_useHardware = FALSE;
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	

	// send notification to control window
	m_pParentWnd->PostMessage(WM_HARDWARECHECK,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// DPropSound dialog


DPropSound::DPropSound(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropSound::IDD)
{

	m_qualityI=0;

	//{{AFX_DATA_INIT(DPropSound)
	m_quality = _T("");
	m_info = _T("");
	m_num_sounds = 8;
	m_use_rsx = FALSE;
	//}}AFX_DATA_INIT
}


BOOL DPropSound::OnInitDialog() 
{

	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDC_TEXT_SOUNDS,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_SOUNDS)->SetWindowText(value);

	_itoa(IDC_USE_RSX,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_USE_RSX)->SetWindowText(value);

	_itoa(IDC_TEXT_SOUNDQUAL,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_SOUNDQUAL)->SetWindowText(value);

	return CPropertyPage::OnInitDialog();

}


void DPropSound::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropSound)
	DDX_Control(pDX, IDC_COMBO_QUALITY, m_qualityC);
	DDX_CBString(pDX, IDC_COMBO_QUALITY, m_quality);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Text(pDX, IDC_NUM_SOUNDS, m_num_sounds);
	DDV_MinMaxInt(pDX, m_num_sounds, 0, 32);
	DDX_Check(pDX, IDC_USE_RSX, m_use_rsx);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {
		m_qualityC.SetCurSel(m_qualityI);
	}
	else {
		m_qualityI=m_qualityC.GetCurSel();
	}

}


BEGIN_MESSAGE_MAP(DPropSound, CPropertyPage)
	//{{AFX_MSG_MAP(DPropSound)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropSound message handlers
/////////////////////////////////////////////////////////////////////////////
// DPropOpenGL property page

IMPLEMENT_DYNCREATE(DPropOpenGL, CPropertyPage)

DPropOpenGL::DPropOpenGL(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropOpenGL::IDD)
{
	m_nurbsModeIndex = 0;

	//{{AFX_DATA_INIT(DPropOpenGL)
	m_dither = FALSE;
	m_info = _T("");
	m_info1 = _T("");
	m_tfilter = FALSE;
	m_rsoftware = FALSE;
	m_tmipmap = FALSE;
	//}}AFX_DATA_INIT
}

DPropOpenGL::~DPropOpenGL()
{
}

BOOL DPropOpenGL::OnInitDialog() 
{

	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDC_DITHER,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_DITHER)->SetWindowText(value);

	_itoa(IDC_TFILTER,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TFILTER)->SetWindowText(value);

	_itoa(IDC_RSOFTWARE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_RSOFTWARE)->SetWindowText(value);

	_itoa(IDC_TMIPMAP,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TMIPMAP)->SetWindowText(value);

	return CPropertyPage::OnInitDialog();

}

void DPropOpenGL::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropOpenGL)
	DDX_Control(pDX, IDC_GL_NURBS_MODE, m_nurbsModeC);
	DDX_Check(pDX, IDC_DITHER, m_dither);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Text(pDX, IDC_INFO1, m_info1);
	DDX_Check(pDX, IDC_TFILTER, m_tfilter);
	DDX_Check(pDX, IDC_RSOFTWARE, m_rsoftware);
	DDX_Check(pDX, IDC_TMIPMAP, m_tmipmap);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {
		m_nurbsModeC.SetCurSel(m_nurbsModeIndex);
	}
	else 
		m_nurbsModeIndex = m_nurbsModeC.GetCurSel();

}


BEGIN_MESSAGE_MAP(DPropOpenGL, CPropertyPage)
	//{{AFX_MSG_MAP(DPropOpenGL)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropOpenGL message handlers
/////////////////////////////////////////////////////////////////////////////
// DPropCache property page

IMPLEMENT_DYNCREATE(DPropCache, CPropertyPage)

DPropCache::DPropCache(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropCache::IDD)
{
	//{{AFX_DATA_INIT(DPropCache)
	m_useCache = FALSE;
	m_cacheDirectory = _T("");
	m_roCacheDirectorys = _T("");
	m_verifyDays = 0;
	m_cacheRead = FALSE;
	//}}AFX_DATA_INIT
}

DPropCache::~DPropCache()
{
}

BOOL DPropCache::OnInitDialog() 
{
	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDC_CACHE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHE)->SetWindowText(value);

	_itoa(IDC_TEXT_CACHE1,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_CACHE1)->SetWindowText(value);

	_itoa(IDC_TEXT_CACHE2,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_CACHE2)->SetWindowText(value);

	_itoa(IDC_TEXT_CACHE3,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TEXT_CACHE3)->SetWindowText(value);


	return CPropertyPage::OnInitDialog();

}

void DPropCache::DoDataExchange(CDataExchange* pDX)
{
//	DDV_MinMaxInt(pDX, m_verifyDays, 0, 100000);
//	DDX_Text(pDX, IDC_VERIFY_DAYS, m_verifyDays);
//	DDX_Check(pDX, IDC_CACHE_READ, m_cacheRead);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropCache)
	DDX_Check(pDX, IDC_CACHE, m_useCache);
	DDX_Text(pDX, IDC_DIRECTORY, m_cacheDirectory);
	DDX_Text(pDX, IDC_RO_DIRECTORY, m_roCacheDirectorys);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DPropCache, CPropertyPage)
	//{{AFX_MSG_MAP(DPropCache)
	ON_BN_CLICKED(IDC_DELETE, OnEmptyCache)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropCache message handlers

#define MB (1024l*1024l)

void DPropCache::OnEmptyCache() 
{
}

/////////////////////////////////////////////////////////////////////////////
// DPropCacheSettings property page

IMPLEMENT_DYNCREATE(DPropCacheSettings, CPropertyPage)

DPropCacheSettings::DPropCacheSettings(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropCacheSettings::IDD)
{
	//{{AFX_DATA_INIT(DPropCacheSettings)
	m_cacheVerify = -1;
	m_maxSpace = 0;
	m_minSpaceFree = 0;
	m_verifyDays = 0;
	//}}AFX_DATA_INIT
}

DPropCacheSettings::~DPropCacheSettings()
{
}


BOOL DPropCacheSettings::OnInitDialog() 
{
	CString value;
	char buf[60];
	buf[0]='p';

	_itoa(IDC_CACHEOPT_ALWAYS,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT_ALWAYS)->SetWindowText(value);

	_itoa(IDC_CACHEOPT_ONCE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT_ONCE)->SetWindowText(value);

	_itoa(IDC_CACHEOPT_DAYS,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT_DAYS)->SetWindowText(value);

	_itoa(IDC_CACHEOPT1,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT1)->SetWindowText(value);

	_itoa(IDC_CACHEOPT2,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT2)->SetWindowText(value);

	_itoa(IDC_CACHEOPT3,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT3)->SetWindowText(value);
	
	_itoa(IDC_CACHEOPT4,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_CACHEOPT4)->SetWindowText(value);
	
	_itoa(IDC_DELETE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_DELETE)->SetWindowText(value);

	return CPropertyPage::OnInitDialog();

}

void DPropCacheSettings::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropCacheSettings)
	DDX_Radio(pDX, IDC_CACHEOPT_ALWAYS, m_cacheVerify);
	DDX_Text(pDX, IDC_MAX_SPACE, m_maxSpace);
	DDX_Text(pDX, IDC_MIN_SPACE_FREE, m_minSpaceFree);
	DDX_Text(pDX, IDC_VERIFY_DAYS, m_verifyDays);
	//}}AFX_DATA_MAP
}

//	ON_BN_CLICKED(IDC_CLEAN, OnCacheClean)

BEGIN_MESSAGE_MAP(DPropCacheSettings, CPropertyPage)
	//{{AFX_MSG_MAP(DPropCacheSettings)
	ON_BN_CLICKED(IDC_DELETE, OnCacheDelete)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MIN_SPACE_FREE_SPIN, OnDeltaposMinSpaceFreeSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MAX_SPACE_SPIN, OnDeltaposMaxSpaceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropCacheSettings message handlers

void DPropCacheSettings::OnCacheClean() 
{
//	CWaitCursor wait;

	time_t oldest = 0;
	
	DWORD spaceInUse = 0;
	int filesInUse=0;

	if (!theCache.writeCacheDirValid){
		CString prompt("The cache directory is currently invalid, set a new one");
		AfxMessageBox(prompt,MB_OK);
		return;
	}

	theCache.GetCacheStats(theCache.writeCacheDir,oldest,spaceInUse,filesInUse);
	theCache.spaceUsed = spaceInUse;

	theCache.CheckCache();
	
}

void DPropCacheSettings::OnCacheDelete() 
{
	
	CString prompt;
	time_t oldest = 0;
	DWORD spaceInUse = 0;
	double diskSpace = 0.0f;
	int filesInUse=0;


	if (!theCache.writeCacheDirValid ){
		prompt = "The cache directory is currently invalid, set a new one";
		AfxMessageBox(prompt,MB_OK);
		return;
	}
	diskSpace = (double) theCache.GetFreeDiskSpace() /  (double) MB;

	theCache.Lock();
	{ CWaitCursor wait;
		theCache.GetCacheStats(theCache.writeCacheDir,oldest,spaceInUse,filesInUse);
	}

	prompt = "Do you want to delete all files and directories from : \n";
	prompt += theCache.writeCacheDir;

	CString stat;
	stat.Format("\nCurrently are %d files and %g mb diskspace in use\nfree space on disk : %g mb ",filesInUse,(double) spaceInUse / (1024.0f * 1024.0f),diskSpace);
	prompt += stat;

	theCache.Unlock();


	if (AfxMessageBox(prompt,MB_YESNO) == IDYES) {
		theCache.Lock(); 
		{
			CWaitCursor wait;
			theCache.RemoveFiles(theCache.writeCacheDir,0);
			theCache.lastCacheFlushTime = CTime::GetCurrentTime().GetTime();
			theCache.spaceUsed = 1; // mark its really 0, 0 is check first
		}
		theCache.Unlock();
	}
	
}	


void DPropCacheSettings::OnDeltaposMinSpaceFreeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	UpdateData(TRUE);
	m_minSpaceFree -= pNMUpDown->iDelta;
	if (m_minSpaceFree <1) m_minSpaceFree = 1;
	UpdateData(FALSE);
	
	*pResult = 0;
}

void DPropCacheSettings::OnDeltaposMaxSpaceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	UpdateData(TRUE);
	m_maxSpace -= pNMUpDown->iDelta;

	if (m_maxSpace <1) m_maxSpace = 1;
	UpdateData(FALSE);
	
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// DPropD3D2 property page

IMPLEMENT_DYNCREATE(DPropD3D2, CPropertyPage)

DPropD3D2::DPropD3D2(CGLViewCtrlCtrl *ctrl) : m_ctrl(ctrl), CPropertyPage(DPropD3D2::IDD)
{
	m_isKatmaiProcessor = FALSE;
	m_nurbsModeIndex = 0;

	//{{AFX_DATA_INIT(DPropD3D2)
	m_info = _T("");
	m_useKamtai = FALSE;
	m_softwareLighting = FALSE;
	m_targetFps = 30.0f;
	m_tessellationFactor = 1.0f;
	m_useD3dCulling = FALSE;
	//}}AFX_DATA_INIT
}

DPropD3D2::~DPropD3D2()
{
}

BOOL DPropD3D2::OnInitDialog() 
{
	CString value;
	char buf[60];
	buf[0]='p';

	// TBD : translate strings
	_itoa(IDC_PIII,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_PIII)->SetWindowText(value);

	_itoa(IDC_SOFTWARE_LIGHTING,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_SOFTWARE_LIGHTING)->SetWindowText(value);

	_itoa(IDC_NURBS_MODE,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_NURBS_MODE)->SetWindowText(value);

	_itoa(IDC_TARGET_FPS,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TARGET_FPS)->SetWindowText(value);

	_itoa(IDC_TESS_FACTOR,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_TESS_FACTOR)->SetWindowText(value);

	_itoa(IDC_USE_D3D_CULLING,&buf[1],10);
	m_ctrl->m_translator.Translate(buf,"",value);
	if (value != "") GetDlgItem(IDC_USE_D3D_CULLING)->SetWindowText(value);

	return CPropertyPage::OnInitDialog();
}

void DPropD3D2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPropD3D2)
	DDX_Control(pDX, IDC_PIII, m_useKatmaiC);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Check(pDX, IDC_PIII, m_useKamtai);
	DDX_Check(pDX, IDC_SOFTWARE_LIGHTING, m_softwareLighting);
	DDX_Control(pDX, IDC_NURBS_MODE, m_nurbsModeC);
	DDX_Text(pDX, IDC_TARGET_FPS, m_targetFps);
	DDX_Text(pDX, IDC_TESS_FACTOR, m_tessellationFactor);
	DDX_Check(pDX, IDC_USE_D3D_CULLING, m_useD3dCulling);
	//}}AFX_DATA_MAP




	if (!pDX->m_bSaveAndValidate) {
#ifdef _OGL
	CWnd *w;
	w=GetDlgItem(IDC_SOFTWARE_LIGHTING);
	if (w != NULL) w->ShowWindow(SW_HIDE);
	w=GetDlgItem(IDC_USE_D3D_CULLING);
	if (w != NULL) w->ShowWindow(SW_HIDE);
	m_useKatmaiC.ShowWindow(SW_HIDE);

#endif

		m_useKatmaiC.EnableWindow(m_isKatmaiProcessor);
		//if (!m_isKatmaiProcessor) // hide until 26. Feb Katmai release
		//	m_useKatmaiC.ShowWindow( SW_HIDE   );
		m_nurbsModeC.SetCurSel(m_nurbsModeIndex);
	}
	else 
		m_nurbsModeIndex = m_nurbsModeC.GetCurSel();

}


BEGIN_MESSAGE_MAP(DPropD3D2, CPropertyPage)
	//{{AFX_MSG_MAP(DPropD3D2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TARGET_FPS_SPIN, OnDeltaposTargetFpsSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TESS_FACTOR_SPIN, OnDeltaposTessFactorSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPropD3D2 message handlers

void DPropD3D2::OnDeltaposTargetFpsSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_targetFps -= pNMUpDown->iDelta;
	if (m_targetFps <1) m_targetFps = 1;
	UpdateData(FALSE);
	
	*pResult = 0;
}

void DPropD3D2::OnDeltaposTessFactorSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_tessellationFactor += -0.5* pNMUpDown->iDelta;

	if (m_tessellationFactor <0.00001) m_tessellationFactor = 0.00001;

	UpdateData(FALSE);
	
	*pResult = 0;
}

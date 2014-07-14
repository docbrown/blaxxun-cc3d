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
// DD3DOptions.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "grender.h"
#include "gmodel.h"


#include "DD3DOptions.h"


#ifdef _D3D





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DD3DOptions dialog


DD3DOptions::DD3DOptions(CWnd* pParent /*=NULL*/,GView *view)
	: CDialog(DD3DOptions::IDD, pParent)
{
	m_driverIndex = -1;
	m_modeIndex = -1;
	m_textureModeIndex = -1;
	m_driverIndex = -1;
	m_view = view;

	//{{AFX_DATA_INIT(DD3DOptions)
	m_highlights = TRUE;
	m_dither = FALSE;
	m_alphaStipple = FALSE;
	m_driver = _T("");
	m_tfilter = FALSE;
	m_info = _T("");
	m_use_videomem = FALSE;
	m_dxLevel = _T("");
	m_software = FALSE;
	m_anitalias = FALSE;
	m_useD3DCulling = FALSE;
	//}}AFX_DATA_INIT
}


void DD3DOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(DD3DOptions)
	DDX_Control(pDX, IDC_DXLEVEL, m_dxLevelC);
	DDX_Control(pDX, IDC_DEVICE, m_deviceC);
	DDX_Control(pDX, IDC_DRIVER, m_driverC);
	DDX_Control(pDX, IDC_MODE, m_modeC);
	DDX_Control(pDX, IDC_TEXTURE_MODE, m_textureModeC);
	DDX_Check(pDX, IDC_HIGHLIGHTS, m_highlights);
	DDX_Check(pDX, IDC_DITHER, m_dither);
	DDX_Check(pDX, IDC_STIPPLE, m_alphaStipple);
	DDX_CBString(pDX, IDC_DRIVER, m_driver);
	DDX_Check(pDX, IDC_TFILTER, m_tfilter);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Check(pDX, IDC_USE_VIDEOMEM, m_use_videomem);
	DDX_CBString(pDX, IDC_DXLEVEL, m_dxLevel);
	DDX_Check(pDX, IDC_SOFTWARE, m_software);
	DDX_Check(pDX, IDC_ANTIALIAS, m_anitalias);
	DDX_Check(pDX, IDC_USE_D3DCULLING, m_useD3DCulling);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {

		// FILL listboxes 
		int i;
		m_driverC.ResetContent();
		GD3Device *device = m_view->device;

		if (device && device->bRenderingIsOK) {

		for (i=0; i<device->NumDrivers; i++) {
			int o = m_driverC.AddString(device->Driver[i].Name + " - "+ device->Driver[i].About);
		}
		m_driverC.SetCurSel(m_driverIndex);

		m_modeC.ResetContent();
		for (i=0; i<device->NumModes; i++) {
			CString s;
			s.Format("%d x %d  %d %s",device->Mode[i].w,device->Mode[i].h,device->Mode[i].bpp,device->Mode[i].bThisDriverCanDo ? "": "(not available)");
			int o = m_modeC.AddString(s);;
		}
		m_modeC.SetCurSel(m_modeIndex);


		m_textureModeC.ResetContent();
		for (i=0; i<device->NumTextureFormats; i++) {
			D3DAppTextureFormat *t =&device->TextureFormat[i];
			CString s;
			if (t->bPalettized)  
				s.Format("Palette %d",t->IndexBPP);
			else 
			if (t->AlphaBPP >0)
				s.Format("RGBA %d %d %d %d",t->RedBPP,t->GreenBPP,t->BlueBPP,t->AlphaBPP);
			 else s.Format("RGB %d %d %d",t->RedBPP,t->GreenBPP,t->BlueBPP);

			int o = m_textureModeC.AddString(s);;
		}
		m_textureModeC.SetCurSel(m_textureModeIndex);

		m_deviceC.ResetContent();
		for (i=0; i<device->NumDevices; i++) {
			int o = m_deviceC.AddString(device->Devices[i].Name + " - " + device->Devices[i].About);
		}
		m_deviceC.SetCurSel(m_deviceIndex);
		}

	} else {
		
	}

}


BEGIN_MESSAGE_MAP(DD3DOptions, CDialog)
	//{{AFX_MSG_MAP(DD3DOptions)
	ON_CBN_SELCHANGE(IDC_DRIVER, OnSelchangeDriver)
	ON_CBN_SELCHANGE(IDC_MODE, OnSelchangeMode)
	ON_CBN_SELCHANGE(IDC_TEXTURE_MODE, OnSelchangeTextureMode)
	ON_CBN_SELCHANGE(IDC_DEVICE, OnSelchangeDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DD3DOptions message handlers

void DD3DOptions::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


void DD3DOptions::OnSelchangeDriver() 
{
	int nIndex = m_driverC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_driverIndex = nIndex;
}



void DD3DOptions::OnSelchangeMode() 
{
	int nIndex = m_modeC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_modeIndex = nIndex;
	
}



void DD3DOptions::OnSelchangeTextureMode() 
{
	int nIndex = m_textureModeC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_textureModeIndex = nIndex;
}

#endif

void DD3DOptions::OnSelchangeDevice() 
{
	int nIndex = m_deviceC.GetCurSel();
	if (nIndex == CB_ERR)
		return;
	m_deviceIndex = nIndex;
	
}

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
// doptions.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "doptions.h"
// ??

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DOptions dialog

DOptions::DOptions(CWnd* pParent /*=NULL*/)
	: CDialog(DOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(DOptions)
	m_build_tristrips = FALSE;
	m_gl_cache = FALSE;
	m_print_time = FALSE;
	m_update_colors = FALSE;
	m_draw_frontbuffer = FALSE;
	m_zoom2 = FALSE;
	m_auto_spin = FALSE;
	m_win_x = 0;
	m_win_y = 0;
	m_gl_extensions = FALSE;
	m_degrade = FALSE;
	m_degrade_style = _T("");
	m_save_window_size = FALSE;
	m_zbuffer_16 = FALSE;
	m_use_rsx = FALSE;
	m_num_sounds = 0;
	m_minimize_files = FALSE;
	//}}AFX_DATA_INIT
}

void DOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DOptions)
	DDX_Check(pDX, IDC_BUILD_TRISTRIPS, m_build_tristrips);
	DDX_Check(pDX, IDC_GL_CACHE, m_gl_cache);
	DDX_Check(pDX, IDC_PRINT_TIME, m_print_time);
	DDX_Check(pDX, IDC_UPDATE_COLORS, m_update_colors);
	DDX_Check(pDX, IDC_DRAW_FRONT, m_draw_frontbuffer);
	DDX_Check(pDX, IDC_RENDER_ZOOM2, m_zoom2);
	DDX_Check(pDX, IDC_AUTO_SPIN, m_auto_spin);
	DDX_Text(pDX, IDC_WIN_SIZE_X, m_win_x);
	DDV_MinMaxInt(pDX, m_win_x, 0, 2048);
	DDX_Text(pDX, IDC_WIN_SIZE_Y, m_win_y);
	DDV_MinMaxInt(pDX, m_win_y, 0, 2048);
	DDX_Check(pDX, IDC_GL_EXTENSIONS, m_gl_extensions);
	DDX_Check(pDX, IDC_DEGRADE, m_degrade);
	DDX_CBString(pDX, IDC_DEGRADE_STYLE, m_degrade_style);
	DDX_Check(pDX, IDC_SAVE_WINDOWS_SIZE, m_save_window_size);
	DDX_Check(pDX, IDC_ZBUFFER_16, m_zbuffer_16);
	DDX_Check(pDX, IDC_RSX_SOUND, m_use_rsx);
	DDX_Text(pDX, IDC_NUM_SOUNDS, m_num_sounds);
	DDX_Check(pDX, IDC_MIMIMIZE_FILE, m_minimize_files);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(DOptions, CDialog)
	//{{AFX_MSG_MAP(DOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DOptions message handlers

void DOptions::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DOptions::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// DObjectInfo dialog


DObjectInfo::DObjectInfo(CWnd* pParent /*=NULL*/)
	: CDialog(DObjectInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(DObjectInfo)
	m_convex_faces = FALSE;
	m_fcolor = FALSE;
	m_nfaces = 0;
	m_ntriangles = 0;
	m_nverts = 0;
	m_type = _T("");
	m_tristrips = FALSE;
	m_vcolor = FALSE;
	m_wsizex = 0;
	m_wsizey = 0;
	m_zdepth = 0;
	m_vparameter = FALSE;
	m_last_frame_time = 0.0f;
	m_tris_per_sec = 0;
	m_bbox_min = _T("");
	m_bbox_max = _T("");
	//}}AFX_DATA_INIT
}


void DObjectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DObjectInfo)
	DDX_Check(pDX, IDC_CONVEX_FACES, m_convex_faces);
	DDX_Check(pDX, IDC_FCOLOR, m_fcolor);
	DDX_Text(pDX, IDC_NFACES, m_nfaces);
	DDX_Text(pDX, IDC_NTRIANGLES, m_ntriangles);
	DDX_Text(pDX, IDC_NVERTS, m_nverts);
	DDX_Text(pDX, IDC_OBJECT_TYPE, m_type);
	DDX_Check(pDX, IDC_TRISTRIP, m_tristrips);
	DDX_Check(pDX, IDC_VCOLOR, m_vcolor);
	DDX_Text(pDX, IDC_WSIZEX, m_wsizex);
	DDX_Text(pDX, IDC_WSIZEY, m_wsizey);
	DDX_Text(pDX, IDC_ZDEPTH, m_zdepth);
	DDX_Check(pDX, IDC_VPARAMETER, m_vparameter);
	DDX_Text(pDX, IDC_LAST_FRAME_TIME, m_last_frame_time);
	DDX_Text(pDX, IDC_TRIS_PER_SEC, m_tris_per_sec);
	DDX_Text(pDX, IDC_BBOX_MIN, m_bbox_min);
	DDX_Text(pDX, IDC_BBOX_MAX, m_bbox_max);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DObjectInfo, CDialog)
	//{{AFX_MSG_MAP(DObjectInfo)
	ON_BN_CLICKED(IDC_SCENE_INFO, OnSceneInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DObjectInfo message handlers

void DObjectInfo::OnSceneInfo() 
{
	// TODO: Add your control notification handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// DSceneInfo dialog


DSceneInfo::DSceneInfo(CWnd* pParent /*=NULL*/)
	: CDialog(DSceneInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(DSceneInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DSceneInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DSceneInfo)
	DDX_Control(pDX, IDC_INFO, m_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DSceneInfo, CDialog)
	//{{AFX_MSG_MAP(DSceneInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DSceneInfo message handlers

BOOL DSceneInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// DInfoOpengl dialog


DInfoOpengl::DInfoOpengl(CWnd* pParent /*=NULL*/)
	: CDialog(DInfoOpengl::IDD, pParent)
{
	//{{AFX_DATA_INIT(DInfoOpengl)
	m_extensions = _T("");
	m_renderer = _T("");
	m_vendor = _T("");
	m_version = _T("");
	//}}AFX_DATA_INIT
}


void DInfoOpengl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DInfoOpengl)
	DDX_Text(pDX, IDC_EXTENSIONS, m_extensions);
	DDX_Text(pDX, IDC_RENDERER, m_renderer);
	DDX_Text(pDX, IDC_VENDOR, m_vendor);
	DDX_Text(pDX, IDC_VERSION, m_version);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DInfoOpengl, CDialog)
	//{{AFX_MSG_MAP(DInfoOpengl)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DInfoOpengl message handlers

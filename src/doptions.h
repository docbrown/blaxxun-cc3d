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
// doptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DOptions dialog

class DOptions : public CDialog
{
// Construction
public:
	DOptions(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(DOptions)
	enum { IDD = IDD_OPTIONS };
	BOOL	m_build_tristrips;
	BOOL	m_gl_cache;
	BOOL	m_print_time;
	BOOL	m_update_colors;
	BOOL	m_draw_frontbuffer;
	BOOL	m_zoom3;
	BOOL	m_zoom2;
	BOOL	m_auto_spin;
	int		m_win_x;
	int		m_win_y;
	BOOL	m_gl_extensions;
	BOOL	m_degrade;
	CString	m_degrade_style;
	BOOL	m_save_window_size;
	BOOL	m_zbuffer_16;
	BOOL	m_use_rsx;
	int		m_num_sounds;
	BOOL	m_minimize_files;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(DOptions)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DObjectInfo dialog

class DObjectInfo : public CDialog
{
// Construction
public:
	DObjectInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DObjectInfo)
	enum { IDD = IDD_OBJECT_INFO };
	BOOL	m_convex_faces;
	BOOL	m_fcolor;
	int		m_nfaces;
	int		m_ntriangles;
	int		m_nverts;
	CString	m_type;
	BOOL	m_tristrips;
	BOOL	m_vcolor;
	int		m_wsizex;
	int		m_wsizey;
	int		m_zdepth;
	BOOL	m_vparameter;
	float	m_last_frame_time;
	int		m_tris_per_sec;
	CString	m_bbox_min;
	CString	m_bbox_max;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DObjectInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DObjectInfo)
	afx_msg void OnSceneInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DSceneInfo dialog

class DSceneInfo : public CDialog
{
// Construction
public:
	DSceneInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DSceneInfo)
	enum { IDD = IDD_SCENE_INFO };
	CListBox	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DSceneInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DSceneInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DInfoOpengl dialog

class DInfoOpengl : public CDialog
{
// Construction
public:
	DInfoOpengl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DInfoOpengl)
	enum { IDD = IDD_INFO_OPENGL };
	CString	m_extensions;
	CString	m_renderer;
	CString	m_vendor;
	CString	m_version;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DInfoOpengl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DInfoOpengl)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

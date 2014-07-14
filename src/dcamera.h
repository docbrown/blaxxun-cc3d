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
// dcamera.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DViewpoint dialog

class GCamera;
class CMyglView;

class GvNode;

class GvSceneInfo;


class DViewpoint : public CDialog
{
// Construction
public:
	DViewpoint(CWnd* pParent = NULL,CMyglView *pView = NULL);   // standard constructor

	CMyglView *m_pView;
    // pointer to current glview camera
	GCamera *m_pCamera;
	
	Array<GvNode*> *m_pCameras;
    // Pointer to node of currently selected camera
	GvNode *m_selectedCamera;
    Matrix *m_selectedCameraTransform;
	
	//GvNode *m_selectedCameraKeyframe;
	
	// fill camera list box out of sceneInfo
	int SetCameraList(GvSceneInfo *sceneInfo);



// Dialog Data
	//{{AFX_DATA(DViewpoint)
	enum { IDD = IDD_CAMERA_VIEWPOINT };
	CListBox	m_viewpoint_list;
	int		m_viewpoint_listi;
	float	m_znear;
	float	m_zfar;
	float	m_fovy;
	CString	m_position;
	CString	m_target;
	CString	m_up;
	float	m_speed;
	float	m_distance;
	CString	m_world_up;
	BOOL	m_animate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DViewpoint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DViewpoint)
	virtual void OnOK();
	afx_msg void OnSelcancelViewpointList();
	afx_msg void OnDblclkViewpointList();
	afx_msg void OnApply();
	afx_msg void OnSelchangeViewpointList();
	afx_msg void OnViewpointSave();
	afx_msg void OnViewpointSaveAs();
	afx_msg void OnComputeDistance();
	afx_msg void OnWorldUpSet();
	afx_msg void OnAnimate();
	afx_msg void OnViewpointNext();
	afx_msg void OnViewpointPrev();
	afx_msg void OnAnimateAll();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

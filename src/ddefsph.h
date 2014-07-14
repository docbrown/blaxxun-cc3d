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
// ddefsph.h : header file
//

#include "CFSliderCtrl.h"

// forward
class GSphereDeform;

/////////////////////////////////////////////////////////////////////////////
// DDeformSphere dialog



class DDeformSphere : public CDialog
{
// Construction
public:
	DDeformSphere(CWnd* pParent = NULL,CMyglView *pView=NULL,GSphereDeform *Deform=NULL);	// standard constructor

   CMyglView *m_pView;
	GSphereDeform * m_deform;

   void SetDeform(GSphereDeform *deform);


// Dialog Data
	//{{AFX_DATA(DDeformSphere)
	enum { IDD = IDD_DEFORM_SPHERE };
	float	m_radius;
	float	m_scale;
	BOOL	m_enable;
	BOOL	m_drag;
	BOOL	m_display;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(DDeformSphere)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickedUnsetTransform();
	afx_msg void OnApply();
	afx_msg void OnEnable();
	afx_msg void OnDragit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// DMorph dialog

class GvMorph;

class DMorph : public CDialog
{
public :
   CMyglView *m_pView;
   GvMorph *m_morph;

   // if visible update fields
   void Update() { };  
   void SetMorph(GvMorph *morph) { m_morph = morph; Update();}
   
   float nscale_min,nscale_max;
   int Map(float s) { return (1000.0*(s-nscale_min) / (nscale_max-nscale_min)); }
   float UnMap(float s) { return (nscale_min+ s *  (nscale_max-nscale_min) / 1000.0); }


// Construction
public:
	DMorph(CWnd* pParent = NULL,CMyglView *pView=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DMorph)
	enum { IDD = IDD_MORPH };
	CFSliderCtrl	m_t2_slider;
	CFSliderCtrl	m_t1_slider;
	CFSliderCtrl	m_wave_scale_slider;
	CFSliderCtrl	m_rscale_slider;
	CFSliderCtrl	m_nwaves_slider;
	CFSliderCtrl	m_nscale2_slider;
	CFSliderCtrl	m_nscale1_slider;
	float	m_nscale1;
	float	m_nscale2;
	float	m_nwaves;
	float	m_rscale;
	float	m_wave_offset;
	float	m_wave_radius;
	CString	m_type;
	BOOL	m_inormals;
	float	m_t1;
	float	m_t2;
	BOOL	m_cycle;
	CString	m_rotation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMorph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMorph)
	afx_msg void OnApply();
	afx_msg void OnDefaults();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeNscale1Edit();
	afx_msg void OnChangeNscale2Edit();
	afx_msg void OnChangeNwavesEdit();
	afx_msg void OnChangeRscaleEdit();
	afx_msg void OnChangeWaveOffsetEdit();
	afx_msg void OnChangeWaveScaleEdit();
	virtual void OnOK();
	afx_msg void OnChangeT1Edit();
	afx_msg void OnChangeT2Edit();
	afx_msg void OnSelchangeMorphType();
	afx_msg void OnInormals();
	afx_msg void OnSelendokMorphType();
	afx_msg void OnKillfocusRotation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

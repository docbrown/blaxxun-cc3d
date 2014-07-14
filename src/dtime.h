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
// dtime.h : header file
//

#include "CFSliderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// DTime dialog

class DTime : public CDialog
{
public :
	CMyglView *m_pView;
	int m_enable;



// Construction
public:
	DTime(CWnd* pParent = NULL,CMyglView *pView=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DTime)
	enum { IDD = IDD_TIME };
	CFSliderCtrl	m_dt_slider;
	CSliderCtrl	m_slider_t;
	CButton	m_start;
	float	m_t;
	float	m_t0;
	float	m_t1;
	float	m_dt;
	BOOL	m_wrap;
	BOOL	m_realtime;
	BOOL	m_pause;
	//}}AFX_DATA

	// update time and time slider
	void UpdateT();
	// Update play/pause status
	void UpdatePlay();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DTime)
	afx_msg void OnStart();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeT();
	afx_msg void OnChangeT0();
	afx_msg void OnChangeT1();
	afx_msg void OnChangeDt();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnPause();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

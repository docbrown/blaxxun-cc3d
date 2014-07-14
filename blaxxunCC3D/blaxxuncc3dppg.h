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
// GLViewCtrlPpg.h : Declaration of the CGLViewCtrlPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CGLViewCtrlPropPage : See GLViewCtrlPpg.cpp.cpp for implementation.

class CGLViewCtrlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CGLViewCtrlPropPage)
	DECLARE_OLECREATE_EX(CGLViewCtrlPropPage)

// Constructor
public:
	CGLViewCtrlPropPage();

// Dialog Data
	//{{AFX_DATA(CGLViewCtrlPropPage)
	enum { IDD = IDD_PROPPAGE_GLVIEWCTRL };
	CString	m_src;
	BOOL	m_panel;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CGLViewCtrlPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#if 0
/////////////////////////////////////////////////////////////////////////////
// CRenderingPropPage : Property page dialog

class CRenderingPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRenderingPropPage)
	DECLARE_OLECREATE_EX(CRenderingPropPage)

// Constructors
public:
	CRenderingPropPage();

// Dialog Data
	//{{AFX_DATA(CRenderingPropPage)
	enum { IDD = IDD_PROPPAGE_RENDERING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CRenderingPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CNavigationPropPage dialog

class CNavigationPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CNavigationPropPage)
	DECLARE_OLECREATE_EX(CNavigationPropPage)

// Construction
public:
	CNavigationPropPage();
	~CNavigationPropPage();

// Dialog Data
	//{{AFX_DATA(CNavigationPropPage)
	enum { IDD = IDD_PROPPAGE_NAVIGATION };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNavigationPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNavigationPropPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#endif


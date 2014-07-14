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
// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

extern void Message(const char *message);
extern void Error(const char *message);


/////////////////////////////////////////////////////////////////////////////
// CUrlBox window

class CUrlBox : public CComboBox
{
// Construction
public:
	CUrlBox();
   int m_curSel;


// Attributes
public:

// Operations
public:
   int GetText(int nIndex, CString &val) { GetLBText(nIndex,val); return(val.GetLength()); };
   int GetText(CString &val) { GetWindowText(val); return(val.GetLength()); }
   
   int InsertText(int index, const CString &val) {	
      m_curSel =  InsertString(index,val);         
      SetCurSel(m_curSel);
      SetWindowText(val);
      return(m_curSel);
   }

   int AddText(const CString &val) {
      int index = FindStringExact( 0, val );
      if (index < 0) {
          return m_curSel = InsertText(0,val); 
      } else {
         m_curSel = index;
         SetCurSel(index);
         SetWindowText(val);
         return(m_curSel);
      }
   }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUrlBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUrlBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


class CUrlBar : public CDialogBar  //CToolBar
{
// Attributes
public:

   BOOL m_visible;
   BOOL m_play;

   // set play mode 
   BOOL Play(BOOL mode) {
            if (mode != m_play) {
               m_play = mode;
               if (m_play)
                   StartPlay();
               else StopPlay();
            }
            return(m_play);

   }
   // Play the avi file
   BOOL StartPlay() { 
            if (!m_visible) return(FALSE);       
            return(m_animate.Play( 0, -1, -1)); 
      }
   
   // Stop Play the avi file
   BOOL StopPlay() { return(m_animate.Stop()); }

   // Dialog Data
	
   //{{AFX_DATA(CUrlBar)
	enum { IDD = IDD_URL_BAR };
	CAnimateCtrl	m_animate;
	CUrlBox   m_url;
	//}}AFX_DATA


   // Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CUrlBar)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeUrl();
	afx_msg void OnDropdownUrl();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkUrl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
   BOOL m_saveWindowSize;	// TRUE save / restore mainframe window size

   BOOL m_active; // frame is active
   BOOL m_minimized; // frame is minimized 
   BOOL m_resizing; // frame is resizing 
   BOOL m_fullScreen; // a sub view is fullscreen


// Operations
public:

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;


#endif


/*protected:*/	// control bar embedded members
	CStatusBar	m_wndStatusBar;	// status bar with status line 
	CToolBar	m_wndToolBar;	// the icon tool bar 
	CUrlBar     m_wndUrlBar;	// bar containing URL entry field 

	
	// overwritten
	BOOL PreCreateWindow(CREATESTRUCT& cs); 
   
   BOOL CreateUrlBar();
   
   // add url to combo list box
   int AddUrl(const char *url);



// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnClose();
	afx_msg void OnSelChangeUrl();
	afx_msg void OnViewUrlBar();
	afx_msg void OnUpdateViewUrlBar(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolsLibrary();
	afx_msg void OnUpdateToolsLibrary(CCmdUI* pCmdUI);
	//}}AFX_MSG

	// WM_MOVING
	afx_msg LRESULT OnMoving(WPARAM wParam, LPARAM lParam);

	//WM_SIZING
	afx_msg LRESULT OnSizing(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CThanksTo dialog

class CThanksTo : public CDialog
{
// Construction
public:
	CThanksTo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThanksTo)
	enum { IDD = IDD_THANKS_TO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThanksTo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThanksTo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

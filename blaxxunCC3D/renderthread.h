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
#if !defined(AFX_RENDERTHREAD_H__84F7746B_0CFB_11D2_AF19_44B101C10000__INCLUDED_)
#define AFX_RENDERTHREAD_H__84F7746B_0CFB_11D2_AF19_44B101C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// renderthread.h : header file
//
class CGLViewCtrlApp; 
class CGLViewCtrlCtrl;
class GView;
class GFile;
class GReporter;
		



/////////////////////////////////////////////////////////////////////////////
// CRenderThread thread

class CRenderThread : public CWinThread
{
	DECLARE_DYNCREATE(CRenderThread)

protected:
public :

	CRenderThread(CGLViewCtrlCtrl* pParent=NULL);         

// Attributes
public:
	// signal handles
	HANDLE m_hEventKill; // in  : notify to kill thread 
	HANDLE m_hEventDead; //	out : thread dead 

	HANDLE m_hEventPause;	// in  : enter pause state
	HANDLE m_hEventPaused;  //  out : entered pause state
	HANDLE m_hEventUnpause;	// in  : leave pause state

	CGLViewCtrlCtrl* m_pParent;		// owner window 
	GView	*view;

	CString m_msg; // static message buffer 

// Operations
public:

	// kill thread and delete from other thread 
	void KillThread();
	// signal thread to enter pause state 
	void PauseThread();
	
	// set signal to leave pause state
	void UnpauseThread();


	void Redraw()
	{
		m_pParent->m_mustRedraw = TRUE;
	}


// Overrides
	void Delete();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CRenderThread();

	// Generated message map functions
	//{{AFX_MSG(CRenderThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERTHREAD_H__84F7746B_0CFB_11D2_AF19_44B101C10000__INCLUDED_)

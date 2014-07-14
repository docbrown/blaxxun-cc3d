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
#if !defined(AFX_GVMEDIATHREAD_H__84F7746C_0CFB_11D2_AF19_44B101C10000__INCLUDED_)
#define AFX_GVMEDIATHREAD_H__84F7746C_0CFB_11D2_AF19_44B101C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// gvmediathread.h : header file
//

class GReporter;


/////////////////////////////////////////////////////////////////////////////
// GvMediaThread thread

enum GvMediaType {
		GV_NONE,
		GV_VRML_FILE,
		GV_IMAGE_FILE,
		GV_GIF_MOVIE_FILE,
};

enum GvMediaStatus {
		GV_INITIALIZING,
		GV_RUNNING,
		GV_READY,

};


#ifdef WIN32

class GvMediaThread : public CWinThread
{
	DECLARE_DYNCREATE(GvMediaThread)

public:
	GvMediaThread(GvMediaType type=GV_NONE,const char *fileName=NULL);           // protected constructor used by dynamic creation

// Attributes
public:
	GvMediaStatus m_status;

	GvMediaType m_type;

	CString m_fileName;
	int		m_fileFmt;
	int		m_flags;

	GReporter *m_reporter;



	HANDLE m_hEventKill; // notify to kill thread 
	HANDLE m_hEventDead; //	out, thread dead 

	int m_ret;
	// target

	GvScene *m_scene;	// the vrml scene node
	image *m_image;
	//....

	void KillThread();




// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GvMediaThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

	void Delete();

// Implementation
public:
	virtual ~GvMediaThread();

	// Generated message map functions
	//{{AFX_MSG(GvMediaThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GVMEDIATHREAD_H__84F7746C_0CFB_11D2_AF19_44B101C10000__INCLUDED_)

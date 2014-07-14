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
// gvmediathread.cpp : implementation file
//

#include "stdafx.h"

#include "grender.h"
#include "image.h"
#include "gutils.h"
#include "glutils.h"
#include "gvscene.h"


#include "greadgv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef WIN32
#include "gvmediathread.h"

/////////////////////////////////////////////////////////////////////////////
// GvMediaThread

IMPLEMENT_DYNCREATE(GvMediaThread, CWinThread)

GvMediaThread::GvMediaThread(GvMediaType type,const char *fileName) : 
			m_type(type),m_fileName(fileName), m_flags(0),
			m_status(GV_INITIALIZING),
			m_reporter(NULL),
			m_ret(-1),m_scene(NULL),m_image(NULL)


{
	m_bAutoDelete = FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);

}

GvMediaThread::~GvMediaThread()
{
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);


}

BOOL GvMediaThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int GvMediaThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(GvMediaThread, CWinThread)
	//{{AFX_MSG_MAP(GvMediaThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GvMediaThread message handlers

void GvMediaThread::Delete()
{
	TRACE("GvMediaThread::Delete()\n");

	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void GvMediaThread::KillThread()
{
	// Note: this function is called in the context of other threads,
	//	not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));

	TRACE("Killing media thread ...\n");

	// allow thread to run at higher priority during kill process
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);
	
	TRACE("Killed media thread \n");

	// now delete CWinThread object since no longer necessary
	delete this;
}



int GvMediaThread::Run() 
{
	m_status = GV_RUNNING;
	TRACE("GvMediaThread::Run(%s)\n",(const char *) m_fileName);

	switch (m_type) {
	case GV_VRML_FILE :
		{
			//m_scene->ref();
			// Read vrml file  into scene
			m_ret =  ReadVrmlFile(m_fileName,m_reporter,m_scene,m_flags);
			m_scene->setRedraw(); // force redraw to 
			//m_scene->unref();
		}
		break;
	default :
		m_ret=-1;
		break;
	}

	TRACE("GvMediaThread::Run End (%s)\n",(const char *) m_fileName);

	m_status = GV_READY;
	return m_ret;
}

#endif

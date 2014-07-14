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
// renderthread.cpp : implementation file
//

#include "stdafx.h"

#include "blaxxunCC3D.h"
#include "blaxxunCC3dCtl.h"
#include "renderthread.h"

#include "mmsystem.h"

#include "GRender.h"
#include "GModel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderThread

IMPLEMENT_DYNCREATE(CRenderThread, CWinThread)

CRenderThread::CRenderThread(CGLViewCtrlCtrl* pParent) : m_pParent(pParent)
{
	TRACE("CRenderThread::CRenderThread()\n");

	m_bAutoDelete = FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hEventPause = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventPaused = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventUnpause = CreateEvent(NULL, TRUE, FALSE, NULL);

	view = m_pParent->view;
	ASSERT(view != NULL);
	view->ref();


}

CRenderThread::~CRenderThread()
{
	TRACE("CRenderThread::~CRenderThread()\n");
	view->unref();

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
	CloseHandle(m_hEventPause);
	CloseHandle(m_hEventPaused);
	CloseHandle(m_hEventUnpause);



}

BOOL CRenderThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	TRACE("CRenderThread::InitInstance()\n");
	return TRUE;
}


void CRenderThread::KillThread()
{
	// Note: this function is called in the context of other threads,
	//	not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));

	// allow thread to run at higher priority during kill process
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}


// signal thread to enter pause state 
void CRenderThread::PauseThread()
{

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventPause));
	// could lock : WaitForSingleObject(m_hEventPaused, INFINITE);
}

// set signal to leave pause state
void CRenderThread::UnpauseThread()
{
	VERIFY(SetEvent(m_hEventUnpause));
}



int CRenderThread::ExitInstance()
{
	TRACE("CRenderThread::ExitInstance()\n");
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

void CRenderThread::Delete()
{
	TRACE("CRenderThread::Delete()\n");
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
	VERIFY(SetEvent(m_hEventPaused));
}


BEGIN_MESSAGE_MAP(CRenderThread, CWinThread)
	//{{AFX_MSG_MAP(CRenderThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderThread message handlers


// check timeSenors
// draw

BOOL CRenderThread::OnIdle(LONG lCount) 
{
	gbool drawIt = m_pParent->m_mustRedraw || view->getRedraw(); 


	if (!drawIt) { // trigger idle function 
		view->Lock();
		m_pParent->OnDoTimer(9999);
		view->Unlock();
		if (!m_pParent->m_mustRedraw) return FALSE;
	}

	m_pParent->m_mustRedraw = FALSE;

	view->Lock();
	#ifdef _D3D
		// reset position on screen, necessary for Direct3D because OnMove not called
		view->OnMove(0,0); 

	#endif
    TRACE("\nRenderOnDraw() %p\n",this);

	gbool bWait = FALSE;

	if (view->updateIsComplex) 
    {
		bWait = TRUE;
		BeginWaitCursor();
	}


	// draw 
	view->DrawScene(m_pParent->m_clientRect);

	// enable texture mapping after first update 
	if (view->firstUpdate) 
    {
	   view->firstUpdate = 0;
 	   if (view->initialTextureMode >0) 
       {
	     if (view->GetTexturing() == 0) 
         {
		     view->SetTexturing(1);
			 Redraw();
		 }
	  }
	}
	if (bWait) EndWaitCursor();

	m_pParent->m_endTime= timeGetTime();	// time last rendering cycle stopped

	if (m_pParent->m_displayFrameRate) {
		m_pParent->m_frames ++;

		if ( m_pParent->m_frames == 10) {
			m_msg.Format("%.2f frames per sec  (%.0d ms) \n",  ( 10000.0f /  (float) (m_pParent->m_endTime-m_pParent->m_startTime)  ), view->lastFrameTime);
			m_pParent->m_frames = 0;
			m_pParent->m_startTime= timeGetTime(); 
			//Message(msg,PROGRESS_MESSAGE);	
	      ::PostMessage(m_pParent->GetSafeHwnd(),WM_STATUSMESSAGE, (WPARAM) (const char *) m_msg, (LPARAM) PROGRESS_MESSAGE) ;
		}

	}
	view->Unlock();
	return TRUE;
}

// the thread's run loop
int CRenderThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// return CWinThread::Run();
	// acquire and dispatch messages until a WM_QUIT message is received.

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	// forever until an kill signal received 
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
	{
		// check if we should enter pause state 
		if (WaitForSingleObject(m_hEventPause, 0) == WAIT_OBJECT_0 ) {
			VERIFY(SetEvent(m_hEventPaused));
			VERIFY(ResetEvent(m_hEventPause));
			HANDLE handles[2];
			handles[0]=m_hEventUnpause;
			handles[1]=m_hEventKill;
			TRACE("Thread Pause .... \n");

			if (WaitForMultipleObjects(2,&handles[0],FALSE,INFINITE) == (WAIT_OBJECT_0)) {
				VERIFY(ResetEvent(m_hEventUnpause));
				TRACE("Thread Pause continue\n");
			}
			VERIFY(ResetEvent(m_hEventPaused));
			continue; 
		}

	TRY {

		// phase1: check to see if we can do idle work
		if (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			// pump message, but quit on WM_QUIT
			if (!PumpMessage()) {
				VERIFY(SetEvent(m_hEventKill));
				break;
				// return ExitInstance();
			}
			lIdleCount = 0;


		} 
		else OnIdle(lIdleCount++);


	}
	CATCH_ALL(e)
	{
		// Note: DELETE_EXCEPTION(e) not required.
		TRACE0("Exception in render thread\n");


	}
	END_CATCH_ALL
	
	} // Loop 
	return 0;

}

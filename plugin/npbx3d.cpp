/* ----   copyright note ----------------------------------------------------

     Copyright (c) 1996 by
     blaxxun interactive, Munich, Germany
     All rights reserved

     This software is furnished under a license  and may be  used and  copied
     only in  accordance of the  terms of such license and with the inclusion
     of the above  copyright note.  This software or any  other copies therof
     may not be provided or otherwise made available to any other person.  No
     title to and ownership of the software is hereby transferred.

     The information in this software is subject to change without notice and
     should not be construed as a commitment by blaxxun interactive.

   ----   copyright note ------------------------------------------------- */

/* ----   file information  -------------------------------------------------

   Project:    Passport Pro
   Subsystem:  PLUGIN DLL
   Component:  
   Description:
   !!! used methods of passport control
   passport.Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
 		           const RECT& rect, CWnd* pParentWnd, UINT nID)
   passport.Destroy() or equivalent
   passport.setPlugin(CNpbx3d * plugin)
   passport.notifyURL(const char * url, const char * file, Reason_t reason);

   Module:     npbx3d.cpp
   Author:     Kristof Nast-Kolb

   Revison:    $Revision $

   Date:       $Date $

   Modification History:

  $Log $
   ----   file information  ---------------------------------------------- */

/*
	27.10.97 HG removed rect left, top because already relative to NS-Window
*/

#include "stdafx.h"
#include "npbx3d.h"
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* NP-SDK Info :
http://home.netscape.com/eng/mozilla/3.0/handbook/plugins/pguide.htm
*/

// CNpbx3d construction

// used to register this object instance in the window property
LPSTR CNpbx3d::_ThisLookUp = "this ptr";
static const int CHECK_TIMER = 200;
static const int NO_OF_DOWNLOADS = 10;
static const char * BX3BLANK = "blank.bx3";
#ifdef _TRACE 
static FILE * file = NULL;
#endif
//static CMapStringToString  cacheIndex;


StreamEntry::StreamEntry(const char * urlp, const char * targetp)
{
    type    = StreamNew;
    stream  = NULL;
    url     = urlp;
    target  = targetp;
    cache   = "";
    mimeType= "";
    modified = size = 0;
}

StreamEntry::StreamEntry()
{
    type    = StreamNew;
    stream  = NULL;
    url     = "";
    target  = "";
    cache   = "";
    mimeType= "";
    modified = size = 0;
}

StreamEntry::StreamEntry(StreamEntry * pStream) // copy for cache
{
    copy(pStream);
}

void StreamEntry::copy(StreamEntry * pStream )
{
    type    = StreamLoaded;
    stream  = NULL;
    url     = pStream->url;
    cache   = pStream->cache;
    target  = "";
    mimeType= pStream->mimeType;
    modified= pStream->modified;
    size    = pStream->size;
}

StreamEntry::~StreamEntry()
{
    stream = NULL;
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\.
////\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//.
// PluginWndProc
//
//	static member function of CPluginWindow
//	this is the window proc used to subclass the plugin window the 
//	navigator created and passed in NPP_SetWindow (npshell.c)
//
LRESULT CALLBACK
CNpbx3d::PluginWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (hWnd == 0) return 0;

	// pull out the instance object receiving the message
	CNpbx3d* pluginObj = (CNpbx3d*)GetProp(hWnd, CNpbx3d::_ThisLookUp);

	// message switch
	switch (Msg) {
		
		case WM_COMMAND:
            // TRACE(" WM_COMMAND >> Notify(WPARAM %d, LPARAM %l) called\n", wParam, lParam);
			return 0;
        case WM_BX_LOAD_URL:
            pluginObj->loadUrl((char *)wParam, (char *)lParam, true);
			return 0;
        case WM_BX_LOAD_FILE:
            pluginObj->loadUrl((char *)wParam, (char *)lParam, false);
            pluginObj->CheckSize(hWnd);
			return 0;
        case WM_BX_STATUS_MSG:
            //TRACE(" NETSCAPE PLUGIN >> StatusMsg(%s) called\n", (char *)wParam);
			NPN_Status(pluginObj->m_instance, (char *)wParam);
            //pluginObj->checkStreams(); // just to be sure that we didn't miss a download
            //pluginObj->CheckSize(hWnd);
            return 0;
        case WM_BX_KILL_STREAM:
            pluginObj->killStreams();
			return 0;
		default:
            // TRACE(" NETSCAPE PLUGIN >> default message (0x%x) called\n", Msg);
            pluginObj->checkStreams(); // just to be sure that we didn't miss a download
		    return CallWindowProc(pluginObj->GetWndProc(), hWnd, Msg, wParam, lParam);
            // return 0;
	};
	return 0;
}

//------------------------------------------------------------------------------
//---          TIMER CALLBACK FKT                                            ---
//------------------------------------------------------------------------------

void CALLBACK 
CNpbx3d::PluginTimer(HWND  hwnd, UINT  uMsg, UINT  idEvent, DWORD dwTime)
{
    if (hwnd == 0) return;

	// pull out the instance object receiving the message
	CNpbx3d* pluginObj = (CNpbx3d*)GetProp(hwnd, CNpbx3d::_ThisLookUp);
    if (pluginObj)
	{
		pluginObj->checkStreams(); // just to be sure that we didn't miss a download
		pluginObj->CheckSize(hwnd);
	}
}

// construction with embed parameters
CNpbx3d::CNpbx3d(NPP instance, int16 argc, char* argn[], char* argv[], bool useJava)
{
    m_instance      = instance;
    m_argc          = argc;
    m_cc3d          = NULL;
    m_rect.left     = m_rect.top = m_rect.right = m_rect.bottom = 0;
	// initialized in SetWindow
	_hPluginWnd = 0;
	_pfnDefault = 0;
	m_TimerID   = 0;
    m_streamsChecked= false;
    m_activated = false;
    noOfDownloads   = NO_OF_DOWNLOADS;
    curDownloads    = 0;
    if (useJava)
        m_javaCtrl = new CJavaCtrl(instance);
    else
        m_javaCtrl = NULL;

    m_argn.SetSize( m_argc );
    m_argv.SetSize( m_argc );
    for ( int i = 0; i < m_argc ; i++)
    {
        if ( argn[i] )
            m_argn.Add( argn[i] );
        else
            m_argn.Add( "" );
        if ( argv[i] )
            m_argv.Add( argv[i] );
        else
            m_argv.Add( "" );
    }

    isInitialized   = false;
#ifdef _TRACE 
    if (file == NULL)
    {
        file = ::fopen( "C:\\contact.log", "w");
    }
#endif
}

// destructor
CNpbx3d::~CNpbx3d()
{
    if ( m_TimerID != 0 )
    {
        ::KillTimer(NULL, m_TimerID);
    }
    // restore the old window proc and delete the property
	if (_pfnDefault) {
		::SetWindowLong(_hPluginWnd, GWL_WNDPROC, (LONG)_pfnDefault);
		::RemoveProp(_hPluginWnd, CNpbx3d::_ThisLookUp);
    }

    if (m_cc3d) {
        m_cc3d->setObserverWnd(0);
        m_cc3d->DestroyWindow();
        delete m_cc3d;
        m_cc3d = NULL;
	}
    if (m_javaCtrl)
        delete m_javaCtrl;

    Detach();

    int i;
    for ( i = 0; i < m_streams.GetSize(); i++) delete m_streams.GetAt(i);
    m_streams.RemoveAll();
	
#ifdef CACHE    
    CString key;
    StreamEntry * pStream;
    for( POSITION pos = cacheIndex.GetStartPosition(); pos != NULL; )    
    {
        cacheIndex.GetNextAssoc( pos, key, (CObject*&)pStream ); 
        delete pStream;
    }
	cacheIndex.RemoveAll();
#endif
#ifdef _TRACE 
    if (file)
        ::fclose(file);
#endif
}

// public methods for control

// get embed parameter by name
const char * CNpbx3d::getParam(const char * name)
{
    for (int i = 0; i < m_argc ; i++)
    {
        if (!stricmp(m_argn[i], name))
            return m_argv[i];
    }
    return "";
}


// public methods called by plugin

void CNpbx3d::CheckSize(HWND hWnd)
{
    if (isInitialized) // initialized
    {
        RECT r;

        GetParent()->GetClientRect(&r);
        long x = r.right - r.left;
        long y = r.bottom - r.top;
        if (x != m_rect.right - m_rect.left + marginRect.right + marginRect.left 
            || y != m_rect.bottom - m_rect.top + marginRect.bottom + marginRect.top)
        {
            m_rect.right  = x - marginRect.right - marginRect.left + m_rect.left;
            m_rect.bottom = y - marginRect.bottom - marginRect.top + m_rect.top;
                //TRACE("new size %d %d\n", width, height);
	        m_cc3d->MoveWindow(0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);
//            MoveWindow(m_rect.left, m_rect.top, 
//                m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
        }
    }
}

// set window, create control if window changed
void  CNpbx3d::setWindow(HWND window, uint32 x, uint32 y, uint32 width, uint32 height)
{
    if (!window)
        return;
    //TRACE( "window %d rectangle %ld %ld %ld %ld\n",window, x, y, width, height);

	RECT r;
	::GetClientRect(::GetParent(window), &r);
	marginRect.left		= x - r.left;
	marginRect.top		= y - r.top;
	marginRect.right	= r.right - (x + width);
	marginRect.bottom	= r.bottom - (y + height);
    m_rect.left   = (long) x;
    m_rect.top    = (long) y;
    
	if (m_rect.right != (long)(x+width) || m_rect.bottom != (long)(y+height))
    {
        m_rect.right  = (long) (x + width);
        m_rect.bottom = (long) (y + height);

        //TRACE( "window rectangle %ld %ld %ld %ld\n",m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
        if (isInitialized)
        {
	        m_cc3d->MoveWindow(0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);
        }
//        MoveWindow(m_rect.left, m_rect.top, 
//            m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
    }


    if (!GetWndProc() && window != NULL) 
    {   
		Attach(window); 
	    _hPluginWnd = window;
	    // subclass
	    _pfnDefault = (WNDPROC)::SetWindowLong(window, GWL_WNDPROC, (LONG)CNpbx3d::PluginWndProc);

		// register "this" with the window structure
        ::SetProp(window, CNpbx3d::_ThisLookUp, (HANDLE)this);
        if (m_cc3d == NULL)
        {
		    InitControl();
//            ::RedrawWindow(::GetParent(window), NULL, NULL, RDW_INVALIDATE | RDW_ERASE );
        }
    }

}
 
bool CNpbx3d::InitControl()
{
    RECT    rect;

    ASSERT ( m_cc3d == NULL );
	ASSERT ( _hPluginWnd != 0 );

    rect.left = 0;
    rect.top = 0;
    rect.bottom = m_rect.bottom - m_rect.top;
    rect.right = m_rect.right - m_rect.left;

	AfxEnableControlContainer();

    NPN_Status(m_instance, "Starting blaxxun Contact  ...");

	m_cc3d = new CblaxxunCC3D();

	if ( m_cc3d != NULL && m_cc3d->Create(NULL, WS_VISIBLE, rect, this, 1 ) && m_cc3d->m_hWnd)
	{
        ::SetProp(_hPluginWnd, "CC3D", (HANDLE)m_cc3d);
		// is this ok 
		m_cc3d->setObserverWnd((long)_hPluginWnd);
		//TRACE( " NETSCAPE PLUGIN >> control successfully created.\n");
        NPN_Status(m_instance, "blaxxun Contact initialized ");
        if (m_world.url.GetLength() && m_world.url.Find(BX3BLANK) < 0)
            m_cc3d->loadURLfromFile2(m_world.url, m_world.mimeType, m_world.modified, m_world.size, m_world.cache);

        //m_cc3d->MoveWindow(0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, TRUE);
        //m_cc3d->SetWindowPos( &wndTopMost, 0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, SWP_SHOWWINDOW);
        if (m_javaCtrl)
            m_javaCtrl->setCC3D(m_cc3d);

		// parse parameters 
		for (int i = 0; i < m_argc ; i++)
		{
			const char *argn=m_argn[i];
			const char *argv=m_argv[i];

			if (!argn) continue;
			if (!argv) continue;

			if (strcmp(argn,"VRML-DASHBOARD")==0) {
				if ( stricmp(argv,"false")==0 || stricmp(argv,"0")==0)
					m_cc3d->setNavigationPanel(FALSE);
			} else
			if (strcmp(argn,"FULLSCREEN-MODE")==0) {  // 4.3
				if ( strlen(argv)>0) // no api call
					;
			} else
			if (strcmp(argn,"FULLSCREEN")==0) { 
				if ( stricmp(argv,"true")==0 || stricmp(argv,"1")==0)
					m_cc3d->setFullscreen(TRUE);

			} else
			if (strcmp(argn,"TIMER-INTERVAL")==0) {  // 4.3
				if ( strlen(argv)>0 )
					m_cc3d->setTimerInterval(atoi(argv));
			} else
			if (strcmp(argn,"AVATAR-URL")==0) {  // 4.3
				if ( strlen(argv)>0)
					m_cc3d->setMyAvatarURL(argv);
			} else
			if (strcmp(argn,"AVATAR-DISPLAY")==0) {
				if ( stricmp(argv,"true")==0 || stricmp(argv,"1")==0)
					m_cc3d->setThirdPersonView(TRUE);
			} 
			else {
				TRACE("Unknown embed argument %s = %s \n",argn,argv);
			}
		}


        isInitialized = true;
		m_TimerID = ::SetTimer(_hPluginWnd, 0, CHECK_TIMER, (TIMERPROC) CNpbx3d::PluginTimer ); 
		return true;
	}
    delete m_cc3d;
    m_cc3d = NULL;

    ::MessageBox(NULL,"Error loading blaxxun Contact plugin 3D.","blaxxun Contact plugin 3D", MB_ICONERROR|MB_SYSTEMMODAL);
    //NPN_Status(m_instance, "Error loading blaxxun CC3D.");
	return false;
}

// notify after loading of an url is finished/interupted                              
void CNpbx3d::notifyURL(NPStream *stream, const char * url, const char * cfile, Reason_t reason)
{
    StreamEntry *pStream = NULL;
    bool isTopFile;

    if (!stream) // invalid stream
    {
        return;
    }

    if (stream->notifyData)
    {
        isTopFile = false;
        pStream = (StreamEntry *)stream->notifyData;
        if (!isValidStream(pStream)) // shouldn't happen
        {
            pStream = new StreamEntry(url, "");
            m_streams.Add(pStream);
        }
        pStream->cache    = cfile;
    }
    else // no load request from cc3d
    {
    	CString urls  = url;
        isTopFile = true;
        pStream = findStream(urls);
        pStream->cache    = cfile;

        int i;
        if (urls.Find("file") == 0)
        {
            char * subst;
            if ( (i = urls.Find("%7C")) > 0)
            {
		        pStream->url = urls.Left(i) + ":" + urls.Mid(i+3);
            }
            if (urls.Find("////") > 0 && (subst = strstr(cfile, "\\\\\\")))
            {
                subst++;
                pStream->cache = subst;
            }
        }
    }
//    if (m_world.type != StreamLoaded)
//    {
//        isTopFile = true;
//    }

    pStream->modified = stream->lastmodified;
    pStream->size     = stream->end;

#ifdef _TRACE 
    if (file)
    {
        CString s = ctime (&pStream->modified);
        CString trace;
        trace.Format("to_cc3d: %s, cache=<%s>, modtime=<%s>,code=<%d>\n", url, pStream->cache, s, reason);
        ::fwrite(trace, sizeof (char ), trace.GetLength(), file);
        ::fflush(file);
    }
#endif

    /*
    if (reason == LoadOK)
    {
        struct _stat stat;   
        if (_stat( cache, &stat ) == 0)
        {
            if ((unsigned int)stat.st_size < stream->end)
            {
#ifdef _TRACE 
                if (file)
                {
                    CString trace;
                    trace.Format("wrong size of cache file <%s> for %s\nsize <%d> instead of <%d>", cache, urls, stat.st_size, stream->end);
                    fwrite(trace, sizeof (char ), trace.GetLength(), file);
                }
#endif
                DeleteFile(cache); // delete it and try to reload it
                if (pStream)
                    pStream->type = StreamNew;
                curDownloads--;
                return;
            }
        }
        else
        {
#ifdef _TRACE 
            if (file)
            {
                CString trace;
                trace.Format("missing cache file <%s> for %s\n", cache, urls);
                fwrite(trace, sizeof (char ), trace.GetLength(), file);
            }
#endif
        }
    }
    */

    if (isTopFile) // loadurlfrom file, its the first top level file
    {
        if (m_world.type != StreamLoaded)
        {
            m_world.copy(pStream);
        }
		if (m_cc3d && m_cc3d->m_hWnd)
        {
            if (reason == LoadOK)
            {
		        if (m_world.url.Find(BX3BLANK) < 0) // ignore blank.bx3
                    m_cc3d->loadURLfromFile2(pStream->url, pStream->mimeType, pStream->modified, pStream->size, pStream->cache);
            }
            else
                m_cc3d->onUrlNotify2(pStream->url, pStream->mimeType, pStream->modified, pStream->size, pStream->cache,reason);
        }
    }
    else // loadrequest from CC3D
    {
		if (m_cc3d && m_cc3d->m_hWnd)
            m_cc3d->onUrlNotify2(pStream->url, pStream->mimeType, pStream->modified, pStream->size, pStream->cache,reason);
    }
    
    if (reason == UserBreak)
	{
	    for ( int i = 0 ; i < m_streams.GetSize() ; i++)
	    {
            ((StreamEntry *)m_streams[i])->type = StreamDeleted;
        }
		checkStreams();
	}
#ifdef CACHE    
    else if (reason == LoadOK && pStream && pStream->modified) // insert into own cache index, ignore cgi (modified=0)
		cacheIndex.SetAt(urls, new StreamEntry(pStream));
#endif

    curDownloads--;
    pStream->type = StreamDeleted;
    checkStreams();
}

uint16    CNpbx3d::newStream(NPStream    *stream, NPMIMEType mimeType)
{
    StreamEntry *pStream = (StreamEntry *)stream->notifyData;

    if (pStream)
    {
        if (!isValidStream(pStream)) // shouldn't happen
        {
            return NP_NORMAL;
        }

        pStream->stream    = stream;
        pStream->mimeType  = mimeType;
        if (pStream->type == StreamRequestStream)
        {
            pStream->type = StreamStream;
            return NP_NORMAL;
        }
        else
        {
            pStream->type = StreamFile;
            return NP_ASFILEONLY;
        }
    }

    // new stream so add it, normally world
    StreamEntry *s = new StreamEntry(stream->url, "");
    s->stream   = stream;
    s->type     = StreamFile;
    s->mimeType = mimeType;
    m_streams.Add(s);
    curDownloads++;
    return NP_ASFILEONLY;
}

void    CNpbx3d::writeStream(NPStream    *stream, void *buffer, int32 len)
{
    StreamEntry *pStream = (StreamEntry *)stream->notifyData;


    if (pStream)
    {
        //!!! read streaming unused right now by cc3d
        if (!isValidStream(pStream)) // shouldn't happen
        {
            return;
        }
        if (pStream->type == StreamStream && m_cc3d && m_cc3d->m_hWnd)
        {
            //!!! receive stream buffer!!! m_cc3d->onUrlNotify(((StreamEntry *)m_streams[i])->url,m_streams[i]->mimeType,*buffer,LoadStream);
        }
    }
}

void    CNpbx3d::destroyStream(NPStream    *stream,NPError reason)
{
	//TRACE("Destroy stream %s %d\n",stream->url,reason);
	//if (reason >0) {
	//}

    StreamEntry *pStream = (StreamEntry *)stream->notifyData;

    if (!pStream || !isValidStream(pStream)) // shouldn't happen
    {
        return;
    }

    if (pStream && pStream->type == StreamStream)
    {
		if (m_cc3d && m_cc3d->m_hWnd)
        {
	        if (reason >0) 
            {
                m_cc3d->onUrlNotify(pStream->url,pStream->mimeType,"",NetworkError);
	        }
            else
            {
                m_cc3d->onUrlNotify2(pStream->url,pStream->mimeType,pStream->modified, pStream->size, "",LoadOK);
	        }
        }
        pStream->type = StreamDeleted;
        curDownloads--;
        checkStreams();
        return;
    }

    if (reason == UserBreak)
	{
	    for ( int i = 0 ; i < m_streams.GetSize() ; i++)
	    {
            ((StreamEntry *)m_streams[i])->type = StreamDeleted;
        }
		checkStreams();
	}
}

void CNpbx3d::checkStreams()
{
    StreamEntry *pStream;
    NPError err;

    if (m_streamsChecked)
        return;
    m_streamsChecked = true;

	for ( int i = 0 ; i < m_streams.GetSize() ; i++)
	{
        pStream = ((StreamEntry *)m_streams[i]);
        if (pStream->target.GetLength() && pStream->type == StreamNew)
        {
		    if (NPN_GetURL(m_instance, pStream->url.GetBuffer(0), pStream->target) != NPERR_NO_ERROR)
            {
                //::ShellExecute(NULL, "open", (LPCTSTR)pStream->url, NULL, NULL, SW_SHOW );                        pStream->url = pStream->url.Left(pStream->url.Find('?'));
                pStream->type = StreamErr; // remember to try once again
            }
#ifdef _TRACE 
            if (file)
            {
                time_t t = time(NULL);
                CString trace;
                trace.Format("to_netscape<%s> at <%d>\n", pStream->url, t);
                ::fwrite(trace, sizeof (char ), trace.GetLength(), file);
                ::fflush(file);
            }
#endif
            pStream->type = StreamDeleted; // forget it
        }
        else if ((pStream->type == StreamNew || pStream->type == StreamNewStream)
            && curDownloads < noOfDownloads )
        {
            curDownloads++;
            if (pStream->type == StreamNew)
                pStream->type = StreamRequest;
            else
                pStream->type = StreamRequestStream;
#ifdef _TRACE 
            if (file)
            {
                CString trace = "to_netscape: " + pStream->url + "\n";
                ::fwrite(trace, sizeof (char ), trace.GetLength(), file);
                ::fflush(file);
            }
#endif
		    if (NPN_GetURLNotify(m_instance, pStream->url.GetBuffer(0), NULL, pStream) != NPERR_NO_ERROR)
                pStream->type = StreamErr; // remember to try once again
        }
        else if (pStream->type == StreamErr) // now try again
        {
            if (pStream->target.GetLength())
            {
		        NPN_GetURL(m_instance, pStream->url.GetBuffer(0), pStream->target);
                pStream->type = StreamDeleted; // forget it
            }
            else
            {
		        err = NPN_GetURLNotify(m_instance, pStream->url.GetBuffer(0), NULL, pStream);
                if (err == NPERR_NO_ERROR)
                    pStream->type = StreamRequest;
                else // forget it
                {
			        m_cc3d->onUrlNotify(pStream->url,"", "", NetworkError);
                    pStream->type = StreamDeleted; // forget it
                }
            }
        }
        else if (pStream->type == StreamLoaded) // inform CC3D
        {
		    if (m_cc3d && m_cc3d->m_hWnd)
			    m_cc3d->onUrlNotify2(pStream->url,pStream->mimeType,pStream->modified, pStream->size, pStream->cache, LoadOK);
        }
        else if (pStream->type == StreamDeleted) 
        {
            delete pStream;
            m_streams.RemoveAt(i);
            i--;
        }
	}
    m_streamsChecked = false;
}

void    CNpbx3d::killStreams()
{

    int i;
	if (m_streamsChecked || m_streams.GetSize() == 1) // ignore kill for the first stream
		return;
    m_streamsChecked = true;
    for ( i = 0; i < m_streams.GetSize(); i++) 
    {
        if ( ((StreamEntry *)m_streams[i])->stream && (((StreamEntry *)m_streams[i])->type == StreamFile || ((StreamEntry *)m_streams[i])->type == StreamStream) )
            NPN_DestroyStream(m_instance, ((StreamEntry *)m_streams[i])->stream, NPERR_NO_ERROR);
        ((StreamEntry *)m_streams[i])->type = StreamDeleted;
    }
    m_streamsChecked = false;
    checkStreams();
}

void    CNpbx3d::loadUrl(const char * urls, const char * target, bool loadAnchor) 
{
    if (!m_activated && !IsIconic() && !IsWindowVisible())
    //if (!m_activated && !m_cc3d->IsIconic() && !m_cc3d->IsWindowVisible())
    {
        RECT r;
        GetParent()->GetWindowRect(&r);
        m_activated = true;
	    GetParent()->SetWindowPos(NULL, 0, 0, r.right - r.left - 1, r.bottom - r.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER );
	    GetParent()->SetWindowPos(NULL, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER );
        //GetParent()->Invalidate();
    }

    CString url = urls;
    StreamEntry *pStream;

    if (url.Find(':') == 1)
    {
        url = "file:///" + url;
    }
    if (url.Find(':') == 1)
    {
        url = "file:///" + url;
    }
    if (url.Find("javascript:") == 0)
    {
        if (target && strlen(target) > 0)
            NPN_GetURL(m_instance, url, target);
        else
            NPN_GetURL(m_instance, url, "_self");
        return;
    }
    if (!loadAnchor)
    {
        pStream = new StreamEntry(url, "");
        if (target && !strncmp(target, "stream", 5))
            pStream->type = StreamNewStream;
    }
    else if (target && strlen(target) > 0)
    {
        pStream = new StreamEntry(url, target);
    }
    else
    {
        pStream = new StreamEntry(url, "_self");
    }

#ifdef CACHE    
    StreamEntry * cache;
	if ( cacheIndex.Lookup( url, ( CObject*& ) cache ) )
	{
		FILE * f;
		if ((f = fopen( cache->cache, "r")) != NULL)
		{
			fclose(f);
			pStream->copy(cache);
		}
	}
#endif

#ifdef _TRACE 
    if (file)
    {
        time_t t = time(NULL);
        CString trace;
        trace.Format("cc3drequest<%s> at <%d>\n", url, t);
        ::fwrite(trace, sizeof (char ), trace.GetLength(), file);
        ::fflush(file);
    }
#endif
    m_streams.Add(pStream);
}

bool   CNpbx3d::isValidStream( StreamEntry * stream )
{
    int i;
	for ( i = 0 ; i < m_streams.GetSize() ; i++) // check whether stream still there
	{
        if (stream == ((StreamEntry *)m_streams[i]))
            return true;
    }
    return false;
}

StreamEntry *   CNpbx3d::findStream( CString url )
{
    int i;
	for ( i = 0 ; i < m_streams.GetSize() ; i++) // check whether stream still there
	{
        if (url == ((StreamEntry *)m_streams[i])->url)
            return ((StreamEntry *)m_streams[i]);
    }

    // shouldn't happen, but lets create a new stream
    StreamEntry * s = new StreamEntry(url, "");
    m_streams.Add(s);
    return s;
}

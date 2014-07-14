#include "stdafx.h"
#include "npbxpro.h"

#ifdef _DEBUG 
	#include "tracewin.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNpbxpro::CNpbxpro(NPP instance, int16 argc, char* argn[], char* argv[])
{
	#ifdef _DEBUG 
		CMfxTrace::Init();
	#endif

    this->instance  = instance;
    this->argc      = argc;
    this->argn      = argn;
    this->argv      = argv;
    window          = NULL;
    passport        = NULL;
	mainWnd			= NULL;
    rect.left = rect.top = rect.right = rect.bottom = 0;
}

CNpbxpro::~CNpbxpro()
{
	if (mainWnd) 
	{
		mainWnd->PostMessage(WM_CLOSE);
		mainWnd->Detach();
		delete mainWnd;
		mainWnd = NULL;
	}

	if (passport)
	{
		delete passport;
		passport = NULL;
	}
}

const char * CNpbxpro::getParam(const char * name)
{
    for (int i = 0; i < argc ; i++)
    {
        if (!strcmp(argn[i], name))
            return argv[i];
    }
    return NULL;
}

void CNpbxpro::setWindow(HWND window, uint32 x, uint32 y, uint32 width, uint32 height)
{
    if (rect.left != (long)x || rect.top != (long)y 
        || rect.right != (long)(x + width) || rect.bottom != (long)(y + height))
    {
        rect.left   = (long) x;
        rect.top    = (long) y;
        rect.right  = (long) (x + width);
        rect.bottom = (long) (y + height);
        TRACE(" NETSCAPE PLUGIN >> window rectangle %d %d %d %d\n",rect.left, rect.top, rect.right, rect.bottom);
    }

    if (window != NULL)
    {
        if (this->window != NULL && this->window != window)
        {
            if (passport && mainWnd)
            {
				mainWnd->PostMessage(WM_CLOSE);
            }
            this->window = NULL;
        }

        if (this->window == NULL)
        {
            this->window = window;

            if (!passport)
            {
                passport = new CPassport();
            }
            
			mainWnd = new CWnd();
            mainWnd->Attach(window);

            BOOL rcb = false;
            AfxEnableControlContainer( );

            if ( (rcb=passport->Create("PPro", "Passport Pro browser", WS_VISIBLE, rect, mainWnd, 1 )))
            {
                TRACE(( " NETSCAPE PLUGIN >> control successfully created\n"));
				passport->SetNetscapeServer(getParam("server"));
				passport->SetNetscapePort(getParam("port"));
				passport->SetNetscapeScene(getParam("scene"));
				passport->SetNetscapeOpenSession();
            }
            else
            {
                TRACE(( " NETSCAPE PLUGIN >> control create failed\n"));
            }
        }
    }
}
 
void CNpbxpro::loadURL(const char * url)
{
	TRACE(" NETSCAPE PLUGIN >> CNpbxpro::loadURL(const char * %s)\n", url);
    NPN_GetURLNotify(instance, url, NULL, NULL);
}

void CNpbxpro::notifyURL(const char * url, const char * file, Reason_t reason)
{
    if (passport)
    {
		// passport.SetNetscapeNotifyUrl(const char * url, const char * file, Reason_t reason);
    }
}

BEGIN_MESSAGE_MAP(CNpbxpro, CWinApp)
	ON_MESSAGE( WM_PASSPORT_NOTIFY,	OnNotify )
	ON_MESSAGE( WM_PASSPORT_LOAD_URL, OnLoadUrl )
	ON_MESSAGE( WM_PASSPORT_LOAD_DOC, OnLoadDoc )
	//{{AFX_MSG_MAP(CNpbxpro)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CNpbxpro::OnNotify(WPARAM wParam, LPARAM lParam)
{
    TRACE(" NETSCAPE PLUGIN >> OnNotify(WPARAM %d, LPARAM %l) called\n", wParam, lParam);
	return 0;
}

LRESULT CNpbxpro::OnLoadUrl(WPARAM wParam, LPARAM lParam)
{
    TRACE(" NETSCAPE PLUGIN >> OnLoadUrl(WPARAM %d, LPARAM %l) called\n", wParam, lParam);
	return 0;
}

LRESULT CNpbxpro::OnLoadDoc(WPARAM wParam, LPARAM lParam)
{
    TRACE(" NETSCAPE PLUGIN >> OnLoadDoc(WPARAM %d, LPARAM %l) called\n", wParam, lParam);
	return 0;
}

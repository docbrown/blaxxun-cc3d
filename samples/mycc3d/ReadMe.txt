========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : MyCC3D
========================================================================

blaxxunCC3D Control demo application using  Microsoft Visual C


http://www.blaxxun.com/developer/content/client/cc3d
15/05/08 

How this sample was created :

Step 1:
an MFC - SDI Application was generated using Microsoft Developer Studio

Step 2:
the Active X Control blaxxunCC3D was added to the project
using Dev Studio : Project->Add To Project->Components and Controls-> Registered Active X Controls
blaxxunCC3D
Generated files blaxxuncc3d.*

This step need to be redone if the CC3D interface changes.
(Regenerating these files in some other dummy project and copying the 
files is sufficient)


Step 3:

Control added to Viewclass
blaxxunCC3D added to include directorys paths
Document Open command supported via view->OnIntialUpdate
Document Save command via redirectio to view
overrides for WM_DESTORY and WM_ERASEBACKGROUND
added a Open Url dialog box

Step 4:
COM-Eai wrapper classes in CC3DApi.*
EventOutObserver sample implementation in CC3DHelpers.*

Simple sample code:
void CMyCC3DView::OnAddGeometry() (called from the Edit menu)
Adds a spheres with a TouchSensor
use debugger to see trace output from eventOutObserver callback function





Result:
tiny little VRML browser as standalone application
with file open / save / filename drag & drop
URL Linkss will pop up an HTML Browser.

Required OS DLL'S mfc42.dll msvcrt.dll and Internet Explorer DLL's
(hlink.dll, urlmon.dll ...)



Limitations:
CC3D status bar display not yet rooted to our status bar
new command doesn't clear the CC3D content


-----------------------------------------------------------



AppWizard has created this MyCC3D application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your MyCC3D application.

MyCC3D.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CMyCC3DApp application class.

MyCC3D.cpp
    This is the main application source file that contains the application
    class CMyCC3DApp.

MyCC3D.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Developer Studio.

res\MyCC3D.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file MyCC3D.rc.

res\MyCC3D.rc2
    This file contains resources that are not edited by Microsoft 
	Developer Studio.  You should place all resources not
	editable by the resource editor in this file.

MyCC3D.reg
    This is an example .REG file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .REG
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.

MyCC3D.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the
    CMainFrame class.  Edit this toolbar bitmap along with the
    array in MainFrm.cpp to add more toolbar buttons.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

MyCC3DDoc.h, MyCC3DDoc.cpp - the document
    These files contain your CMyCC3DDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CMyCC3DDoc::Serialize).

MyCC3DView.h, MyCC3DView.cpp - the view of the document
    These files contain your CMyCC3DView class.
    CMyCC3DView objects are used to view CMyCC3DDoc objects.



/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named MyCC3D.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Developer Studio reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC40XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC40DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////

; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyCC3DView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyCC3D.h"
LastPage=0

ClassCount=6
Class1=CMyCC3DApp
Class2=CMyCC3DDoc
Class3=CMyCC3DView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_OPEN_URL
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=DOpenUrl
Resource3=IDR_MAINFRAME

[CLS:CMyCC3DApp]
Type=0
HeaderFile=MyCC3D.h
ImplementationFile=MyCC3D.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CMyCC3DApp

[CLS:CMyCC3DDoc]
Type=0
HeaderFile=MyCC3DDoc.h
ImplementationFile=MyCC3DDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CMyCC3DDoc

[CLS:CMyCC3DView]
Type=0
HeaderFile=MyCC3DView.h
ImplementationFile=MyCC3DView.cpp
Filter=C
LastObject=CMyCC3DView
BaseClass=CView
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_ADD_GEOMETRY



[CLS:CAboutDlg]
Type=0
HeaderFile=MyCC3D.cpp
ImplementationFile=MyCC3D.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_OPEN_URL
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_ADD_GEOMETRY
Command13=ID_VIEW_TOOLBAR
Command14=ID_VIEW_STATUS_BAR
Command15=ID_APP_ABOUT
CommandCount=15

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_OPEN_URL]
Type=1
Class=DOpenUrl
ControlCount=5
Control1=65535,static,1342177283
Control2=65535,static,1342308480
Control3=IDOK,button,1342373889
Control4=IDC_URL,edit,1350631552
Control5=IDCANCEL,button,1342373888

[CLS:DOpenUrl]
Type=0
HeaderFile=DOpenUrl.h
ImplementationFile=DOpenUrl.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC


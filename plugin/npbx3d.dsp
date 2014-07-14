# Microsoft Developer Studio Project File - Name="npbx3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=npbx3d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "npbx3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "npbx3d.mak" CFG="npbx3d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "npbx3d - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "npbx3d - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl5.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "npbx3d - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "c:\CC3d\plugin\release"
# PROP Intermediate_Dir "c:\CC3d\plugin\release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../export" /I "../import/netscape" /I "../nsjava/_gen" /I "../nsjava" /I "../plugin" /I "../../../exports" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink5.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /entry:"CC3DMain" /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "npbx3d - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\CC3d\plugin\Debug"
# PROP Intermediate_Dir "c:\CC3d\plugin\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../export" /I "../import/netscape" /I "../nsjava/_gen" /I "../nsjava" /I "../plugin" /I "../../../exports" /D "DEBUG" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink5.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"CC3DMain" /subsystem:windows /dll /incremental:no /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "npbx3d - Win32 Release"
# Name "npbx3d - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\blaxxuncc3d.cpp
# End Source File
# Begin Source File

SOURCE=..\nsjava\browser.cpp
# End Source File
# Begin Source File

SOURCE=..\nsjava\eventin.cpp
# End Source File
# Begin Source File

SOURCE=..\nsjava\eventout.cpp
# End Source File
# Begin Source File

SOURCE=..\nsjava\javactrl.cpp
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\netscape_plugin_Plugin.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\node.cpp
# End Source File
# Begin Source File

SOURCE=.\npbx3d.cpp
# End Source File
# Begin Source File

SOURCE=.\npbx3d.def
# End Source File
# Begin Source File

SOURCE=.\npshell.cpp
# End Source File
# Begin Source File

SOURCE=.\npwin.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_Browser.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_exception_InvalidEventInException.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_exception_InvalidEventOutException.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_exception_InvalidNodeException.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_exception_InvalidRouteException.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_exception_InvalidVrmlException.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_field_EventIn.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_field_EventOut.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\nsjava\_stubs\vrml_external_Node.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\blaxxuncc3d.h
# End Source File
# Begin Source File

SOURCE=.\blaxxunVRML.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\java_lang_IllegalArgumentException.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\java_lang_Object.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\javactrl.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\netscape_plugin_Plugin.h
# End Source File
# Begin Source File

SOURCE=.\npbx3d.h
# End Source File
# Begin Source File

SOURCE=.\npbx3d_res.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_Browser.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_exception_InvalidEventInException.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_exception_InvalidEventOutException.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_exception_InvalidNodeException.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_exception_InvalidRouteException.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_exception_InvalidVrmlException.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_field_EventIn.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_field_EventOut.h
# End Source File
# Begin Source File

SOURCE=..\nsjava\_gen\vrml_external_Node.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\npbx3d.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section npbx3d : {F20CCB82-EE55-11D0-81D6-444553540000}
# 	2:5:Class:CPassport
# 	2:10:HeaderFile:passport.h
# 	2:8:ImplFile:passport.cpp
# End Section
# Section npbx3d : {F20CCB6D-EE55-11D0-81D6-444553540000}
# 	2:21:DefaultSinkHeaderFile:passport.h
# 	2:16:DefaultSinkClass:CPassport
# End Section
# Section npbx3d : {4B6E3011-6E45-11D0-9309-0020AFE05CC8}
# 	2:5:Class:CGLViewCtrl1
# 	2:10:HeaderFile:glviewctrl.h
# 	2:8:ImplFile:glviewctrl.cpp
# End Section
# Section npbx3d : {4B6E3013-6E45-11D0-9309-0020AFE05CC8}
# 	2:21:DefaultSinkHeaderFile:blaxxuncc3d.h
# 	2:16:DefaultSinkClass:CblaxxunCC3d
# End Section

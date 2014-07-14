# Microsoft Developer Studio Project File - Name="blaxxuncc3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=blaxxuncc3d - Win32 D3D Debug Katmai
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blaxxuncc3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blaxxuncc3d.mak" CFG="blaxxuncc3d - Win32 D3D Debug Katmai"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blaxxuncc3d - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 OGL Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 Win32 Rel Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 D3D Debug Katmai" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 D3D Release Katmai" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blaxxuncc3d - Win32 OGL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "blaxxuncc3d - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\Release"
# PROP Intermediate_Dir "C:\cc3d\Release\obj"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /GX /Zi /Ox /Oi /Os /Op /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /I "navpanel\\" /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "NDEBUG" /D "_D3D" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Og /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "C:\cc3d\Release" /d "NDEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 C:\cc3d\Release\libjpeg.lib C:\cc3d\Release\zlib.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT LINK32 /map /nodefaultlib
# Begin Custom Build - Registering OLE control...
OutDir=C:\cc3d\Release
TargetPath=C:\cc3d\Release\blaxxuncc3d.ocx
InputPath=C:\cc3d\Release\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\cc3d\Debug"
# PROP Intermediate_Dir "C:\cc3d\Debug\obj"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /Gm /GX /Zi /Od /Ob1 /Gf /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /I "navpanel\\" /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_DEBUG" /D "_DEBUG_RAPID" /D "_MBCS" /D "_AFXDLL" /D "_D3D" /D "_NS_MODAL_THREAD" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /D "_DEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "C:\cc3d\Debug" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 uuid.lib C:\cc3d\debug\zlib.lib C:\cc3d\debug\libjpeg.lib wininet.lib ..\dxsdk5\lib\ddraw.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT LINK32 /force
# Begin Custom Build - Registering OLE control...
OutDir=C:\cc3d\Debug
TargetPath=C:\cc3d\Debug\blaxxuncc3d.ocx
InputPath=C:\cc3d\Debug\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\DebugU"
# PROP BASE Intermediate_Dir ".\DebugU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DebugU"
# PROP Intermediate_Dir ".\DebugU"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_DEBUG" /D "_UNICODE" /D "_AFXDLL" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /D "_DEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 uuid.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /debug /machine:I386
# Begin Custom Build - Registering OLE control...
OutDir=.\DebugU
TargetPath=.\DebugU\blaxxuncc3d.ocx
InputPath=.\DebugU\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ReleaseU"
# PROP BASE Intermediate_Dir ".\ReleaseU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ReleaseU"
# PROP Intermediate_Dir ".\ReleaseU"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "NDEBUG" /D "_UNICODE" /D "_AFXDLL" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 uuid.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering OLE control...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\blaxxuncc3d.ocx
InputPath=.\ReleaseU\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\GLViewCt"
# PROP BASE Intermediate_Dir ".\GLViewCt"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\OGLRel"
# PROP Intermediate_Dir "C:\cc3d\OGLRel\obj"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /Op /Oy /Ob1 /Gf /Gy /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_D3D" /D "_GV" /D "_VRML" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /Zi /Ox /Oi /Os /Op /I "..\src\core\gl" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "NDEBUG" /D "_OGL" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Og
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "OGLRel" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"C:\cc3d\Release\blaxxuncc3d.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /map /debug /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 opengl32.lib glu32.lib C:\cc3d\OGLRel\libjpeg.lib  C:\cc3d\OGLRel\zlib.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"C:\cc3d\OGLRel\blaxxunCC3Dgl.ocx"
# SUBTRACT LINK32 /pdb:none /map /nodefaultlib
# Begin Custom Build - Registering OLE control...
OutDir=C:\cc3d\OGLRel
TargetPath=C:\cc3d\OGLRel\blaxxunCC3Dgl.ocx
InputPath=C:\cc3d\OGLRel\blaxxunCC3Dgl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Win32 Rel Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "blaxxunc"
# PROP BASE Intermediate_Dir "blaxxunc"
# PROP BASE Target_Ext "ocx"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\RelS"
# PROP Intermediate_Dir "C:\cc3d\RelS"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /O1 /Op /Oy /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_D3D" /D "_GV" /D "_VRML" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O1 /Op /Oy /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /D "NDEBUG" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# SUBTRACT BASE MTL /nologo
# ADD MTL /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /i "C:\cc3d\Release" /d "NDEBUG" /d "_AFXDLL"
# SUBTRACT BASE RSC /x
# ADD RSC /l 0x409 /i "C:\cc3d\Release" /d "NDEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 C:\cc3d\Release\zlib.lib C:\cc3d\Release\libjpeg.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT BASE LINK32 /map /debug /nodefaultlib
# ADD LINK32 C:\cc3d\Release\zlib.lib C:\cc3d\Release\libjpeg.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /map /machine:I386
# SUBTRACT LINK32 /debug /nodefaultlib
# Begin Custom Build - Registering OLE control...
OutDir=C:\cc3d\RelS
TargetPath=C:\cc3d\RelS\blaxxuncc3d.ocx
InputPath=C:\cc3d\RelS\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Debug Katmai"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "blaxxunc"
# PROP BASE Intermediate_Dir "blaxxunc"
# PROP BASE Target_Ext "ocx"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\cc3d\DebugK"
# PROP Intermediate_Dir "C:\cc3d\DebugK\obj"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /Ob1 /Gf /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "navpanel\\" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_D3D" /D "_GV" /D "_VRML" /D "_GLVIEW_DLL" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Gz /MDd /W3 /Gm /GX /ZI /Od /Ob1 /I "..\intel\include" /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /I "navpanel\\" /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_KATMAI" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /D "_DEBUG" /mktyplib203 /win32
# SUBTRACT BASE MTL /nologo
# ADD MTL /D "_DEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /i "C:\cc3d\Debug" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "C:\cc3d\Debug" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib C:\cc3d\debug\zlib.lib C:\cc3d\debug\libjpeg.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT BASE LINK32 /force
# ADD LINK32 uuid.lib C:\cc3d\debugk\zlib.lib C:\cc3d\debugk\libjpeg.lib wininet.lib ..\dxsdk5\lib\ddraw.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386
# SUBTRACT LINK32 /force
# Begin Custom Build - Registering OLE control...
OutDir=C:\cc3d\DebugK
TargetPath=C:\cc3d\DebugK\blaxxuncc3d.ocx
InputPath=C:\cc3d\DebugK\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Release Katmai"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "blaxxun0"
# PROP BASE Intermediate_Dir "blaxxun0"
# PROP BASE Target_Ext "ocx"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\RelK"
# PROP Intermediate_Dir "C:\cc3d\RelK\obj"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /O1 /Op /Oy /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "navpanel\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_D3D" /D "_GV" /D "_VRML" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /Zi /Ox /Oi /Os /Op /Gf /Gy /I "..\intel\include" /I "..\src\core\d3d" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /I "navpanel\\" /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_KATMAI" /D "NDEBUG" /D "_D3D" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# SUBTRACT BASE MTL /nologo
# ADD MTL /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x409 /i "C:\cc3d\Release" /d "NDEBUG" /d "_AFXDLL"
# SUBTRACT BASE RSC /x
# ADD RSC /l 0x409 /i "C:\cc3d\Release" /d "NDEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 C:\cc3d\Release\zlib.lib C:\cc3d\Release\libjpeg.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /debug /machine:I386
# SUBTRACT BASE LINK32 /map /nodefaultlib
# ADD LINK32 C:\cc3d\Relk\libjpeg.lib C:\cc3d\Relk\zlib.lib ..\dxsdk5\lib\ddraw.lib wininet.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /profile /map /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib
# Begin Custom Build - Registering OLE control...
OutDir=C:\cc3d\RelK
TargetPath=C:\cc3d\RelK\blaxxuncc3d.ocx
InputPath=C:\cc3d\RelK\blaxxuncc3d.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "blaxxunc"
# PROP BASE Intermediate_Dir "blaxxunc"
# PROP BASE Target_Ext "ocx"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "c:\cc3d\OGLDebug"
# PROP Intermediate_Dir "c:\cc3d\OGLDebug\obj"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /MD /W3 /GX /O1 /Op /Oy /I "..\src\core\gl" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_OGL" /D "_GV" /D "_VRML" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /GX /Zi /Od /Op /Oy /I "..\src\core\gl" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_DEBUG" /D "_OGL" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_GLVIEW_DLL" /D "_WINDLL" /D "_NS_MODAL_THREAD" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT BASE MTL /nologo
# ADD MTL /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "OGLRel" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "OGLDebug" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"C:\cc3d\Release\blaxxuncc3d.bsc"
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib C:\cc3d\Release\zlib.lib C:\cc3d\release\libjpeg.lib wininet.lib ..\dxsdk5\lib\ddraw.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /pdb:"C:\cc3d\Release\blaxxuncc3dgl.pdb" /machine:I386 /out:"C:\cc3d\OGLRel\blaxxunCC3Dgl.ocx"
# SUBTRACT BASE LINK32 /pdb:none /map /debug /nodefaultlib
# ADD LINK32 opengl32.lib glu32.lib c:\cc3d\OGLDebug\zlib.lib c:\cc3d\OGLDebug\libjpeg.lib wininet.lib ..\dxsdk5\lib\ddraw.lib ..\dxsdk5\lib\dsound.lib ../dxsdk5/lib/dxguid.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /out:"C:\cc3d\OGLDebug\blaxxunCC3Dgl.ocx"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Registering OLE control...
OutDir=c:\cc3d\OGLDebug
TargetPath=C:\cc3d\OGLDebug\blaxxunCC3Dgl.ocx
InputPath=C:\cc3d\OGLDebug\blaxxunCC3Dgl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "blaxxuncc3d - Win32 Release"
# Name "blaxxuncc3d - Win32 Debug"
# Name "blaxxuncc3d - Win32 Unicode Debug"
# Name "blaxxuncc3d - Win32 Unicode Release"
# Name "blaxxuncc3d - Win32 OGL Release"
# Name "blaxxuncc3d - Win32 Win32 Rel Static"
# Name "blaxxuncc3d - Win32 D3D Debug Katmai"
# Name "blaxxuncc3d - Win32 D3D Release Katmai"
# Name "blaxxuncc3d - Win32 OGL Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\src\core\ARRAY.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\ASTREAM.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\build.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\collide.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gl\copengl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\dsutil3d.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\G3Render.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\G3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GBuildShell.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GCAMERA.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\GCameraUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\Gclass.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\GD3Device.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GDisplayTree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gdssound.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GFindNode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GFont.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gfuzzy.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GGEOM.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\ghull.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GifRead.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GioTraversal.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gkatmai.cpp

!IF  "$(CFG)" == "blaxxuncc3d - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Win32 Rel Static"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Debug Katmai"

# ADD CPP /O2 /Ob2 /D "_USE_INTEL_COMPILER" -Qoption,c,-ip_inline_max_total_stats=20000
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Release Katmai"

# ADD CPP /Zi /O2 /Ob2 /D "_USE_INTEL_COMPILER" /FR -Qoption,c,-ip_inline_max_total_stats=20000
# SUBTRACT CPP /nologo /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Debug"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\core\gl\GLRENDER.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\GLUTILS.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\Gmodel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GMorph.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gnurbs.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gnurbsutil.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GOptimizeTraversal.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gperlinnoise.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GRaySelect.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GReadGv.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GReporter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\grsxsound.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GShell.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GShellI.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GSHELLIO.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\GSHELLOP.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\gshelltriang.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GSound.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GTrace.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GTransparency.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GUnzip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gurlcache.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\GUTILS.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\GZReadStream.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\IDither.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\overlap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\Point.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\PointA.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\Quaternation.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\rapid.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\src\core\RT.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\scan.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\TKDIB.CPP
# End Source File
# Begin Source File

SOURCE=..\src\core\tkrgb.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\tomesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\Triangulate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vbplist.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsexpr.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsfields.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsfunction.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsparse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsstatement.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\wavread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\WWW.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\src\core\Array.h
# End Source File
# Begin Source File

SOURCE=..\src\core\ArrayNew.h
# End Source File
# Begin Source File

SOURCE=..\src\core\astream.h
# End Source File
# Begin Source File

SOURCE=..\src\core\colors.inc
# End Source File
# Begin Source File

SOURCE=..\src\core\gl\COpenGL.h
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\d3dmacs.h
# End Source File
# Begin Source File

SOURCE=..\src\jpeghgnt\dibio.h
# End Source File
# Begin Source File

SOURCE=..\src\jpeghgnt\dibio.pro
# End Source File
# Begin Source File

SOURCE=.\dispids.h
# End Source File
# Begin Source File

SOURCE=..\src\core\dsutil3d.h
# End Source File
# Begin Source File

SOURCE=..\src\core\media\rma\exvsurf.h
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\G3Render.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GBuildShell.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GCamera.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GCameraUtil.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gcaps.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GClass.h
# End Source File
# Begin Source File

SOURCE=..\src\core\d3d\GD3Device.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GDEFORM.H
# End Source File
# Begin Source File

SOURCE=..\src\core\GDisplayTree.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gdssound.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GEvent.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GFindNode.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GFont.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gfuzzy.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GGeom.h
# End Source File
# Begin Source File

SOURCE=..\src\core\ghull.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GifRead.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GIoTraversal.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gkatmai.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gl\glrender.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GLUtils.h
# End Source File
# Begin Source File

SOURCE=.\glviewvrml.h
# End Source File
# Begin Source File

SOURCE=..\src\glviewvrml.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gmidi.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GModel.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gnoisetable.i
# End Source File
# Begin Source File

SOURCE=..\src\core\gnurbs.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GOptimizeTraversal.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gpanel.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gperlinnoise.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GRaySelect.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GReadGv.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GRender.h
# End Source File
# Begin Source File

SOURCE=..\src\core\grenderbase.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GReporter.h
# End Source File
# Begin Source File

SOURCE=..\src\core\media\rma\grmaplay.h
# End Source File
# Begin Source File

SOURCE=..\src\core\grsxsound.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GShell.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GShellI.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GShellIO.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GShellOp.h
# End Source File
# Begin Source File

SOURCE=.\gshelltriang.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gshelltriang.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GSound.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GSurface.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GTrace.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GTransparency.h
# End Source File
# Begin Source File

SOURCE=..\src\core\GUnzip.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gurlcache.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gutils.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gzreadstream.h
# End Source File
# Begin Source File

SOURCE=..\src\core\IDither.h
# End Source File
# Begin Source File

SOURCE=..\src\jpeghgnt\image.h
# End Source File
# Begin Source File

SOURCE=..\src\jpeghgnt\JPEGREAD.H
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\jsmath.h
# End Source File
# Begin Source File

SOURCE=..\src\core\MallocDbg.h
# End Source File
# Begin Source File

SOURCE=..\src\core\Matrix.h
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\matvec.h
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\moments.h
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\obb.h
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\overlap.h
# End Source File
# Begin Source File

SOURCE=..\src\jpeghgnt\png_read.h
# End Source File
# Begin Source File

SOURCE=..\src\core\Point.h
# End Source File
# Begin Source File

SOURCE=..\src\core\pointa.h
# End Source File
# Begin Source File

SOURCE=..\src\core\Quaternation.h
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\rapid.h
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\rapid_version.h
# End Source File
# Begin Source File

SOURCE=..\src\core\RT.h
# End Source File
# Begin Source File

SOURCE=..\src\core\RTABST.H
# End Source File
# Begin Source File

SOURCE=..\src\core\Stack.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\system.h
# End Source File
# Begin Source File

SOURCE=..\src\jpeghgnt\TARGAIO.H
# End Source File
# Begin Source File

SOURCE=..\src\core\tomesh.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vbbasic.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vbplist.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsexpr.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsfields.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsfunction.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsparse.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\vsstatement.h
# End Source File
# Begin Source File

SOURCE=..\src\core\www.h
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\y_tab.h
# End Source File
# Begin Source File

SOURCE=..\src\core\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\src\core\zlib\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\blaxxuncc3d.ico
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3dCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\ccexa.cur
# End Source File
# Begin Source File

SOURCE=.\ccexas.cur
# End Source File
# Begin Source File

SOURCE=.\ccfly.cur
# End Source File
# Begin Source File

SOURCE=.\ccflys.cur
# End Source File
# Begin Source File

SOURCE=.\ccpan.cur
# End Source File
# Begin Source File

SOURCE=.\ccpans.cur
# End Source File
# Begin Source File

SOURCE=.\ccrot.cur
# End Source File
# Begin Source File

SOURCE=.\ccrots.cur
# End Source File
# Begin Source File

SOURCE=.\ccwalks.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\cursor11.cur
# End Source File
# Begin Source File

SOURCE=.\cursor12.cur
# End Source File
# Begin Source File

SOURCE=.\cursor13.cur
# End Source File
# Begin Source File

SOURCE=.\cursor14.cur
# End Source File
# Begin Source File

SOURCE=.\cursor15.cur
# End Source File
# Begin Source File

SOURCE=.\cursor16.cur
# End Source File
# Begin Source File

SOURCE=.\cursor17.cur
# End Source File
# Begin Source File

SOURCE=.\cursor18.cur
# End Source File
# Begin Source File

SOURCE=.\cursor19.cur
# End Source File
# Begin Source File

SOURCE=.\cursor20.cur
# End Source File
# Begin Source File

SOURCE=.\cursor21.cur
# End Source File
# Begin Source File

SOURCE=.\cursor22.cur
# End Source File
# Begin Source File

SOURCE=.\cursor23.cur
# End Source File
# Begin Source File

SOURCE=.\navpanel\pixs\over\cursor23.cur
# End Source File
# Begin Source File

SOURCE=.\cursor24.cur
# End Source File
# Begin Source File

SOURCE=.\cursor25.cur
# End Source File
# Begin Source File

SOURCE=.\navpanel\pixs\grau\cursor25.cur
# End Source File
# Begin Source File

SOURCE=.\ifdown.bmp
# End Source File
# Begin Source File

SOURCE=.\navpanel\pixs\down\pfd1.bmp
# End Source File
# Begin Source File

SOURCE=.\navpanel\pixs\over\pfo1.bmp
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "gul"

# PROP Default_Filter ""
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\core\gul\common\guar_bincoeff.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guar_bincoeff.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guar_exact.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guar_exact.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guge_intersect.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guge_intersect.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guge_linear.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guge_normalize.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_basics.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_basics.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_io.cpp

!IF  "$(CFG)" == "blaxxuncc3d - Win32 Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Win32 Rel Static"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Debug Katmai"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Release Katmai"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_io.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_regularize.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_regularize.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_split.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_split.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_triangulate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gugr_triangulate.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gul_error.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gul_io.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gul_matrix.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gul_types.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gul_vector.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guma_sorting.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\guma_sorting.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_basics.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_basics.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_derivatives.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_derivatives.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_divide.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_divide.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_intersect.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_intersect.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_linearize.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_linearize.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_tesselate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_tesselate.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gunu_tesselate1.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gust_new.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gust_pool.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gust_pool.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gul\common\gust_pooltbl.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "vrml"

# PROP Default_Filter ""
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gv2Geometry.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gv2Group.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvAsciiText.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\include\gvavatar.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvBaseColor.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvBASIC.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvBlinker.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvBsp.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvCallback.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvCHILDLIST.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvComplexity.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvCONE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvConnection.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvCOORDINATE3.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvCUBE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvCYLINDER.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvDB.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvDEBUGERROR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvDICT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvDIRECTIONALLIGHT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvEdit.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvELEMENT.H
# End Source File
# Begin Source File

SOURCE=..\src\core\GVersion.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvExtrusion.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvfield.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvfielddata.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvFIELDS.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvFontStyle.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvGROUP.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\include\gvhandle.h
# End Source File
# Begin Source File

SOURCE=..\projects\willamette\gvheadmodel.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gviewcontrol.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvINDEXEDFACESET.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvINDEXEDLINESET.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvIndexedTriangleStripSet.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvINFO.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvINPUT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvInterpolators.h
# End Source File
# Begin Source File

SOURCE=..\src\core\gvisibility.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvKeyword.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvKtx.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvLabel.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\include\gvlayer.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvLEVELOFDETAIL.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvLISTS.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvLOD.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvmaterial.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMATERIALBINDING.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMATRIXTRANSFORM.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\include\gvmediathread.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFCOLOR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFFLOAT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFInt32.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFLONG.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFNode.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFRotation.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFString.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\include\gvmftime.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFVEC2F.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMFVEC3F.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvModifier.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvMorph.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvNameSpace.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvnode.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvNODES.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvNodeShell.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvNORMAL.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvNORMALBINDING.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\include\gvnurbs.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvORTHOGRAPHICCAMERA.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvPath.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvPendulum.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvPERSPECTIVECAMERA.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvPLIST.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvPOINTLIGHT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvPOINTSET.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvProto.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvREADERROR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvROTATION.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvROTOR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSCALE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvScene.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvScript.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSensors.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSEPARATOR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFBITMASK.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFBOOL.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFCOLOR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFENUM.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFFLOAT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFIMAGE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFInt32.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFLONG.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFMATRIX.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFNode.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFROTATION.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFSTRING.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFTime.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFVEC2F.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSFVEC3F.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSHAPEHINTS.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvShuttle.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSound.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSPHERE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSPOTLIGHT.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSTATE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSTRING.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSUBFIELD.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvsubnode.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSweep.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvSWITCH.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvtexture2.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\Gvtexture2transform.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTEXTURECOORDINATE2.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTextureCoordinateBinding.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTextureRotor.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTextureShuttle.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTimeTransform.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTransform.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTRANSFORMSEPARATOR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTRANSLATION.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvTraverse.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvUNKNOWNNODE.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvWebFx.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvWWWANCHOR.H
# End Source File
# Begin Source File

SOURCE=..\src\vrml\INCLUDE\GvWWWINLINE.H
# End Source File
# End Group
# Begin Group "src No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\vrml\SRC\Gv2Geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\Gv2Group.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvBaseColor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvBlinker.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvBsp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvCHILDLIST.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvComplexity.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvCOORDINATE3.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvDB.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvDEBUGERROR.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvDICT.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvDIRECTIONALLIGHT.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvElement.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvField.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvFieldData.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvFontStyle.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvGeometry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvhandle.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvInput.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvInterpolators.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gvisibility.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvKeyword.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvKtx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvLEVELOFDETAIL.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvLISTS.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvLOD.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\Gvmaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvMATERIALBINDING.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmediathread.cpp
# ADD CPP /I "..\..\..\blaxxunCC3D"
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvMFields.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvModifier.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvMorph.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvNAME.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvNameSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\Gvnode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvNodeShell.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvNORMAL.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvNORMALBINDING.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnurbs.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnurbsinventor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvORTHOGRAPHICCAMERA.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvPath.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvPERSPECTIVECAMERA.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvPLIST.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvPOINTLIGHT.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvProto.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvREADERROR.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvScene.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvScript.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSensors.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSEPARATOR.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSFields.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSHAPEHINTS.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSound.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSPOTLIGHT.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSTRING.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSweep.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvSWITCH.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\Gvtexture2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvTEXTURECOORDINATE2.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvTextureCoordinateBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvTimeTransform.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvTransforms.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvTRANSFORMSEPARATOR.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\Gvtraverse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvUNKNOWNNODE.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvWebFx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvWWWANCHOR.CPP
# End Source File
# Begin Source File

SOURCE=..\src\vrml\SRC\GvWWWINLINE.CPP
# End Source File
# End Group
# End Group
# End Group
# Begin Group "blaxxuncc3d"

# PROP Default_Filter ""
# Begin Group "header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\blaxxuncc3d.h
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3dCtl.h
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3dInterface.h
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3dPpg.h
# End Source File
# Begin Source File

SOURCE=.\blaxxunVRML.h
# End Source File
# Begin Source File

SOURCE=.\DAbout.h
# End Source File
# Begin Source File

SOURCE=.\DConsole.h
# End Source File
# Begin Source File

SOURCE=..\dxsdk5\dxmedia\include\ddstream.h
# End Source File
# Begin Source File

SOURCE=.\dlgmenuthread.h
# End Source File
# Begin Source File

SOURCE=.\dpad.h
# End Source File
# Begin Source File

SOURCE=.\DProp.h
# End Source File
# Begin Source File

SOURCE=.\DWorldInfo.h
# End Source File
# Begin Source File

SOURCE=.\gconfig.h
# End Source File
# Begin Source File

SOURCE=.\helpers.h
# End Source File
# Begin Source File

SOURCE=.\renderthread.h
# End Source File
# Begin Source File

SOURCE=..\..\RSX2\include\RSX.H
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\stdafx_atl.h
# End Source File
# End Group
# Begin Group "src No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\blaxxuncc3d.cpp
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3d.def
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3d.odl

!IF  "$(CFG)" == "blaxxuncc3d - Win32 Release"

# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Debug"

# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Debug"

# ADD MTL /h ".\GLViewInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Release"

# ADD MTL /h ".\GLViewInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Release"

# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Win32 Rel Static"

# ADD BASE MTL /h ".\blaxxuncc3dInterface.h"
# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Debug Katmai"

# ADD BASE MTL /h ".\blaxxuncc3dInterface.h"
# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Release Katmai"

# ADD BASE MTL /h ".\blaxxuncc3dInterface.h"
# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Debug"

# ADD BASE MTL /h ".\blaxxuncc3dInterface.h"
# ADD MTL /h ".\blaxxuncc3dInterface.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3d.rc

!IF  "$(CFG)" == "blaxxuncc3d - Win32 Release"

# ADD BASE RSC /l 0x409 /i "C:\cc3d\Release"
# SUBTRACT BASE RSC /i "C:\cc3d\Release"
# ADD RSC /l 0x409 /i "C:\cc3d\Release"
# SUBTRACT RSC /i "C:\cc3d\Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Debug"

# ADD BASE RSC /l 0x409 /i "C:\cc3d\Debug"
# SUBTRACT BASE RSC /i "C:\cc3d\Debug"
# ADD RSC /l 0x409 /i "C:\cc3d\Debug"
# SUBTRACT RSC /i "C:\cc3d\Debug"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Release"

# ADD BASE RSC /l 0x409 /i "C:\cc3d\OGLRel"
# SUBTRACT BASE RSC /i "OGLRel"
# ADD RSC /l 0x409 /i "C:\cc3d\OGLRel"
# SUBTRACT RSC /i "OGLRel"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Win32 Rel Static"

# ADD BASE RSC /l 0x409 /i "C:\cc3d\RelS"
# SUBTRACT BASE RSC /i "C:\cc3d\Release"
# ADD RSC /l 0x409 /i "C:\cc3d\RelS"
# SUBTRACT RSC /i "C:\cc3d\Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Debug Katmai"

# ADD BASE RSC /l 0x409 /i "C:\cc3d\DebugK"
# SUBTRACT BASE RSC /i "C:\cc3d\Debug"
# ADD RSC /l 0x409 /i "C:\cc3d\DebugK"
# SUBTRACT RSC /i "C:\cc3d\Debug"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Release Katmai"

# ADD BASE RSC /l 0x409 /i "C:\cc3d\RelK"
# SUBTRACT BASE RSC /i "C:\cc3d\Release"
# ADD RSC /l 0x409 /i "C:\cc3d\RelK"
# SUBTRACT RSC /i "C:\cc3d\Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Debug"

# ADD BASE RSC /l 0x409 /i "c:\cc3d\OGLDebug"
# SUBTRACT BASE RSC /i "OGLDebug"
# ADD RSC /l 0x409 /i "c:\cc3d\OGLDebug"
# SUBTRACT RSC /i "OGLDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3dCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\blaxxuncc3dPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\blaxxunVRML.idl

!IF  "$(CFG)" == "blaxxuncc3d - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MIDL blaxxunVRML.idl
OutDir=C:\cc3d\Release
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /Oicf /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                                 blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MIDL
OutDir=C:\cc3d\Debug
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                      blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=C:\cc3d\OGLRel
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /Oicf /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                                 blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 Win32 Rel Static"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - MIDL blaxxunVRML.idl
OutDir=C:\cc3d\RelS
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /Oicf /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                                 blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Debug Katmai"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MIDL
OutDir=C:\cc3d\DebugK
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                      blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 D3D Release Katmai"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - MIDL blaxxunVRML.idl
OutDir=C:\cc3d\RelK
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /Oicf /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                                 blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "blaxxuncc3d - Win32 OGL Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=c:\cc3d\OGLDebug
InputPath=.\blaxxunVRML.idl

"$(OutDir)\blaxxunVRML.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	midl /Oicf /h blaxxunVRML.h /iid blaxxunVRML_i.c /proxy blaxxunVRML_p.c                                                                                                                                                                                 blaxxunVRML.idl 
	copy blaxxunVRML.tlb $(OutDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\blaxxunVRML_i.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\DConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgmenuthread.cpp
# End Source File
# Begin Source File

SOURCE=.\dpad.cpp
# End Source File
# Begin Source File

SOURCE=.\DProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DWorldInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\gnavigation.cpp
# End Source File
# Begin Source File

SOURCE=.\helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\renderthread.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# End Group
# Begin Group "doc files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Authors
# End Source File
# Begin Source File

SOURCE=..\ChangeLog
# End Source File
# Begin Source File

SOURCE=..\docs\cc3d\contact3d_class_overview.doc
# End Source File
# Begin Source File

SOURCE=..\docs\cc3d\contact3d_overview.doc
# End Source File
# Begin Source File

SOURCE=..\Install
# End Source File
# Begin Source File

SOURCE=..\News
# End Source File
# Begin Source File

SOURCE=..\Readme
# End Source File
# End Group
# End Target
# End Project
# Section blaxxuncc3d : {4B6E3011-6E45-11D0-9309-0020AFE05CC8}
# 	2:5:Class:CblaxxunCC3D
# 	2:10:HeaderFile:blaxxuncc3d1.h
# 	2:8:ImplFile:blaxxuncc3d1.cpp
# End Section
# Section blaxxuncc3d : {4B6E3013-6E45-11D0-9309-0020AFE05CC8}
# 	2:21:DefaultSinkHeaderFile:blaxxuncc3d1.h
# 	2:16:DefaultSinkClass:CblaxxunCC3D
# End Section

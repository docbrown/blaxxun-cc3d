# Microsoft Developer Studio Project File - Name="GLVIEW" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GLVIEW - Win32 D3D Rel Katmai
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "glview.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "glview.mak" CFG="GLVIEW - Win32 D3D Rel Katmai"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLVIEW - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GLVIEW - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "GLVIEW - Win32 D3D Debug" (based on "Win32 (x86) Application")
!MESSAGE "GLVIEW - Win32 D3D Rel" (based on "Win32 (x86) Application")
!MESSAGE "GLVIEW - Win32 D3D Debug Katmai" (based on "Win32 (x86) Application")
!MESSAGE "GLVIEW - Win32 D3D Rel Katmai" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "c:\cc3d\glview\WinRel"
# PROP Intermediate_Dir "c:\cc3d\glview\WinRel\obj"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MT /W3 /GX /Ox /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_X86_" /D "C_OLE1" /D "NT" /D "OGL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /G5 /Gz /MT /W4 /GX /Ox /Oi /Op /I ".\core\gl" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "NDEBUG" /D "OGL" /D "_OGL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "_OGL" /d "NDEBUG" /d "_ABOUT_RELEASE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 nafxcw.lib olecli32.lib olesvr32.lib opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /machine:IX86
# ADD LINK32 opengl32.lib glu32.lib  .\core\zlib\releaseStatic\zlib.lib .\jpeghgnt\releaseStatic\libjpeg.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib urlmon.lib ..\dxsdk5\lib\ddraw.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /machine:IX86
# SUBTRACT LINK32 /profile /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\glview\WinDebug"
# PROP Intermediate_Dir "c:\cc3d\glview\WinDebug\obj"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_X86_" /D "C_OLE1" /D "NT" /D "OGL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /G5 /Gz /MTd /W3 /Gm /GX /Zi /Od /Ob1 /I ".\core\gl" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_DEBUG" /D "_AVI" /D "_HMF" /D "_OGL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Gf
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_ABOUT_RELEASE" /d "_OGL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 olecli32.lib olesvr32.lib opengl32.lib glu32.lib glaux.lib nafxcwd.lib /nologo /subsystem:windows /debug /machine:IX86
# ADD LINK32 opengl32.lib glu32.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib .\core\zlib\debugStatic\zlib.lib .\jpeghgnt\debugStatic\libjpeg.lib urlmon.lib ..\dxsdk5\lib\ddraw.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /profile /debug /machine:IX86
# SUBTRACT LINK32 /nodefaultlib /force

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\GLVIEW__"
# PROP BASE Intermediate_Dir ".\GLVIEW__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\glview\D3DDbg"
# PROP Intermediate_Dir "c:\cc3d\glview\D3DDbg\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G4 /MTd /W3 /Gm /GX /Zi /Od /Ob1 /I "\glview\src\core" /I "\glview\src" /I "\glview\src\core\gl" /I "\glview\src\vrml\include" /I "\glview\src\JPEGHGNT" /D "_DEBUG" /D "_AVI" /D "_HMF" /D "_GV" /D "WIN32" /D "_WINDOWS" /D "OGL" /D "_OGL" /D "_DXF" /D "_VRML" /D "_WWW" /D "_MBCS" /Fr /Yu"stdafx.h" /c
# SUBTRACT BASE CPP /Gf
# ADD CPP /nologo /G5 /Gz /MTd /W3 /Gm /GX /Zi /Od /Ob1 /I ".\core\d3d" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_DEBUG" /D "_AVI" /D "_HMF" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Gf
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_ABOUT_RELEASE"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_ABOUT_RELEASE" /d "_D3D"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib wininet.lib /nologo /subsystem:windows /debug /machine:IX86
# SUBTRACT BASE LINK32 /profile /incremental:no /nodefaultlib /force
# ADD LINK32 ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib .\core\zlib\debugStatic\zlib.lib .\jpeghgnt\debugStatic\libjpeg.lib urlmon.lib ..\dxsdk5\lib\ddraw.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /debug /machine:IX86 /out:"c:\cc3d\glview\D3DDbg/GLViewD3D.exe"
# SUBTRACT LINK32 /profile /map /nodefaultlib /force

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\GLVIEW__"
# PROP BASE Intermediate_Dir ".\GLVIEW__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\glview\D3DRel"
# PROP Intermediate_Dir "c:\cc3d\glview\D3DRel\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /Op /Gf /I "\glview\src" /I "\glview\src\core\d3d" /I "\glview\src\core" /I "\glview\src\vrml\include" /I "\glview\src\vrmlscript" /I "\glview\src\JPEGHGNT" /D "NDEBUG" /D "_EVALUATION" /D "_GV" /D "WIN32" /D "_WINDOWS" /D "_D3D" /D "_DXF" /D "_VRML" /D "_WWW" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G5 /Gz /MT /W3 /GX /Zi /Ox /Oi /Op /I ".\core\d3d" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "NDEBUG" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_ABOUT_DEMO_EMD" /d "_D3D"
# ADD RSC /l 0x409 /d "_D3D" /d "NDEBUG" /d "_ABOUT_RELEASE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ddraw.lib uuid.lib wininet.lib \glview\src\jpeghgnt\libjpeg.lib /nologo /subsystem:windows /incremental:no /machine:IX86
# SUBTRACT BASE LINK32 /profile /map /debug /nodefaultlib /force
# ADD LINK32 .\core\zlib\releaseStatic\zlib.lib .\jpeghgnt\releaseStatic\libjpeg.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib urlmon.lib ..\dxsdk5\lib\ddraw.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:IX86 /out:"c:\cc3d\glview\D3DRel\GLViewD3D.exe"
# SUBTRACT LINK32 /profile /nodefaultlib /force

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GLVIEW__"
# PROP BASE Intermediate_Dir "GLVIEW__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\glview\D3DDbgK"
# PROP Intermediate_Dir "c:\cc3d\glview\D3DDbgK\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /Ob1 /I ".\core\d3d" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /D "_DEBUG" /D "_AVI" /D "_HMF" /D "_D3D" /D "_COM" /D "_GV" /D "WIN32" /D "_WINDOWS" /D "_VRML" /D "_WWW" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Gf
# ADD CPP /nologo /G6 /Gz /MTd /W3 /Gm /GX /Zi /Od /Ob1 /I "..\intel\include" /I ".\core\d3d" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_KATMAI" /D "_DEBUG" /D "_AVI" /D "_HMF" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD -Qoption,c,-ip_inline_max_total_stats=20000 /c
# SUBTRACT CPP /Gf
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_ABOUT_RELEASE" /d "_D3D"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_ABOUT_RELEASE" /d "_D3D"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ..\dxsdk5\lib\ddraw.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib .\core\zlib\debugStatic\zlib.lib .\jpeghgnt\debugStatic\libjpeg.lib ole32.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /debug /machine:IX86 /out:"c:\cc3d\glview\D3DDbg/GLViewD3D.exe"
# SUBTRACT BASE LINK32 /profile /map /nodefaultlib /force
# ADD LINK32 .\core\media\mpg123\mpglib\mpeglib\Debug\mpeglib.lib C:\CC3D\grmaplay\DebugStatic\grmaplay.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib .\core\zlib\debugStatic\zlib.lib .\jpeghgnt\debugStatic\libjpeg.lib urlmon.lib ..\dxsdk5\lib\ddraw.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /incremental:no /debug /machine:IX86 /out:"c:\cc3d\glview\D3DDbgK/GLViewD3D.exe"
# SUBTRACT LINK32 /profile /map /nodefaultlib /force

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GLVIEW__"
# PROP BASE Intermediate_Dir "GLVIEW__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\glview\D3DRelK\"
# PROP Intermediate_Dir "c:\cc3d\glview\D3DRelK\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Gr /MT /W3 /GX /Zi /O2 /Op /Ob2 /I ".\core\d3d" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /D "NDEBUG" /D "_D3D" /D "_COM" /D "_GV" /D "WIN32" /D "_WINDOWS" /D "_VRML" /D "_WWW" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G5 /Gz /MT /W3 /GX /Zi /Ox /Oi /Op /I "..\intel\include" /I ".\core\d3d" /I ".\core" /I ".\jpeghgnt" /I ".\vrml\include" /I ".\vrmlscript" /I "." /I "..\dxsdk5\inc" /I "..\dxsdk5\dxmedia\include" /I "..\import\rma\include" /D "_KATMAI" /D "NDEBUG" /D "_D3D" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD -Qoption,c,-ip_inline_max_total_stats=20000 /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_D3D" /d "NDEBUG" /d "_ABOUT_RELEASE"
# ADD RSC /l 0x409 /d "_D3D" /d "NDEBUG" /d "_ABOUT_RELEASE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uuid.lib ..\dxsdk5\lib\ddraw.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib .\core\zlib\releaseStatic\zlib.lib .\jpeghgnt\releaseStatic\libjpeg.lib ole32.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:IX86 /out:"c:\cc3d\glview\D3DRel\GLViewD3D.exe"
# SUBTRACT BASE LINK32 /profile /nodefaultlib /force
# ADD LINK32 .\core\media\mpg123\mpglib\mpeglib\Debug\mpeglib.lib C:\CC3D\grmaplay\ReleaseStatic\grmaplay.lib .\core\zlib\releaseStatic\zlib.lib .\jpeghgnt\releaseStatic\libjpeg.lib ..\dxsdk5\lib\dsound.lib ..\dxsdk5\lib\dxguid.lib urlmon.lib ..\dxsdk5\lib\ddraw.lib winmm.lib ..\dxsdk5\dxmedia\lib\amstrmid.lib ..\dxsdk5\dxmedia\lib\strmiids.lib /nologo /subsystem:windows /incremental:no /machine:IX86 /out:"c:\cc3d\glview\D3DRelK\GLViewD3D.exe"
# SUBTRACT LINK32 /profile /map /debug /nodefaultlib /force

!ENDIF 

# Begin Target

# Name "GLVIEW - Win32 Release"
# Name "GLVIEW - Win32 Debug"
# Name "GLVIEW - Win32 D3D Debug"
# Name "GLVIEW - Win32 D3D Rel"
# Name "GLVIEW - Win32 D3D Debug Katmai"
# Name "GLVIEW - Win32 D3D Rel Katmai"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\core\ARRAY.CPP
# End Source File
# Begin Source File

SOURCE=.\core\ASTREAM.CPP
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\BUILD.cpp
# End Source File
# Begin Source File

SOURCE=.\CFSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\COLLIDE.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gl\copengl.cpp
# End Source File
# Begin Source File

SOURCE=.\dbackgro.cpp
# End Source File
# Begin Source File

SOURCE=.\dcamera.cpp
# End Source File
# Begin Source File

SOURCE=.\DConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\dd3doptions.cpp
# End Source File
# Begin Source File

SOURCE=.\ddefsph.cpp
# End Source File
# Begin Source File

SOURCE=.\ddrawsty.cpp
# End Source File
# Begin Source File

SOURCE=.\dfog.cpp
# End Source File
# Begin Source File

SOURCE=.\dmateria.cpp
# End Source File
# Begin Source File

SOURCE=.\DMetaVerse.cpp
# End Source File
# Begin Source File

SOURCE=.\dobjnew.cpp
# End Source File
# Begin Source File

SOURCE=.\doptions.cpp
# End Source File
# Begin Source File

SOURCE=.\dprop.cpp
# End Source File
# Begin Source File

SOURCE=.\DRenderFile.cpp
# End Source File
# Begin Source File

SOURCE=.\dstring.cpp
# End Source File
# Begin Source File

SOURCE=.\dstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\dstylep.cpp
# End Source File
# Begin Source File

SOURCE=.\core\dsutil3d.cpp
# End Source File
# Begin Source File

SOURCE=.\dtexture.cpp
# End Source File
# Begin Source File

SOURCE=.\dtime.cpp
# End Source File
# Begin Source File

SOURCE=.\DTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\dtreeeditor.cpp
# End Source File
# Begin Source File

SOURCE=.\dwww.cpp
# End Source File
# Begin Source File

SOURCE=.\docs\FILE_ID.DIZ
# End Source File
# Begin Source File

SOURCE=.\core\d3d\G3Render.cpp
# End Source File
# Begin Source File

SOURCE=.\core\d3d\G3Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GBuildShell.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GCAMERA.CPP
# End Source File
# Begin Source File

SOURCE=.\core\GCameraUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\core\Gclass.cpp
# End Source File
# Begin Source File

SOURCE=.\core\d3d\GD3Device.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GDEFORM.CPP
# End Source File
# Begin Source File

SOURCE=.\core\GDisplayTree.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gdssound.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GDXFIN.CPP
# End Source File
# Begin Source File

SOURCE=.\core\GEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GFindNode.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GFont.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gfuzzy.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GGEOM.CPP
# End Source File
# Begin Source File

SOURCE=.\core\ghull.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GifRead.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GioTraversal.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GKatmai.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD CPP /G6 /W3 /Zi /O2 /Ob2 /D "_USE_INTEL_COMPILER" /FAs
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /O2 /Ob2 /D "_USE_INTEL_COMPILER"
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\core\gl\GLRENDER.CPP
# End Source File
# Begin Source File

SOURCE=.\core\GLRenderBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GLUTILS.CPP
# End Source File
# Begin Source File

SOURCE=.\GLVDOC.CPP
# End Source File
# Begin Source File

SOURCE=.\glview.cpp
# End Source File
# Begin Source File

SOURCE=.\GLVIEW.RC
# End Source File
# Begin Source File

SOURCE=.\docs\GLViewNodes.html
# End Source File
# Begin Source File

SOURCE=.\GLViewNodes.txt
# End Source File
# Begin Source File

SOURCE=.\GLVPDoc.d
# End Source File
# Begin Source File

SOURCE=.\GLVPDOC.HLP
# End Source File
# Begin Source File

SOURCE=.\glvview.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gmidi.cpp
# End Source File
# Begin Source File

SOURCE=.\core\Gmodel.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GMorph.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gnurbs.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gnurbsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GOptimizeTraversal.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gpanel.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gperlinnoise.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GRaySelect.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GReadGV.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GReporter.cpp
# End Source File
# Begin Source File

SOURCE=.\core\grsxsound.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GSHELL.CPP
# End Source File
# Begin Source File

SOURCE=.\core\GShellI.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GSHELLIO.CPP
# End Source File
# Begin Source File

SOURCE=.\core\GSHELLOP.CPP
# End Source File
# Begin Source File

SOURCE=.\core\gshelltriang.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GSound.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GTransparency.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GUnzip.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gurlcache.cpp
# End Source File
# Begin Source File

SOURCE=.\core\GUTILS.CPP
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GV2Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GV2Group.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVBaseColor.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVBlinker.CPP
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvBsp.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVCallback.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVCHILDLIST.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVComplexity.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVCOORDINATE3.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVDB.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVDEBUGERROR.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVDICT.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVDIRECTIONALLIGHT.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVELEMENT.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVFIELD.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVFIELDDATA.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVFontStyle.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVGROUP.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\src\gvhandle.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVINFO.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVINPUT.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVInterpolators.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gvisibility.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvKeyword.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVKtx.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVLabel.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\src\gvlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVLEVELOFDETAIL.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVLISTS.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVLOD.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVMATERIAL.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVMATERIALBINDING.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\src\gvmediathread.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVMFields.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvModifier.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVMorph.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVNAME.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvNameSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVNODE.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVNodeShell.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVNORMAL.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVNORMALBINDING.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\src\gvnurbs.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\src\gvnurbsinventor.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVORTHOGRAPHICCAMERA.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVPath.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVPERSPECTIVECAMERA.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVPLIST.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVPOINTLIGHT.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\Src\GvProto.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVREADERROR.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvScene.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvScript.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSensors.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSEPARATOR.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSFields.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSHAPEHINTS.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvSound.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSPOTLIGHT.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSTRING.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSweep.CPP
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVSWITCH.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVTEXTURE2.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVTEXTURECOORDINATE2.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GvTextureCoordinateBinding.cpp

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVTimeTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVTransforms.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVTRANSFORMSEPARATOR.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVTRAVERSE.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVUNKNOWNNODE.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVWebFx.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVWWWANCHOR.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vrml\SRC\GVWWWINLINE.CPP

!IF  "$(CFG)" == "GLVIEW - Win32 Release"

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 Debug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Debug Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "GLVIEW - Win32 D3D Rel Katmai"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\core\GZReadStream.cpp
# End Source File
# Begin Source File

SOURCE=.\core\IDither.cpp
# End Source File
# Begin Source File

SOURCE=.\LICENSE.TXT
# End Source File
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\core\MATRIX.CPP
# End Source File
# Begin Source File

SOURCE=.\morepfd.cpp
# End Source File
# Begin Source File

SOURCE=.\ORDER.TXT
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\OVERLAP.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\parse.ypp
# End Source File
# Begin Source File

SOURCE=.\pfdinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\pixform.cpp
# End Source File
# Begin Source File

SOURCE=.\core\POINT.CPP
# End Source File
# Begin Source File

SOURCE=.\core\POINTA.CPP
# End Source File
# Begin Source File

SOURCE=.\core\Quaternation.cpp
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\RAPID.cpp
# End Source File
# Begin Source File

SOURCE=.\docs\README.TXT
# End Source File
# Begin Source File

SOURCE=.\core\RONNY.CPP
# End Source File
# Begin Source File

SOURCE=.\core\RT.CPP
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\scan.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\scan.lpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD BASE CPP /Yc"STDAFX.H"
# ADD CPP /Yc"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\core\TKDIB.CPP
# End Source File
# Begin Source File

SOURCE=.\core\tkrgb.cpp
# End Source File
# Begin Source File

SOURCE=.\core\tomesh.cpp
# End Source File
# Begin Source File

SOURCE=.\core\Triangulate.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vbplist.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsexpr.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsfields.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsparse.cpp
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsstatement.cpp
# End Source File
# Begin Source File

SOURCE=.\core\wavread.cpp
# End Source File
# Begin Source File

SOURCE=.\docs\WHATSNEW.TXT
# End Source File
# Begin Source File

SOURCE=.\core\WWW.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\core\ADSCODES.H
# End Source File
# Begin Source File

SOURCE=.\core\Array.h
# End Source File
# Begin Source File

SOURCE=.\core\ArrayNew.h
# End Source File
# Begin Source File

SOURCE=.\core\astream.h
# End Source File
# Begin Source File

SOURCE=.\core\avi\AVISUP.H
# End Source File
# Begin Source File

SOURCE=..\blaxxunCC3D\blaxxunVRML.h
# End Source File
# Begin Source File

SOURCE=.\CFSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\core\colors.inc
# End Source File
# Begin Source File

SOURCE=..\dxsdk5\dxmedia\include\control.h
# End Source File
# Begin Source File

SOURCE=.\core\gl\COpenGL.h
# End Source File
# Begin Source File

SOURCE=.\core\d3d\d3dmacs.h
# End Source File
# Begin Source File

SOURCE=.\dbackgro.h
# End Source File
# Begin Source File

SOURCE=.\dcamera.h
# End Source File
# Begin Source File

SOURCE=.\dconsole.h
# End Source File
# Begin Source File

SOURCE=.\DConsole1.h
# End Source File
# Begin Source File

SOURCE=.\dd3doptions.h
# End Source File
# Begin Source File

SOURCE=.\ddefsph.h
# End Source File
# Begin Source File

SOURCE=.\ddrawsty.h
# End Source File
# Begin Source File

SOURCE=..\dxsdk5\dxmedia\include\ddstream.h
# End Source File
# Begin Source File

SOURCE=.\dfog.h
# End Source File
# Begin Source File

SOURCE=.\jpeghgnt\DIBIO.H
# End Source File
# Begin Source File

SOURCE=.\jpeghgnt\DIBIO.PRO
# End Source File
# Begin Source File

SOURCE=.\dmateria.h
# End Source File
# Begin Source File

SOURCE=.\DMetaVerse.h
# End Source File
# Begin Source File

SOURCE=.\dobjnew.h
# End Source File
# Begin Source File

SOURCE=.\doptions.h
# End Source File
# Begin Source File

SOURCE=.\dprop.h
# End Source File
# Begin Source File

SOURCE=.\DRenderFile.h
# End Source File
# Begin Source File

SOURCE=.\dstring.h
# End Source File
# Begin Source File

SOURCE=.\dstyle.h
# End Source File
# Begin Source File

SOURCE=.\dstylep.h
# End Source File
# Begin Source File

SOURCE=.\core\dsutil3d.h
# End Source File
# Begin Source File

SOURCE=.\dtexture.h
# End Source File
# Begin Source File

SOURCE=.\dtime.h
# End Source File
# Begin Source File

SOURCE=.\DTransform.h
# End Source File
# Begin Source File

SOURCE=.\DTreeEditor.h
# End Source File
# Begin Source File

SOURCE=.\DWWW.h
# End Source File
# Begin Source File

SOURCE=..\dxsdk5\dxmedia\include\evcode.h
# End Source File
# Begin Source File

SOURCE=.\core\media\rma\exvsurf.h
# End Source File
# Begin Source File

SOURCE=.\core\d3d\G3Render.h
# End Source File
# Begin Source File

SOURCE=.\core\GBuildShell.h
# End Source File
# Begin Source File

SOURCE=.\core\GCamera.h
# End Source File
# Begin Source File

SOURCE=.\core\GCameraUtil.h
# End Source File
# Begin Source File

SOURCE=.\core\gcaps.h
# End Source File
# Begin Source File

SOURCE=.\core\GClass.h
# End Source File
# Begin Source File

SOURCE=.\gconfig.h
# End Source File
# Begin Source File

SOURCE=.\core\d3d\GD3Device.h
# End Source File
# Begin Source File

SOURCE=.\core\GDeform.h
# End Source File
# Begin Source File

SOURCE=.\core\GDisplayTree.h
# End Source File
# Begin Source File

SOURCE=.\core\gdssound.h
# End Source File
# Begin Source File

SOURCE=.\core\GDxfIn.h
# End Source File
# Begin Source File

SOURCE=.\core\GEdit.h
# End Source File
# Begin Source File

SOURCE=.\core\GEvent.h
# End Source File
# Begin Source File

SOURCE=.\core\GFindNode.h
# End Source File
# Begin Source File

SOURCE=.\core\GFont.h
# End Source File
# Begin Source File

SOURCE=.\core\gfuzzy.h
# End Source File
# Begin Source File

SOURCE=.\core\GGeom.h
# End Source File
# Begin Source File

SOURCE=.\core\ghull.h
# End Source File
# Begin Source File

SOURCE=.\core\GifRead.h
# End Source File
# Begin Source File

SOURCE=.\core\GioTraversal.h
# End Source File
# Begin Source File

SOURCE=.\core\gkatmai.h
# End Source File
# Begin Source File

SOURCE=.\core\gl\glrender.h
# End Source File
# Begin Source File

SOURCE=.\core\GLRenderBitmap.h
# End Source File
# Begin Source File

SOURCE=.\core\GLUtils.h
# End Source File
# Begin Source File

SOURCE=.\GLVDOC.H
# End Source File
# Begin Source File

SOURCE=.\glview.h
# End Source File
# Begin Source File

SOURCE=.\GLViewHdr.h
# End Source File
# Begin Source File

SOURCE=.\glviewvrml.h
# End Source File
# Begin Source File

SOURCE=.\glvview.h
# End Source File
# Begin Source File

SOURCE=.\core\gmidi.h
# End Source File
# Begin Source File

SOURCE=.\core\gmodel.h
# End Source File
# Begin Source File

SOURCE=.\core\gnurbs.h
# End Source File
# Begin Source File

SOURCE=.\core\gnurbsutil.h
# End Source File
# Begin Source File

SOURCE=.\core\GOptimizeTraversal.h
# End Source File
# Begin Source File

SOURCE=.\core\gpanel.h
# End Source File
# Begin Source File

SOURCE=.\core\gperlinnoise.h
# End Source File
# Begin Source File

SOURCE=.\core\GRaySelect.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GReadGv.h
# End Source File
# Begin Source File

SOURCE=.\core\GRender.h
# End Source File
# Begin Source File

SOURCE=.\core\grenderbase.h
# End Source File
# Begin Source File

SOURCE=.\core\GReporter.h
# End Source File
# Begin Source File

SOURCE=.\core\media\rma\grmaplay.h
# End Source File
# Begin Source File

SOURCE=.\core\grsxsound.h
# End Source File
# Begin Source File

SOURCE=.\core\GShell.h
# End Source File
# Begin Source File

SOURCE=.\core\GShellI.h
# End Source File
# Begin Source File

SOURCE=.\core\GShellIO.h
# End Source File
# Begin Source File

SOURCE=.\core\GShellOp.h
# End Source File
# Begin Source File

SOURCE=.\core\gshelltriang.h
# End Source File
# Begin Source File

SOURCE=.\core\GSound.h
# End Source File
# Begin Source File

SOURCE=.\core\GSurface.h
# End Source File
# Begin Source File

SOURCE=.\core\GTrace.h
# End Source File
# Begin Source File

SOURCE=.\core\GTransparency.h
# End Source File
# Begin Source File

SOURCE=.\core\GUnzip.h
# End Source File
# Begin Source File

SOURCE=.\core\gurlcache.h
# End Source File
# Begin Source File

SOURCE=.\core\gutils.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gv2Geometry.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gv2Group.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvAsciiText.H
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvavatar.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvBaseColor.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvBASIC.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvBlinker.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvBsp.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvCallback.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvCHILDLIST.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvComplexity.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvCONE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvConnection.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvCOORDINATE3.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvCUBE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvCYLINDER.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvDB.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvDEBUGERROR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvDICT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvDIRECTIONALLIGHT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvEdit.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvELEMENT.H
# End Source File
# Begin Source File

SOURCE=.\core\GVersion.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvExtrusion.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvfield.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvfielddata.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvFIELDS.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvFontStyle.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvGROUP.H
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvhandle.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvINDEXEDFACESET.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvINDEXEDLINESET.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvIndexedTriangleStripSet.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvINFO.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvINPUT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvInterpolators.h
# End Source File
# Begin Source File

SOURCE=.\core\gvisibility.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvKeyword.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvKtx.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvLabel.h
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvlayer.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvLEVELOFDETAIL.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvLISTS.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvLOD.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvmaterial.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMATERIALBINDING.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMATRIXTRANSFORM.H
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvmediathread.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFCOLOR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFFLOAT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFInt32.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFLONG.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFNode.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFRotation.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFString.H
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvmftime.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFVEC2F.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMFVEC3F.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvModifier.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvMorph.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvNameSpace.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvnode.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvNODES.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvNodeShell.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvNORMAL.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvNORMALBINDING.H
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvnurbs.h
# End Source File
# Begin Source File

SOURCE=.\vrml\include\gvnurbsinventor.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvORTHOGRAPHICCAMERA.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvPath.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvPendulum.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvPERSPECTIVECAMERA.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvPLIST.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvPOINTLIGHT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvPOINTSET.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvProto.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvREADERROR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvROTATION.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvROTOR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSCALE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvScene.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvScript.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSensors.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSEPARATOR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFBITMASK.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFBOOL.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFCOLOR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFENUM.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFFLOAT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFIMAGE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFInt32.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFLONG.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFMATRIX.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFNode.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFROTATION.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFSTRING.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFTime.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFVEC2F.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSFVEC3F.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSHAPEHINTS.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvShuttle.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSound.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSPHERE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSPOTLIGHT.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSTATE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSTRING.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSUBFIELD.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvsubnode.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSweep.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvSWITCH.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvtexture2.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\Gvtexture2transform.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTEXTURECOORDINATE2.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTextureCoordinateBinding.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTextureRotor.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTextureShuttle.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTimeTransform.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTransform.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTRANSFORMSEPARATOR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTRANSLATION.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvTraverse.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvUNKNOWNNODE.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvWebFx.h
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvWWWANCHOR.H
# End Source File
# Begin Source File

SOURCE=.\vrml\INCLUDE\GvWWWINLINE.H
# End Source File
# Begin Source File

SOURCE=.\core\gzreadstream.h
# End Source File
# Begin Source File

SOURCE=.\core\IDither.h
# End Source File
# Begin Source File

SOURCE=.\jpeghgnt\image.H
# End Source File
# Begin Source File

SOURCE=.\jpeghgnt\JPEGREAD.H
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\jsmath.h
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\core\MallocDbg.h
# End Source File
# Begin Source File

SOURCE=.\core\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\Matvec.h
# End Source File
# Begin Source File

SOURCE=.\core\midstuff.h
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\Moments.h
# End Source File
# Begin Source File

SOURCE=.\morepfd.h
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\Obb.h
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\Overlap.h
# End Source File
# Begin Source File

SOURCE=.\pfdinfo.h
# End Source File
# Begin Source File

SOURCE=.\pixform.h
# End Source File
# Begin Source File

SOURCE=.\jpeghgnt\png_read.h
# End Source File
# Begin Source File

SOURCE=.\core\Point.h
# End Source File
# Begin Source File

SOURCE=.\core\pointa.h
# End Source File
# Begin Source File

SOURCE=.\core\Quaternation.h
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\Rapid.h
# End Source File
# Begin Source File

SOURCE=.\core\collision\Rapid\RAPID_VErsion.H
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\core\ronny.h
# End Source File
# Begin Source File

SOURCE=..\..\RSX2\include\RSX.H
# End Source File
# Begin Source File

SOURCE=.\core\RT.h
# End Source File
# Begin Source File

SOURCE=.\core\RTABST.H
# End Source File
# Begin Source File

SOURCE=.\core\Stack.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\stdafx_atl.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\system.h
# End Source File
# Begin Source File

SOURCE=.\jpeghgnt\TARGAIO.H
# End Source File
# Begin Source File

SOURCE=.\core\TOMESH.H
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vbbasic.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vbplist.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsexpr.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsfields.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsfunction.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsparse.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\vsstatement.h
# End Source File
# Begin Source File

SOURCE=.\core\wavread.h
# End Source File
# Begin Source File

SOURCE=.\core\www.h
# End Source File
# Begin Source File

SOURCE=.\vrmlscript\y_tab.h
# End Source File
# Begin Source File

SOURCE=.\core\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\core\zlib\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\BITMAP1.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\glview.avi
# End Source File
# Begin Source File

SOURCE=.\res\GLView.ICO
# End Source File
# Begin Source File

SOURCE=.\res\GLView.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ICON1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\TOOLBAR.BMP
# End Source File
# End Group
# Begin Group "gul"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\core\gul\common\guar_bincoeff.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guar_bincoeff.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guar_exact.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guar_exact.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guge_intersect.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guge_intersect.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guge_linear.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guge_normalize.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_basics.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_basics.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_io.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_io.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_regularize.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_regularize.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_split.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_split.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_triangulate.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gugr_triangulate.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gul_error.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gul_io.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gul_matrix.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gul_types.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gul_vector.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guma_sorting.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\guma_sorting.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_basics.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_basics.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_derivatives.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_derivatives.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_divide.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_divide.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_intersect.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_intersect.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_linearize.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_linearize.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_tesselate.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_tesselate.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gunu_tesselate1.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gust_new.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gust_pool.cpp
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gust_pool.h
# End Source File
# Begin Source File

SOURCE=.\core\gul\common\gust_pooltbl.h
# End Source File
# End Group
# End Target
# End Project

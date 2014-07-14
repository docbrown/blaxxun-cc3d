# Microsoft Developer Studio Project File - Name="jpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jpeg - Win32 OGL Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jpeg.mak" CFG="jpeg - Win32 OGL Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 D3D Release Katmai" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 D3D Debug Katmai" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 OGL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 OGL Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\CC3D\Release"
# PROP Intermediate_Dir "C:\CC3D\Release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /GX /O2 /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"c:\cc3d\Release\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\CC3D\Debug"
# PROP Intermediate_Dir "C:\CC3D\Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"C:\CC3D\Debug\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "jpeg___W"
# PROP BASE Intermediate_Dir "jpeg___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\debugStatic"
# PROP Intermediate_Dir ".\debugStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /GX /Zi /Od /I "..\core\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MTd /W3 /GX /Zi /Od /I "..\core\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\Debug\libjpeg.lib"
# ADD LIB32 /nologo /out:".\DebugStatic\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jpeg___0"
# PROP BASE Intermediate_Dir "jpeg___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\releaseStatic"
# PROP Intermediate_Dir ".\releaseStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /Zi /O2 /I "..\core\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GX /Zi /O2 /I "..\core\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\Release\libjpeg.lib"
# ADD LIB32 /nologo /out:".\ReleaseStatic\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jpeg___W"
# PROP BASE Intermediate_Dir "jpeg___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\RelK"
# PROP Intermediate_Dir "C:\cc3d\RelK\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MD /W3 /GX /O2 /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /Zi /Ot /Oi /Oy /Ob1 /Gf /Gy /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"c:\cc3d\Release\libjpeg.lib"
# ADD LIB32 /nologo /out:"c:\cc3d\RelK\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "jpeg___0"
# PROP BASE Intermediate_Dir "jpeg___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\cc3d\DebugK"
# PROP Intermediate_Dir "C:\cc3d\DebugK\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /GX /ZI /Od /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"C:\CC3D\Debug\libjpeg.lib"
# ADD LIB32 /nologo /out:"C:\CC3D\DebugK\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "jpeg___0"
# PROP BASE Intermediate_Dir "jpeg___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\OGLDebug"
# PROP Intermediate_Dir "c:\cc3d\OGLDebug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"C:\CC3D\Debug\libjpeg.lib"
# ADD LIB32 /nologo /out:"c:\cc3d\OGLDebug\libjpeg.lib"

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jpeg___W"
# PROP BASE Intermediate_Dir "jpeg___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\OGLRel"
# PROP Intermediate_Dir "C:\cc3d\OGLRel\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MD /W3 /GX /O2 /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /GX /Zi /O2 /I "..\core\zlib" /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"c:\cc3d\Release\libjpeg.lib"
# ADD LIB32 /nologo /out:"C:\cc3d\OGLRel\libjpeg.lib"

!ENDIF 

# Begin Target

# Name "jpeg - Win32 Release"
# Name "jpeg - Win32 Debug"
# Name "jpeg - Win32 Debug Static"
# Name "jpeg - Win32 Release Static"
# Name "jpeg - Win32 D3D Release Katmai"
# Name "jpeg - Win32 D3D Debug Katmai"
# Name "jpeg - Win32 OGL Debug"
# Name "jpeg - Win32 OGL Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\cderror.h
# End Source File
# Begin Source File

SOURCE=.\cdjpeg.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\cjpeg.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ckconfig.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\coderules.doc
# End Source File
# Begin Source File

SOURCE=.\DIBIO.C
# End Source File
# Begin Source File

SOURCE=.\djpeg.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\example.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\filelist.doc
# End Source File
# Begin Source File

SOURCE=.\IMAGE.C
# End Source File
# Begin Source File

SOURCE=.\IMAGE1.C
# End Source File
# Begin Source File

SOURCE=.\install.doc
# End Source File
# Begin Source File

SOURCE=.\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\jconfig.doc
# End Source File
# Begin Source File

SOURCE=.\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\jerror.c
# End Source File
# Begin Source File

SOURCE=.\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=.\jpegread.c
# End Source File
# Begin Source File

SOURCE=.\jpegtran.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\jutils.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg.doc
# End Source File
# Begin Source File

SOURCE=.\libpng\png.c
# End Source File
# Begin Source File

SOURCE=.\png_read.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngrcb.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngread.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngtest.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libpng\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\libpng\pngwutil.c
# End Source File
# Begin Source File

SOURCE=.\rdbmp.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdcolmap.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdgif.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdjpgcom.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdppm.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdrle.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdswitch.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdtarga.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\structure.doc
# End Source File
# Begin Source File

SOURCE=.\TARGAIO.C
# End Source File
# Begin Source File

SOURCE=.\usage.doc
# End Source File
# Begin Source File

SOURCE=.\wizard.doc
# End Source File
# Begin Source File

SOURCE=.\wrbmp.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wrgif.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wrjpgcom.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wrppm.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wrrle.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\wrtarga.c

!IF  "$(CFG)" == "jpeg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 Release Static"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Release Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 D3D Debug Katmai"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jpeg - Win32 OGL Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\dibio.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\image1.h
# End Source File
# Begin Source File

SOURCE=.\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jdct.h
# End Source File
# Begin Source File

SOURCE=.\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\jerror.h
# End Source File
# Begin Source File

SOURCE=.\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\JPEGREAD.H
# End Source File
# Begin Source File

SOURCE=.\jversion.h
# End Source File
# Begin Source File

SOURCE=.\libpng\png.h
# End Source File
# Begin Source File

SOURCE=.\png_read.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\dibio.pro
# End Source File
# Begin Source File

SOURCE=.\image1.pro
# End Source File
# End Target
# End Project

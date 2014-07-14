# Microsoft Developer Studio Project File - Name="zlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zlib - Win32 OGL Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak" CFG="zlib - Win32 OGL Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 D3D Release Katmai" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 D3D Debug Katmai" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 OGL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 OGL Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CC3D/src/core/zlib", JIBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zlib - Win32 Release"

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
# ADD CPP /nologo /G5 /Gz /MD /W3 /GX /O2 /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\CC3D\Debug"
# PROP Intermediate_Dir "C:\CC3D\Debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 Debug Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zlib___W"
# PROP BASE Intermediate_Dir "zlib___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\debugStatic"
# PROP Intermediate_Dir ".\debugStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MTd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 Release Static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zlib___0"
# PROP BASE Intermediate_Dir "zlib___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\releaseStatic"
# PROP Intermediate_Dir ".\releaseStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gz /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 D3D Release Katmai"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zlib___W"
# PROP BASE Intermediate_Dir "zlib___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\RelK"
# PROP Intermediate_Dir "C:\cc3d\RelK\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MD /W3 /GX /O2 /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /Zi /Ot /Oi /Oy /Ob1 /Gf /Gy /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 D3D Debug Katmai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zlib___0"
# PROP BASE Intermediate_Dir "zlib___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\cc3d\DebugK"
# PROP Intermediate_Dir "C:\cc3d\DebugK\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /GX /ZI /Od /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 OGL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "zlib___0"
# PROP BASE Intermediate_Dir "zlib___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "c:\cc3d\OGLDebug"
# PROP Intermediate_Dir "c:\cc3d\OGLDebug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MDd /W3 /GX /Zi /Od /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 OGL Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "zlib___0"
# PROP BASE Intermediate_Dir "zlib___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\cc3d\OGLRel"
# PROP Intermediate_Dir "C:\cc3d\OGLRel\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gz /MD /W3 /GX /O2 /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MD /W3 /Zi /O2 /I "\client\glview\src\core\d3d" /I "\client\glview\src\core" /I "\client\glview\src\jpeghgnt" /I "\client\glview\src\vrml\include" /I "\client\glview\src" /I "\client\glview\src\vrmlscript" /I "\client\glview\blaxxuncc3d" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "zlib - Win32 Release"
# Name "zlib - Win32 Debug"
# Name "zlib - Win32 Debug Static"
# Name "zlib - Win32 Release Static"
# Name "zlib - Win32 D3D Release Katmai"
# Name "zlib - Win32 D3D Debug Katmai"
# Name "zlib - Win32 OGL Debug"
# Name "zlib - Win32 OGL Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\adler32.c
# End Source File
# Begin Source File

SOURCE=.\compress.c
# End Source File
# Begin Source File

SOURCE=.\crc32.c
# End Source File
# Begin Source File

SOURCE=.\deflate.c
# End Source File
# Begin Source File

SOURCE=.\deflate.h
# End Source File
# Begin Source File

SOURCE=.\gzio.c
# End Source File
# Begin Source File

SOURCE=.\infblock.c
# End Source File
# Begin Source File

SOURCE=.\infblock.h
# End Source File
# Begin Source File

SOURCE=.\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\inffast.c
# End Source File
# Begin Source File

SOURCE=.\inffast.h
# End Source File
# Begin Source File

SOURCE=.\inflate.c
# End Source File
# Begin Source File

SOURCE=.\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\infutil.c
# End Source File
# Begin Source File

SOURCE=.\infutil.h
# End Source File
# Begin Source File

SOURCE=.\trees.c
# End Source File
# Begin Source File

SOURCE=.\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zutil.c
# End Source File
# Begin Source File

SOURCE=.\zutil.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

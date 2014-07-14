# Microsoft Developer Studio Project File - Name="gul" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gul - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit\
 NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den\
 Befehl
!MESSAGE 
!MESSAGE NMAKE /f "gul.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "gul.mak" CFG="gul - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "gul - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "gul - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "gul - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "../win32" /I "../common" /I "../../gul/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "gul - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "../win32" /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "gul - Win32 Release"
# Name "gul - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\common\guar_bincoeff.cpp
# End Source File
# Begin Source File

SOURCE=..\common\guar_exact.cpp
# End Source File
# Begin Source File

SOURCE=..\common\guge_intersect.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gugr_basics.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gugr_io.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gugr_regularize.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gugr_split.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gugr_triangulate.cpp
# End Source File
# Begin Source File

SOURCE=..\common\guma_sorting.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_basics.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_derivatives.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_divide.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_intersect.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_linearize.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_tesselate.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gunu_tesselate1.cpp
# End Source File
# Begin Source File

SOURCE=..\common\gust_pool.cpp
# End Source File
# End Group
# Begin Group "Header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\common\guar_bincoeff.h
# End Source File
# Begin Source File

SOURCE=..\common\guar_exact.h
# End Source File
# Begin Source File

SOURCE=..\common\guge_intersect.h
# End Source File
# Begin Source File

SOURCE=..\common\guge_linear.h
# End Source File
# Begin Source File

SOURCE=..\common\guge_normalize.h
# End Source File
# Begin Source File

SOURCE=..\common\gugr_basics.h
# End Source File
# Begin Source File

SOURCE=..\common\gugr_io.h
# End Source File
# Begin Source File

SOURCE=..\common\gugr_regularize.h
# End Source File
# Begin Source File

SOURCE=..\common\gugr_split.h
# End Source File
# Begin Source File

SOURCE=..\common\gugr_triangulate.h
# End Source File
# Begin Source File

SOURCE=..\common\gul_error.h
# End Source File
# Begin Source File

SOURCE=..\common\gul_io.h
# End Source File
# Begin Source File

SOURCE=..\common\gul_matrix.h
# End Source File
# Begin Source File

SOURCE=..\common\gul_types.h
# End Source File
# Begin Source File

SOURCE=..\common\gul_vector.h
# End Source File
# Begin Source File

SOURCE=..\common\guma_sorting.h
# End Source File
# Begin Source File

SOURCE=..\common\gunu_basics.h
# End Source File
# Begin Source File

SOURCE=..\common\gunu_derivatives.h
# End Source File
# Begin Source File

SOURCE=..\common\gunu_divide.h
# End Source File
# Begin Source File

SOURCE=..\common\gunu_intersect.h
# End Source File
# Begin Source File

SOURCE=..\common\gunu_linearize.h
# End Source File
# Begin Source File

SOURCE=..\common\gunu_tesselate.h
# End Source File
# Begin Source File

SOURCE=..\common\gust_new.h
# End Source File
# Begin Source File

SOURCE=..\common\gust_pool.h
# End Source File
# Begin Source File

SOURCE=..\common\gust_pooltbl.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# End Target
# End Project

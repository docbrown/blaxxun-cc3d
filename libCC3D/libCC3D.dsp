# Microsoft Developer Studio Project File - Name="libCC3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libCC3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libCC3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libCC3D.mak" CFG="libCC3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libCC3D - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl5.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Od /I "..\src\core\gl" /I "..\src\core" /I "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /D "_DEBUG" /D "_OGL" /FR /YX /FD /I /hg/download/graphics/Mesa-3.0/include" " /c
# SUBTRACT CPP /X
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Target

# Name "libCC3D - Win32 Debug"
# Begin Source File

SOURCE=..\src\core\array.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\astream.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\build.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\collide.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gcamera.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gcamerautil.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gclass.cpp
# End Source File
# Begin Source File

SOURCE=.\gconfig.h
# End Source File
# Begin Source File

SOURCE=..\src\core\ghull.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gifread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\giotraversal.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gkatmai.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gl\glrender.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\glutils.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gmodel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gnurbs.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gperlinnoise.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\greadgv.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\greporter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gshell.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gshelli.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gshellio.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gshellop.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gutils.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gv2geometry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gv2group.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvbasecolor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvblinker.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvbsp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvcallback.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvchildlist.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvcomplexity.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvcoordinate3.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvdb.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvdebugerror.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvdict.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvdirectionallight.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvedit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvelement.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvfield.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvfielddata.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvfontstyle.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvgeometry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvgroup.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvhandle.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvinfo.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvinput.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvinterpolators.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\gvisibility.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvkeyword.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvktx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvlabel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvlevelofdetail.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvlists.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvlod.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmaterialbinding.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmediathread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmfields.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmodifier.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvmorph.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvname.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnamespace.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnodeshell.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnormal.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnormalbinding.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvnurbs.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvorthographiccamera.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvpath.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvperspectivecamera.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvplist.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvpointlight.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvproto.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvreaderror.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvscene.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvscript.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvsensors.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvseparator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvsfields.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvshapehints.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvsound.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvspotlight.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvstring.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvsweep.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvswitch.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtexture2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtexturecoordinate2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtexturecoordinatebinding.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtimetransform.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtransforms.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtransformseparator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvtraverse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvunknownnode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvwebfx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvwwwanchor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrml\src\gvwwwinline.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\overlap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\point.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\pointa.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\quaternation.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\collision\rapid\rapid.cpp
# End Source File
# Begin Source File

SOURCE=..\src\core\rt.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vrmlscript\scan.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\src\core\triangulate.cpp
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
# End Target
# End Project

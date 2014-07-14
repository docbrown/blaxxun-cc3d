# Microsoft Developer Studio Generated NMAKE File, Based on libCC3D.dsp
!IF "$(CFG)" == ""
CFG=libCC3D - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libCC3D - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libCC3D - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libCC3D.mak" CFG="libCC3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libCC3D - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Debug
INTDIR=.
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\libCC3D.a" "$(OUTDIR)\libCC3D.bsc"

!ELSE 

ALL : "$(OUTDIR)\libCC3D.a" "$(OUTDIR)\libCC3D.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\array.o"
	-@erase "$(INTDIR)\array.sbr"
	-@erase "$(INTDIR)\astream.o"
	-@erase "$(INTDIR)\astream.sbr"
	-@erase "$(INTDIR)\build.o"
	-@erase "$(INTDIR)\build.sbr"
	-@erase "$(INTDIR)\collide.o"
	-@erase "$(INTDIR)\collide.sbr"
	-@erase "$(INTDIR)\gclass.o"
	-@erase "$(INTDIR)\gclass.sbr"
	-@erase "$(INTDIR)\gifread.o"
	-@erase "$(INTDIR)\gifread.sbr"
	-@erase "$(INTDIR)\glrender.o"
	-@erase "$(INTDIR)\glrender.sbr"
	-@erase "$(INTDIR)\glutils.o"
	-@erase "$(INTDIR)\glutils.sbr"
	-@erase "$(INTDIR)\gperlinnoise.o"
	-@erase "$(INTDIR)\gperlinnoise.sbr"
	-@erase "$(INTDIR)\greadgv.o"
	-@erase "$(INTDIR)\greadgv.sbr"
	-@erase "$(INTDIR)\greporter.o"
	-@erase "$(INTDIR)\greporter.sbr"
	-@erase "$(INTDIR)\gshell.o"
	-@erase "$(INTDIR)\gshell.sbr"
	-@erase "$(INTDIR)\gshelli.o"
	-@erase "$(INTDIR)\gshelli.sbr"
	-@erase "$(INTDIR)\gshellio.o"
	-@erase "$(INTDIR)\gshellio.sbr"
	-@erase "$(INTDIR)\gshellop.o"
	-@erase "$(INTDIR)\gshellop.sbr"
	-@erase "$(INTDIR)\gutils.o"
	-@erase "$(INTDIR)\gutils.sbr"
	-@erase "$(INTDIR)\gv2geometry.o"
	-@erase "$(INTDIR)\gv2geometry.sbr"
	-@erase "$(INTDIR)\gv2group.o"
	-@erase "$(INTDIR)\gv2group.sbr"
	-@erase "$(INTDIR)\gvbasecolor.o"
	-@erase "$(INTDIR)\gvbasecolor.sbr"
	-@erase "$(INTDIR)\gvblinker.o"
	-@erase "$(INTDIR)\gvblinker.sbr"
	-@erase "$(INTDIR)\gvbsp.o"
	-@erase "$(INTDIR)\gvbsp.sbr"
	-@erase "$(INTDIR)\gvcallback.o"
	-@erase "$(INTDIR)\gvcallback.sbr"
	-@erase "$(INTDIR)\gvchildlist.o"
	-@erase "$(INTDIR)\gvchildlist.sbr"
	-@erase "$(INTDIR)\gvcomplexity.o"
	-@erase "$(INTDIR)\gvcomplexity.sbr"
	-@erase "$(INTDIR)\gvcoordinate3.o"
	-@erase "$(INTDIR)\gvcoordinate3.sbr"
	-@erase "$(INTDIR)\gvdb.o"
	-@erase "$(INTDIR)\gvdb.sbr"
	-@erase "$(INTDIR)\gvdebugerror.o"
	-@erase "$(INTDIR)\gvdebugerror.sbr"
	-@erase "$(INTDIR)\gvdict.o"
	-@erase "$(INTDIR)\gvdict.sbr"
	-@erase "$(INTDIR)\gvdirectionallight.o"
	-@erase "$(INTDIR)\gvdirectionallight.sbr"
	-@erase "$(INTDIR)\gvedit.o"
	-@erase "$(INTDIR)\gvedit.sbr"
	-@erase "$(INTDIR)\gvelement.o"
	-@erase "$(INTDIR)\gvelement.sbr"
	-@erase "$(INTDIR)\gvfield.o"
	-@erase "$(INTDIR)\gvfield.sbr"
	-@erase "$(INTDIR)\gvfielddata.o"
	-@erase "$(INTDIR)\gvfielddata.sbr"
	-@erase "$(INTDIR)\gvfontstyle.o"
	-@erase "$(INTDIR)\gvfontstyle.sbr"
	-@erase "$(INTDIR)\gvgeometry.o"
	-@erase "$(INTDIR)\gvgeometry.sbr"
	-@erase "$(INTDIR)\gvgroup.o"
	-@erase "$(INTDIR)\gvgroup.sbr"
	-@erase "$(INTDIR)\gvhandle.o"
	-@erase "$(INTDIR)\gvhandle.sbr"
	-@erase "$(INTDIR)\gvinfo.o"
	-@erase "$(INTDIR)\gvinfo.sbr"
	-@erase "$(INTDIR)\gvinput.o"
	-@erase "$(INTDIR)\gvinput.sbr"
	-@erase "$(INTDIR)\gvinterpolators.o"
	-@erase "$(INTDIR)\gvinterpolators.sbr"
	-@erase "$(INTDIR)\gvisibility.o"
	-@erase "$(INTDIR)\gvisibility.sbr"
	-@erase "$(INTDIR)\gvkeyword.o"
	-@erase "$(INTDIR)\gvkeyword.sbr"
	-@erase "$(INTDIR)\gvktx.o"
	-@erase "$(INTDIR)\gvktx.sbr"
	-@erase "$(INTDIR)\gvlabel.o"
	-@erase "$(INTDIR)\gvlabel.sbr"
	-@erase "$(INTDIR)\gvlayer.o"
	-@erase "$(INTDIR)\gvlayer.sbr"
	-@erase "$(INTDIR)\gvlevelofdetail.o"
	-@erase "$(INTDIR)\gvlevelofdetail.sbr"
	-@erase "$(INTDIR)\gvlists.o"
	-@erase "$(INTDIR)\gvlists.sbr"
	-@erase "$(INTDIR)\gvlod.o"
	-@erase "$(INTDIR)\gvlod.sbr"
	-@erase "$(INTDIR)\gvmaterial.o"
	-@erase "$(INTDIR)\gvmaterial.sbr"
	-@erase "$(INTDIR)\gvmaterialbinding.o"
	-@erase "$(INTDIR)\gvmaterialbinding.sbr"
	-@erase "$(INTDIR)\gvmediathread.o"
	-@erase "$(INTDIR)\gvmediathread.sbr"
	-@erase "$(INTDIR)\gvmfields.o"
	-@erase "$(INTDIR)\gvmfields.sbr"
	-@erase "$(INTDIR)\gvmodifier.o"
	-@erase "$(INTDIR)\gvmodifier.sbr"
	-@erase "$(INTDIR)\gvmorph.o"
	-@erase "$(INTDIR)\gvmorph.sbr"
	-@erase "$(INTDIR)\gvname.o"
	-@erase "$(INTDIR)\gvname.sbr"
	-@erase "$(INTDIR)\gvnamespace.o"
	-@erase "$(INTDIR)\gvnamespace.sbr"
	-@erase "$(INTDIR)\gvnode.o"
	-@erase "$(INTDIR)\gvnode.sbr"
	-@erase "$(INTDIR)\gvnodeshell.o"
	-@erase "$(INTDIR)\gvnodeshell.sbr"
	-@erase "$(INTDIR)\gvnormal.o"
	-@erase "$(INTDIR)\gvnormal.sbr"
	-@erase "$(INTDIR)\gvnormalbinding.o"
	-@erase "$(INTDIR)\gvnormalbinding.sbr"
	-@erase "$(INTDIR)\gvnurbs.o"
	-@erase "$(INTDIR)\gvnurbs.sbr"
	-@erase "$(INTDIR)\gvorthographiccamera.o"
	-@erase "$(INTDIR)\gvorthographiccamera.sbr"
	-@erase "$(INTDIR)\gvpath.o"
	-@erase "$(INTDIR)\gvpath.sbr"
	-@erase "$(INTDIR)\gvperspectivecamera.o"
	-@erase "$(INTDIR)\gvperspectivecamera.sbr"
	-@erase "$(INTDIR)\gvplist.o"
	-@erase "$(INTDIR)\gvplist.sbr"
	-@erase "$(INTDIR)\gvpointlight.o"
	-@erase "$(INTDIR)\gvpointlight.sbr"
	-@erase "$(INTDIR)\gvproto.o"
	-@erase "$(INTDIR)\gvproto.sbr"
	-@erase "$(INTDIR)\gvreaderror.o"
	-@erase "$(INTDIR)\gvreaderror.sbr"
	-@erase "$(INTDIR)\gvscene.o"
	-@erase "$(INTDIR)\gvscene.sbr"
	-@erase "$(INTDIR)\gvscript.o"
	-@erase "$(INTDIR)\gvscript.sbr"
	-@erase "$(INTDIR)\gvsensors.o"
	-@erase "$(INTDIR)\gvsensors.sbr"
	-@erase "$(INTDIR)\gvseparator.o"
	-@erase "$(INTDIR)\gvseparator.sbr"
	-@erase "$(INTDIR)\gvsfields.o"
	-@erase "$(INTDIR)\gvsfields.sbr"
	-@erase "$(INTDIR)\gvshapehints.o"
	-@erase "$(INTDIR)\gvshapehints.sbr"
	-@erase "$(INTDIR)\gvsound.o"
	-@erase "$(INTDIR)\gvsound.sbr"
	-@erase "$(INTDIR)\gvspotlight.o"
	-@erase "$(INTDIR)\gvspotlight.sbr"
	-@erase "$(INTDIR)\gvstring.o"
	-@erase "$(INTDIR)\gvstring.sbr"
	-@erase "$(INTDIR)\gvsweep.o"
	-@erase "$(INTDIR)\gvsweep.sbr"
	-@erase "$(INTDIR)\gvswitch.o"
	-@erase "$(INTDIR)\gvswitch.sbr"
	-@erase "$(INTDIR)\gvtexture2.o"
	-@erase "$(INTDIR)\gvtexture2.sbr"
	-@erase "$(INTDIR)\gvtexturecoordinate2.o"
	-@erase "$(INTDIR)\gvtexturecoordinate2.sbr"
	-@erase "$(INTDIR)\gvtexturecoordinatebinding.o"
	-@erase "$(INTDIR)\gvtexturecoordinatebinding.sbr"
	-@erase "$(INTDIR)\gvtimetransform.o"
	-@erase "$(INTDIR)\gvtimetransform.sbr"
	-@erase "$(INTDIR)\gvtransforms.o"
	-@erase "$(INTDIR)\gvtransforms.sbr"
	-@erase "$(INTDIR)\gvtransformseparator.o"
	-@erase "$(INTDIR)\gvtransformseparator.sbr"
	-@erase "$(INTDIR)\gvtraverse.o"
	-@erase "$(INTDIR)\gvtraverse.sbr"
	-@erase "$(INTDIR)\gvunknownnode.o"
	-@erase "$(INTDIR)\gvunknownnode.sbr"
	-@erase "$(INTDIR)\gvwebfx.o"
	-@erase "$(INTDIR)\gvwebfx.sbr"
	-@erase "$(INTDIR)\gvwwwanchor.o"
	-@erase "$(INTDIR)\gvwwwanchor.sbr"
	-@erase "$(INTDIR)\gvwwwinline.o"
	-@erase "$(INTDIR)\gvwwwinline.sbr"
	-@erase "$(INTDIR)\matrix.o"
	-@erase "$(INTDIR)\matrix.sbr"
	-@erase "$(INTDIR)\overlap.o"
	-@erase "$(INTDIR)\overlap.sbr"
	-@erase "$(INTDIR)\parse.o"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\point.o"
	-@erase "$(INTDIR)\point.sbr"
	-@erase "$(INTDIR)\pointa.o"
	-@erase "$(INTDIR)\pointa.sbr"
	-@erase "$(INTDIR)\quaternation.o"
	-@erase "$(INTDIR)\quaternation.sbr"
	-@erase "$(INTDIR)\rapid.o"
	-@erase "$(INTDIR)\rapid.sbr"
	-@erase "$(INTDIR)\rt.o"
	-@erase "$(INTDIR)\rt.sbr"
	-@erase "$(INTDIR)\scan.o"
	-@erase "$(INTDIR)\scan.sbr"
	-@erase "$(INTDIR)\triangulate.o"
	-@erase "$(INTDIR)\triangulate.sbr"
	-@erase "$(INTDIR)\vbplist.o"
	-@erase "$(INTDIR)\vbplist.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vsexpr.o"
	-@erase "$(INTDIR)\vsexpr.sbr"
	-@erase "$(INTDIR)\vsfields.o"
	-@erase "$(INTDIR)\vsfields.sbr"
	-@erase "$(INTDIR)\vsfunction.o"
	-@erase "$(INTDIR)\vsfunction.sbr"
	-@erase "$(INTDIR)\vsparse.o"
	-@erase "$(INTDIR)\vsparse.sbr"
	-@erase "$(INTDIR)\vsstatement.o"
	-@erase "$(INTDIR)\vsstatement.sbr"
	-@erase "$(OUTDIR)\libCC3D.bsc"
	-@erase "$(OUTDIR)\libCC3D.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

#CPP=xicl5.exe
#CPP_PROJ=/nologo /MLd /W3 /GX /Od /I "..\src\core\gl" /I "..\src\core" /I\
# "..\src\jpeghgnt" /I "..\src\vrml\include" /I "..\src\vrmlscript" /I "." /D\
# "_DEBUG" /D "_OGL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\libCC3D.pch" /YX\
# /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /I\
# /hg/download/graphics/Mesa-3.0/include" " /c 

CPP=c++.exe
CPP_PROJ=-MMD -I . -I ../src/core  -I ../src/core/gl -I ../src/jpeghgnt -I /client/cc3d/src/vrml/include -I ../src/vrmlscript -I /hg/download/graphics/Mesa-3.0/include -D_OGL -D_DEBUG -w -c 



CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

.c{$(CPP_OBJS)}.o::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.o::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.o::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libCC3D.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\array.sbr" \
	"$(INTDIR)\astream.sbr" \
	"$(INTDIR)\build.sbr" \
	"$(INTDIR)\collide.sbr" \
	"$(INTDIR)\gclass.sbr" \
	"$(INTDIR)\gifread.sbr" \
	"$(INTDIR)\glrender.sbr" \
	"$(INTDIR)\glutils.sbr" \
	"$(INTDIR)\gperlinnoise.sbr" \
	"$(INTDIR)\greadgv.sbr" \
	"$(INTDIR)\greporter.sbr" \
	"$(INTDIR)\gshell.sbr" \
	"$(INTDIR)\gshelli.sbr" \
	"$(INTDIR)\gshellio.sbr" \
	"$(INTDIR)\gshellop.sbr" \
	"$(INTDIR)\gutils.sbr" \
	"$(INTDIR)\gv2geometry.sbr" \
	"$(INTDIR)\gv2group.sbr" \
	"$(INTDIR)\gvbasecolor.sbr" \
	"$(INTDIR)\gvblinker.sbr" \
	"$(INTDIR)\gvbsp.sbr" \
	"$(INTDIR)\gvcallback.sbr" \
	"$(INTDIR)\gvchildlist.sbr" \
	"$(INTDIR)\gvcomplexity.sbr" \
	"$(INTDIR)\gvcoordinate3.sbr" \
	"$(INTDIR)\gvdb.sbr" \
	"$(INTDIR)\gvdebugerror.sbr" \
	"$(INTDIR)\gvdict.sbr" \
	"$(INTDIR)\gvdirectionallight.sbr" \
	"$(INTDIR)\gvedit.sbr" \
	"$(INTDIR)\gvelement.sbr" \
	"$(INTDIR)\gvfield.sbr" \
	"$(INTDIR)\gvfielddata.sbr" \
	"$(INTDIR)\gvfontstyle.sbr" \
	"$(INTDIR)\gvgeometry.sbr" \
	"$(INTDIR)\gvgroup.sbr" \
	"$(INTDIR)\gvhandle.sbr" \
	"$(INTDIR)\gvinfo.sbr" \
	"$(INTDIR)\gvinput.sbr" \
	"$(INTDIR)\gvinterpolators.sbr" \
	"$(INTDIR)\gvisibility.sbr" \
	"$(INTDIR)\gvkeyword.sbr" \
	"$(INTDIR)\gvktx.sbr" \
	"$(INTDIR)\gvlabel.sbr" \
	"$(INTDIR)\gvlayer.sbr" \
	"$(INTDIR)\gvlevelofdetail.sbr" \
	"$(INTDIR)\gvlists.sbr" \
	"$(INTDIR)\gvlod.sbr" \
	"$(INTDIR)\gvmaterial.sbr" \
	"$(INTDIR)\gvmaterialbinding.sbr" \
	"$(INTDIR)\gvmediathread.sbr" \
	"$(INTDIR)\gvmfields.sbr" \
	"$(INTDIR)\gvmodifier.sbr" \
	"$(INTDIR)\gvmorph.sbr" \
	"$(INTDIR)\gvname.sbr" \
	"$(INTDIR)\gvnamespace.sbr" \
	"$(INTDIR)\gvnode.sbr" \
	"$(INTDIR)\gvnodeshell.sbr" \
	"$(INTDIR)\gvnormal.sbr" \
	"$(INTDIR)\gvnormalbinding.sbr" \
	"$(INTDIR)\gvnurbs.sbr" \
	"$(INTDIR)\gvorthographiccamera.sbr" \
	"$(INTDIR)\gvpath.sbr" \
	"$(INTDIR)\gvperspectivecamera.sbr" \
	"$(INTDIR)\gvplist.sbr" \
	"$(INTDIR)\gvpointlight.sbr" \
	"$(INTDIR)\gvproto.sbr" \
	"$(INTDIR)\gvreaderror.sbr" \
	"$(INTDIR)\gvscene.sbr" \
	"$(INTDIR)\gvscript.sbr" \
	"$(INTDIR)\gvsensors.sbr" \
	"$(INTDIR)\gvseparator.sbr" \
	"$(INTDIR)\gvsfields.sbr" \
	"$(INTDIR)\gvshapehints.sbr" \
	"$(INTDIR)\gvsound.sbr" \
	"$(INTDIR)\gvspotlight.sbr" \
	"$(INTDIR)\gvstring.sbr" \
	"$(INTDIR)\gvsweep.sbr" \
	"$(INTDIR)\gvswitch.sbr" \
	"$(INTDIR)\gvtexture2.sbr" \
	"$(INTDIR)\gvtexturecoordinate2.sbr" \
	"$(INTDIR)\gvtexturecoordinatebinding.sbr" \
	"$(INTDIR)\gvtimetransform.sbr" \
	"$(INTDIR)\gvtransforms.sbr" \
	"$(INTDIR)\gvtransformseparator.sbr" \
	"$(INTDIR)\gvtraverse.sbr" \
	"$(INTDIR)\gvunknownnode.sbr" \
	"$(INTDIR)\gvwebfx.sbr" \
	"$(INTDIR)\gvwwwanchor.sbr" \
	"$(INTDIR)\gvwwwinline.sbr" \
	"$(INTDIR)\matrix.sbr" \
	"$(INTDIR)\overlap.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\point.sbr" \
	"$(INTDIR)\pointa.sbr" \
	"$(INTDIR)\quaternation.sbr" \
	"$(INTDIR)\rapid.sbr" \
	"$(INTDIR)\rt.sbr" \
	"$(INTDIR)\scan.sbr" \
	"$(INTDIR)\triangulate.sbr" \
	"$(INTDIR)\vbplist.sbr" \
	"$(INTDIR)\vsexpr.sbr" \
	"$(INTDIR)\vsfields.sbr" \
	"$(INTDIR)\vsfunction.sbr" \
	"$(INTDIR)\vsparse.sbr" \
	"$(INTDIR)\vsstatement.sbr"

"$(OUTDIR)\libCC3D.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=ar 
LIB32_FLAGS= $(OUTDIR)/libCC3D.a 
LIB32_OBJS= \
	"$(INTDIR)\array.o" \
	"$(INTDIR)\astream.o" \
	"$(INTDIR)\build.o" \
	"$(INTDIR)\collide.o" \
	"$(INTDIR)\gclass.o" \
	"$(INTDIR)\gifread.o" \
	"$(INTDIR)\glrender.o" \
	"$(INTDIR)\glutils.o" \
	"$(INTDIR)\gperlinnoise.o" \
	"$(INTDIR)\greadgv.o" \
	"$(INTDIR)\greporter.o" \
	"$(INTDIR)\gshell.o" \
	"$(INTDIR)\gshelli.o" \
	"$(INTDIR)\gshellio.o" \
	"$(INTDIR)\gshellop.o" \
	"$(INTDIR)\gutils.o" \
	"$(INTDIR)\gv2geometry.o" \
	"$(INTDIR)\gv2group.o" \
	"$(INTDIR)\gvbasecolor.o" \
	"$(INTDIR)\gvblinker.o" \
	"$(INTDIR)\gvbsp.o" \
	"$(INTDIR)\gvcallback.o" \
	"$(INTDIR)\gvchildlist.o" \
	"$(INTDIR)\gvcomplexity.o" \
	"$(INTDIR)\gvcoordinate3.o" \
	"$(INTDIR)\gvdb.o" \
	"$(INTDIR)\gvdebugerror.o" \
	"$(INTDIR)\gvdict.o" \
	"$(INTDIR)\gvdirectionallight.o" \
	"$(INTDIR)\gvedit.o" \
	"$(INTDIR)\gvelement.o" \
	"$(INTDIR)\gvfield.o" \
	"$(INTDIR)\gvfielddata.o" \
	"$(INTDIR)\gvfontstyle.o" \
	"$(INTDIR)\gvgeometry.o" \
	"$(INTDIR)\gvgroup.o" \
	"$(INTDIR)\gvhandle.o" \
	"$(INTDIR)\gvinfo.o" \
	"$(INTDIR)\gvinput.o" \
	"$(INTDIR)\gvinterpolators.o" \
	"$(INTDIR)\gvisibility.o" \
	"$(INTDIR)\gvkeyword.o" \
	"$(INTDIR)\gvktx.o" \
	"$(INTDIR)\gvlabel.o" \
	"$(INTDIR)\gvlayer.o" \
	"$(INTDIR)\gvlevelofdetail.o" \
	"$(INTDIR)\gvlists.o" \
	"$(INTDIR)\gvlod.o" \
	"$(INTDIR)\gvmaterial.o" \
	"$(INTDIR)\gvmaterialbinding.o" \
	"$(INTDIR)\gvmediathread.o" \
	"$(INTDIR)\gvmfields.o" \
	"$(INTDIR)\gvmodifier.o" \
	"$(INTDIR)\gvmorph.o" \
	"$(INTDIR)\gvname.o" \
	"$(INTDIR)\gvnamespace.o" \
	"$(INTDIR)\gvnode.o" \
	"$(INTDIR)\gvnodeshell.o" \
	"$(INTDIR)\gvnormal.o" \
	"$(INTDIR)\gvnormalbinding.o" \
	"$(INTDIR)\gvnurbs.o" \
	"$(INTDIR)\gvorthographiccamera.o" \
	"$(INTDIR)\gvpath.o" \
	"$(INTDIR)\gvperspectivecamera.o" \
	"$(INTDIR)\gvplist.o" \
	"$(INTDIR)\gvpointlight.o" \
	"$(INTDIR)\gvproto.o" \
	"$(INTDIR)\gvreaderror.o" \
	"$(INTDIR)\gvscene.o" \
	"$(INTDIR)\gvscript.o" \
	"$(INTDIR)\gvsensors.o" \
	"$(INTDIR)\gvseparator.o" \
	"$(INTDIR)\gvsfields.o" \
	"$(INTDIR)\gvshapehints.o" \
	"$(INTDIR)\gvsound.o" \
	"$(INTDIR)\gvspotlight.o" \
	"$(INTDIR)\gvstring.o" \
	"$(INTDIR)\gvsweep.o" \
	"$(INTDIR)\gvswitch.o" \
	"$(INTDIR)\gvtexture2.o" \
	"$(INTDIR)\gvtexturecoordinate2.o" \
	"$(INTDIR)\gvtexturecoordinatebinding.o" \
	"$(INTDIR)\gvtimetransform.o" \
	"$(INTDIR)\gvtransforms.o" \
	"$(INTDIR)\gvtransformseparator.o" \
	"$(INTDIR)\gvtraverse.o" \
	"$(INTDIR)\gvunknownnode.o" \
	"$(INTDIR)\gvwebfx.o" \
	"$(INTDIR)\gvwwwanchor.o" \
	"$(INTDIR)\gvwwwinline.o" \
	"$(INTDIR)\matrix.o" \
	"$(INTDIR)\overlap.o" \
	"$(INTDIR)\parse.o" \
	"$(INTDIR)\point.o" \
	"$(INTDIR)\pointa.o" \
	"$(INTDIR)\quaternation.o" \
	"$(INTDIR)\rapid.o" \
	"$(INTDIR)\rt.o" \
	"$(INTDIR)\scan.o" \
	"$(INTDIR)\triangulate.o" \
	"$(INTDIR)\vbplist.o" \
	"$(INTDIR)\vsexpr.o" \
	"$(INTDIR)\vsfields.o" \
	"$(INTDIR)\vsfunction.o" \
	"$(INTDIR)\vsparse.o" \
	"$(INTDIR)\vsstatement.o"

"$(OUTDIR)\libCC3D.a" : "$(OUTDIR)"  $(LIB32_OBJS)
    $(LIB32) $(LIB32_FLAGS) $(LIB32_OBJS)


!IF "$(CFG)" == "libCC3D - Win32 Debug"
SOURCE=..\src\core\array.cpp
DEP_CPP_ARRAY=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\array.o"	"$(INTDIR)\array.sbr" : $(SOURCE) $(DEP_CPP_ARRAY)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\astream.cpp
DEP_CPP_ASTRE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\astream.o"	"$(INTDIR)\astream.sbr" : $(SOURCE) $(DEP_CPP_ASTRE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\collision\rapid\build.cpp
DEP_CPP_BUILD=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\collision\rapid\matvec.h"\
	"..\src\core\collision\rapid\moments.h"\
	"..\src\core\collision\rapid\obb.h"\
	"..\src\core\collision\rapid\rapid.h"\
	"..\src\core\collision\rapid\rapid_version.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\build.o"	"$(INTDIR)\build.sbr" : $(SOURCE) $(DEP_CPP_BUILD)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\collision\rapid\collide.cpp
DEP_CPP_COLLI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\collision\rapid\matvec.h"\
	"..\src\core\collision\rapid\obb.h"\
	"..\src\core\collision\rapid\overlap.h"\
	"..\src\core\collision\rapid\rapid.h"\
	"..\src\core\collision\rapid\rapid_version.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\collide.o"	"$(INTDIR)\collide.sbr" : $(SOURCE) $(DEP_CPP_COLLI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gclass.cpp
DEP_CPP_GCLAS=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gclass.o"	"$(INTDIR)\gclass.sbr" : $(SOURCE) $(DEP_CPP_GCLAS)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gifread.cpp
DEP_CPP_GIFRE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gifread.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gifread.o"	"$(INTDIR)\gifread.sbr" : $(SOURCE) $(DEP_CPP_GIFRE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gl\glrender.cpp
DEP_CPP_GLREN=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\glrender.o"	"$(INTDIR)\glrender.sbr" : $(SOURCE) $(DEP_CPP_GLREN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\glutils.cpp
DEP_CPP_GLUTI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gifread.h"\
	"..\src\core\glutils.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\idither.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\dibio.h"\
	"..\src\jpeghgnt\dibio.pro"\
	"..\src\jpeghgnt\image.h"\
	"..\src\jpeghgnt\jpegread.h"\
	"..\src\jpeghgnt\png_read.h"\
	"..\src\jpeghgnt\targaio.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\glutils.o"	"$(INTDIR)\glutils.sbr" : $(SOURCE) $(DEP_CPP_GLUTI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gperlinnoise.cpp
DEP_CPP_GPERL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gnoisetable.i"\
	"..\src\core\gperlinnoise.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gperlinnoise.o"	"$(INTDIR)\gperlinnoise.sbr" : $(SOURCE)\
 $(DEP_CPP_GPERL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\greadgv.cpp
DEP_CPP_GREAD=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gshellio.h"\
	"..\src\core\gshellop.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\gzreadstream.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\greadgv.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdb.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\greadgv.o"	"$(INTDIR)\greadgv.sbr" : $(SOURCE) $(DEP_CPP_GREAD)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\greporter.cpp
DEP_CPP_GREPO=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\greporter.o"	"$(INTDIR)\greporter.sbr" : $(SOURCE)\
 $(DEP_CPP_GREPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gshell.cpp
DEP_CPP_GSHEL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\collision\rapid\rapid.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gkatmai.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gshelltriang.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\tomesh.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gshell.o"	"$(INTDIR)\gshell.sbr" : $(SOURCE) $(DEP_CPP_GSHEL)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gshelli.cpp
DEP_CPP_GSHELL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\collision\rapid\rapid.h"\
	"..\src\core\colors.inc"\
	"..\src\core\gclass.h"\
	"..\src\core\gkatmai.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gshellop.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\tomesh.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gshelli.o"	"$(INTDIR)\gshelli.sbr" : $(SOURCE) $(DEP_CPP_GSHELL)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gshellio.cpp
DEP_CPP_GSHELLI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\giotraversal.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gshellio.h"\
	"..\src\core\gshellop.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gshellio.o"	"$(INTDIR)\gshellio.sbr" : $(SOURCE)\
 $(DEP_CPP_GSHELLI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gshellop.cpp
DEP_CPP_GSHELLO=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gshellop.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gshellop.o"	"$(INTDIR)\gshellop.sbr" : $(SOURCE)\
 $(DEP_CPP_GSHELLO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gutils.cpp
DEP_CPP_GUTIL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gutils.o"	"$(INTDIR)\gutils.sbr" : $(SOURCE) $(DEP_CPP_GUTIL)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gv2geometry.cpp
DEP_CPP_GV2GE=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\ggeom.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\goptimizetraversal.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvextrusion.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtraverse.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gv2geometry.o"	"$(INTDIR)\gv2geometry.sbr" : $(SOURCE)\
 $(DEP_CPP_GV2GE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gv2group.cpp
DEP_CPP_GV2GR=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvbsp.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gv2group.o"	"$(INTDIR)\gv2group.sbr" : $(SOURCE) $(DEP_CPP_GV2GR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvbasecolor.cpp
DEP_CPP_GVBAS=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvbasecolor.o"	"$(INTDIR)\gvbasecolor.sbr" : $(SOURCE)\
 $(DEP_CPP_GVBAS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvblinker.cpp
DEP_CPP_GVBLI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvswitch.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvblinker.o"	"$(INTDIR)\gvblinker.sbr" : $(SOURCE)\
 $(DEP_CPP_GVBLI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvbsp.cpp
DEP_CPP_GVBSP=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\ghull.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvbsp.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtraverse.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvbsp.o"	"$(INTDIR)\gvbsp.sbr" : $(SOURCE) $(DEP_CPP_GVBSP)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvcallback.cpp
DEP_CPP_GVCAL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvcallback.o"	"$(INTDIR)\gvcallback.sbr" : $(SOURCE)\
 $(DEP_CPP_GVCAL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvchildlist.cpp
DEP_CPP_GVCHI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvplist.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvchildlist.o"	"$(INTDIR)\gvchildlist.sbr" : $(SOURCE)\
 $(DEP_CPP_GVCHI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvcomplexity.cpp
DEP_CPP_GVCOM=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvcomplexity.o"	"$(INTDIR)\gvcomplexity.sbr" : $(SOURCE)\
 $(DEP_CPP_GVCOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvcoordinate3.cpp
DEP_CPP_GVCOO=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvcoordinate3.o"	"$(INTDIR)\gvcoordinate3.sbr" : $(SOURCE)\
 $(DEP_CPP_GVCOO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvdb.cpp
DEP_CPP_GVDB_=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdb.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvdb.o"	"$(INTDIR)\gvdb.sbr" : $(SOURCE) $(DEP_CPP_GVDB_)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvdebugerror.cpp
DEP_CPP_GVDEB=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdebugerror.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvdebugerror.o"	"$(INTDIR)\gvdebugerror.sbr" : $(SOURCE)\
 $(DEP_CPP_GVDEB) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvdict.cpp
DEP_CPP_GVDIC=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvdict.o"	"$(INTDIR)\gvdict.sbr" : $(SOURCE) $(DEP_CPP_GVDIC)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvdirectionallight.cpp
DEP_CPP_GVDIR=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvdirectionallight.o"	"$(INTDIR)\gvdirectionallight.sbr" : \
$(SOURCE) $(DEP_CPP_GVDIR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvedit.cpp
DEP_CPP_GVEDI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvedit.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvedit.o"	"$(INTDIR)\gvedit.sbr" : $(SOURCE) $(DEP_CPP_GVEDI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvelement.cpp
DEP_CPP_GVELE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvelement.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvelement.o"	"$(INTDIR)\gvelement.sbr" : $(SOURCE)\
 $(DEP_CPP_GVELE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvfield.cpp
DEP_CPP_GVFIE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfields.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvfield.o"	"$(INTDIR)\gvfield.sbr" : $(SOURCE) $(DEP_CPP_GVFIE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvfielddata.cpp
DEP_CPP_GVFIEL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvunknownnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvfielddata.o"	"$(INTDIR)\gvfielddata.sbr" : $(SOURCE)\
 $(DEP_CPP_GVFIEL) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvfontstyle.cpp
DEP_CPP_GVFON=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvfontstyle.o"	"$(INTDIR)\gvfontstyle.sbr" : $(SOURCE)\
 $(DEP_CPP_GVFON) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvgeometry.cpp
DEP_CPP_GVGEO=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtraverse.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvgeometry.o"	"$(INTDIR)\gvgeometry.sbr" : $(SOURCE)\
 $(DEP_CPP_GVGEO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvgroup.cpp
DEP_CPP_GVGRO=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvgroup.o"	"$(INTDIR)\gvgroup.sbr" : $(SOURCE) $(DEP_CPP_GVGRO)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvhandle.cpp
DEP_CPP_GVHAN=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\goptimizetraversal.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvhandle.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvhandle.o"	"$(INTDIR)\gvhandle.sbr" : $(SOURCE) $(DEP_CPP_GVHAN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvinfo.cpp
DEP_CPP_GVINF=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvinfo.o"	"$(INTDIR)\gvinfo.sbr" : $(SOURCE) $(DEP_CPP_GVINF)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvinput.cpp
DEP_CPP_GVINP=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gzreadstream.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdebugerror.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvinput.o"	"$(INTDIR)\gvinput.sbr" : $(SOURCE) $(DEP_CPP_GVINP)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvinterpolators.cpp
DEP_CPP_GVINT=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvinterpolators.o"	"$(INTDIR)\gvinterpolators.sbr" : $(SOURCE)\
 $(DEP_CPP_GVINT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\gvisibility.cpp
DEP_CPP_GVISI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvisibility.o"	"$(INTDIR)\gvisibility.sbr" : $(SOURCE)\
 $(DEP_CPP_GVISI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvkeyword.cpp
DEP_CPP_GVKEY=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvkeyword.o"	"$(INTDIR)\gvkeyword.sbr" : $(SOURCE)\
 $(DEP_CPP_GVKEY) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvktx.cpp
DEP_CPP_GVKTX=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvktx.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmodifier.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvktx.o"	"$(INTDIR)\gvktx.sbr" : $(SOURCE) $(DEP_CPP_GVKTX)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvlabel.cpp
DEP_CPP_GVLAB=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvlabel.o"	"$(INTDIR)\gvlabel.sbr" : $(SOURCE) $(DEP_CPP_GVLAB)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvlayer.cpp
DEP_CPP_GVLAY=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvbsp.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlayer.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmodifier.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvlayer.o"	"$(INTDIR)\gvlayer.sbr" : $(SOURCE) $(DEP_CPP_GVLAY)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvlevelofdetail.cpp
DEP_CPP_GVLEV=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvlevelofdetail.o"	"$(INTDIR)\gvlevelofdetail.sbr" : $(SOURCE)\
 $(DEP_CPP_GVLEV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvlists.cpp
DEP_CPP_GVLIS=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvlists.o"	"$(INTDIR)\gvlists.sbr" : $(SOURCE) $(DEP_CPP_GVLIS)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvlod.cpp
DEP_CPP_GVLOD=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvlod.o"	"$(INTDIR)\gvlod.sbr" : $(SOURCE) $(DEP_CPP_GVLOD)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvmaterial.cpp
DEP_CPP_GVMAT=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtraverse.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvmaterial.o"	"$(INTDIR)\gvmaterial.sbr" : $(SOURCE)\
 $(DEP_CPP_GVMAT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvmaterialbinding.cpp
DEP_CPP_GVMATE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvmaterialbinding.o"	"$(INTDIR)\gvmaterialbinding.sbr" : $(SOURCE)\
 $(DEP_CPP_GVMATE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvmediathread.cpp
DEP_CPP_GVMED=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\greadgv.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvmediathread.o"	"$(INTDIR)\gvmediathread.sbr" : $(SOURCE)\
 $(DEP_CPP_GVMED) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvmfields.cpp
DEP_CPP_GVMFI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvmfields.o"	"$(INTDIR)\gvmfields.sbr" : $(SOURCE)\
 $(DEP_CPP_GVMFI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvmodifier.cpp
DEP_CPP_GVMOD=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmodifier.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvmodifier.o"	"$(INTDIR)\gvmodifier.sbr" : $(SOURCE)\
 $(DEP_CPP_GVMOD) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvmorph.cpp
DEP_CPP_GVMOR=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvmorph.o"	"$(INTDIR)\gvmorph.sbr" : $(SOURCE) $(DEP_CPP_GVMOR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvname.cpp
DEP_CPP_GVNAM=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvname.o"	"$(INTDIR)\gvname.sbr" : $(SOURCE) $(DEP_CPP_GVNAM)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvnamespace.cpp
DEP_CPP_GVNAME=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvnamespace.o"	"$(INTDIR)\gvnamespace.sbr" : $(SOURCE)\
 $(DEP_CPP_GVNAME) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvnode.cpp
DEP_CPP_GVNOD=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gnurbs.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gsurface.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvbsp.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdb.h"\
	"..\src\vrml\include\gvdebugerror.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvextrusion.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvktx.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlayer.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmodifier.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvnurbs.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvscript.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvunknownnode.h"\
	"..\src\vrml\include\gvwebfx.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvnode.o"	"$(INTDIR)\gvnode.sbr" : $(SOURCE) $(DEP_CPP_GVNOD)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvnodeshell.cpp
DEP_CPP_GVNODE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvnodeshell.o"	"$(INTDIR)\gvnodeshell.sbr" : $(SOURCE)\
 $(DEP_CPP_GVNODE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvnormal.cpp
DEP_CPP_GVNOR=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvnormal.o"	"$(INTDIR)\gvnormal.sbr" : $(SOURCE) $(DEP_CPP_GVNOR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvnormalbinding.cpp
DEP_CPP_GVNORM=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvnormalbinding.o"	"$(INTDIR)\gvnormalbinding.sbr" : $(SOURCE)\
 $(DEP_CPP_GVNORM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvnurbs.cpp
DEP_CPP_GVNUR=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\ggeom.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\gnurbs.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnurbs.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtraverse.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvnurbs.o"	"$(INTDIR)\gvnurbs.sbr" : $(SOURCE) $(DEP_CPP_GVNUR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvorthographiccamera.cpp
DEP_CPP_GVORT=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvorthographiccamera.o"	"$(INTDIR)\gvorthographiccamera.sbr" : \
$(SOURCE) $(DEP_CPP_GVORT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvpath.cpp
DEP_CPP_GVPAT=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvpath.o"	"$(INTDIR)\gvpath.sbr" : $(SOURCE) $(DEP_CPP_GVPAT)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvperspectivecamera.cpp
DEP_CPP_GVPER=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvperspectivecamera.o"	"$(INTDIR)\gvperspectivecamera.sbr" : \
$(SOURCE) $(DEP_CPP_GVPER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvplist.cpp
DEP_CPP_GVPLI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvplist.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvplist.o"	"$(INTDIR)\gvplist.sbr" : $(SOURCE) $(DEP_CPP_GVPLI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvpointlight.cpp
DEP_CPP_GVPOI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvpointlight.o"	"$(INTDIR)\gvpointlight.sbr" : $(SOURCE)\
 $(DEP_CPP_GVPOI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvproto.cpp
DEP_CPP_GVPRO=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\greadgv.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvscript.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexture2.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvproto.o"	"$(INTDIR)\gvproto.sbr" : $(SOURCE) $(DEP_CPP_GVPRO)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvreaderror.cpp
DEP_CPP_GVREA=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvreaderror.o"	"$(INTDIR)\gvreaderror.sbr" : $(SOURCE)\
 $(DEP_CPP_GVREA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvscene.cpp
DEP_CPP_GVSCE=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvscene.o"	"$(INTDIR)\gvscene.sbr" : $(SOURCE) $(DEP_CPP_GVSCE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvscript.cpp
DEP_CPP_GVSCR=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\mallocdbg.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfields.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvscript.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtraverse.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfields.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsparse.h"\
	"..\src\vrmlscript\vsstatement.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvscript.o"	"$(INTDIR)\gvscript.sbr" : $(SOURCE) $(DEP_CPP_GVSCR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvsensors.cpp
DEP_CPP_GVSEN=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvsensors.o"	"$(INTDIR)\gvsensors.sbr" : $(SOURCE)\
 $(DEP_CPP_GVSEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvseparator.cpp
DEP_CPP_GVSEP=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvseparator.o"	"$(INTDIR)\gvseparator.sbr" : $(SOURCE)\
 $(DEP_CPP_GVSEP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvsfields.cpp
DEP_CPP_GVSFI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdebugerror.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvkeyword.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvsfields.o"	"$(INTDIR)\gvsfields.sbr" : $(SOURCE)\
 $(DEP_CPP_GVSFI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvshapehints.cpp
DEP_CPP_GVSHA=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvshapehints.o"	"$(INTDIR)\gvshapehints.sbr" : $(SOURCE)\
 $(DEP_CPP_GVSHA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvsound.cpp
DEP_CPP_GVSOU=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gsound.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtraverse.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvsound.o"	"$(INTDIR)\gvsound.sbr" : $(SOURCE) $(DEP_CPP_GVSOU)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvspotlight.cpp
DEP_CPP_GVSPO=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvspotlight.o"	"$(INTDIR)\gvspotlight.sbr" : $(SOURCE)\
 $(DEP_CPP_GVSPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvstring.cpp
DEP_CPP_GVSTR=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvstring.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvstring.o"	"$(INTDIR)\gvstring.sbr" : $(SOURCE) $(DEP_CPP_GVSTR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvsweep.cpp
DEP_CPP_GVSWE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvsweep.o"	"$(INTDIR)\gvsweep.sbr" : $(SOURCE) $(DEP_CPP_GVSWE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvswitch.cpp
DEP_CPP_GVSWI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvswitch.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvswitch.o"	"$(INTDIR)\gvswitch.sbr" : $(SOURCE) $(DEP_CPP_GVSWI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtexture2.cpp
DEP_CPP_GVTEX=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gifread.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gversion.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\greadgv.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtraverse.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtexture2.o"	"$(INTDIR)\gvtexture2.sbr" : $(SOURCE)\
 $(DEP_CPP_GVTEX) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtexturecoordinate2.cpp
DEP_CPP_GVTEXT=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtexturecoordinate2.o"	"$(INTDIR)\gvtexturecoordinate2.sbr" : \
$(SOURCE) $(DEP_CPP_GVTEXT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtexturecoordinatebinding.cpp
DEP_CPP_GVTEXTU=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtexturecoordinatebinding.o"\
	"$(INTDIR)\gvtexturecoordinatebinding.sbr" : $(SOURCE) $(DEP_CPP_GVTEXTU)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtimetransform.cpp
DEP_CPP_GVTIM=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtimetransform.o"	"$(INTDIR)\gvtimetransform.sbr" : $(SOURCE)\
 $(DEP_CPP_GVTIM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtransforms.cpp
DEP_CPP_GVTRA=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtranslation.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtransforms.o"	"$(INTDIR)\gvtransforms.sbr" : $(SOURCE)\
 $(DEP_CPP_GVTRA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtransformseparator.cpp
DEP_CPP_GVTRAN=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtransformseparator.o"	"$(INTDIR)\gvtransformseparator.sbr" : \
$(SOURCE) $(DEP_CPP_GVTRAN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvtraverse.cpp
DEP_CPP_GVTRAV=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gbuildshell.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfont.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\ggeom.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\glutils.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\goptimizetraversal.h"\
	"..\src\core\grayselect.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gshelli.h"\
	"..\src\core\gtransparency.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\greadgv.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmodifier.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvproto.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvscript.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvtraverse.h"\
	"..\src\vrml\include\gvunknownnode.h"\
	"..\src\vrml\include\gvwebfx.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvtraverse.o"	"$(INTDIR)\gvtraverse.sbr" : $(SOURCE)\
 $(DEP_CPP_GVTRAV) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvunknownnode.cpp
DEP_CPP_GVUNK=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvreaderror.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvunknownnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvunknownnode.o"	"$(INTDIR)\gvunknownnode.sbr" : $(SOURCE)\
 $(DEP_CPP_GVUNK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvwebfx.cpp
DEP_CPP_GVWEB=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2geometry.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvasciitext.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasecolor.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvblinker.h"\
	"..\src\vrml\include\gvcallback.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvcomplexity.h"\
	"..\src\vrml\include\gvcone.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvcoordinate3.h"\
	"..\src\vrml\include\gvcube.h"\
	"..\src\vrml\include\gvcylinder.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvdirectionallight.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfontstyle.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvindexedfaceset.h"\
	"..\src\vrml\include\gvindexedlineset.h"\
	"..\src\vrml\include\gvindexedtrianglestripset.h"\
	"..\src\vrml\include\gvinfo.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvinterpolators.h"\
	"..\src\vrml\include\gvlabel.h"\
	"..\src\vrml\include\gvlevelofdetail.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvlod.h"\
	"..\src\vrml\include\gvmaterial.h"\
	"..\src\vrml\include\gvmaterialbinding.h"\
	"..\src\vrml\include\gvmatrixtransform.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvmodifier.h"\
	"..\src\vrml\include\gvmorph.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvnodes.h"\
	"..\src\vrml\include\gvnodeshell.h"\
	"..\src\vrml\include\gvnormal.h"\
	"..\src\vrml\include\gvnormalbinding.h"\
	"..\src\vrml\include\gvorthographiccamera.h"\
	"..\src\vrml\include\gvpendulum.h"\
	"..\src\vrml\include\gvperspectivecamera.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvpointlight.h"\
	"..\src\vrml\include\gvpointset.h"\
	"..\src\vrml\include\gvrotation.h"\
	"..\src\vrml\include\gvrotor.h"\
	"..\src\vrml\include\gvscale.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvshapehints.h"\
	"..\src\vrml\include\gvshuttle.h"\
	"..\src\vrml\include\gvsound.h"\
	"..\src\vrml\include\gvsphere.h"\
	"..\src\vrml\include\gvspotlight.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvsweep.h"\
	"..\src\vrml\include\gvswitch.h"\
	"..\src\vrml\include\gvtexture2.h"\
	"..\src\vrml\include\gvtexture2transform.h"\
	"..\src\vrml\include\gvtexturecoordinate2.h"\
	"..\src\vrml\include\gvtexturecoordinatebinding.h"\
	"..\src\vrml\include\gvtexturerotor.h"\
	"..\src\vrml\include\gvtextureshuttle.h"\
	"..\src\vrml\include\gvtimetransform.h"\
	"..\src\vrml\include\gvtransform.h"\
	"..\src\vrml\include\gvtransformseparator.h"\
	"..\src\vrml\include\gvtranslation.h"\
	"..\src\vrml\include\gvwebfx.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvwebfx.o"	"$(INTDIR)\gvwebfx.sbr" : $(SOURCE) $(DEP_CPP_GVWEB)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvwwwanchor.cpp
DEP_CPP_GVWWW=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvseparator.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtraverse.h"\
	"..\src\vrml\include\gvwwwanchor.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvwwwanchor.o"	"$(INTDIR)\gvwwwanchor.sbr" : $(SOURCE)\
 $(DEP_CPP_GVWWW) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrml\src\gvwwwinline.cpp
DEP_CPP_GVWWWI=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\gvisibility.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\greadgv.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvavatar.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvbsp.h"\
	"..\src\vrml\include\gvchildlist.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvelement.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvgroup.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmediathread.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnamespace.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscene.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrml\include\gvtraverse.h"\
	"..\src\vrml\include\gvwwwinline.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gvwwwinline.o"	"$(INTDIR)\gvwwwinline.sbr" : $(SOURCE)\
 $(DEP_CPP_GVWWWI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\matrix.cpp
DEP_CPP_MATRI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\matrix.o"	"$(INTDIR)\matrix.sbr" : $(SOURCE) $(DEP_CPP_MATRI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\collision\rapid\overlap.cpp
DEP_CPP_OVERL=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\collision\rapid\matvec.h"\
	"..\src\core\collision\rapid\rapid.h"\
	"..\src\core\collision\rapid\rapid_version.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\overlap.o"	"$(INTDIR)\overlap.sbr" : $(SOURCE) $(DEP_CPP_OVERL)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\parse.cpp
DEP_CPP_PARSE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsparse.h"\
	"..\src\vrmlscript\vsstatement.h"\
	"..\src\vrmlscript\y_tab.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\parse.o"	"$(INTDIR)\parse.sbr" : $(SOURCE) $(DEP_CPP_PARSE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\point.cpp
DEP_CPP_POINT=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\point.o"	"$(INTDIR)\point.sbr" : $(SOURCE) $(DEP_CPP_POINT)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\pointa.cpp
DEP_CPP_POINTA=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\pointa.o"	"$(INTDIR)\pointa.sbr" : $(SOURCE) $(DEP_CPP_POINTA)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\quaternation.cpp
DEP_CPP_QUATE=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\quaternation.o"	"$(INTDIR)\quaternation.sbr" : $(SOURCE)\
 $(DEP_CPP_QUATE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\collision\rapid\rapid.cpp
DEP_CPP_RAPID=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\collision\rapid\matvec.h"\
	"..\src\core\collision\rapid\obb.h"\
	"..\src\core\collision\rapid\overlap.h"\
	"..\src\core\collision\rapid\rapid.h"\
	"..\src\core\collision\rapid\rapid_version.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\rapid.o"	"$(INTDIR)\rapid.sbr" : $(SOURCE) $(DEP_CPP_RAPID)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\rt.cpp
DEP_CPP_RT_CP=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\rt.o"	"$(INTDIR)\rt.sbr" : $(SOURCE) $(DEP_CPP_RT_CP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\scan.cpp
DEP_CPP_SCAN_=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsparse.h"\
	"..\src\vrmlscript\vsstatement.h"\
	"..\src\vrmlscript\y_tab.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\scan.o"	"$(INTDIR)\scan.sbr" : $(SOURCE) $(DEP_CPP_SCAN_)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\core\triangulate.cpp
DEP_CPP_TRIAN=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\gshell.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubnode.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\triangulate.o"	"$(INTDIR)\triangulate.sbr" : $(SOURCE)\
 $(DEP_CPP_TRIAN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\vbplist.cpp
DEP_CPP_VBPLI=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\vbplist.o"	"$(INTDIR)\vbplist.sbr" : $(SOURCE) $(DEP_CPP_VBPLI)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\vsexpr.cpp
DEP_CPP_VSEXP=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\mallocdbg.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfields.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfields.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsstatement.h"\
	"..\src\vrmlscript\y_tab.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\vsexpr.o"	"$(INTDIR)\vsexpr.sbr" : $(SOURCE) $(DEP_CPP_VSEXP)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\vsfields.cpp
DEP_CPP_VSFIE=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\gperlinnoise.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\mallocdbg.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gv2group.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvbsp.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfields.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvscript.h"\
	"..\src\vrml\include\gvsensors.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfields.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsstatement.h"\
	"..\src\vrmlscript\y_tab.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\vsfields.o"	"$(INTDIR)\vsfields.sbr" : $(SOURCE) $(DEP_CPP_VSFIE)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\vsfunction.cpp
DEP_CPP_VSFUN=\
	"..\src\core\array.h"\
	"..\src\core\arraynew.h"\
	"..\src\core\astream.h"\
	"..\src\core\gcamera.h"\
	"..\src\core\gcamerautil.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gfuzzy.h"\
	"..\src\core\gl\glrender.h"\
	"..\src\core\gmodel.h"\
	"..\src\core\gperlinnoise.h"\
	"..\src\core\grender.h"\
	"..\src\core\grenderbase.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\mallocdbg.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\quaternation.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\core\www.h"\
	"..\src\jpeghgnt\image.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfields.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvpath.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrml\include\gvsubnode.h"\
	"..\src\vrmlscript\jsmath.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfields.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsparse.h"\
	"..\src\vrmlscript\vsstatement.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\vsfunction.o"	"$(INTDIR)\vsfunction.sbr" : $(SOURCE)\
 $(DEP_CPP_VSFUN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\vsparse.cpp
DEP_CPP_VSPAR=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\greporter.h"\
	"..\src\core\gutils.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsparse.h"\
	"..\src\vrmlscript\vsstatement.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\vsparse.o"	"$(INTDIR)\vsparse.sbr" : $(SOURCE) $(DEP_CPP_VSPAR)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\vrmlscript\vsstatement.cpp
DEP_CPP_VSSTA=\
	"..\src\core\array.h"\
	"..\src\core\astream.h"\
	"..\src\core\gclass.h"\
	"..\src\core\gutils.h"\
	"..\src\core\mallocdbg.h"\
	"..\src\core\matrix.h"\
	"..\src\core\point.h"\
	"..\src\core\pointa.h"\
	"..\src\core\rt.h"\
	"..\src\core\rtabst.h"\
	"..\src\core\stack.h"\
	"..\src\vrml\include\gvbasic.h"\
	"..\src\vrml\include\gvconnection.h"\
	"..\src\vrml\include\gvdict.h"\
	"..\src\vrml\include\gvfield.h"\
	"..\src\vrml\include\gvfielddata.h"\
	"..\src\vrml\include\gvfields.h"\
	"..\src\vrml\include\gvinput.h"\
	"..\src\vrml\include\gvlists.h"\
	"..\src\vrml\include\gvmfcolor.h"\
	"..\src\vrml\include\gvmffloat.h"\
	"..\src\vrml\include\gvmfint32.h"\
	"..\src\vrml\include\gvmflong.h"\
	"..\src\vrml\include\gvmfnode.h"\
	"..\src\vrml\include\gvmfrotation.h"\
	"..\src\vrml\include\gvmfstring.h"\
	"..\src\vrml\include\gvmftime.h"\
	"..\src\vrml\include\gvmfvec2f.h"\
	"..\src\vrml\include\gvmfvec3f.h"\
	"..\src\vrml\include\gvnode.h"\
	"..\src\vrml\include\gvplist.h"\
	"..\src\vrml\include\gvsfbitmask.h"\
	"..\src\vrml\include\gvsfbool.h"\
	"..\src\vrml\include\gvsfcolor.h"\
	"..\src\vrml\include\gvsfenum.h"\
	"..\src\vrml\include\gvsffloat.h"\
	"..\src\vrml\include\gvsfimage.h"\
	"..\src\vrml\include\gvsfint32.h"\
	"..\src\vrml\include\gvsflong.h"\
	"..\src\vrml\include\gvsfmatrix.h"\
	"..\src\vrml\include\gvsfnode.h"\
	"..\src\vrml\include\gvsfrotation.h"\
	"..\src\vrml\include\gvsfstring.h"\
	"..\src\vrml\include\gvsftime.h"\
	"..\src\vrml\include\gvsfvec2f.h"\
	"..\src\vrml\include\gvsfvec3f.h"\
	"..\src\vrml\include\gvstring.h"\
	"..\src\vrml\include\gvsubfield.h"\
	"..\src\vrmlscript\system.h"\
	"..\src\vrmlscript\vbbasic.h"\
	"..\src\vrmlscript\vbplist.h"\
	"..\src\vrmlscript\vsexpr.h"\
	"..\src\vrmlscript\vsfields.h"\
	"..\src\vrmlscript\vsfunction.h"\
	"..\src\vrmlscript\vsstatement.h"\
	".\gconfig.h"\
	".\stdafx.h"\
	

"$(INTDIR)\vsstatement.o"	"$(INTDIR)\vsstatement.sbr" : $(SOURCE)\
 $(DEP_CPP_VSSTA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 


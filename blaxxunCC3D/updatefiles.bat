@ECHO UpdateFiles.bat to copy files residing in program directory to out directory
@REM  Also copy Java & Trustlib files 

copy blaxxunCC3D.wrl release
copy blaxxunCC3D.gif release
copy blaxxunVRML.tlb release
copy docs\overview.htm release\overview.html

copy blaxxunCC3D.wrl debug
copy blaxxunCC3D.gif debug
copy blaxxunVRML.tlb debug
copy docs\overview.htm .\debug\overview.html

copy blaxxunCC3D.wrl OGLRel
copy blaxxunCC3D.gif OGLRel
copy blaxxunVRML.tlb OGLRel
copy docs\overview.htm .\OGLRel\overview.html


@ REM OUT Directory 
@mkdir .\out
@mkdir .\out\com
@mkdir .\out\com\wrapper
@mkdir .\out\netscape
copy release\blaxxunCC3D.ocx .\out
copy OGLRel\blaxxunCC3Dgl.ocx .\out\blaxxunCC3Dgl.ocx
copy blaxxunCC3D.wrl .\out
copy blaxxunCC3D.gif .\out
copy blaxxunVRML.tlb .\out
copy blaxxunVRML.* .\out\com

@REM already in top directory
del .\out\com\blaxxunVRML.tlb
copy blaxxunVRML_i.* .\out\com
copy blaxxunCC3D.odl .\out\com
copy blaxxunCC3DInterface.* .\out\com

copy release\blaxxunCC3D.tlb .\out\com
copy docs\overview.htm .\out\overview.html
copy docs\bxilogo.gif .\out
copy readme.install.txt .\out
copy reg.bat .\out
copy ..\plugin\release\np*.dll .\out\netscape
copy ..\plugin\blaxxunCC3D.* .\out\com\wrapper


rem copy \winnt40\system32\mfc42.dll .\out
rem copy \winnt40\system32\msvcrt.dll .\out


@mkdir .\out\java
@mkdir .\out\java\classes
@mkdir .\out\java\classes\vrml
@mkdir .\out\java\trustlib
@mkdir .\out\java\trustlib\blaxxunvrml
@mkdir .\out\java\trustlib\blaxxuncc3d
copy \winnt40\java\trustlib\blaxxunvrml .\out\java\trustlib\blaxxunVRML
copy \winnt40\java\trustlib\blaxxuncc3d .\out\java\trustlib\blaxxuncc3d

copy blaxxunVRML.* .\docs\EAI\com
copy blaxxunVRML_i.* .\docs\EAI\com
copy blaxxunCC3D.odl .\docs\EAI\com
copy ..\plugin\blaxxunCC3D.* .\docs\EAI\com


copy \winnt40\java\trustlib\blaxxunVRML\summary.txt .\docs\EAI\blaxxunVRML.summary.txt
copy \winnt40\java\trustlib\blaxxunCC3D\summary.txt .\docs\EAI\blaxxunCC3D.summary.txt

xcopy /s \winnt40\java\classes\vrml\*.class .\out\java\classes\vrml\
xcopy /s \winnt40\java\classes\vrml\*.txt .\out\java\classes\vrml\
del .\out\java\classes\vrml\vrml.ncb

xcopy /s \winnt40\java\classes\vrml\*.* .\java\classes\vrml\
del .\java\classes\vrml\vrml.ncb
del /s .\java\classes\vrml\*.class

@mkdir .\java\classes\trustlib\blaxxunvrml
@mkdir .\java\classes\trustlib\blaxxuncc3d
copy \winnt40\java\trustlib\blaxxunvrml .\java\classes\trustlib\blaxxunvrml
copy \winnt40\java\trustlib\blaxxuncc3d .\java\classes\trustlib\blaxxuncc3d




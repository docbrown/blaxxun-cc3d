@ECHO Creating blaxxunCC3D COM Java Interface classes
echo %WINDIR%

rem copy blaxxunVRML.tlb release\
rem copy blaxxunVRML.tlb debug\
rem copy blaxxunVRML.tlb OGLRel\
c:\msjdk\bin\jactivex /javatlb /l tmp1.lst c:\cc3d\relk\blaxxunCC3d.tlb
c:\msjdk\bin\jactivex /javatlb /l tmp2.lst c:\cc3d\relk\blaxxunVRML.tlb
c:\msjdk\bin\jvc /x- /nomessage @tmp2.lst
c:\msjdk\bin\jvc /x- /nomessage @tmp1.lst
del tmp1.lst
del tmp2.lst













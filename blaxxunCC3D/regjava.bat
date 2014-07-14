@ECHO Creating blaxxunCC3D COM Java Interface classes
copy blaxxunVRML.tlb release\
copy blaxxunVRML.tlb debug\
copy blaxxunVRML.tlb OGLRel\
javatlb /U:T release\blaxxunCC3d.tlb
javatlb /U:T release\blaxxunVRML.tlb



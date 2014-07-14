# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Java Virtual Machine Java Workspace" 0x0809

!IF "$(CFG)" == ""
CFG=vrml - Java Virtual Machine Debug
!MESSAGE No configuration specified.  Defaulting to vrml - Java Virtual Machine\
 Debug.
!ENDIF 

!IF "$(CFG)" != "vrml - Java Virtual Machine Release" && "$(CFG)" !=\
 "vrml - Java Virtual Machine Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "vrml.mak" CFG="vrml - Java Virtual Machine Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vrml - Java Virtual Machine Release" (based on\
 "Java Virtual Machine Java Workspace")
!MESSAGE "vrml - Java Virtual Machine Debug" (based on\
 "Java Virtual Machine Java Workspace")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "vrml - Java Virtual Machine Debug"
JAVA=jvc.exe

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\vrml\external\Node.class"\
 "$(OUTDIR)\vrml\external\Browser.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFVec3f.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFBool.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFTime.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFVec3f.class"\
 "$(OUTDIR)\vrml\external\field\EventOut.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventOutObserver.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidVrmlException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidEventOutException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidNodeException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidRouteException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidEventInException.class"\
 "$(OUTDIR)\vrml\external\field\EventIn.class"\
 "$(OUTDIR)\vrml\external\field\FieldTypes.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFString.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFImage.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFString.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFTime.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFBool.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFImage.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFVec3f.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFString.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFString.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMField.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFVec3f.class"\
 "$(OUTDIR)\vrml\field\ConstField.class" "$(OUTDIR)\vrml\node\Script.class"\
 "$(OUTDIR)\vrml\Event.class" "$(OUTDIR)\vrml\Field.class"\
 "$(OUTDIR)\vrml\InvalidEventInException.class"\
 "$(OUTDIR)\vrml\InvalidExposedFieldException.class"\
 "$(OUTDIR)\vrml\InvalidFieldChangeException.class"\
 "$(OUTDIR)\vrml\InvalidFieldException.class"\
 "$(OUTDIR)\vrml\InvalidRouteException.class" "$(OUTDIR)\vrml\BaseNode.class"\
 "$(OUTDIR)\vrml\InvalidVRMLSyntaxException.class"\
 "$(OUTDIR)\vrml\InvalidEventOutException.class"\
 "$(OUTDIR)\vrml\node\Node.class"\
 "$(OUTDIR)\vrml\external\blaxxunCC3dControl.class"

CLEAN : 
	-@erase "$(INTDIR)\vrml\BaseNode.class"
	-@erase "$(INTDIR)\vrml\Event.class"
	-@erase "$(INTDIR)\vrml\external\blaxxunCC3dControl.class"
	-@erase "$(INTDIR)\vrml\external\Browser.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidEventInException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidEventOutException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidNodeException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidRouteException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidVrmlException.class"
	-@erase "$(INTDIR)\vrml\external\field\EventIn.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFBool.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFImage.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFTime.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOut.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMField.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutObserver.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFBool.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFImage.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFTime.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\FieldTypes.class"
	-@erase "$(INTDIR)\vrml\external\Node.class"
	-@erase "$(INTDIR)\vrml\Field.class"
	-@erase "$(INTDIR)\vrml\field\ConstField.class"
	-@erase "$(INTDIR)\vrml\InvalidEventInException.class"
	-@erase "$(INTDIR)\vrml\InvalidEventOutException.class"
	-@erase "$(INTDIR)\vrml\InvalidExposedFieldException.class"
	-@erase "$(INTDIR)\vrml\InvalidFieldChangeException.class"
	-@erase "$(INTDIR)\vrml\InvalidFieldException.class"
	-@erase "$(INTDIR)\vrml\InvalidRouteException.class"
	-@erase "$(INTDIR)\vrml\InvalidVRMLSyntaxException.class"
	-@erase "$(INTDIR)\vrml\node\Node.class"
	-@erase "$(INTDIR)\vrml\node\Script.class"

# ADD BASE JAVA /O
# ADD JAVA /O

!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\vrml\external\Node.class"\
 "$(OUTDIR)\vrml\external\Browser.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFVec3f.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFBool.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFTime.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFVec3f.class"\
 "$(OUTDIR)\vrml\external\field\EventOut.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventOutObserver.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidVrmlException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidEventOutException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidNodeException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidRouteException.class"\
 "$(OUTDIR)\vrml\external\exception\InvalidEventInException.class"\
 "$(OUTDIR)\vrml\external\field\EventIn.class"\
 "$(OUTDIR)\vrml\external\field\FieldTypes.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFString.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFImage.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFString.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFTime.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFBool.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFNode.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFImage.class"\
 "$(OUTDIR)\vrml\external\field\EventOutSFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFVec3f.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventInSFString.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventInMFString.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFFloat.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMField.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFVec2f.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFInt32.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFColor.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFRotation.class"\
 "$(OUTDIR)\vrml\external\field\EventOutMFVec3f.class"\
 "$(OUTDIR)\vrml\field\ConstField.class" "$(OUTDIR)\vrml\node\Script.class"\
 "$(OUTDIR)\vrml\Event.class" "$(OUTDIR)\vrml\Field.class"\
 "$(OUTDIR)\vrml\InvalidEventInException.class"\
 "$(OUTDIR)\vrml\InvalidExposedFieldException.class"\
 "$(OUTDIR)\vrml\InvalidFieldChangeException.class"\
 "$(OUTDIR)\vrml\InvalidFieldException.class"\
 "$(OUTDIR)\vrml\InvalidRouteException.class" "$(OUTDIR)\vrml\BaseNode.class"\
 "$(OUTDIR)\vrml\InvalidVRMLSyntaxException.class"\
 "$(OUTDIR)\vrml\InvalidEventOutException.class"\
 "$(OUTDIR)\vrml\node\Node.class"\
 "$(OUTDIR)\vrml\external\blaxxunCC3dControl.class"

CLEAN : 
	-@erase "$(INTDIR)\vrml\BaseNode.class"
	-@erase "$(INTDIR)\vrml\Event.class"
	-@erase "$(INTDIR)\vrml\external\blaxxunCC3dControl.class"
	-@erase "$(INTDIR)\vrml\external\Browser.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidEventInException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidEventOutException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidNodeException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidRouteException.class"
	-@erase "$(INTDIR)\vrml\external\exception\InvalidVrmlException.class"
	-@erase "$(INTDIR)\vrml\external\field\EventIn.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInMFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFBool.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFImage.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFTime.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventInSFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOut.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMField.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutMFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutObserver.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFBool.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFColor.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFFloat.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFImage.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFInt32.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFNode.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFRotation.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFString.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFTime.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFVec2f.class"
	-@erase "$(INTDIR)\vrml\external\field\EventOutSFVec3f.class"
	-@erase "$(INTDIR)\vrml\external\field\FieldTypes.class"
	-@erase "$(INTDIR)\vrml\external\Node.class"
	-@erase "$(INTDIR)\vrml\Field.class"
	-@erase "$(INTDIR)\vrml\field\ConstField.class"
	-@erase "$(INTDIR)\vrml\InvalidEventInException.class"
	-@erase "$(INTDIR)\vrml\InvalidEventOutException.class"
	-@erase "$(INTDIR)\vrml\InvalidExposedFieldException.class"
	-@erase "$(INTDIR)\vrml\InvalidFieldChangeException.class"
	-@erase "$(INTDIR)\vrml\InvalidFieldException.class"
	-@erase "$(INTDIR)\vrml\InvalidRouteException.class"
	-@erase "$(INTDIR)\vrml\InvalidVRMLSyntaxException.class"
	-@erase "$(INTDIR)\vrml\node\Node.class"
	-@erase "$(INTDIR)\vrml\node\Script.class"

# ADD BASE JAVA /g
# ADD JAVA /g

!ENDIF 

################################################################################
# Begin Target

# Name "vrml - Java Virtual Machine Release"
# Name "vrml - Java Virtual Machine Debug"

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"

!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\external\Node.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\Node.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\Node.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\Browser.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\Browser.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\Browser.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFVec3f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFNode.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFBool.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFBool.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFBool.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFColor.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFFloat.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFInt32.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFRotation.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFRotation.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFRotation.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFTime.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFTime.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFTime.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFVec2f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFVec3f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOut.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOut.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOut.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFNode.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutObserver.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutObserver.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutObserver.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\exception\InvalidVrmlException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\exception\InvalidVrmlException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\exception\InvalidVrmlException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\exception\InvalidEventOutException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\exception\InvalidEventOutException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\exception\InvalidEventOutException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\exception\InvalidNodeException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\exception\InvalidNodeException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\exception\InvalidNodeException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\exception\InvalidRouteException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\exception\InvalidRouteException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\exception\InvalidRouteException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\exception\InvalidEventInException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\exception\InvalidEventInException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\exception\InvalidEventInException.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventIn.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventIn.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventIn.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\FieldTypes.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\FieldTypes.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\FieldTypes.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFString.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFImage.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFImage.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFImage.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFNode.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFVec2f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFColor.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFInt32.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFRotation.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFRotation.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFRotation.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFString.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFTime.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFTime.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFTime.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFBool.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFBool.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFBool.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFNode.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFNode.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFImage.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFImage.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFImage.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutSFFloat.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutSFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutSFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFFloat.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFVec3f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFVec2f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFRotation.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFRotation.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFRotation.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInSFString.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInSFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInSFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFColor.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFInt32.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventInMFString.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventInMFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventInMFString.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFFloat.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFFloat.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMField.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMField.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMField.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFVec2f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFVec2f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFInt32.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFInt32.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFColor.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFColor.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFRotation.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFRotation.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFRotation.class" : $(SOURCE)\
 "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\field\EventOutMFVec3f.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\field\EventOutMFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\field\EventOutMFVec3f.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\field\ConstField.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\field\ConstField.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\field\ConstField.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\node\Script.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\node\Script.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\node\Script.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Event.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\Event.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\Event.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Field.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\Field.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\Field.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidEventInException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidEventInException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidEventInException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidExposedFieldException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidExposedFieldException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidExposedFieldException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidFieldChangeException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidFieldChangeException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidFieldChangeException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidFieldException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidFieldException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidFieldException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidRouteException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidRouteException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidRouteException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BaseNode.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\BaseNode.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\BaseNode.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidVrmlSyntaxException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidVRMLSyntaxException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidVRMLSyntaxException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\InvalidEventOutException.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\InvalidEventOutException.class" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\InvalidEventOutException.class" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\node\Node.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\node\Node.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\node\Node.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\external\blaxxunCC3DControl.java

!IF  "$(CFG)" == "vrml - Java Virtual Machine Release"


"$(INTDIR)\vrml\external\blaxxunCC3dControl.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "vrml - Java Virtual Machine Debug"


"$(INTDIR)\vrml\external\blaxxunCC3dControl.class" : $(SOURCE) "$(INTDIR)"
   $(JAVA) $(JAVA_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

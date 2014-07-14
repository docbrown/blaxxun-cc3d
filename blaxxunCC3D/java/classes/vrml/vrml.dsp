# Microsoft Developer Studio Project File - Name="vrml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Java Virtual Machine Java Project" 0x0809

CFG=vrml - Java Virtual Machine Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vrml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vrml.mak" CFG="vrml - Java Virtual Machine Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vrml - Java Virtual Machine Release" (based on\
 "Java Virtual Machine Java Project")
!MESSAGE "vrml - Java Virtual Machine Debug" (based on\
 "Java Virtual Machine Java Project")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
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
# ADD BASE JAVA /g
# ADD JAVA /g

!ENDIF 

# Begin Target

# Name "vrml - Java Virtual Machine Release"
# Name "vrml - Java Virtual Machine Debug"
# Begin Group "Source Files"

# PROP Default_Filter "java;html"
# Begin Source File

SOURCE=.\BaseNode.java
# End Source File
# Begin Source File

SOURCE=.\external\blaxxunCC3DControl.java
# End Source File
# Begin Source File

SOURCE=.\external\Browser.java
# End Source File
# Begin Source File

SOURCE=.\field\ConstField.java
# End Source File
# Begin Source File

SOURCE=.\Event.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventIn.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFColor.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFFloat.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFInt32.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFNode.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFRotation.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFString.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFVec2f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInMFVec3f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFBool.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFColor.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFFloat.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFImage.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFInt32.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFNode.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFRotation.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFString.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFTime.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFVec2f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventInSFVec3f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOut.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFColor.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFFloat.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMField.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFInt32.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFNode.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFRotation.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFString.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFVec2f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutMFVec3f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutObserver.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFBool.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFColor.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFFloat.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFImage.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFInt32.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFNode.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFRotation.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFString.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFTime.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFVec2f.java
# End Source File
# Begin Source File

SOURCE=.\external\field\EventOutSFVec3f.java
# End Source File
# Begin Source File

SOURCE=.\Field.java
# End Source File
# Begin Source File

SOURCE=.\external\field\FieldTypes.java
# End Source File
# Begin Source File

SOURCE=.\external\exception\InvalidEventInException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidEventInException.java
# End Source File
# Begin Source File

SOURCE=.\external\exception\InvalidEventOutException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidEventOutException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidExposedFieldException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidFieldChangeException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidFieldException.java
# End Source File
# Begin Source File

SOURCE=.\external\exception\InvalidNodeException.java
# End Source File
# Begin Source File

SOURCE=.\external\exception\InvalidRouteException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidRouteException.java
# End Source File
# Begin Source File

SOURCE=.\external\exception\InvalidVrmlException.java
# End Source File
# Begin Source File

SOURCE=.\InvalidVrmlSyntaxException.java
# End Source File
# Begin Source File

SOURCE=.\external\Node.java
# End Source File
# Begin Source File

SOURCE=.\node\Node.java
# End Source File
# Begin Source File

SOURCE=.\node\Script.java
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

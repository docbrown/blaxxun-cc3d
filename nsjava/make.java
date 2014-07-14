################################################################################
#### make file for plugins ####
#
#  
#  Setting in your environment or the command line will determine what you 
#   build.  
#
#   SDK_ROOT    the root directory for this sdk (i.e. [drive]:\nspi30)
#
#   JDK_ROOT    the root for the java compiler (javac). If not specified
#               defaults to SDK_ROOT
#
#   DEBUG       debug version   
#   OPT         java optimization   
#
#   i.e. 
#   nmake -f makefile DEBUG=1 JDK_ROOT=c:\jdk
#   
# Will create four directory:
#   objs        your objects and plugin
#   _gen        your headers from java classes
#   _stubs      your stubs from java classes
#   
################################################################################

# adding java classes and stubs generation rules
.SUFFIXES : .class .h .java

!if !defined(SDK_ROOT)
SDK_ROOT = D:\prg\netscape\api
!endif

!if !defined(JDK_ROOT)
JDK_ROOT = D:\java\jdk\
!endif

!if "$(OS)" == "Windows_NT"
NULL=
!else
NULL=nul
!endif

TARGET  = npbxcc3d

################################################################################
# Objects files
#
JAVAOBJS    = \
\client\CC3D\nsjava\vrml\external\Browser.class\
\client\CC3D\nsjava\vrml\external\Node.class\
\client\CC3D\nsjava\vrml\external\exception\InvalidEventInException.class\
\client\CC3D\nsjava\vrml\external\exception\InvalidEventOutException.class\
\client\CC3D\nsjava\vrml\external\exception\InvalidNodeException.class\
\client\CC3D\nsjava\vrml\external\exception\InvalidRouteException.class\
\client\CC3D\nsjava\vrml\external\exception\InvalidVrmlException.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFColor.class\
\client\CC3D\nsjava\vrml\external\field\EventOut.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFNode.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFVec2f.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFVec3f.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFRotation.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFColor.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFVec3f.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFVec2f.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFInt32.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFNode.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFString.class\
\client\CC3D\nsjava\vrml\external\field\EventOutMFFloat.class\
\client\CC3D\nsjava\vrml\external\field\EventIn.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFString.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFInt32.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFColor.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFString.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFImage.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFRotation.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFVec2f.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFVec3f.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFFloat.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFFloat.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFImage.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFNode.class\
\client\CC3D\nsjava\vrml\external\field\FieldTypes.class\
\client\CC3D\nsjava\vrml\external\field\EventOutObserver.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFVec2f.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFInt32.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFColor.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFFloat.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFTime.class\
\client\CC3D\nsjava\vrml\external\field\EventInMFNode.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFVec3f.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFRotation.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFBool.class\
\client\CC3D\nsjava\vrml\external\field\EventInSFInt32.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFString.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFBool.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFTime.class\
\client\CC3D\nsjava\vrml\external\field\EventOutSFRotation.class

GEN     = \
        .\_gen\vrml_external_Browser.h \
        .\_gen\vrml_external_Node.h \
        .\_gen\vrml_external_field_EventIn.h \
        .\_gen\vrml_external_field_EventOut.h \
        .\_gen\vrml_external_exception_InvalidEventInException.h \
        .\_gen\vrml_external_exception_InvalidEventOutException.h \
        .\_gen\vrml_external_exception_InvalidNodeException.h \
        .\_gen\vrml_external_exception_InvalidRouteException.h \
        .\_gen\vrml_external_exception_InvalidVrmlException.h

STUBS   = \
        .\_stubs\vrml_external_Browser.c \
        .\_stubs\vrml_external_Node.c \
        .\_stubs\vrml_external_field_EventIn.c \
        .\_stubs\vrml_external_field_EventOut.c \
        .\_stubs\vrml_external_exception_InvalidEventInException.c \
        .\_stubs\vrml_external_exception_InvalidEventOutException.c \
        .\_stubs\vrml_external_exception_InvalidNodeException.c \
        .\_stubs\vrml_external_exception_InvalidRouteException.c \
        .\_stubs\vrml_external_exception_InvalidVrmlException.c

NETSCAPE = \
        .\_gen\java_lang_Object.h \
        .\_gen\java_lang_RuntimeException.h \
        .\_gen\java_lang_Exception.h \
        .\_gen\java_lang_Throwable.h \
        .\_gen\java_lang_IllegalArgumentException.h \
        .\_gen\netscape_plugin_Plugin.h \
        .\_stubs\netscape_plugin_Plugin.c

################################################################################
# Options and flags
#
CLASS_OPT = -classpath "d:\prg\netscape\api\classes\java_30.zip;."

!if "$(OPT)" == "1"

JAVA_OPTS	= -O

!else

JAVA_OPTS   =

!endif

################################################################################
# Tools
#
JAVAC   = $(JDK_ROOT)\bin\javac.exe 
JAVAH   = $(SDK_ROOT)\bin\win32\javah.exe	# must be the jri version


################################################################################
# Explicit Rules
#

all : java

dirs : 
    if not exist _gen mkdir _gen
    if not exist _stubs mkdir _stubs

java : dirs $(JAVAOBJS) $(NETSCAPE) $(GEN) $(STUBS)

.\_gen\netscape_plugin_Plugin.h : 
    $(JAVAH) $(CLASS_OPT) -jri -d _gen netscape.plugin.Plugin

.\_stubs\netscape_plugin_Plugin.c :
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs netscape.plugin.Plugin

.\_gen\java_lang_Object.h : 
    $(JAVAH) $(CLASS_OPT) -jri -d _gen java.lang.Object

.\_gen\java_lang_IllegalArgumentException.h : 
    $(JAVAH) $(CLASS_OPT) -jri -d _gen java.lang.IllegalArgumentException

.\_gen\java_lang_RuntimeException.h : 
    $(JAVAH) $(CLASS_OPT) -jri -d _gen java.lang.RuntimeException

.\_gen\java_lang_Throwable.h : 
    $(JAVAH) $(CLASS_OPT) -jri -d _gen java.lang.Throwable

.\_gen\java_lang_Exception.h : 
    $(JAVAH) $(CLASS_OPT) -jri -d _gen java.lang.Exception

.\_gen\vrml_external_Browser.h : vrml\external\Browser.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.Browser

.\_gen\vrml_external_Node.h : vrml\external\Node.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.Node

.\_gen\vrml_external_field_EventIn.h :  vrml\external\field\EventIn.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.field.EventIn

.\_gen\vrml_external_field_EventOut.h :  vrml\external\field\EventOut.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.field.EventOut

.\_gen\vrml_external_exception_InvalidEventInException.h : vrml\external\exception\InvalidEventInException.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.exception.InvalidEventInException

.\_gen\vrml_external_exception_InvalidEventOutException.h :  vrml\external\exception\InvalidEventOutException.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.exception.InvalidEventOutException

.\_gen\vrml_external_exception_InvalidNodeException.h :  vrml\external\exception\InvalidNodeException.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.exception.InvalidNodeException

.\_gen\vrml_external_exception_InvalidVrmlException.h :  vrml\external\exception\InvalidVrmlException.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.exception.InvalidVrmlException

.\_gen\vrml_external_exception_InvalidRouteException.h :  vrml\external\exception\InvalidRouteException.class
    $(JAVAH) $(CLASS_OPT) -jri -d _gen vrml.external.exception.InvalidRouteException

.\_stubs\vrml_external_Browser.c : vrml\external\Browser.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.Browser

.\_stubs\vrml_external_Node.c : vrml\external\Node.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.Node

.\_stubs\vrml_external_field_EventIn.c : vrml\external\field\EventIn.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.field.EventIn

.\_stubs\vrml_external_field_EventOut.c :  vrml\external\field\EventOut.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.field.EventOut

.\_stubs\vrml_external_exception_InvalidEventInException.c :  vrml\external\exception\InvalidEventInException.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.exception.InvalidEventInException

.\_stubs\vrml_external_exception_InvalidEventOutException.c :  vrml\external\exception\InvalidEventOutException.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.exception.InvalidEventOutException

.\_stubs\vrml_external_exception_InvalidNodeException.c :  vrml\external\exception\InvalidNodeException.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.exception.InvalidNodeException

.\_stubs\vrml_external_exception_InvalidVrmlException.c :  vrml\external\exception\InvalidVrmlException.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.exception.InvalidVrmlException

.\_stubs\vrml_external_exception_InvalidRouteException.c :  vrml\external\exception\InvalidRouteException.class
    $(JAVAH) $(CLASS_OPT) -jri -stubs -d _stubs vrml.external.exception.InvalidRouteException


clean :
        d:\prg\mks\mksnt\rm .\vrml\external\*.class
        d:\prg\mks\mksnt\rm .\vrml\external\field\*.class
        d:\prg\mks\mksnt\rm .\vrml\external\exception\*.class
        d:\prg\mks\mksnt\rm .\_gen\*.h
        d:\prg\mks\mksnt\rm .\_stubs\*.c

################################################################################
# Implicit Rules
#
.java.class:
    $(JAVAC) $(CLASS_OPT) $(JAVA_OPTS) $<


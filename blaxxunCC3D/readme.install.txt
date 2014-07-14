

blaxxunCC3D Active X Control Minimal install
============================================

1. Extract the zip-file with path e.g. to c:\blaxxunCC3d

2. copy the *.dll files to your \windows\system directory	(or c:\winnt\system32 on NT 4.0 )

3. from the dos prompt register the control

cd c:\blaxxunCC3d

\windows\system\regsvr32 blaxxunCC3D.ocx

There should be a succeded message

If not, make shure that you have Direct X 3/3a installed,
you can download it from 
	www.microsoft.com/directx
	
	
4. If you need the Java EAI / Passport 2.01

delete any subdirectory vrml blaxxunCC3d blaxxunVRML in
\windows\java\classes 
\windows\java\trustlib

copy the whole directory 
c:\blaxxunCC3d\java\classes\* TO \windows\java\classes
and 
c:\blaxxunCC3d\java\trustlib\* TO \windows\java\trustlib


5. run/ open a wrl file in Internet Explorer

6. run Passport, cslife.ini must be configured to use the blaxxunCC3d Plugin
vrmlIEClassid	4B6E3013-6E45-11D0-9309-0020AFE05CC8
worldviewClassid	B0D7D800-4EBF-11D0-9490-00A02494D8A5
blaxxunCC3dClassid	4B6E3013-6E45-11D0-9309-0020AFE05CC8

7. for sound 
you need Intel RSX 3d sound
www.intel.com/ial/rsx

8. for the OpenGL Version call
regsvr32 blaxxunCC3Dgl.ocx

OpenGL is preinstalled in Windows NT, and starting win Win95 OEM SVR2
otherwise OpenGL for Windows95 is available at ftp.microsoft.com/mslfiles/opengl95.exe


9. for the netscape plugin version
copy netscape\npbx3d.dll to the Netscape plugin directory below the netscape\program
directory,
remove any other VRML plugins
enter 
about:plugins
in the NS URl input line to find about all plugins
JAVA EAI is not supported under NS.







On *nix use automake one level above

cc3dglut is a front end
using GLUT

for Mesa 3.1 in glrender.cpp the usage of vertex arrays
was uncommented because of a crash in _glx_normal 


xcc3d is very basic GLX example 



===========================================


First try on a CC3D port :

mak.bat compiles some core modules with cygwin c++.exe
& Mesa headers

Requires cygwin & Mesa
(Some pathes are hardcoded in mak.bat and libcc3d.mak.)

The makefile was generated from VC but then tweaked:
changed CPP CPP_OPT
.obj to .o


Mesa headers are very strict about GLenum etc.

See stdafx.h for emulation of some WIN32 dependencies

Did not get a linux makefile working yet,
probably names of headerfiles in the source need 
to be lowercased e.g. 
#include "GvNode.h"
===> "include  "gvnode.h"

make shure to extract source files with CRLF to LF conversion
on Linux, otherwise strange errors in the preprocessor.

gcc has been problems with unnamed struct in Classes,
seems ot be fixed now according to cygnus website.


holger@blaxxun.com





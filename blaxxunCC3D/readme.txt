========================================================================
		OLE Control DLL : GLVIEWCTRL  - blaxxunCC3D.ocx
========================================================================

ControlWizard has created this project for your GLVIEWCTRL OLE Control DLL,
which contains 1 control.

Compiler MS-Visual C 5.0

Required directory structure

\CC3D
		\blaxxunCC3D		source for ocx control
		
		\src
			\core			internal graphics core
				\d3d		Direct 3D specific parts
				\ogl		OpenGL specific parts 
				\zlib		zlib- gz compression 
							compiled into zlib.lib zlib.dsw
			\vrml			VRML API
				\include
				\src

			\vrmlscript		VRML-Script/Java Script interpreter
			
			\jpeghgnt		JPEG and other image functions
							compiled into libjpeg.lib
				\libpng		PNG routines
			

\dxsdk\sdk					Direct X 6 SDK files 
		include
		bin

\rsx\						Intel RSX 3D Sound SDK
		include
		

Target Builds:

Win32 Release 				- Win32, Direct 3D, MFC in DLL, _fastcall, RTL : MT-DLL
Win32 Debug

Win32 D3D Debug Katmai 		- "" with PIII support, requires Intel compiler for GKatmai.cpp
Win32 D3D Release Katmai 	- "" with PIII support, requires Intel compiler for GKatmai.cpp

Win32 OGL Rel 				- Win32 OpenGL """
Win32 OGL Debug				- Win32 OpenGL """

Registry Options for Direct 3D Version Tuning :

4.1

Direct3D.useSoftwareLighting		compute own transform & lighting, Default 1 on PIII, 0 otherwise
Direct3D.useD3DCulling				use D3D backface culling on triangles, Default 0
Direct3D.dxlevel					level of Direct 3D Interfaces to use, Default 0, other values  3,5,6 For DX3, DX5, DX6
Direct3D.maxverts					max vertices per drawing primitive, larger VRML IndexedFaceSet get split up	, default 1020


4.201
Direct3D.perspectiveCorrection		texture perspective correction Default : 1, 0 : for possible speed up for software renderers
Direct3D.specular					specular lighting interpolation Default : 1, 0 : for possible speed up for software renderers
Direct3D.rampSize					color ramp size for D3D Ramp Driver, Default 16
General.useTextures					use textures, Default : 1
General.useBackground				use background node, Default : 1	for possible speed up for software renderers



This file contains a summary of what you will find in each of the files
that make up your GLVIEWCTRL OLE Control DLL.

GLViewCtrl.mak
	The Visual C++ project makefile for building your OLE Control.

GLViewCtrl.h
	This is the main include file for the OLE Control DLL.  It
	includes other project-specific includes such as resource.h.

GLViewCtrl.cpp
	This is the main source file that contains code for DLL initialization,
	termination and other bookkeeping.

GLViewCtrl.rc
	This is a listing of the Microsoft Windows resources that the project
	uses.  This file can be directly edited with the Visual C++ resource
	editor.

GLViewCtrl.def
	This file contains information about the OLE Control DLL that
	must be provided to run with Microsoft Windows.

GLViewCtrl.clw
	This file contains information used by ClassWizard to edit existing
	classes or add new classes.  ClassWizard also uses this file to store
	information needed to generate and edit message maps and dialog data
	maps and to generate prototype member functions.

GLViewCtrl.odl
	This file contains the Object Description Language source code for the
	type library.
	The main browser methods have been defined here.
	

/////////////////////////////////////////////////////////////////////////////
GLViewCtrl control:

GLViewCtrlCtl.h
	This file contains the declaration of the CGLViewCtrlCtrl C++ class.

GLViewCtrlCtl.cpp
	This file contains the implementation of the CGLViewCtrlCtrl C++ class.

GLViewCtrlPpg.h
	This file contains the declaration of the CGLViewCtrlPropPage C++ class.

GLViewCtrlPpg.cpp
	This file contains the implementation of the CGLViewCtrlPropPage C++ class.

GLViewCtrlCtl.bmp
	This file contains a bitmap that a container will use to represent the
	CGLViewCtrlCtrl control when it appears on a tool palette.  This bitmap
	is included by the main resource file GLViewCtrl.rc.

/////////////////////////////////////////////////////////////////////////////
Help Support:

MakeHelp.bat
	Use this batch file to create your OLE control's Help file, GLViewCtrl.hlp.

GLViewCtrl.hpj
	This file is the Help Project file used by the Help compiler to create
	your OLE control's Help file.

*.bmp
	These are bitmap files required by the standard Help file topics for
	Microsoft Foundation Class Library standard commands.  These files are
	located in the HLP subdirectory.

GLViewCtrl.rtf
	This file contains the standard help topics for the common properties,
	events and methods supported by many OLE controls.  You can edit
	this to add or remove additional control specific topics.  This file is
	located in the HLP subdirectory.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

stdafx.h, stdafx.cpp
	These files are used to build a precompiled header (PCH) file
	named stdafx.pch and a precompiled types (PCT) file named stdafx.obj.

resource.h
	This is the standard header file, which defines new resource IDs.
	The Visual C++ resource editor reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

ControlWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

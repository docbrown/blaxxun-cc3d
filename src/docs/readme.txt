===========================================================================
GLView version 4.30  10.01.2000
3D OpenGL/Direct 3D Viewer for Windows NT 4.0/95/98/2000
Copyright (C) 1994-1999 Holger Grahn
All Rights Reserved
===========================================================================



System Requirements
===================
Open GL Version
    - Microsoft WINDOWS NT 4.0/98/2000
    - OpenGL compatible Display.
    - wininet.dll + urlcache.dll Windows Internet DLL's 

        or 

    - Microsoft Windows 95
    - OpenGL librarys (opengl32.dll, glu32.dll) for Windows 95
    - wininet.dll + urlcache.dll Windows Internet DLL's

Direct 3D Version
    - Microsoft Windows NT 4.0+SP3/95/98/2000
	- Direct X Runtime  (includes Direct 3D)
    - wininet.dll + urlcache.dll Internet DLL 

		or 

    - Microsoft WINDOWS NT 4.0
	- or  MS-Service pack 3 for NT 4.0
    - wininet.dll + urlcache.dll Internet DLL (supplied)
	 (Windows 3.5 or 3.51 does *not* support Direct X)
	 
Optional Sound Card and Intel RSX 3D Sound
	http://www.intel.com/ial/rsx



Installation 
============
Installation of Demo Version from ZIP file (glview.zip):
Unzip the GLVIEW.zip file to a new directory on the hardisc. A program like WinZip 6.0 supporting 
long file names is recommended.

For Windows95 copy the OpenGL librarys into the Windows system directory or into the GLView directory.

Installation with installation program :
Start setup.exe from the first installation disk or from the DISK1 directory
of the CD-Rom distribution

The Direct X Runtime can be downloaded from 
http://www.microsoft.com/directx



Startup
=======

    glview.exe

OpenGL Specific:
================
Starts glview with a default OpenGL pixel format mode (Double-buffer, RGB)

    glview.exe /x

Starts GLView without GL-initialization, first action should be to setup Pixelformat using menu Options->
Pixel Format -> Set.

GLView automatically use OpenGL acellerator hardware like graphics boards equipped with a 3DLabs
GLint chip.

Direct X:
=========
The Direct X driver can be choosen using the Options->Direct 3D Dialog.
The driver settings will be saved on program exit.
GLView can be flipped to fullscreen using the Options->Fullscreen Menu command.
Fullscreen mode can be terminated by pressing <ESCAPE> once or by popping up the
Options menu again using ALT-O.
The 'F' and the F4 Keyboard key also toggles fullscreen mode.


A 3D file or a VRML-Url can be specified on the commandline.

Please create and configure a network caching directory in WWW->Options->cache directory on a filesystem supporting long filenames.
Note that you can remove any contents from this directory, after loading a bunch of scenes.
GLView will not free files from the cache automatically.

For viewing HTML URL큦 the path to your WWW Browser must also be configured.

You can manually associate GLView with 3D file extensions (e.g. wrl, 3dv) using 
the "Options->Register file types" command.

For slow machines turn on the following options in the Config->options dialog:
    "degrade during move" & degrade style "bounding boxes"
    initial window size x 320 y 240 (or larger)

If the cursor is a busy symbol, the texture icon can still be depressed to turn off
texture mapping. The escape key will first disable animation, and secondly texture mapping.

Support questions regarding the hardware support of Matrox graphics cards should
be send to Matrox directly, once Windows 95 drivers are available from Matrox 
GLView can be adapted to properly use the right pixelformats. Try to enable the 
"16 bit z-buffer" options in "Options->Config" and restart GLView.




Credits
=======

3D Sound by Intel

This software is based in part on the work of ZLIB+ PNG Teams.

This software is based in part on the work of the Independent JPEG Group.

Portions of this software are based in part on the VRMLScript Interpreter
written by Silicon Graphics, Inc., Mountain View, California, USA.



Fileformats
===========
GLView supports VRML 2.0,
GLView fully supports VRML 1.0 files with several extensions. (Please see VRML section.)

GLView supports also a single object fileformat with extension 3dv.
A short description of this file format is supplied in file readme.txt.

Geo format files (like supplied with Intel 3DR) are supported for import and export.

POV object scripts containing smooth_triangles can be exported and 
support for DXF-Input has been implemented.  

Single rgb,dib,bmp,gif,tga,png or jpeg image files are read in as a single textured, rotatable polygon.

Raw files, containing a triangle per line can be imported and exported. 
On import the triangles are automatically converted to a smooth object.

STL files, containing triangles with facet normals can be exported. 

A VRML file can be saved, any comments in the original file are lost. Please use info
nodes for persistent annotations.

Single object VRML 1.0 Files (extension wrl) can be exported after the TOOLS->Combine command has been used. 
GLView will then write a IndexedFaceSet with
coordinate, vertex normal, vertex color (as Material) and vertex parameter (TextureCoordinate2)
information. If a triangulated face set is needed, use the Tools->Triangulate command for
conversion.

Support for import of Wavefront obj files with polygons (f) has been integrated.

Texture files can be a 24 Bit SGI RBB  or dib/bmp images.
Additional formats are JPEG,GIF,TARGA and PPM.

GLView can render an arbitrary scene to a screen resolution independent BMP file.


On Import there is no check for valid fileformats.

WCVT2POV A good Windows 3D File converter program (including 3DS ==> VRML) can be found on:
    http://www.europa.com/~keithr/

For Kinetix 3D Studio Max the VRML 2.0 Export Module is recommended.	




Files
=====
GLVIEW.EXE      executable
GLVIEW.HLP      help file
readme.txt      readme file
order.txt       Order form
licencse.txt    License agreement
whatsnew.txt    History
GLViewNodes.html    GLView VRML extensions
file_id.diz     description
*.3dv           GLView native format 3D Data sets
*.geo           3D Data sets
*.rgb           SGI rgb texture image 
*.dib,*.bmp     texture image files, can also be used as "3d" data-set
*.gif,*.tga,*.jpg          "
*.ppm	        		   "
*.png			PNG image file
*.3dm           Material file
*.obj           Wavefront data files
*.stl           STL triangle format
*.raw           RAW 3D-Triangle format
*.wrl           VRML 3D Scene description 
*.vrml            "
*.iv            SGI OpenInventor files, GLView only support the ascii version
*.ext.gz        gzip compressed files, for automatic unzipping gzip.exe must be in the path.
*.vrl,*.url     a small text file containing a reference to a WWW url (normally a VRML file)
                format currently incompatible with Windows 95 Internet Explorer URL files.



VRML
====
GLView supports VRML 2.0 and VRML 1.0 with several extensions.

GLView can directly acess WWW VRML files without using a WWW Browser.

For using GLView with a WWW Browser, add GLView as a helper application.

Example for Netscape:

Inside Netscape : 
Select the "Options/Preferences" menu item.
Select the "Helper Applications" item in the pulldown.
Click the "New Type" button.
In the Configure New Mime type dialog enter:
    For Mime Type:  x-world
    For Mime Subtype:   x-vrml
In the "Extensions" field enter: wrl,wrl.gz,vrml,wrz

In the "Action" group box click "Launch Application"
In the text entry field enter the full GLView path.

Another option is to save a link to a VRML location inside Netscape 
to the clipboard using 
    Right Mouse Button -> "Copy this link location".

In GLView in the "WWW->Open URL-> url field" or in the url tool bar the 
link can be pasted with <Control> v. GLView is than able to resolve relative
WWWInline nodes to the URL for browsers other than Netscape.

Some WWW references for VRML files are built into GLView. The supplied
url files (vrl extension) are pointing to some interesting, medium complex VRML files. 

If the mouse is dragged over the scenes, GLView searches for Hot-Links (WWWAnchor Nodes).
If a hot-link is displayed, a double-click with the left mouse button will execute the link.
Possible actions are : Jumping to a new camera position, jumping to a new VRML scene, or
spawning the WEB-Browser program specified in the WWW->Options dialog.

VRML 2.0 specific notes
=======================

TouchSensors and other sensors have precendence on a mouse down event.
if the mouse moves above a object with a sensor attached, the status line displays "touch". 
Anchors and touchsensors can also be activated with a double click.


Supported Nodes
===============

+ supported
- not supported
- D3D not supported in D3D Version
* partly 

Grouping nodes
 Anchor 		+
 Billboard 		+ 
 Collision 		+ (including proxy )
 Group 			+
 Transform 		+

Special Groups
 Inline 		+
 LOD 			+
 Switch 		+

Common Nodes
 AudioClip 		  +
 DirectionalLight +		
 PointLight 	  +		
 Script 		  +
 Shape 			  +
 Sound 			  +
 SpotLight 		  +		
 WorldInfo 		  +

Sensors

  CylinderSensor  +
  PlaneSensor 	  +
  ProximitySensor +
  SphereSensor 	  +
  TimeSensor 	  +
  TouchSensor 	  +
  VisibilitySensor + 

 Geometry

  Box 			  +
  Cone 			  +
  Cylinder 		  +
  ElevationGrid   +
  Extrusion 	  +
  IndexedFaceSet  +
  IndexedLineSet  +
  PointSet 		  +
  Sphere 		  +
  Text 			  +

 Geometric Properties

  Color 		  +  (* see notes on direct 3D Ramp-Driver)
  Coordinate 	  +
  Normal 		  +
  TextureCoordinate + 
 
 Appearance

  Appearance 		+
  FontStyle 		+	
						attributes like leftToRight or topToBottom are ignored

  ImageTexture 		+	
						all GLView texture formats supported 
						(D3D - repeat FALSE)
						
  Material 			+
  MovieTexture 		+
  PixelTexture 		+	(- D3D after resize)
  TextureTransform 	+

 Interpolators 

  ColorInterpolator  +	  
  CoordinateInterpolator +
  NormalInterpolator 	+
  OrientationInterpolator + 
  PositionInterpolator	+ 		
  ScalarInterpolator 	+

Bindable Nodes

  Background 			+ (including background textures + color gradients)
  						  (D3D Ramp Driver : single skyColor only)
  Fog 					+
  NavigationInfo 		+
  Viewpoint				+


Script Node
===========
The script language is vrmlscript, a language similar to JavaScript.
The protocol "vrmlscript:" is recognized.
The protocol "javascript:" is treated as vrmlscript
ActiveX/Java support is planned.

PROTO and EXTERNPROTO are implemented,
If possible GLView tries to share nodes between several Proto instances.

PROTOS can contain scripts or other PROTOS.

The Direct 3D Version now  support triangulation of non convex polygons and text.
Triangulation (convex FALSE) can slow down file loading.

The Direct 3D Version currently lacks propper support for color per face. 
ColorPerVertex is supported using the Direct 3D RGB-Emulation driver. GLView treats objects with color per vertex as unlit.
The Direct 3D ramp driver doesnt support color per vertex, colored lights
etc. If a scene appears very dark, switch to the RGB Driver to check if
the scene appears now correctly colored due to color per vertex requirement. 
The VRML 2.0 Background Node MFColor fields also require the D3D support
of color per vertex.

The Tree Editor allows browsing of VRML 2 graphs including inspecting of
ROUTE, Scripts and Proto nodes.
At the top-level all Protos and Routes of the scene are shown.
Nodes must be double clicked to view fields with non default values, to see all fields the menu function Node->Expand all fields must be used.
ProtoInstances are showing in additions to their fields their instanced scene-graph and the prototype definition.


After Tree Editor changes a new VRML 2.0 file could be written.

Specific Route information:

A script function is executed as soon the script node receives an eventIn.
An existing eventsProcessed will not be called, but an initialized or shutdown
script function. An eventOut from a script functions will be send imideately on assignment
to the eventOut field, and not at the end of the script function.

If you open several GLView sessions or if GLView consumes too much
processor resources due to animation, iconize the GLView window or
stop the animation using the toolbar button.




VRML 1.0 specific notes
=======================
The VRML file format specification can be found on
    http://www.wired.com

Good WWW starting points are :
    http://sdsc.edu/vrml/worlds.html
    http://www.ocnus.com/models/

GLView defaults to Gouraud-Shading for VRML scenes. Initial texture mode and texture loading 
depends on "WWW->Options->texture loading"

Note, some files with extension wrl are not VRML 1.0 files, because they
contain other custom nodes or some unsupported Open Inventor nodes. 
GLView supports a bunch of Inventor nodes like QuadMesh, TriangleStrip, FaceSet, BaseColor,
Label ....
Other unsupported nodes are ignored or are causing the reading to stop.

Inventor files can be converted to VRML using a tool like IvToVrml. A related tool
on SGI Workstations is ivQuick which optimizes Inventor scenes and ivcat which translates binary
IV files to the ascii version, which GLView may be able to read.

Some GLView wrl files will not render on other VRML browsers, because of custom GLView nodes.

For material bindings "per vertex indexed" "per face"  "per face indexed" the diffuse (or base color)
channel (if of the material is used. If the emissive color channel supplies more values than the diffuse
channel, the emissive colors are used for the local colors. Scenes based on emissive materials can be
rendered using "Render->non lighted".

Material 0 ist set as a default for the primitive.

Extensions:
for GLView node extensions please see file GLViewNodes.txt


VRML Tree Editor
================
Using the "Tools->Tree Editor" command the VRML scene tree can be examined and/or edited.
Double-Clicking on a icon displays lower level information.
Nodes can be dragged and dropped around, or can be cut/copied/pasted using the edit menu.
Nodes can be pasted in from file or written to file.

GLView automatically resolves DEF/USE references and duplicate node names.
Paste pastes a link to the node referenced by a copy. It큦 equivalent to writing
USE node in a VRML ascii file.

A full copy, with all DEF/USE references local can be achieved by writing the node to a
temporary file. This file can be pasted into the tree using the "Node->Read" command.

Unknown nodes with proper field definitions can be edited as well.

Single value fields can be edited using the Field->Edit command.


Current Limitations:
No Edit of SFBitmask, SFImage fields, 
No Loop Detection on Drag and Drop and Cut & Paste,
If you produce structures like a->b->a GLView will hang.

After node renaming or auto node naming the names of other displayed DEF/USE reference node are not
updated automatically, use View->Refresh instead.


To be done :
===========


OpenGl sorted alpha blend transparency

Use render "Mode->non lighted" as workaround.


Not planned
===========
VRML files with Macintosh end of line (\r) character

Support for the crease angle in ShapeHints, GLView will smooth all edges between
faces with some shared vertex information. 
Workaround :
     Turn on flat-shading for scenes depending on this value.

Flat shading can be forced in VRML files by using a PER_FACE normal binding in VRML 1.0
or by adding 
    vertexPerNormal FALSE
to the Nodes supporting this field in VRML 2.0.



Direct 3D Version
=================
No support for per face color. 
Color per vertex is supported in the Direct 3D RGB Emulation driver. 
Lighting is ignored on a object with color per vertex.
Morphing works best with objects up to 2048 vertices (the limitation of the size of 
Direct 3D execute buffers.)

Window resizing is slow, because all Direct 3D objects has to be recreated.

GLView VRML BoundingBox display and the extension Node Image is currently not supported.

The Ramp Driver has limitations regarding light source attributes.

Very fast texture mapping can be enabled in the RGB driver, by setting
the Direct 3D Texture format to the display format (eg. 6 5 5 or 5 5 5 for high color)
and selecting "Copy" texture blend mode in Rendering -> Texture options.

A secondary Direct 3D board (Dimand Monster,3dfx etc.) can be activated by choosing
the device on Options->Direct3D or using the "s" key (for secondary).
Switching back to the primary device is currently not supported. 
A cursor is not displayed on the secondary device.


OpenGL Version
==============

If VRML 2.0 scenes look to dark, enable the headlight in the 
"WWW->Options->headlight" dialog or using the Camera->headligt Menu command.

If scenes are looking to bright, disable scene lights or
decrease the lightscale factor from 1.0 to 0.5.

Transparent Materials are supported via Alpha-Blending,
the current limitations is, that transparent surfaces should
be at the *end* of the VRML file, to get the expected
effect.
If you enable Rendering->Options->Transparency Transparent objects will be
rendered after all non transparent objects. 


If you enable 16 Bit z-buffer, rendering could look a bit strange.
Workaround: Enter an higher number into Camera-Viewpoints->znear (e.g. from 0.0001 to 1.0) and also lower the zfar number.

In some certain instances GLView is unable to compute a proper scene bounding box 
resulting in a badly adapted z-buffer range. Part of a scene may be invisible. 
This could happen if the VRML file specifies incorrect values for the bboxSize bboxCenter fields of Group nodes like inlines.
To solve this problems a depth range can be recomputed in the Camera->Viewpoint menu 
by clicking the small button right to the znear/zfar values.


If Netscape is hanging because of some problem, GLView can hang too on opening of local files.
GLView trys to get information on the URL of the file from Netscape.
Solution : Kill the hanging Netscape task.
Similar if Internet Explorer is open, Internet Explorer might block GLView due to a DDE Request
targeted to Netscape, close Internet Explorer in this case.


Matrox Graphics boards can do OpenGL acceleration only in some display modes and with a 16 Bit
Z-buffer and the right amount of Memory installed.

Enable a 16-Zbuffer by enabling "Options->Config->16 Bit z-buffer" and restart GLView.
Verify that "Options->GL Info" shows the Matrox driver instead of the Microsoft driver.
If this is not working  try to run GLView with the /x parameter and choose a GL Pixelformat via 
Options->Pixelformat->set depthBits 16 + double-buffer. 

For some data files backface culling must be disabled via Rendering->Options->backface cull.  
For DXF, RAW and GEO files this is done automatically. 
If the picture is still very dark (e.g. with RAW or GEO files) flip the computed normals  
of the object using Rendering->Options->flip normals 

Not all custom texture parameter functions available in the dialog Texture->Parameters are fully implemented.

Rendering with fog enabled seems to be strange, because for a object in the center fog is applied on 
both sides in front and in the back of the viewer. This seems to be an OpenGL problem. 

Some 2D DXF primitives are not converted or not converted correctly and objects with a EKS 
transformation are not transformed correctly. In order to do operations on a DXF-File after import, the 
dataset stored in a display tree must be converted to one single object using the Tools->Combine 
command. 
DXF-Files with optimal import result should contain 3d meshes, 3d polyface meshes (options or the 
polyline entity) or 3d face primitives. 
DXF files can currently not be saved as VRML with Layer and object information preserved.
But they can be saved as one large IndexedFaceSet primitive with local colors applied.

If VRML scene lights are enabled, scenes are often to bright. (e.g. WCVT2POV put *several*
point lights in the scene.) The options are to darken all lights by a factor or to disable
VRML scene lights. This can be done using the WWW->Options dialog.

If the WALK speed seems to be strange, adapt it to the scene by change the value 
"Camera->Viewpoints->distance" or "Camera->Viewpoints->speed".

For authors I recommend to explicitely specify focalDistance nearDistance and farDistance for
supplied VRML 1.0 camera nodes.

The default setting for Walk and pan is "locked to ground plane", movement is parallel to
the ground plane. If you want to walk directly in the viewing direction disable check 
"Camera->Relative Move".   If the scene has a camera with something like a 45 degree look 
on some scene you will need to check this option, if you want to approach the object.


Keyboard Keys
=============

<ESCAPE>	terminate fullscreen rendering (Direct 3D)
			else stop animation
			else disable texturing
			else stop WWW download

 <SPACE>	open viewpoint menu

 PAGE UP	next/prev viewpoint in VRML world
 PAGE DOWN

 LEFT RIGHT	camera movements depending on viewer mode
 UP DOWN
 + -

 f			full screen toggle 
 s			toggle to secondary display adapter (D3D)

 j			jump to object on next click
 m			move camera mode
 w			walk " "
 o			orbit " "




Quick Tour
==========
Start the program
drag around with the mouse 
(this is camera->rotate mode)


    camera->reset 

will bring back the default view


Rendering
=========
try render menu:
    "render->wireframe"         or toolbar button
    "render->flat"                    "
    "render->gouraud"                 "
    "render->vertices"
    "render->bounding box"

..
now once again

    render>wireframe
    & render->lighted   you get a lighted wireframe display

Go back to
    
    "render->gouraud" 

mode.

Material
========

try the Material menu

    material->load  

choose a predefined material from the list box

    material ->edit
    click with the mouse in the diffuse entry field,
    now press EDIT to edit the diffuse color using the Windows color dialog box
    choose a color , Exit & Apply

To change the background color use
    material->Background


Texture Mapping
===============

To load a texture use

    material->Load texture

choose a texture e.g. brick.rgb

To change OpenGL texture mapping parameters

    material->Texture Options

Enter into the "scale" edit field:

    3 3 3 <Enter>

The texture uvw parameters are scaled by (3,3,3) resulting in a smaller texture image.

    0.3 0.3 0.3 <Enter>

will make the texture larger.

Close the dialog using <Escape> or "Close".  

Try the filtering options only if you have time or a hardware acellerator

Switch off texture mapping using 
    Render->textured    or toolbar button


Special effects
===============

Deforming
=========
On a Standard 3DV model (e.g. ronny the default data set)
Say 
    Tools->deform->sphere
    enable the check boxes for enable and drag + Press "Apply"

    drag around with the mouse, the model should deform to a imaginery sphere.
    Use shift + drag to drag in the z-direction

    If you do no see an effect, change the radius edit field to higher numbers (negative
    numbers (e.g. -0.9 )are also working, the points on the surface are moving away from
    the sphere).



Morphing
========
On a standard objecttype data sets (e.g. ronny.3dv, cow.3dv, ...)
Open
    Tools->Morph
    "OK"

The vertices of the object are dynamically interpolated into a sphere.

Open 
    Tools->Morph
again to change morph options.

Use the t0 t1 sliders to select an interestinng sub-range of the animation.
Choose a different Morph Type like 
    "Cylinder", "Box", "Cone" ...
    + press "Apply"


Note that the default alignment axis of the morph is the z-Axis == up, 
but standard objects are aligned y-axis up.

As an example on how to apply a custom morph axis, see the Morphed*.wrl
GLView VRML examples.

You can stop the animation by toggling the play toolbar button.

It큦 possible to save the intermediate shapes to VRML or 3DV.
Intermediates Shapes of the deform process, are currently not available for save.

Advanced:
Some types for image warping is also possible using the morph node. Creating
a VRML file with a fine meshed surface. E.g. by using GLView->File->New Object
create a fine tesselated plane, apply your texture and include a morph node.
Example HeidiMorph.wrl




To create a spinning logo :
==========================

    "File->NewFontLogo"


    enter the logo text.

    "ok"

a 3D logo should now come up.

Press "Camera->Spin" to let the logo spinning.

If you want it textured check the "Texture Parameters" check box in the dialog, and load
a texture like brick.rgb with Material->LoadTexture

It큦 possible to create some funny symbols by using addtional TrueType fonts from
CorelDraw or similar programs, and using symbol fonts like windings.

Try also to use different fonts, or font styles like Bold, Italic.

To create some symbols for custom vrml files, choose a wingdings fonts and lookup a
character code e.g. by using the Windows->Accessories->Character Map program.




To create an 3D Landscape from Image
====================================
Press "File->New Object"
select "Heightfield from Image"
select bitmap image as source for landscape.

Use Tools->Tree editor to change height scaling.



Basic navigation:
================
For a "one object" data file or any scene after Camera->Reset has been applied
use Camera->Orbit mode
    Mouse Movements orbits or spins the viewer around the object
    Control + horizontal movement zooms
    Shift + horizontal movement rolls , 

    Cursor keys are also orbiting around the object
    + and - are zooming

    Movement is always relative to the point where the mouse
    first was pressed.


For a "scene" data file like a room or house etc.
use Camera->Walk mode
    vertical mouse movements are moving in and out the scene
    horizontal mouse movements are turning the view direction to the left or right
    Shift + vertical mouse movements are turning the view direction to up or downt
    Control +  mouse movements are moving left/right up/down

    Cursor keys are also moving in/out / moving left/right 
    <Control>Cursor keys are also turning up/down turning left/right
    <Shift> Cursor keys are also orbiting around the current camera target
    + and - are moving up/down




Internet Access
===============
If you have direct Internet access, GLView can load Internet VRML + Texture files.

Choose an Internet VRML url to open:
    a) choose or enter a url using WWW->Open Url
    b) drag a GLView vrl/url file from the file manager into the GLView Window
    c) find a VRML (wrl extension) link in your Web-Browser
        copy the link. (Netscape->Right Mouse Button->Copy this link location)
       + paste it into the url bar location field

The "Stop" Button will be enabled, the logo animation will start. After a while 
a Receiving message ... will appear. Once the file is completely retreived, GLView
will display. During Retreival you can continue, walking the current scene.
To abort network operation press the stop button. It might take a while, until
the actual operation will be finished.
GLView will place all retreived files into the GLView cache directory.
You need to manually clean up this directory.



Performance tips
================
General :
use a small window size (a default (small) size can be specified in Options->Config)
use a small dataset
use a graphics card with fast pixel transfer.

make sure Rendering -> Options -> Backface culling is checked (might be disabled by GLVIEW)
(for VRML files a ShapeHints node in the file will have precedence )

For DXF-Files use the Tools->Combine command

Texture-mapping is quite fast in non-lighted mode : select Render->Non lighted
If accepatable disable Texture Options->perspective correction

Use wireframe, or lighted vertices modes for complex data set to adjust camera,
than switch to gouraud or enable "Options->Config->degrade during move" with 
style set to bounding box.

Enable "Render->Style->Vertex->Drawstyle" only for data sets with few vertices.

Try to check options->build_tristrips before loading a file.

Try to disable VRML scene lights using the www->options dialog and/or 
try the Tools->Combine command,eventually with checking Options->Config->build triangle strips
before.

Textures image files could be prescaled to a power of two, and should not be to large.
eg. 128*128 might be enough.

If a scene is to slow with texture mapping enabled, quick disable texture mapping using
the <escape> key.



How to find more data
=====================
Internet: there are different sites with 3D data-sets,
    see also the VRML references above.

Very good 3D Models can be found and purchased from Viewpoint Data Labs.

Compuserve Forums:
    COMANIM: some DXF / Viewpoint datasets
    GRAPHDEV: RAW /OBJ/DXF + Tools for creating raw files e.g. BlobSculptor

for size reasons the evaluation version contains only a few small to modest size models.



3DV-Fileformat
==============

Please the document 3dfmt.txt



RAW fileformat
==============
each line describes a triangle with explicit coordinates

x1 y1 z1 x2 y2 z2 x3 y3 z3

GEO fileformat
==============
Header :
#of verts #of faces scalefactor(1)
Vertex table :  length # of verts 
    x y z                   <== coordinate of vert i
Face table : length # of faces 
    facelength vertexindices for faced  


Note: vertex indices the in face table are starting from 1


facetn facent



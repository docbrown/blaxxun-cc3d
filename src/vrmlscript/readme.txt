
Some modifications, javscript language features
and new object model (vsfields.*) done by
holger@blaxxun.com
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

VRMLScript Parser and Interpreter
Copyright 1996, Silicon Graphics, Inc.
ALL RIGHTS RESERVED

Author(s)    : Chris Marrin (cmarrin@sgi.com)
               Jim Kent (jkent@sgi.com)
--

UNPUBLISHED -- Rights reserved under the copyright laws of the United
States.   Use of a copyright notice is precautionary only and does not
imply publication or disclosure.

U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:  Use, duplication or
disclosure by the Government is subject to restrictions as set forth in
FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013
and/or in similar or successor clauses in the FAR, or the DOD or NASA
FAR Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.

Permission to to use, copy, distribute, and modify this software is
hereby granted without fee, provided that:  (i) the software and any
modification thereof is designed for use only in connection with the
parsing/interpretation of VRMLScript,
(ii) the above copyright notices and this permission notice appear in
all copies of the software, any modification thereof, and related
documentation, and (iii) the name of Silicon Graphics may not be used
in any advertising or publicity relating to the software without the
specific, prior written permission of Silicon Graphics.

THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
EVENT SHALL SILICON GRAPHICS BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT
ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

Please place the following credit in 
  1) About Box,
  2) README file, and
  3) Documentation,
for the product(s) you develop using the VRML 2.0 Draft 2.0 Parser:

  "Portions of this software are based in part on the VRMLScript Interpreter
   written by Silicon Graphics, Inc., Mountain View, California, USA."


=======================================================================

VRMLScript Parser and Interpreter
===============

What you get:
-------------

Source code (C++, Lex, and Yacc) that parses VRMLScript according to the
VRMLScript reference (http://vrml.sgi.com/moving-worlds/spec/vrmlscript.html).
Error handling is done using the facilitied available in YACC and LEX.
The result of the parsing operation is a set of data structures which can
be executed using the included interpreter.

Source code for a sample usage of the code, "interp.cpp". This is a simple 
command-line test program that does the following:
  -- parses the program included on the command line.
  -- creates dummy eventOuts and fields
  -- executes an eventIn function with a variety of input values

What you need:
--------------

Microsoft Developer Studio, version 4.0 or above.

Optional:

Flex and Bison, the GNU implementations of Lex and Yacc.  The C++
source code for the lexer and parser are included in this distribution,
so Flex and Bison are only really necessary if you are modifying the
parser/lexer.  Source for Bison and Flex is available from many locations, 
for example:
	ftp://gatekeeper.dec.com/pub/GNU/
Executable versions of Flex and Bison for Windows 95
are available from:
	ftp://ftp.winsite.com/pub/pc/win95/programr/flexbison.zip


Support/questions:
------------------

This is free software and, therefore, is not officially supported by 
Silicon Graphics Inc.   That said, as we update the code with fixes
supplied by us or others, we will make available the updated versions.
If you port this code to any other platforms we will publish a pointer to
your site.

Please feel free to post bug reports and bug fixes via email to 
cosmoplayerbugs@sgi.com and/or via the newsgroup
news://vrml.sgi.com/sgi.cosmoplayer.


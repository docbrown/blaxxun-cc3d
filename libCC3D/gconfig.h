/******************************************************************************
GConfig.h		-  config info

$Revision: 1.6 $
$Log: gconfig.h,v $
Revision 1.6  1999/04/07 15:14:43  tom
empty message



******************************************************************************/





//#define _COM
#define _GV
#define _VRML
#define _WWW

// TEMP 
//#undef _EVALUATION

#define _G_VRML1		// VRML 1.0 Node
#define _G_INVENTOR
#define _G_VRML2
#define _G_MPEG4		// additional nodes from MPEG4


// test 
// uncomment for deletion of VRML 1.0 / inventor nodes uspport 
// #undef _G_VRML1		// VRML 1.0 Node
// #undef _G_INVENTOR


// image  types
#define _IMAGE
#define _RGB
#define _JPEG
#define _PNG
#define _TARGA
#define _GIF
#define _DIB
#define _PPM

#define _PCX
#undef _PCX




// GLView light 
#if defined(_GLVIEW_LE) || defined(Linux)

#define _3DV
#undef _DXF
#undef _RAW
#undef _GEO
#undef _HMF
#undef _OBJ
#undef _POV
#undef _STL
#undef _RGB

#elif defined(_GLVIEW_IE)

#undef _3DV
#undef _DXF
#undef _RAW
#undef _GEO
#undef _HMF
#undef _OBJ
#undef _POV
#undef _STL

// #define _RGB
#undef _RGB

#define _GV_MINIMAL

#else

#define _DXF
#define _RAW
#define _GEO
#define _OBJ
#define _STL
#define _POV
#define _RGB
#define _3DV

#endif

#ifdef WIN32

// sound
// RSX 
//#define G_RSX

// Microsoft Direct Sound
#define G_DSOUND

// Active Movie  / Direct X Media
#define G_AMOVIE

// Real Player G2 RMA Movie 
//#define G_RMA_MOVIE


#endif



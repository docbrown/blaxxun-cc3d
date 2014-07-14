

// mesa GLU is outdated
// try some workarounds

#ifndef GLU_VERSION_1_2 

#define GLUtesselator GLUtriangulatorObj 

#endif


// new in Mesa 3.1 ?
#ifdef GLCALLBACK

#	ifndef CALLBACK
#		define CALLBACK GLCALLBACK
#	endif

#else
#	ifndef CALLBACK
#		define 	CALLBACK
#	endif
#	define GLCALLBACK CALLBACK
#endif


/****           Callback function prototypes    ****/

/* gluQuadricCallback */
typedef void (GLCALLBACK* GLUquadricErrorProc) (GLenum);

/* gluTessCallback */
typedef void (GLCALLBACK* GLUtessBeginProc)        (GLenum);
typedef void (GLCALLBACK* GLUtessEdgeFlagProc)     (GLboolean);
typedef void (GLCALLBACK* GLUtessVertexProc)       (void *);
typedef void (GLCALLBACK* GLUtessEndProc)          (void);
typedef void (GLCALLBACK* GLUtessErrorProc)        (GLenum);
typedef void (GLCALLBACK* GLUtessCombineProc)      (GLdouble[3],
                                                  void*[4], 
                                                  GLfloat[4],
                                                  void** );
typedef void (GLCALLBACK* GLUtessBeginDataProc)    (GLenum, void *);
typedef void (GLCALLBACK* GLUtessEdgeFlagDataProc) (GLboolean, void *);
typedef void (GLCALLBACK* GLUtessVertexDataProc)   (void *, void *);
typedef void (GLCALLBACK* GLUtessEndDataProc)      (void *);
typedef void (GLCALLBACK* GLUtessErrorDataProc)    (GLenum, void *);
typedef void (GLCALLBACK* GLUtessCombineDataProc)  (GLdouble[3],
                                                  void*[4], 
                                                  GLfloat[4],
                                                  void**,
                                                  void* );

/* gluNurbsCallback */
typedef void (GLCALLBACK* GLUnurbsErrorProc)   (GLenum);







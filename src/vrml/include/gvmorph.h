/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/
#ifndef  _GvMorph_
#define  _GvMorph_

#ifdef _G_VRML1

// HG 
#include <gvsfenum.h>
#include <gvsfvec2f.h>
#include <gvsfvec3f.h>
#include <gvsffloat.h>
#include <gvsfbool.h>
#include <gvsfrotation.h>
#include <gvgroup.h>
#include <gvsubnode.h>
#include <gvsfnode.h>

class GMorphObject;


class GvMorph : public GvGroup {

    GV_NODE_HEADER(GvMorph);

  public:
	enum MorphType {
		NONE,
		MELT,
    	SPHERE,
 		CYLINDER,
		CONE,
		BOX,
		HANTEL,
 		WAVE,
		WAVING,
		SWIRL,
        PINCH,
        TAPER,
        TWIST,
        BEND,
		DNOISE,
		MAP_MAX,
	};

    // Fields
	GvSFEnum		type;
	
    GvSFBool		local;	// morph each object to its own bounding box
	
    GvSFNode        transform;  // transformation node 
    GvSFRotation    rotation;   // single rotation field 

   
    GvSFBool		hermite; // use hermite interpolation using vertex normals
    GvSFBool		intersect;

    // animation control values      
    GvSFFloat	speed;
	GvSFBool		on;
    GvSFBool    cycle;  // cycle time internally back / forth

    GvSFFloat   t0;    // morph only in this intervall  
    GvSFFloat   t1;
   
    Point scenter;

    float sradius;
    float sradius2;

    GvSFFloat rscale; //UI
      
      /* hermite interpolation */
      GvSFFloat nscale1; // UI
      GvSFFloat nscale2;

      /* wave */
      GvSFFloat wave_nwaves; // UI
      GvSFFloat wave_offset;
      GvSFFloat wave_radius; // UI

      /* swirl */
      GvSFFloat swirl_nwaves;


	  /* waving */
	  GvSFVec2f amplitude1;		// ammount of waving, begin/end
	  GvSFVec2f amplitude2;		// ammount of waving, begin/end
	  GvSFFloat phaseShift1;	// ammount of phaseShift 1
	  GvSFFloat phaseShift2;	// ammount of phaseShift 2
	  GvSFVec2f phaseScale1;	// ammount of phaseScale begin/end
	  GvSFVec2f phaseScale2;	// ammount of phaseScale begin/end



      int has_map_matrix;
      Matrix map_matrix,inverse_map_matrix;


	  int init;
	  Array<GMorphObject*> objects;

	  // reset parameters to default value
	  void ResetParams(void);

	  // compute in between for time t
	  virtual int Do(GMorphObject &object, float t); 
	
     int Do(GTraversal &state);        
     
     int Do(GRenderTraversal &state);   
	  int Do(GglRenderTraversal &state);   


		  
   int InterpolatePoints(
        	int mode,
        	float t,
        	int np,
        	const Point *p1,const Point *n1,
        	const Point *p2,const Point *n2,
        	Point *pout, Point *nout );

	int WavePoints(
        int mode,
        double t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout);
	
	int WavingPoints(
        int mode,
        double t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout);



	int InterpolatePointsToSphere(int mode,
        float t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout);

	int SwirlPoints(
        int mode,
        double t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout);
   
   // apply deformation functions on points
   int DeformPoints(
        double t,       
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout, 
        int (*func)(const Point *p,float amount, Point *pout));

	

};
#endif _G_VRML1


#endif /* _GvMorph_ */

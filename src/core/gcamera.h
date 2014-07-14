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
// GCamera Class header file

#ifndef _gcamera_h
#define _gcamera_h

//#include "point.h"

class GvPerspectiveCamera;
class GvOrthographicCamera;
class GvViewpoint;


class GCamera {
public:

   static float znearFactor; // a factor to compute znear from zfar : znear = zfar * znearFactor
   // Limiting values
   static float panMax;
   static float orbitMax;
   static float zoomMax;
   static float rollMax;
   static float dollyMax;

   
   // compute znearFactor based on z-buffer depth 
   static float SetZNearFactor(int zbufferDepth=24);
    
   Point position;			// camera position = eye location
   Point target;			// camera target point = destination
   Point up;				// 
   float width,height;		// field, size of the view rectangle in view plane
   int perspective;			// perspective projection flag

   float znear,zfar;        // near far clipping planes

   // additional variables to provide a absolute frame of movement
   Point worldUp;			// absolute up-vector
   float worldUpDist;       // distance from origin together with worldUp defines a plane
   Point worldCenter;		// center of world
   BBox  worldBBox;         // world bounding box

   BBox  worldLimit;        // world bounding box limit

   Point orbitCenter;		// center point for orbit 

   // computed matrices
   Matrix viewMatrix;		// transform world to viewpoint
   Matrix projectionMatrix;	// project 

   int world2pValid,p2worldValid;
   Matrix world2p;			// world 2 projection (device) space
   Matrix p2world;			// projection 2 world space

   // get matrix transforming from world 2 projection (device) space
   const Matrix& World2p();
   const Matrix& P2World();

   // transform point from world 2 projection space
   void Project(const Point &pin, Point &pout);
   int  UnProject(const Point &p, Point &pout);

	// compute a ray defined by start & ending point in world space
	// input : x,y coordinate in normalized view space 0..1


   // output objP, objP1 start / end point 
	void ComputeWorldRay(float x,float y,Point &objP,Point &objP2,
							  float znear=0.0,float zfar=1.0);


   void SetWorldReference(const BBox &bbox);
   void SetWorldReference(const BBox &bbox, const Point &center, const Point &up, float distance=0.0);
   void ComputeWorldUpFromUp();
   
   // Limit view to this box
   void SetWorldLimit(const BBox &bbox,float scale=1.0);



   float aspect; 			// Aspect ratio of screen window
   float fovy;				// Field of view
   
   float widthPixel,heightPixel;	// size of window in pixel
   float widthWorld,heightWorld; 	// actual field, after checking for aspect

   GCamera();

   GCamera& SetByVolume(float x1,float x2,float y1,float y2); 
   GCamera& SetByVolume(const Point &p1,const Point &p2); 
   
   GCamera& SetAspect(float Aspect) { aspect = Aspect; return(*this); }

   GCamera& operator+=(const Point &p);
  
   GCamera& OrbitX(float dw,int worldRelative=0);  	// Rotate position around target
   GCamera& OrbitY(float dw,int worldRelative=0);
   GCamera& PanX(float dw,int worldRelative=0);		// rotate target around position
   GCamera& PanY(float dw,int worldRelative=0);

   // move in x /y Z direction
   GCamera& DollyX(float d,int worldRelative=0);
   GCamera& DollyY(float d,int worldRelative=0);
   GCamera& DollyZ(float d,int worldRelative=0);
   
   // zoom by camera field
   GCamera& Zoom(float f); 


   // rotate up vector along viewaxis
   GCamera& Roll(float dw);	 
   
   // Zoom distance to target
   GCamera& ZoomDistance(float f);


   // look left/right, up/down
   // world relative means, axis reference is worldUp instead of up
   GCamera& Pan(float wx,float wy,int worldRelative=0); 

   // rotate around target
   GCamera& Orbit(float wx,float wy,int worldRelative=0); 

   // rotate around orbitCenter
   GCamera& OrbitCenter(float wx,float wy,int worldRelative=0); 
   
   // move left/right up/down in/out
   GCamera& Dolly(float dx,float dy,float dz,int worldRelative=0);

   GCamera& PanMax(float wx,float wy,int worldRelative=0) 
   {
       if (wx<-panMax) wx = -panMax; else if (wx>panMax) wx =panMax;
       if (wy<-panMax) wy = -panMax; else if (wy>panMax) wy =panMax;
       return(Pan(wx,wy,worldRelative));
   }       

   GCamera& OrbitMax(float wx,float wy,int worldRelative=0) 
   {
       if (wx<-orbitMax) wx = -orbitMax; else if (wx>orbitMax) wx = orbitMax;
       if (wy<-orbitMax) wy = -orbitMax; else if (wy>orbitMax) wy = orbitMax;
       return(Orbit(wx,wy,worldRelative));
   }       

   GCamera& OrbitCenterMax(float wx,float wy,int worldRelative=0) 
   {
       if (wx<-orbitMax) wx = -orbitMax; else if (wx>orbitMax) wx = orbitMax;
       if (wy<-orbitMax) wy = -orbitMax; else if (wy>orbitMax) wy = orbitMax;
       return(OrbitCenter(wx,wy,worldRelative));
   }       


   GCamera& DollyMax(float dx,float dy,float dz,int worldRelative=0) 
   {
       return(Dolly(dx,dy,dz,  worldRelative));
   }

   // zoom by camera field
   GCamera& ZoomMax(float f) 
   {
       return(Zoom(f));
   }       


   // rotate up vector along viewaxis
   GCamera& RollMax(float dw)
   {
       if (dw<-rollMax) dw = -rollMax; else if (dw>rollMax) dw =rollMax;
       return(Roll(dw));
   }       



   // vector pointing to position
   Point Direction() { return(position-target); };
   Point NormDirection () {  Point dir =position-target; 
   							dir.Normalize();
							return(dir);
   						 };

   // set direction to target 
   void SetTargetDirection(const Point &dir);
   void CheckUpVector();

   // vector pointing to target
   Point TargetDirection () { return(target-position); };
   Point NormTargetDirection() {  Point dir =target-position; 
   							dir.Normalize();
							return(dir);
   						 };

   // Vector pointing to the "right", the x axis of the camera 
   Point Right() { return CrossProd(up,NormDirection()); };
   Point NormRight() { Point dir(Right());
   					   dir.Normalize(); 
   					   return(dir);
   					 };

   // Vector pointing to the "right", in world x axis of the camera 
   Point WorldRight();
   Point WorldNormRight();

   // vector pointing to target, in world "z" axis of the camera
   Point WorldTargetDirection ();
   Point WorldNormTargetDirection();

   
   float TargetDistance() { Point dir=Direction(); return(dir.Len()); }

   double GetFovY();

   // Compute the  view orientation matrix
   int GetViewMatrix( Matrix &m);

   
   Point &Up() { return(up); }
   Point &Target() { return(target); }
   Point &Position() { return(position); }

   
   // Changed management
   int changed;     // each time setting is change, increment
   int changeLevel;
   gbool isDefault;   // current setting is a default camera computed
   gbool zrangeIsDefault;   // current z-range setting is a default camera computed
   gbool targetDistanceIsDefault; // camera target distance is default
   
   // recompute z range, to view scene
   int SetZRange(const BBox &bbox);

   // recompute target distance based on bbox
   int SetTargetDistance(const BBox &bbox);
   
   // call on each change, to invalidate state of computed variables
   virtual void OnChanged();

   gbool Changed() const { return(changed>0); }
   
   gbool IsDefault() const { return(isDefault); }
   
   // set size of output window in pixel to allow for coordinate transformation
   void SetFieldPixel(int width,int height)  // if with = dx, call with width=dx-1
   		{ widthPixel = width;
		  heightPixel = height;
		}

   // Transform distance from pixel to world space 
   void Pixel2WorldDelta(const Point &ppixel, Point &pworld) 
   { pworld.Set(widthWorld * ppixel.x / widthPixel,
				 heightWorld * ppixel.y / heightPixel,0.0);
   }

   // transform by matrix
   GCamera& Transform(const Matrix &m);

   // set by a GvLib cameraNode 
   int  Set(GvPerspectiveCamera *c,const Matrix *cameraTransform=NULL); // in GModel.c
   int  Set(GvOrthographicCamera *c,const Matrix *cameraTransform=NULL); // in GModel.c
   int  Set(GvViewpoint *c,const Matrix *cameraTransform=NULL); // in GModel.c

   // compute the view & projection matrix
   int UpdateViewProjectionMatrix();

   //gl
#ifdef _OGL
   void glSet();
#endif


};


// Information about view transformation

class GViewInfo 
{
public:
    BBox viewVolume;

    MatrixAndInverse view;
    MatrixAndInverse cam;
    
    MatrixAndInverse object;
    
    int object2viewOk;
    MatrixAndInverse object2view;

    int View2World(const Point &p, Point &result);
    int World2View(const Point &p, Point &result);

    int World2Cam(const Point &p, Point &result);
    int Cam2World(const Point &p, Point &result);
    
    int View2Object(const Point &p, Point &result)
    {
        const Matrix& m=View2Object();
        result = m * p;
        return(1);
    }

    int Object2View(const Point &p, Point &result) 
    {
        const Matrix& m=Object2View();
        result = m * p;
        return(1);
    }



    int glGet();



    GViewInfo() { object2viewOk=0;}

    void SetObject(const Matrix &m) {
        object.Set(m);
        object2viewOk=0;
    }
    
    void SetView(const Matrix &m) {
        view.Set(m);
        object2viewOk=0;
    }

    void SetCam(const Matrix &m) {
        cam.Set(m);
        //object2viewOk=0;
    }


    const Matrix & Object2View() 
    {
        if (!object2viewOk) {
            object2view.m = object.m;
            object2view.Append(view.m);
        }
        return (object2view.M());
    }

    const Matrix & View2Object() 
    {
        if (!object2viewOk) {
            object2view.m = object.m;
            object2view.Append(view.m);
        }
        return (object2view.Inverse());
    }


};




#endif


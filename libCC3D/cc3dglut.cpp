/*
 * CC3D minimal demo
 * using GLUT
 *
 * started from glutskel.c
 * A skeleton/template GLUT program - Written by Brian Paul and in the public domain.
 */

// CC3D global headers
#include "stdafx.h"
#include "greporter.h"
#include "gversion.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// GL / Mesa includes

//#include <GL/gl.h>
#include <GL/glut.h>


//#include <stdio.h>
#include <stdlib.h>

// CC3D includes

#include "glrender.h"
#include "gmodel.h"
#include "gvdb.h"


// the CC3D toplevel view object
static GView *view=NULL;


static GLfloat Xrot = 0, Yrot = 0, Zrot = 0;
static GLboolean Anim = GL_FALSE;



// idle function
static void OnIdle( void )
{

	// check for a camera animation
    if (view->AnimatingCamera()) {
        if (view->AnimateCameraStep()==2) {
        };
		view->setRedraw();
    }
    view->TriggerTimeSensors(); // update browser time stamp and trigger timesensors
	
    if (view->getRedraw()) // we should redraw
       glutPostRedisplay();
}

static void Idle( void )
{
   Xrot += 0.0;
   Yrot += 2.0;
   Zrot += 2.0;

   GCamera &camera = view->GetCamera();
   camera.Orbit(Yrot*0.01,Yrot*0.01);
   view->setRedraw();

   OnIdle();
}


// repaint
static void OnPaint( void )
{
   if (view) {
	RECT r;
	view->DrawScene(r);
   }	

   glutSwapBuffers();
}


static void OnSize( int width, int height )
{
   if (view)
       view->Resize(width,height);
}

static float rotateStep = 0.2;
static float walkStep = 0.2;

static void OnCameraSeekto(int x, int y) 
{
   if (view->Select(x,y,0,GView::JUMP_OBJECT)) {
		//Message("Jumping to object");
		//Redraw();
   }
}

static void OnMouseDblClick(int x, int y,int nFlags)
{
    if (view->Select(x,y,nFlags,GView::JUMP_WWWAnchor)) {
 	  // camera selection is already done	at this stage 
	  if (view->executeNode) {
			//ExecuteAnchorNode((GvNode*)view->executeNode);
	  }
	}
}



static void OnKey( unsigned char key, int x, int y )
{
  
   if (!view) return;
  
   GCamera &camera = view->GetCamera();
   
   switch (key) {
      case '*':  // speed up 
	  		walkStep *= 1.414;
	  		rotateStep *= 1.414;
			break;
      case '/':  // speed down 
	  		walkStep /= 1.414;
	  		rotateStep /= 1.414;
			break;			
      case 'w':
   		 // forward 	
	 	 camera.Dolly(0,0,walkStep);
   		 break;
      case 'y':
      case 'z':
   		 // back 	
	 	 camera.Dolly(0,0,-walkStep);
   		 break;
      case 'e':
   		 // slide left
	 	 camera.Dolly(-walkStep,0,0);
   		 break;
      case 'r':
   		 // slide right
	 	 camera.Dolly(walkStep,0,0);
   		 break;
      case 'c':
   		 // down
	 	 camera.Dolly(0,-walkStep,0);
   		 break;
      case 'v':
   		 // up 
	 	 camera.Dolly(0,walkStep,0);
   		 break;

      case 's':
   		 // look left 
	 	 camera.Pan(-rotateStep,0);
   		 break;

      case 'a':
   		 // look right 
	 	 camera.Pan(rotateStep,0);
   		 break;

      case 'f':

   		 // look down 
	 	 camera.Pan(0,-rotateStep);
   		 break;

      case 'd':
   		 // look up
	 	 camera.Pan(0,rotateStep);
   		 break;
		 
      case 'j':
	 	 OnCameraSeekto(x,y);
   		 break;
      case 'q':
      case 27:
         exit(0);
         break;
		 
      default :
	  	fprintf(stderr,"OnKey %d %d %d \n",key,x,y);
		break;
		 
   }
   if (camera.Changed()) view->setRedraw();
   //glutPostRedisplay();
}


static void OnSpecialKey( int key, int x, int y )
{
 
   if (!view) return;
   
   int modifiers =  glutGetModifiers();
   //GLUT_ACTIVE_SHIFT GLUT_ACTIVE_CTRL GLUT_ACTIVE_ALT
  
   GCamera &camera = view->GetCamera();
  
   switch (key) {
      case GLUT_KEY_UP:
	 	 camera.Orbit(0,rotateStep);
         break;
      case GLUT_KEY_DOWN:
	 	 camera.Orbit(0,-rotateStep);
         break;
      case GLUT_KEY_LEFT:
	 	 camera.Orbit(rotateStep,0);
         break;
      case GLUT_KEY_RIGHT:
	 	 camera.Orbit(-rotateStep,0);
         break;
		 
      case GLUT_KEY_PAGE_UP:
		 view->SetPrevViewpoint(view->cameraAnimate);
	  	 break;
		 
      case GLUT_KEY_PAGE_DOWN:
 		view->SetNextViewpoint(view->cameraAnimate);	  
	  	 break;
		 
	  case GLUT_KEY_HOME:
		if (view->AnimatingCamera()) 
			view->AnimateCameraStop();
	
		// try to reset to entry viewpoint
		if (!view->SetViewpoint(0,1))
			// no viewpoint defined, then zoom out
			view->CameraReset();
		  
	  	 break;
	  case GLUT_KEY_END:
	  	 break;
	  case GLUT_KEY_INSERT:
	  	 break;
	  default :
	  	fprintf(stderr,"OnKey %d %d %d \n",key,x,y);
		break;
		 
   }
   if (camera.Changed()) view->setRedraw();
   //glutPostRedisplay();
}

static void OnMouse(int button, int state, int x, int y )
{
 
   if (!view) return;
   
   int modifiers =  glutGetModifiers();
   //GLUT_ACTIVE_SHIFT GLUT_ACTIVE_CTRL GLUT_ACTIVE_ALT
  
   GCamera &camera = view->GetCamera();
   
   printf("OnMouse %d  state = %d (%d %d) \n",button,state,x,y);
   switch (button) {
   case 0 : {
	    }
	    break;	
   case 1 : {
	    }
	    break;	
   }
   
}

static void OnMotion(int x, int y )
{
    if (!view) return;
   
   int modifiers =  glutGetModifiers();
   //GLUT_ACTIVE_SHIFT GLUT_ACTIVE_CTRL GLUT_ACTIVE_ALT
  
   GCamera &camera = view->GetCamera();
}

static void OnVisibility(int state)
{
	printf("ON Visibility state = %d \n",state);
}


// load a new world, for now only local files are supported in GFile for Linux

static int LoadWorld(const char *url, const char* homeUrl = NULL)
{
	int ret;
	GFile *loader;
	
	loader = new GFile();
	if (!loader) return -1;
	
	loader->ref();
	
	if (homeUrl) {
		loader->SetHome(homeUrl);
		loader->SetRefedFrom(homeUrl,"Load");
	}
   	
	loader->Set(url,0,0);

   	// process url 
   	ret = loader->Process();
	
	// we should have some local file name 	
    	if (ret >=0) {
		if (loader->localFileUnzipped.GetLength()>0) {
			ret = view->ReadModel(loader);
			if (ret>=0) {
				view->setRedraw();
			}				
		}	
	}
	loader->unref();

	return ret;
}


const char *helptext = 
"a tiny VRML browser\n"
"by holger@blaxxun.com\n"
"Keyboard controls: \n"
"\ta s/ d f - look left-right / up-down\n"
"\t * / walk speed \n"
"\t w z/ e r / c v - walk in-out / left-right / up-down\n"
"\t Page Up / Down / Home - viewpoints\n"
"\t q <ESC> - exit\n"
;


void help(const char *progname=_PROGRAM)
{
	printf("usage : %s someVrmlFile.wrl\n",progname);

	printf(helptext);

}


int main( int argc, char *argv[] )
{
   int argi=1;	
   int ret=0;

   // init reporter object (needed !)

   GReporter *reporter = new GReporter();
   GReporter::current = reporter;
   
   // init node tables
   GvDB::init();

   // create a view	
   view = new GView();
   if (!view) return(-1);	
   //view->SetReporter(reporter);

   // glut init stuff
   
   glutInit( &argc, argv );
   glutInitWindowPosition( 0, 0 );
   glutInitWindowSize( 400, 400 );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutCreateWindow(argv[0]);
   glutReshapeFunc( OnSize );
   glutKeyboardFunc( OnKey );
   glutSpecialFunc( OnSpecialKey );
   glutDisplayFunc( OnPaint );
   
   // more 
   glutMouseFunc( OnMouse );
   glutMotionFunc( OnMotion );
   // glutEntryFunc
   // glutMenuStateFunc
   glutVisibilityFunc( OnVisibility );
   // joystick etc
   
   // init view	
   view->Initialize((HWND) NULL,NULL);
   view->Resize( 400, 400 );

   ret=0;
   
   if (argc==1 || (argi<argc && streq(argv[argi],"-h"))) {
   	   argi++;
	   help(argv[0]);
   }

   // file argument ????????????ß
 
   if (argi<argc) {
	 ret=LoadWorld(argv[argi]);
	 argi++;
   }	  
   else {  
	//ret=LoadWorld("/home/holger/x/0cache/www.blaxxun.com/vrml/avatars/boygirl/boygirl.wrl");
	//ret=LoadWorld("/home/holger/x/0cache/catilaporte.com/vrml/TalentShow/catizap.wrl");
   }
   
   if (ret<0)  {
	reporter->Error("Can not read input file");
	exit(-1);
   }		                        
   
   // the idle function, sampling timeSensors & checking the redraw flag
   glutIdleFunc(OnIdle);

   glutMainLoop();

   // cleanup	
   if (view) {
	view->Term();
	delete view;
	view = NULL;
   }	
   GvDB::term();

   return 0;
}


/*
 * CC3D minimal GLX Mesa demo 
 * 
 *
 */


// CC3D global configuration
#include "stdafx.h"

// GL / Mesa includes

#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>
#include <stdlib.h>

// CC3D includes

#include "glrender.h"
#include "gmodel.h"
#include "gvdb.h"


// the CC3D toplevel view object
static GView *view;

static GLXContext ctx;

static void redraw( Display *dpy, Window w )
{
   printf("Redraw event\n");

   if (view) {
		RECT r;
		view->DrawScene(r);
   }	
   glXSwapBuffers( dpy, w );
}



static void resize( unsigned int width, unsigned int height )
{
   printf("Resize event %d %d\n",width,height);

   glViewport( 0, 0, width, height );
   if (view)
       view->Resize(width,height);
}



static Window make_rgb_db_window( Display *dpy,
				  unsigned int width, unsigned int height )
{
   int attrib[] = {
            GLX_RGBA,
		    GLX_RED_SIZE, 1,
		    GLX_GREEN_SIZE, 1,
		    GLX_BLUE_SIZE, 1,
		    GLX_DOUBLEBUFFER,
		    None
   };

   int scrnum;
   XSetWindowAttributes attr;
   unsigned long mask;
   Window root;
   Window win;
   XVisualInfo *visinfo;

   scrnum = DefaultScreen( dpy );
   root = RootWindow( dpy, scrnum );

   visinfo = glXChooseVisual( dpy, scrnum, attrib );
   if (!visinfo) {
      printf("Error: couldn't get an RGB, Double-buffered visual\n");
      exit(1);
   }

   /* window attributes */
   attr.background_pixel = 0;
   attr.border_pixel = 0;
   attr.colormap = XCreateColormap( dpy, root, visinfo->visual, AllocNone);
   attr.event_mask = StructureNotifyMask | ExposureMask;
   mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

   win = XCreateWindow( dpy, root, 0, 0, width, height,
		        0, visinfo->depth, InputOutput,
		        visinfo->visual, mask, &attr );

   ctx = glXCreateContext( dpy, visinfo, NULL, True );

   glXMakeCurrent( dpy, win, ctx );

   return win;
}


static void event_loop( Display *dpy )
{
   XEvent event;

   while (1) {
     XNextEvent( dpy, &event );

     switch (event.type) {
	 case Expose:
	    redraw( dpy, event.xany.window );
	    break;
	 case ConfigureNotify:
	    resize( event.xconfigure.width, event.xconfigure.height );
	    break;
      }
   }
}



int main( int argc, char *argv[] )
{
   	Display *dpy;
   	Window win;

   	dpy = XOpenDisplay(NULL);

   	win = make_rgb_db_window( dpy, 300, 300 );

   	XMapWindow( dpy, win );

    GvDB::init();

	view = new GView();

    glXMakeCurrent( dpy, win, ctx ); 

	view->Initialize((HWND) win,NULL);
	view->Resize( 300, 300 );

   	event_loop( dpy );

	view->Term();
	delete view;

    return 0;
}

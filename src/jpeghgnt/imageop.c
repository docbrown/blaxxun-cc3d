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

#include <stdio.h>

#include "image.h"
#include "string.h"


/*
  iout[xdest:width,ydest:height] = iin[xstart:width,ystart:height]
  iout + iin should have same depth
  no error checking !
*/

int image_copy(image *iin,int xstart,int ystart, int width, int height,
	       image *iout,int xdest,int ydest)
{ IBYTE *o,*irow,*i,*orow;
  int y,xsize;

  xsize = width * iin->bytesperpixel;
  for ( y = height,
	i = IPIX(iin,xstart,ystart),
	o = IPIX(iout,xdest,ydest);

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     memcpy(o,i,xsize);
  }
  return(0);
}


image_apply_brush(image *iin, int x,int y, int sizex, sizey,
			int (*brush)(int i,int j,IBYTE *irow,int fmt, int * data)
		       int *data
		 )
{  int sizex2,sizex2;
   int i,j;
   IBYTE *irow;
   sizex2 = sizex / 2;
   sizey2 = sizey / 2;
   x-=sizex2;
   y-=sizey2;
   imin=0;
   jmin=0;
   if (x+sizex-1) <0
   for (j=imin,irow=IPIX(iin,x,y)
	j<sizey;
	j++,irow+=iin->bytesperscan)

	for (i=imin; i<sizex; i++)
	  brush(i,j,irow+i*iin*bytesperpixel,iin->fmt,data);


  return(0);

}

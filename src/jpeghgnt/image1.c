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
/******************************************************************************


   Copyright (c) 1990 - 1993, 1994  by Holger Grahn

   Purpose :
                image extension routines

   Functionality :

   Changes :

   Todo :


******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "image.h"
#include "image1.h"

#ifndef min

#define min(a,b)    ((a)<(b) ? (a) : (b))
#define max(a,b)    ((a)>(b) ? (a) : (b))

#endif


/* make a operation on SHORT component type images
  operations:


*/

int image_short_byte_op(image *im,int op,ISHORT *iconst,image *imsrc)
{ IBYTE *irow;
  IBYTE *isrcrow;
  int cnt;
  int channels;
  if (im->bytesperpixel == 0) return(IME_BADFMT);
  channels = im->bytesperpixel  / sizeof(ISHORT);

  /* for all rows */
  for ( irow = im->image,cnt= min(im->height,imsrc->height),
        isrcrow= imsrc->image;
        cnt >0;
        irow+= im->bytesperscan,cnt--,
        isrcrow+= imsrc->bytesperscan)
  {
    ISHORT *idestp,*idest_end;
    IBYTE *isrcp = isrcrow;
    idestp = (ISHORT *) irow;
    idest_end = idestp+im->width*channels;

    switch (op) {
    case IMOP_NOOP: break;
    case IMOP_COPY_DEST :
           while (idestp <idest_end) *idestp++ = *isrcp++;
           break;
    case IMOP_COPY_SRC :
           while (idestp <idest_end) *isrcp++ = (IBYTE) *idestp++; // truncate
           break;
    case IMOP_COPY_SRC_CLAMP :
           while (idestp <idest_end)  {
                ISHORT x;
                x = *isrcp++;
                if (x<0) x= 0;
                else if(x>255) x = 255;
                *idestp++ = x;
           }
           break;
    case IMOP_ADD :
           while (idestp <idest_end) *idestp++ += *isrcp++;
           break;
    case IMOP_SUB :
           while (idestp <idest_end) *idestp++ -= *isrcp++;
           break;
    case IMOP_MUL :
           while (idestp <idest_end) *idestp++ *= *isrcp++;
           break;
    case IMOP_DIV :
           while (idestp <idest_end) *idestp++ /= *isrcp++;
           break;
    case IMOP_MIN :
           while (idestp <idest_end)  { int a=*idestp;int b= *isrcp++;
                                        *idestp++ = min(a,b); }
           break;
    case IMOP_DIVC :
           while (idestp <idest_end) {
               switch(channels) {
               case 4:
                    *idestp++ /= iconst[3];
               case 3:
                    *idestp++ /= iconst[2];
               case 2:
                    *idestp++ /= iconst[1];
               case 1:
                    *idestp++ /= iconst[0];
               }
           }
           break;

    default :
      return (IME_BADOP);
    }
  }
  return(0);
}

#if 1

/*
*/

int image_short_op(image *im,int op,irgb_short_color *p,image *im2)
{ IBYTE *irow;
  int cnt;
#define BPERPIX 4
  if (im->bytesperpixel == 0) return(IME_BADFMT);
  if (im->bytesperpixel != BPERPIX) return(IME_BADFMT);
  /* for all rows */
  for ( irow = im->image,cnt= im->height;
        cnt >0;
        irow+= im->bytesperscan,cnt--)
  {
    ISHORT *i;
    int x;
    i = (ISHORT *) irow;
    switch (op) {
    case IMOP_NOOP: break;
    case IMOP_SETR : for (x=im->width; x>0; i+=BPERPIX, x--) { i[0]=p->r; } break;
    case IMOP_SETG : for (x=im->width; x>0; i+=BPERPIX, x--) { i[1]=p->g; } break;
    case IMOP_SETB : for (x=im->width; x>0; i+=BPERPIX, x--) { i[2]=p->b; } break;
    case IMOP_SETA : for (x=im->width; x>0; i+=BPERPIX, x--) { i[3]=p->a; } break;
    case IMOP_SETRGB:for (x=im->width; x>0; i+=BPERPIX, x--) {i[0]=p->r; i[1]=p->g;i[2]=p->b; } break;
    case IMOP_SETRGBA : for (x=im->width; x>0; i+=BPERPIX, x--) {i[0]=p->r; i[1]=p->g;i[2]=p->b;i[3]=p->a; } break;

    case IMOP_SETA_IF_RGBEQ :
        for (x=im->width; x>0; i+=BPERPIX, x--) {int eq;
            eq = (p->r == i[0]) && (p->g == i[1]) && (p->g == i[2]);
            if (eq) i[3]=p->a;
        }
        break;
    default :
      return (IME_BADOP);
    }
  }
  return(0);
}
#undef BPERPIX
#endif

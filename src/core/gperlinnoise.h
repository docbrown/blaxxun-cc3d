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

gfloat bias(gfloat a, gfloat b);

gfloat gain(gfloat a, gfloat b);

/* noise functions over 1, 2, and 3 dimensions */

gfloat Noise1(gfloat arg);
gfloat Noise2(const gfloat vec[]);
gfloat Noise3(const gfloat vec[]);
gfloat Noise(const gfloat vec[], int len);

inline gfloat Noise2(const Point vec) { return Noise2(&vec.x); }
inline gfloat Noise3(const Point vec) { return Noise3(&vec.x); }

gfloat turbulence(gfloat *v, gfloat freq);

void initNoise();

#define MAX_OCTAVES 32

gfloat
turbulence( Vector point, gfloat H, gfloat lacunarity, int octaves );





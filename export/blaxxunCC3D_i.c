/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Tue Mar 30 15:51:07 1999
 */
/* Compiler settings for E:\client\CC3D\blaxxunCC3D\blaxxuncc3d.odl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_blaxxunCC3DLib = {0x4B6E3010,0x6E45,0x11D0,{0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8}};


const IID DIID_IblaxxunCC3D = {0x4B6E3011,0x6E45,0x11D0,{0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8}};


const IID DIID_IblaxxunCC3DEvents = {0x4B6E3012,0x6E45,0x11D0,{0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8}};


const CLSID CLSID_blaxxunCC3D = {0x4B6E3013,0x6E45,0x11D0,{0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8}};


#ifdef __cplusplus
}
#endif


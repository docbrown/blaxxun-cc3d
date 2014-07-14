
#ifndef _CC3D_API_H_
#define _CC3D_API_H_


#ifdef EXPORT_GAPI
#define GAPI __declspec(dllexport)
#else // EXPORT_GAPI
#define GAPI __declspec(dllimport)
#endif // EXPORT_GAPI

#endif _CC3D_API_H_


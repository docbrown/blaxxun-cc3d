
#ifndef _png_read_h
#define _png_read_h

#ifdef __cplusplus
extern "C" {
#endif


/* read a png file. 

  ret <0 error 
*/
extern int png_read(const char *file_name,image *im);

#ifdef __cplusplus
}
#endif


#endif


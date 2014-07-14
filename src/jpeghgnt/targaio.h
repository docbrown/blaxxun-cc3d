#ifndef _targa_H
#define _targa_H

#ifdef __cplusplus
extern "C" {
#endif

extern int targa_read(image *im,const char * tga_in_file);
extern int targa_write(image *im, const char *tga_out_file, int tgaformat, int rle_flag);

#ifdef __cplusplus
}
#endif

#endif

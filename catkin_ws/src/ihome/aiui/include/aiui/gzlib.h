/*

*/

#ifndef gzlib__h
#define gzlib__h

#ifdef __cplusplus
extern "C"
{
#endif
//解压gzip压缩后的数据
extern int gzuncompress(unsigned char *zdata, unsigned long nzdata,unsigned char *data, unsigned long *ndata);
// 将数据进行gzip压缩 
extern int gzcompress(unsigned char *data, unsigned long ndata,unsigned char *zdata, unsigned long *nzdata);

#ifdef __cplusplus
}
#endif

#endif

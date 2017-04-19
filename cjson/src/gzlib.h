/*

*/

#ifndef gzlib__h
#define gzlib__h

//解压gzip压缩后的数据
int gzuncompress(unsigned char *zdata, unsigned long nzdata,unsigned char *data, unsigned long *ndata);
// 将数据进行gzip压缩 
int gzcompress(unsigned char *data, unsigned long ndata,unsigned char *zdata, unsigned long *nzdata);

#endif

/*

*/

#ifndef gzlib__h
#define gzlib__h

#ifdef __cplusplus
extern "C"
{
#endif
//��ѹgzipѹ���������
extern int gzuncompress(unsigned char *zdata, unsigned long nzdata,unsigned char *data, unsigned long *ndata);
// �����ݽ���gzipѹ�� 
extern int gzcompress(unsigned char *data, unsigned long ndata,unsigned char *zdata, unsigned long *nzdata);

#ifdef __cplusplus
}
#endif

#endif

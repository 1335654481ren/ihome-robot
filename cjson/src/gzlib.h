/*

*/

#ifndef gzlib__h
#define gzlib__h

//��ѹgzipѹ���������
int gzuncompress(unsigned char *zdata, unsigned long nzdata,unsigned char *data, unsigned long *ndata);
// �����ݽ���gzipѹ�� 
int gzcompress(unsigned char *data, unsigned long ndata,unsigned char *zdata, unsigned long *nzdata);

#endif

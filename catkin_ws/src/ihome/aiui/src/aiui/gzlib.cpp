#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gzlib.h"
#include "zlib.h"
///////////////////////////////////////////////////////
//函数:gzuncompress()
//功能:解压gzip压缩后的数据
//参数1:待解压的数据,char*即可 
//参数2:待解压数据的长度,int即可
//参数3:用于接收解压后数据存放的缓存空间,char*即可 in-out参数
//参数4:用于接收解压后数据存放的缓存空间的长度,int即可
///////////////////////////////////////////////////////
int gzuncompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
	int err = 0;
	z_stream d_stream = { 0 }; 
	static char dummy_head[2] = { 0x8 + 0x7 * 0x10, (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF, };
	d_stream.zalloc = NULL;
	d_stream.zfree = NULL;
	d_stream.opaque = NULL;
	d_stream.next_in = zdata;
	d_stream.avail_in = 0;
	d_stream.next_out = data;
	//只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
	if(inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) 
		return -1;
	//if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
	while(d_stream.total_out < *ndata && d_stream.total_in < nzdata) 
	{
		d_stream.avail_in = d_stream.avail_out = 1; 
		if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) 
			break;
		if(err != Z_OK) 
		{
			if(err == Z_DATA_ERROR) 
			{
				d_stream.next_in = (Bytef*)dummy_head;
				d_stream.avail_in = sizeof(dummy_head);
				if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
				{
					return -1;
				}
			} 
			else 
				return -1;
		}
	}
	if(inflateEnd(&d_stream) != Z_OK) 
		return -1;
	*ndata = d_stream.total_out;
	return 0;
}

///////////////////////////////////////////////////////
//函数:gzcompress()
//功能:对数据采用gzip压缩
//参数1:待压缩的数据,char*即可 
//参数2:待压缩数据的长度,int即可
//参数3:用于接收压缩后数据存放的缓存空间,char*即可 in-out参数
//参数4:用于接收压缩后数据存放的缓存空间的长度,int即可
///////////////////////////////////////////////////////
int gzcompress(Byte *data, uLong ndata, Byte *zdata, uLong *nzdata)
{
    z_stream c_stream;
    int err = 0;
 
    if(data && ndata > 0) {
        c_stream.zalloc = NULL;
        c_stream.zfree = NULL;
        c_stream.opaque = NULL;
        //只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
        if(deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
        c_stream.next_in  = data;
        c_stream.avail_in  = ndata;
        c_stream.next_out = zdata;
        c_stream.avail_out  = *nzdata;
        while(c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
            if(deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
        }
        if(c_stream.avail_in != 0) return c_stream.avail_in;
        for(;;) {
            if((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
            if(err != Z_OK) return -1;
        }
        if(deflateEnd(&c_stream) != Z_OK) return -1;
        *nzdata = c_stream.total_out;
        return 0;
    }
    return -1;
}
/*
int main_test(int argc, char* argv[])
{	
	//用于压缩的char*数据
	char* str = "hello world!";
	
	//数据压缩
	uLong uBufferLen = strlen(str) * 100; //此处长度需要足够大以容纳压缩后数据
	unsigned char* uBuffer = (unsigned char*)calloc((uInt)uBufferLen, 1);

	gzcompress((Byte*)str, (uLong)strlen(str), (Byte*)uBuffer, (uLong*)&uBufferLen);
	printf("data long:%d\n", uBufferLen);
	int i = 0;
	printf("data after gzcompress:\n");
	for (i = 0; i < uBufferLen; i++)
		printf("%X ", (unsigned char)uBuffer[i]);
	printf("\n");

	printf("------------------------------------------------\n");
	//数据解压
	uLong uDestBufferLen = strlen(str); //此处长度需要足够大以容纳解压缩后数据
	char* uDestBuffer = (char*)calloc((uInt)uDestBufferLen, 1);
	gzuncompress((Byte*)uBuffer, (uLong)uBufferLen, (Byte*)uDestBuffer, (uLong*)&uDestBufferLen);
	printf("data long:%d\n", uDestBufferLen);
	printf("data after gzuncompress:\n%s\n", uDestBuffer);

	return 0;	
}
*/
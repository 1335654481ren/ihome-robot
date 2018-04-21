#include <stdio.h>
#include <curl/curl.h>

//这是libcurl接收数据的回调函数，相当于recv的死循环  
//其中stream可以自定义数据类型，这里我传入的是文件保存路径  
  
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {  
    int len = size * nmemb;  
    int written = len;   
    printf("-----------:%s\n",ptr);  
    return written;  
}  

int main(void)
{
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */ 
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl) {
		/* First set the URL that is about to receive our POST. This URL can
		   just as well be a https:// URL if that is what should receive the
		   data. */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://28.115.33.234:8081/attc/httpservices/phone/locked/ies");
		/* Now specify the POST data */ 
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "plateNo=EQ00003&locked=2");
		//指定回调函数  
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  
		//这个变量可作为接收或传递数据的作用  
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, "data.html"); 
		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			/* always cleanup */ 
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			return -1;    	
		}
		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;
}

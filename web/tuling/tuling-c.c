/***************************************************************
文件命名：tuling_test.c
函数功能：向图灵服务器发送请求，并接受返回的结果
编程人员：smartfrog
编程时间：2017-06-05 14:54:42
修改时间：
版本编号：v1.0.0
技术支持：wheretogo0815@163.com 
测试方法：./tuling_test ,然后输入要测试的命令(例如:讲一段相声)
****************************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>


#define PORT 80
#define SIZE 1024

//key   :是用户在图灵官网 http://www.tuling123.com/ 申请到的APIkey,共32位
//userid:在官网未找到，这里是随便写的。
char request_data1[100] = "{\"key\":\"23771317366d42ca8cbbc978a6a87364\",\"userid\":\"123456\",\"info\":\"";
char request[SIZE] = {0};

int main()
{
    int sockfd, ret, fd,len;
    struct sockaddr_in addr;
    struct hostent *pURL = NULL;
    char send_contents[SIZE];
    char input_buf[128], recv_buf[SIZE];
    char str_len[8];
    fd_set   t_set;
    struct timeval  tv;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
    {
        printf("致命错误:%s\n",strerror(errno));
        return -1;
    }

    pURL = gethostbyname("www.tuling123.com");  //获取IP地址等信息

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr_list[0]);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("致命错误:%s\n",strerror(errno));
        goto END;
    }
    printf("连接成功\n");
    
    FD_ZERO(&t_set);
    FD_SET(sockfd, &t_set);
    tv.tv_sec= 5;
    tv.tv_usec= 0;

    while(1)
    {
        memset(input_buf, 0, 128);
        printf("输入要请求的数据：");
        scanf("%s",input_buf);
        if(!strcmp(input_buf,"quit") || !strcmp(input_buf,"q"))
        {
            goto END;
        }
        else if(!strcmp(input_buf,"test"))
        {
            strcpy(input_buf,"讲个笑话");   //终端输入的编码可能有问题，此处默认保留一份正确的UTF-8编码示例
        }
        sprintf(request,"%s%s%s",request_data1,input_buf,"\"}");
        
        memset(str_len, 0, 8);
        len = strlen(request);
        sprintf(str_len, "%d", len);

        memset(send_contents, 0, SIZE);
        strcat(send_contents, "POST /openapi/api HTTP/1.1\n");
        strcat(send_contents, "Host: www.tuling123.com\n");
        strcat(send_contents, "Content-Type: application/json; charset=UTF-8\n");
        strcat(send_contents, "Content-Length: ");
        strcat(send_contents, str_len);
        strcat(send_contents, "\n\n");
        strcat(send_contents, request); //要发送给服务器的数据部分
        strcat(send_contents, "\n\n");

        ret = send(sockfd,send_contents,strlen(send_contents),0);
        if (ret < 0) 
        {
            printf("发送失败:%s\n",strerror(errno));
            goto END;
        }
        else
        {
            printf("*************************************\n");
            printf("消息发送成功，共发送了%d个字节！\n", ret);
        }
        
        memset(recv_buf, 0, SIZE);
        fd = select(sockfd+1, &t_set, NULL, NULL, &tv);
        if(fd < 0)
        {
            printf("致命错误:%s\n",strerror(errno));
            goto END;
        }
        else if(fd == 0)
        {
            printf("等待超时");
            continue;
        }
        
        ret = recv(sockfd, recv_buf, SIZE,0);
        if(ret < 0)
        {
            printf("致命错误:%s\n",strerror(errno));
            goto END;
        }
        else
        {
            printf("消息接收成功，共接收了%d个字节！\n", ret);
            printf("----%s*************************************\n\n", recv_buf);
        }
    }

END:    
    close(sockfd);
    
    return 0;
}
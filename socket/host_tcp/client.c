/*
 * socket_client.c
 *
 *  Created on: Mar 15, 2014
 *      Author: nerohwang
 */
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>       //pthread_t , pthread_attr_t and so on.
#include<stdio.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<string.h>          //Func :memset
#include<unistd.h>          //Func :close,write,read
#include<netdb.h>
#define SOCK_PORT       10000
#define BUFFER_LENGTH   1024
#define HOSTNAME        "ihome.51mypc.cn"
/* 
作用：将网络地址转化为IP 
参数：ipbuf是输出缓冲区, host是要转化的域名, maxlen是缓冲区大小 
返回值：返回-1是失败，0是成功 
*/  
int get_ip_from_host(char *ipbuf, const char *host, int maxlen)  
{  
    struct sockaddr_in sa;  
    sa.sin_family = AF_INET;  
    if (inet_aton(host, &sa.sin_addr) == 0)   
    {   
        struct hostent *he;  
        he = gethostbyname(host);  
        if (he == NULL)  
            return -1;  
        memcpy(&sa.sin_addr,he->h_addr, sizeof(struct in_addr));  
    }   
    strncpy(ipbuf, inet_ntoa(sa.sin_addr), maxlen);  
    return 0;  
}

int main()
{
    int sockfd;
    int tempfd;
    struct sockaddr_in s_addr_in;
    char data_send[BUFFER_LENGTH];
    char data_recv[BUFFER_LENGTH];
    char ipbuf[20];

    get_ip_from_host(ipbuf, HOSTNAME, 20);

    printf("Get hostname :%s ip: %s\n", HOSTNAME, ipbuf);

    memset(data_send,0,BUFFER_LENGTH);
    memset(data_recv,0,BUFFER_LENGTH);

    sockfd = socket(AF_INET,SOCK_STREAM,0);       //ipv4,TCP
        fprintf(stderr,"socket error!\n");
        exit(1);
    }

    //before func connect, set the attr of structure sockaddr.
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_addr.s_addr = inet_addr(ipbuf);      //trans char * to in_addr_t
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_port = htons(SOCK_PORT);

    tempfd = connect(sockfd,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
    if(tempfd == -1)
    {
        fprintf(stderr,"Connect error! \n");
        exit(1);
    }

    while(1)
    {
        printf("Please input something you wanna say(input \"quit\" to quit):\n");
        gets(data_send);
        //scanf("%[^\n]",data_send);         //or you can also use this
        tempfd = write(sockfd,data_send,BUFFER_LENGTH);
        if(tempfd == -1)
        {
            fprintf(stderr,"write error\n");
            exit(0);
        }

        if(strcmp(data_send,"quit") == 0)  //quit,write the quit request and shutdown client
        {
            break;
        }
        else
        {
            tempfd = read(sockfd,data_recv,BUFFER_LENGTH);
            assert(tempfd != -1);
            printf("%s\n",data_recv);
            memset(data_send,0,BUFFER_LENGTH);
            memset(data_recv,0,BUFFER_LENGTH);
        }
    }

    int ret = shutdown(sockfd,SHUT_WR);       //or you can use func close()--<unistd.h> to close the fd
    assert(ret != -1);
    return 0;
}


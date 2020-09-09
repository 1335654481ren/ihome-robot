#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_MSG_LEN 1024

int main(int argc,char *arg[])
{
    struct sockaddr_in servAddr;
    struct hostent *host = NULL;
    int Port = 0,socketFd;
    char buf[MAX_MSG_LEN] = {0};
    
    if(argc<3)
    {
        printf("please input IP and port number!\r\n");
        return -1;
    }
    
    if((host = gethostbyname(arg[1]))==NULL)
    {
        return -1;
    }    
    //                     ipV4        TCP     0
    if((socketFd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        return -1;
    }    
    printf("socket fd = %d\n",socketFd);
    memset(&servAddr,0,sizeof(struct sockaddr_in));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port   = htons(atoi(arg[2]));
    servAddr.sin_addr    = (*(struct in_addr *)host->h_addr);
    
    if(connect(socketFd,(struct sockaddr *)&servAddr,sizeof(struct sockaddr))==-1)
    {
        perror("connect:");
        return -1;
    }
    
    while(1)
    {
        printf("input:");
        scanf("%s",buf);
        if(send(socketFd,buf,sizeof(buf),0)==-1)
        {
            perror("send:");
        }
        {
            printf("send successful!\r\n");
        }
        memset(buf,0,sizeof(buf));
    }
    close(socketFd);
    return 0;
}

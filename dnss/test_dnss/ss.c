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
#define BACKLOG     10 

int main(int argc,char *arg[])
{
    struct sockaddr_in servAddr,clidAddr;
    struct hostent *host = NULL;
    int Port = 0,socketFd,sin_size;
    socklen_t peerlen;
    int recLen = 0;
    char buf[MAX_MSG_LEN] = {0};
    
    if(argc<2)
    {
        printf("please input port number!\r\n");
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
    servAddr.sin_port   = htons(atoi(arg[1]));
    servAddr.sin_addr.s_addr= INADDR_ANY;
    
    if(bind(socketFd,(struct sockaddr *)&servAddr,sizeof(struct sockaddr))==-1)
    {
        perror("bind:");
    }
    else
    {
        printf("bind success \r\n");
    }
    
    if(listen(socketFd,BACKLOG)==-1)
    {
        perror("listen:");
    }
    else
    {
        printf("Listening...\r\n");
    }
    sin_size=sizeof(struct sockaddr_in);
    
    if((socketFd=accept(socketFd,(struct sockaddr *)&clidAddr,&sin_size))==-1)
    {
            perror("accept:");
            return -1;
    }
        else
    {
            printf("accept successful!\r\n");
    }
    
    while(1)
    {
        memset(buf,0,sizeof(buf));
        if((recLen = recv(socketFd,buf,MAX_MSG_LEN,0))==-1)
        {
            perror("recv:");
        }
        else
        {
            if(recLen>0)
            {
                recLen = 0;
                printf("Receive a message:%s\r\n",buf);
            }
        }
    }    
    close(socketFd);
    return 0;
}

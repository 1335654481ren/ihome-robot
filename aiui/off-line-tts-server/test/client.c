#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 12345
#define BUFF_LEN 16*1024
//#define SERVER_IP "127.0.0.1"

/*
    client:
            socket-->sendto-->revcfrom-->close
*/

int main(int argc, char* argv[])
{
    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    if(argc < 3)
    {
        printf("usage:./speak Ip word\n");
        return -1;
    }

    bzero(&ser_addr, sizeof(ser_addr));

    ser_addr.sin_family = AF_INET;

    ser_addr.sin_addr.s_addr = inet_addr( argv[1] ) ;
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换
    ser_addr.sin_port = htons(SERVER_PORT);  //注意网络序转换

    int nMsgLen = strlen(argv[2]);

    char* msg = (char*)malloc(nMsgLen+1);

    bzero(msg,nMsgLen+1);

    strncpy(msg,argv[2],nMsgLen);

    struct sockaddr* dst = (struct sockaddr*)&ser_addr;

    socklen_t len = sizeof(*dst);

    sendto(client_fd, msg, nMsgLen, 0, dst, len);

    close(client_fd);

    free(msg);

    return 0;
}

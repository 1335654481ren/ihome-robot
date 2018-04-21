#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
#define HOSTNAME "tcp.ngrok.xiaomiqiu.cn"
#define PORT "41220"
int main(int argc,char *argv[])
{
	int sockfd;
	char sendbuffer[200];
	char recvbuffer[200];
	//  char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber,nbytes;
//	if(argc!=3)
//	{
//		fprintf(stderr,"Usage :%s hostname portnumber\a\n",argv[0]);
//		exit(1);
//	}
	if((host=gethostbyname(HOSTNAME))==NULL)
	{
		herror("Get host name error\n");
		exit(1);
	}
	if((portnumber=atoi(PORT))<0)
	{
		fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);
		exit(1);
	}
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		exit(1);
	}
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Connect error:%s\n",strerror(errno));
		exit(1);
	}
	while(1)
	{
		printf("Please input your word:\n");
		scanf("%s",sendbuffer);
		printf("\n");
		if(strcmp(sendbuffer,"quit")==0)
		break;
		send(sockfd,sendbuffer,sizeof(sendbuffer),0);
		recv(sockfd,recvbuffer,200,0);
		printf("recv data of my world is :%s\n",recvbuffer);
	}
	// if((nbytes=read(sockfd,buffer,1024))==-1)
	//{
	// fprintf(stderr,"read error:%s\n",strerror(errno));
	// exit(1);
	//  }
	// buffer[nbytes]='\0';
	// printf("I have received %s\n",buffer);
	close(sockfd);
	exit(0);
}
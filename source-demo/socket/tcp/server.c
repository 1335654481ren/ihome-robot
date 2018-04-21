#include<sys/socket.h>
#include <unistd.h>// for close function
#include <string.h> // for bzero function
#include<stdio.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<netinet/in.h>
#include <stdlib.h>
#define SERV_PORT 5555
#define SERV_IP "192.168.66.134"//mx27 board ip
#define BACKLOG 10 //the counts of connect can keep in wait queen
#define MAXBUFSIZE 200
char buf[MAXBUFSIZE]; //receive buf 
char str_to_send[200] ="important notice:to cerebrate China log on moon successful on 12nd,Jan,2010. everyone has a day free from work!/n"; 
int main(int argc, char **argv)
{
	int sockfd,sockfd_client;
	socklen_t sin_size; // used in accept(),but i don't know what it means
	printf("#####################################################/n");
	printf("socket receive text        by pafone  30th,April,2009/n");
	printf("server ip:%s port:%d         /n",SERV_IP,SERV_PORT);
	printf("#####################################################/n");
	struct sockaddr_in my_addr;//local ip info
	struct sockaddr_in serv_addr,client_sockaddr; //server ip info
	int serverport;
	if(argc == 2)
	{
		serverport = atoi(argv[1]);
	}
	else
	{
		serverport = SERV_PORT;
	}
	if(-1 == (sockfd = socket(AF_INET,SOCK_STREAM,0)) )
	{
		perror("error in create socket/n");
		exit(0);
	}
	//set the sockaddr_in struct
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serverport);//server listening port
	serv_addr.sin_addr.s_addr = INADDR_ANY;//here is the specia in listening tcp connect
	bzero(&serv_addr.sin_zero,8);
//bind , the ip and port information is aready in the sockaddr
	if(-1 == bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr)))
	{
		perror("bind error/n");
		exit(0);
	}
	printf("bind seccessful/n");
	
	if(-1 == listen(sockfd,BACKLOG))
	{
		perror("lisenning");
		exit(1);
	}
	printf("the server is listenning.../n");
	//accept
	printf("before accept:sockfd_client is %d/n",sockfd_client);
	if(-1 == (sockfd_client = accept(sockfd,(struct sockaddr*)&client_sockaddr,&sin_size)))
	{
		perror("accept");
		exit(1);
	}
	printf("accept connect./n");
	int recvbytes;//the number of bytes receive from socket
//	char buffer[200];
//	printf("sockfd_client is %d/n",sockfd_client);
//	recvbytes = recv(sockfd_client,buffer,200,0);
	if(-1 == (recvbytes = recv(sockfd_client,buf,MAXBUFSIZE,0)))//changed here
	if(-1 == recvbytes)
	{
		perror("receive");
		exit(1);
	}
	printf("%dbytes receive from connect:%s/n",recvbytes,buf);
	close(sockfd);
	close(sockfd_client);
}

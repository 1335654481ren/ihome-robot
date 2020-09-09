#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
 
 
int main(int argc,char *argv[])
{
	// 1.创建udp通信socket  
	int udp_socket_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(udp_socket_fd < 0 )
	{
		perror("creat socket fail\n");
		return -1;
	}
 
	//2.设置UDP的地址并绑定 
	struct sockaddr_in  local_addr = {0};
	local_addr.sin_family  = AF_INET; //使用IPv4协议
	local_addr.sin_port	= htons(8989);   //网络通信都使用大端格式
	local_addr.sin_addr.s_addr = INADDR_ANY;//让系统检测本地网卡，自动绑定本地IP
 
	int ret = bind(udp_socket_fd,(struct sockaddr*)&local_addr,sizeof(local_addr));
	if(ret < 0)
	{
		perror("bind fail:");
		close(udp_socket_fd);
		return -1;
	}
	else
	{
		printf("recv ready!!!\n");
	}
	
	struct sockaddr_in  src_addr = {0};  //用来存放对方(信息的发送方)的IP地址信息
	int len = sizeof(src_addr);	//地址信息的大小
	char buf[1024] = {0};//消息缓冲区
	//3 循环接收客户发送过来的数据  
	while(1)
	{
		ret = recvfrom(udp_socket_fd, buf, sizeof(buf), 0, (struct sockaddr *)&src_addr, &len);
		if(ret == -1)
		{
			break;
		}
		printf("[%s:%d]",inet_ntoa(src_addr.sin_addr),ntohs(src_addr.sin_port));//打印消息发送方的ip与端口号
		printf("recv = %s\n",buf);
		if(strcmp(buf, "exit") == 0)
		{
			break;
		}
		memset(buf, 0, sizeof(buf));//清空存留消息
	}
	
	//4 关闭通信socket
	close(udp_socket_fd);
}
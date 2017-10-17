/*
 *  A library for Linux ultrasonic_12.c communication
 *
 *  Author: Renxl, 2017
 *
 *  Version: 0.1
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>       //pthread_t , pthread_attr_t and so on.
#include <stdio.h>
#include <netinet/in.h>      //structure sockaddr_in
#include <arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include <assert.h>          //Func :assert
#include <string.h>          //Func :memset
#include <unistd.h>          //Func :close,write,read
#include <netdb.h>
#include <pthread.h>
#include <math.h>
#include <error.h>
#include <cstdlib>  
#include<iostream>

#include "ros/ros.h" 
#include <robot_msgs/Speek.h> 
#include "aiui.h"

using namespace std;

#define SOCK_PORT 	10001
#define BUFFER_LENGTH 	1024
#define SERVER_IP	"127.0.0.1"
#define PORT_NUM	12
#define FRAME_LEN 	50 
#define FRAME_HEAD_LEN 2


//tcp服务定义
#define READ_BUFFER_SIZE 4
#define CLIENT_MAX	10
#define SOCK_PORT 	10001
#define BUFFER_LENGTH 	1024
#define MAX_CONN_LIMIT 	512

pthread_mutex_t mutex;  
pthread_cond_t cond;
unsigned char data_buff[50];
int data_lenght = 50;

static void client_handle(void * sock_fd);   //Only can be seen in the file			

int main(int argc,char **argv)
{
	int sockfd_server;
	int sockfd;
	int fd_temp;
	
  	unsigned char id = 0;
  	unsigned char type = 0;
	struct sockaddr_in s_addr_in;
	struct sockaddr_in s_addr_client;
	int client_length;
	memset(data_buff,0x00,50);
	
	ros::init(argc, argv, "say");
	ros::NodeHandle n;	
	ros::ServiceClient client = n.serviceClient<robot_msgs::Speek>("/speek"); 

	sockfd_server = socket(AF_INET,SOCK_STREAM,0);  //ipv4,TCP
	assert(sockfd_server != -1);

	//before bind(), set the attr of structure sockaddr.
	memset(&s_addr_in,0,sizeof(s_addr_in));
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);  //trans addr from uint32_t host byte order to network byte order.
	s_addr_in.sin_port = htons(SOCK_PORT);          //trans port from uint16_t host byte order to network byte order.
    //将本地地址绑定到所创建的套接字上  
    if( bind(sockfd_server, (struct sockaddr*)&s_addr_in, sizeof(s_addr_in)) == -1){  
    	printf("bind socket error\n");  
    	exit(0);  
    } 
	fd_temp = listen(sockfd_server,MAX_CONN_LIMIT);
	if(fd_temp == -1)
	{
		fprintf(stderr,"listen error!\n");
		exit(1);
	}

	while(ros::ok())
	{
		printf("waiting for new connection...\n");
		client_length = sizeof(s_addr_client);
		//Block here. Until server accpets a new connection.
		sockfd = accept(sockfd_server,(struct sockaddr*)(&s_addr_client),(socklen_t *)(&client_length));
		if(sockfd == -1)
		{
			fprintf(stderr,"Accept error!\n");
			continue;                               //ignore current socket ,continue while loop.
		}
		while(ros::ok())
		{
			int recv_length;
			usleep(1000*100);
			if((recv_length = recv(sockfd,data_buff,data_lenght,0)) <= 0)
			{
				printf("read data error!!!!!!!!!!!!!\n");
				break;
			}else{
				std::stringstream arg1;
			  	std::stringstream arg2;
			    robot_msgs::Speek srv;  
			    srv.request.id = 2;  
			    srv.request.type = 12;	
			    arg1 << data_buff;
				arg2 << "vali";
				cout<<arg1 << endl;
			    srv.request.arg1 = arg1.str();
				srv.request.arg2 = arg2.str();
			    if (client.call(srv))  
			    {  
			      ROS_INFO("code: %d ", srv.response.code);
			    }
	            printf("recv_length = %d  = %s\n ",recv_length,data_buff);
			}
		}
		
	}
	//Clear
	int ret = shutdown(sockfd_server,SHUT_WR); //shut down the all or part of a full-duplex connection.
	assert(ret != -1);
	printf("Server shuts down\n");
	return 0;
} 

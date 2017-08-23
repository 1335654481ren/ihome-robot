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

#include "ros/ros.h"
#include <math.h>
#include <ros/time.h>
#include <math.h>
#include "ros/ros.h"  
#include <robot_msgs/Speek.h> 
#include <cstdlib>  
#include "aiui.h"

using namespace std;

#define SOCK_PORT 	10001
#define BUFFER_LENGTH 	1024
#define SERVER_IP	"127.0.0.1"
#define PORT_NUM	12
#define FRAME_LEN 	50 
#define FRAME_HEAD_LEN 2

ros::Time time_ref;
int seq_ref = 0;
ros::Time time_ros;
int sequence = 0;

ros::ServiceClient client;

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

int main(char argc,char **argv)
{
	int sockfd_server;
	int sockfd;
	int fd_temp;
	struct sockaddr_in s_addr_in;
	struct sockaddr_in s_addr_client;
	int client_length;
	memset(data_buff,0x00,50);
	pthread_mutex_init(&mutex, NULL);  

	sockfd_server = socket(AF_INET,SOCK_STREAM,0);  //ipv4,TCP
	assert(sockfd_server != -1);

	//before bind(), set the attr of structure sockaddr.
	memset(&s_addr_in,0,sizeof(s_addr_in));
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);  //trans addr from uint32_t host byte order to network byte order.
	s_addr_in.sin_port = htons(SOCK_PORT);          //trans port from uint16_t host byte order to network byte order.
	fd_temp = bind(sockfd_server,(struct scokaddr *)(&s_addr_in),sizeof(s_addr_in));
	if(fd_temp == -1)
	{
		fprintf(stderr,"bind error!\n");
		exit(1);
	}
	fd_temp = listen(sockfd_server,MAX_CONN_LIMIT);
	if(fd_temp == -1)
	{
		fprintf(stderr,"listen error!\n");
		exit(1);
	}

	while(1)
	{
		printf("\033[1;32;40m waiting for new connection...\n\033[0m");
		pthread_t thread_id;
		client_length = sizeof(s_addr_client);

		//Block here. Until server accpets a new connection.
		sockfd = accept(sockfd_server,(struct sockaddr_*)(&s_addr_client),(socklen_t *)(&client_length));
		if(sockfd == -1)
		{
			fprintf(stderr,"Accept error!\n");
			continue;                               //ignore current socket ,continue while loop.
		}
		printf("\033[1;32;40m A new connection occurs!\n\033[0m");
		if(pthread_create(&thread_id,NULL,(void *)(&client_handle),(void *)(&sockfd)) == -1)
		{
			fprintf(stderr,"pthread_create error!\n");
			break;                                  //break while loop
		}else{
			pthread_detach(thread_id);
		}
	}
	//Clear
	int ret = shutdown(sockfd_server,SHUT_WR); //shut down the all or part of a full-duplex connection.
	assert(ret != -1);
	printf("Server shuts down\n");
	return 0;
} 
/**
*	客户端线程
*/
static void client_handle(void * sock_fd)
{
	int fd = *((int *)sock_fd);
	int recv_length;
	
	while(1)
	{
		usleep(1000*100);
		//if(recv_length = read(sockfd,data_recv,BUFFER_LENGTH) == -1)
		if(recv_length = recv(fd,data_buff,data_lenght,MSG_NOSIGNAL) == -1)
		{
			printf("send data error!!!!!!!!!!!!!\n");
			break;
		}else{
			for(int k = 0; k < 50 ; k++)
            {
                printf("%02x ",data_buff[k]);
            }
            printf("\n");
		}
	}
	printf("Client shuts down\n");
	//Clear
	close(fd);            //close a file descriptor.
	pthread_exit(NULL);   //terminate calling thread!
}


int send_data()
{
	std::stringstream arg1;
	std::stringstream arg2;
	robot_msgs::Speek srv;  
	srv.request.id = 2;  
	srv.request.type = 11;	
	arg1 << "";
	arg2 << argv[4];
	srv.request.arg1 = arg1.str();
	srv.request.arg2 = arg2.str();
	if (client.call(srv))  
	{  
		ROS_INFO("code: %d", srv.response.code);
	}  
	else  
	{  
		ROS_ERROR("Failed to call service add_two_ints"); 
		return -1;
	}
	return 0; 
}
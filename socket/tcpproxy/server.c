/*
 * multi_thread_socket_server.c
 *
 *  Created on: Mar 14, 2014
 *      Author: nerohwang
 */
#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>       //pthread_t , pthread_attr_t and so on.
#include<stdio.h>
#include<linux/types.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<string.h>          //Func :memset
#include<unistd.h>          //Func :close,write,read
#include<time.h>
//#define DRV_DEBUG 1
#if DRV_DEBUG
    #define DEB(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEB(fmt, ...) 
#endif

#define CLIENT_MAX	10
#define SOCK_PORT 	10001
#define BUFFER_LENGTH 	1024
#define MAX_CONN_LIMIT 	512     //MAX connection limit
#define RTK_PORT	8888
#define RTK_SERVER_IP   "192.168.1.99"

char rtk_buff[BUFFER_LENGTH] = "1234567890.......";
int rtk_data_length;
int client_current_count = 0;

struct remote_client{
	int port;
	char reomte_ip[INET_ADDRSTRLEN];
	unsigned long rx_packets;
	unsigned long rx_bytes;
	time_t	start_time;
	time_t	end_time;
}server ={ RTK_PORT,RTK_SERVER_IP,0,0};
  
typedef struct node  
{  
	int id;
	pthread_t thread_id;
	pthread_mutex_t mutex;  
	pthread_cond_t cond;
	int fd;
	char client_ip[INET_ADDRSTRLEN];
	int port;
	unsigned long start_cnt;
	unsigned long tx_packets;
	unsigned long tx_bytes;
	unsigned long error;
	time_t start_time;
	time_t end_time;
    struct node *next;     
}NODE; 
NODE *client = NULL;

pthread_mutex_t mutex;  
pthread_cond_t cond;

static void Data_handle(void * sock_fd);   //Only can be seen in the file
static void rtk_client();
//链表操作函数
void DEB_node(NODE *pnode);
void DEB_close(NODE *pnode);
NODE *create_node(int id);
NODE *insert_node(NODE *phead, NODE *pnode);
NODE *delete_node(NODE *phead, int id);
int get_list_len(NODE *phead);
NODE *search_node(NODE *phead, int id);
static void window(void);
int main()
{
	int sockfd_server;
	int sockfd;
	int fd_temp;
	struct sockaddr_in s_addr_in;
	struct sockaddr_in s_addr_client;
	int client_length;
	NODE *pnode = NULL;
	pthread_t rtk_thread,windows_thread;

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

	if(pthread_create(&rtk_thread,NULL,(void *)(&rtk_client),(void *)(&sockfd)) == -1)
	{   
		fprintf(stderr,"pthread_create error!\n");
	}
	else{
		DEB("\033[1;32;40m create rtk_client thread!\n\033[0m");
	}
	
	if(pthread_create(&windows_thread,NULL,(void *)(&window),NULL) == -1)
	{   
		fprintf(stderr,"pthread_create error!\n");
	}
	else{
		DEB("\033[1;32;40m create rtk_client thread!\n\033[0m");
	}
	while(1)
	{
		DEB("\033[1;32;40m waiting for new connection...\n\033[0m");
		pthread_t thread_id;
		client_length = sizeof(s_addr_client);

		//Block here. Until server accpets a new connection.
		sockfd = accept(sockfd_server,(struct sockaddr_*)(&s_addr_client),(socklen_t *)(&client_length));
		if(sockfd == -1)
		{
			fprintf(stderr,"Accept error!\n");
			continue;                               //ignore current socket ,continue while loop.
		}
		pnode = create_node(client_current_count);
		pnode->fd = sockfd;
		pthread_mutex_init(&(pnode->mutex), NULL);
		pthread_cond_init(&(pnode->cond), NULL);
		inet_ntop(AF_INET, &s_addr_client.sin_addr, pnode->client_ip, INET_ADDRSTRLEN);
		pnode->port = ntohs(s_addr_client.sin_port); 
		DEB("\033[1;32;40m A new connection occurs!\033[0m");
		if(pthread_create(&thread_id,NULL,(void *)(&Data_handle),(void *)(&sockfd)) == -1)
		{
			fprintf(stderr,"pthread_create error!\n");
			free(pnode);
			pnode = NULL;
			break;                                  //break while loop
		}else{
			pnode->thread_id = thread_id;
			client = insert_node(client,pnode);
			DEB("Insert:\n");
			DEB_node(pnode);
			pthread_detach(thread_id);
			client_current_count++;
		}
	}
	//Clear
	int ret = shutdown(sockfd_server,SHUT_WR); //shut down the all or part of a full-duplex connection.
	assert(ret != -1);

	DEB("Server shuts down\n");
	return 0;
} 
//打印一个节点
void DEB_node(NODE *pnode)
{	
	if(pnode == NULL)
	  return;
	printf("\033[1;32;40m Client[%d]: ip:%s port:%d start_time:%s Tx packets:%ld Tx bytes:%ld \n\033[0m",pnode->id,pnode->client_ip,pnode->port,ctime(&pnode->start_time),pnode->tx_packets,pnode->tx_bytes);
}
//打印一个节点
void DEB_close(NODE *pnode)
{	
	if(pnode == NULL)
	  return;
	printf("\033[1;34;40m Client[%d]: ip:%s port:%d start_time:%s end_time:%s\n Tx packets:%ld Tx bytes:%ld Drop packets:%d\n\033[0m",pnode->id,pnode->client_ip,pnode->port,ctime(&pnode->start_time),ctime(&pnode->end_time),pnode->tx_packets,pnode->tx_bytes,pnode->start_cnt - pnode->tx_packets);
}
void show_status()
{
	NODE *pnode = NULL;
	printf("\r\nServer Status\n");
	printf("Server info:\n");
	printf("\033[1;32;40m Remote Ip:%s Port:%d \n\033[0m",server.reomte_ip,server.port);
	printf("\033[1;32;40m RX packets:%ld RX bytes:%ld \n\033[0m",server.rx_packets,server.rx_bytes);
	printf("\033[1;32;40m Server start date: %s \n\033[0m",ctime(&server.start_time));	
//	pthread_mutex_lock(&mutex);
	pnode = client;
	printf("\033[1;32;40m Now total client: %d \n\033[0m",get_list_len(pnode));
	while(pnode != NULL)
	{
		DEB_node(pnode);
		pnode = pnode->next;
	}
//	pthread_mutex_unlock(&mutex);
}
static void window(void)
{
	char cmd;
	sleep(10);
	while(1){
		//printf("Input cmd:");
		//cmd = getchar();
		sleep(1);
		show_status();
		printf("\033[2J");
		printf("\033[50A");
	}
}
static void Data_handle(void * sock_fd)
{
	int fd = *((int *)sock_fd);
	int recv_length;
	char data_recv[BUFFER_LENGTH];
	const char * data_send = "Server has received your request!\n";
	struct timeval now;
	struct timespec timeout;
	NODE *pnode = NULL;
	pnode = search_node(client,fd);
	//DEB("i am the client:\n");
	//DEB_node(pnode);
	pnode->start_cnt = server.rx_packets;
	time(&pnode->start_time); 
	while(1)
	{
		pthread_cond_wait( &pnode->cond, &pnode->mutex);
		if(recv_length = send(fd,rtk_buff,rtk_data_length,MSG_NOSIGNAL) == -1)
		{
			DEB("send data error!!!!!!!!!!!!!\n");
			break;
		}else{
			pnode->tx_packets++;
			pnode->tx_bytes += rtk_data_length;
			DEB("\nsend data %d to client%d ok!\n",recv_length,pnode->id);
		}
	}
	time(&pnode->end_time);
	pnode->error = server.rx_packets - pnode->start_cnt;
	DEB_close(pnode);
	client = delete_node(client,fd);
	client_current_count--;
	//Clear
	close(fd);            //close a file descriptor.
	pthread_exit(NULL);   //terminate calling thread!
}

static void rtk_client( void )
{
	int sockfd;
	int tempfd;
	struct sockaddr_in s_addr_in;
	char data_recv[BUFFER_LENGTH];
	sockfd = socket(AF_INET,SOCK_STREAM,0);       //ipv4,TCP
	if(sockfd == -1)
	{
		fprintf(stderr,"socket error!\n");
		exit(1);
	}

	//before func connect, set the attr of structure sockaddr.
	memset(&s_addr_in,0,sizeof(s_addr_in));
	s_addr_in.sin_addr.s_addr = inet_addr(RTK_SERVER_IP);      //trans char * to in_addr_t
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_port = htons(RTK_PORT);

	tempfd = connect(sockfd,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
	if(tempfd == -1)
	{
		fprintf(stderr,"Connect error! \n");
		exit(1);
	}
	else{
		DEB("connect to rtk server!\n");
	}
	time(&server.start_time);
	while(1)
	{ 
		sleep(1);
		memset(rtk_buff,0,BUFFER_LENGTH);
		rtk_data_length = read(sockfd,rtk_buff,BUFFER_LENGTH);
		server.rx_bytes += rtk_data_length;
		server.rx_packets++;
		pthread_mutex_lock(&mutex);
		NODE *pnode = client;
		while(pnode != NULL)
		{
			pthread_cond_signal(&pnode->cond); //解除所有线程的阻塞
			DEB("send signal to client %s\n",pnode->client_ip);
			if(pnode->next == NULL)
				break;
			pnode = pnode->next;
		}
		pthread_mutex_unlock(&mutex);
	}
	time(&server.end_time);
	int ret = shutdown(sockfd,SHUT_WR);//or you can use func close()--<unistd.h> to close the fd
	assert(ret != -1);
}
//初始化一个节点
NODE *create_node(int id)
{
	NODE *pnode;
	pnode = (NODE *)malloc(sizeof(NODE));
	memset(pnode,0x00,sizeof(NODE));
	pnode->id = id;//初始化数据域
	pnode->next = NULL;//初始化指针域为NULL
	return pnode;
}
//创建一个链表节点(尾插法)
NODE *insert_node(NODE *phead, NODE *pnode)
{
	NODE *ptmp = phead;
	pthread_mutex_lock(&mutex);
	if(NULL == ptmp){//当链表为空，直接返回初始化的结点
			DEB("list is empty, insert first node!\n");
			pthread_mutex_unlock(&mutex);
			return pnode;
	}else{
			while(ptmp->next != NULL){//找到最后一个结点，插在尾部
					ptmp = ptmp->next;
			}
			ptmp->next = pnode;
	}
	pthread_mutex_unlock(&mutex);
	return phead;
}
  //删除节点
NODE *delete_node(NODE *phead, int fd)
{
	NODE *ptmp = phead;
	NODE *tmp = NULL;
	pthread_mutex_lock(&mutex);
	if(NULL == phead){//处理链表为空的情况
			DEB("link is empty, del fail\n");
			pthread_mutex_unlock(&mutex);
			return NULL;
	}else if(phead->fd == fd){//单独处理第一个结点
			phead = phead->next;
			DEB("delete node:");
			DEB_node(ptmp);
			free(ptmp);
			ptmp = NULL;
	}else{
			while(ptmp->next != NULL && ptmp->next->fd != fd){ //没找完 && 没找到
					ptmp = ptmp->next;
			}
			if(NULL == ptmp->next){ //没找到
					DEB("del id NOT FOUND\n");
					pthread_mutex_unlock(&mutex);
					return phead;
			}
			if(ptmp->next->fd == fd){ //找到目标结点删除之
					tmp = ptmp->next;
					ptmp->next = tmp->next;
					DEB("delete_node:\n");
					DEB_node(tmp);
					free(tmp);
					tmp = NULL;
			}
	}
	pthread_mutex_unlock(&mutex);
	return phead;
}
//求链表长度
int get_list_len(NODE *phead)
{
		pthread_mutex_lock(&mutex);
		int len = 0;//计数器
		NODE *ptmp = phead;

		while(ptmp != NULL){//遍历链表，计数器加1
				len++;
				ptmp = ptmp->next;
		}
		pthread_mutex_unlock(&mutex);

		return len;
}
//按值查找
NODE *search_node(NODE *phead, int fd)
{
	pthread_mutex_lock(&mutex);
	NODE *ptmp = phead;

	if(NULL == phead){    //链表为空，直接返回
			pthread_mutex_unlock(&mutex);
			return NULL;
	}
	while(ptmp->fd != fd && ptmp->next != NULL){//没找到key && 链表没找完
			ptmp = ptmp->next; //指针移动
	}
	if(ptmp->fd == fd) //找到key，返回该结点地址
	{
			pthread_mutex_unlock(&mutex);
			return ptmp;
	}	
	if(ptmp->next == NULL)//没找到，返回空
	{
			pthread_mutex_unlock(&mutex);
			return NULL;
	}
}

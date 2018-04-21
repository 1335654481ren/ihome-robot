/* =========================================================================

DESCRIPTION
   uart

Copyright (c) 2015 by  USTC iFLYTEK, Co,LTD.  All Rights Reserved.
============================================================================ */

/* =========================================================================

                             REVISION

when            who              why
--------        ---------        -------------------------------------------
2015/10/15     kunzhang        Created.
============================================================================ */

/* ------------------------------------------------------------------------
** Includes
** ------------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <stdarg.h>
#include "uart.h"
#include "aiui.h"
/* ------------------------------------------------------------------------
** Global Variable Definitions
** ------------------------------------------------------------------------ */
static int recv_index = 0;
static unsigned char recv_buf[RECV_BUF_LEN];
static unsigned char recvbuf[4096];
static unsigned int big_buf_len = 0;
static unsigned int big_buf_index = 0;
static void* big_buf = NULL;
UART_HANDLE uart_hd;                             

/* ------------------------------------------------------------------------
** Function Definitions
** ------------------------------------------------------------------------ */

void* uart_recv(void* arg)
{
	uartData *uart = (uartData *)arg;
	char revbuff[2048];
	int revlen = 0;
	while(uart->running){
		usleep(100000);
		memset(revbuff, 0, sizeof(revbuff));
		if((revlen = read(uart->fd,revbuff,sizeof(revbuff))) == -1){
			usleep(100000);
			continue;
		}
		uart->uart_rec_cb(revbuff, revlen, uart->user_data);
	}
	return NULL;
}


unsigned char CalcCheckCode(unsigned char *data,int data_len)
{
	unsigned char checkCode = 0;
	for (int i = 0; i < data_len - 1; i++)
	{
		checkCode += data[i];
	}
	checkCode = (char)(~checkCode + 1);
	return checkCode;
}

void uart_rec(const void *msg, unsigned int msglen, void *user_data)
{
	for(int i = 0 ; i < msglen; i++ )
	{
		recvbuf[recv_index++] = ((unsigned char*)msg)[i];
//		printf("%02x ", ((unsigned char*)msg)[i]);
		if(recv_index == 5){
			if(recvbuf[0] == 0xa5 && recvbuf[1] == 0x01){
				big_buf_len = ((unsigned int)(recvbuf[4] & 0xff) << 8) + (recvbuf[3] & 0xff);
//				printf("\ndata-len = %d\n ", big_buf_len);
			}else{
				recv_index = 0;
				big_buf_len = 0;
				memset(recvbuf, 0x00,4096);
			}
		}
		if(recv_index != 0 && big_buf_len != 0 && recv_index == 8 + big_buf_len )
		{
//			printf("\n==== %d , ==== %d \n",recv_index,big_buf_len);
//			printf(" ---- %d == %d ----\n",recvbuf[recv_index -1],CalcCheckCode(recvbuf, recv_index));
			if (recvbuf[0] == 0xa5 && recvbuf[1] == 0x01){
				if( recvbuf[recv_index -1] == CalcCheckCode(recvbuf, recv_index)){
					process_recv((unsigned char*)recvbuf, recv_index);   //接受消息处理
					recv_index = 0;
					big_buf_len = 0;
					memset(recvbuf, 0x00,4096);
				}else{
					recv_index = 0;
					big_buf_len = 0;
					memset(recvbuf, 0x00,4096);
				}
			}else{
				recv_index = 0;
				big_buf_len = 0;
				memset(recvbuf, 0x00,4096);
			}
		}
	}
	printf("\n");
}

/*
void uart_rec(const void *msg, unsigned int msglen, void *user_data)
{
	printf("uart recv %d\n", msglen);
	//过滤不以A5 01开头的无效数据
	if(big_buf == NULL && recv_index + msglen >= 2){
		if(recv_index == 0){
			if(((unsigned char*)msg)[0] != SYNC_HEAD || ((unsigned char*)msg)[1] != SYNC_HEAD_SECOND){
				printf("recv data not SYNC HEAD, drop\n");
				return;
			}
		}else if(recv_index == 1){
			if(recv_buf[0] != SYNC_HEAD || ((unsigned char*)msg)[0] != SYNC_HEAD_SECOND){
				printf("recv data not SYNC HEAD, drop\n");
				recv_index = 0;
				return;
			}
		}
	}

	//不断接收串口字节，构造完成消息
	int copy_len;
	if(big_buf != NULL)
	{
		copy_len = big_buf_len - big_buf_index < msglen? big_buf_len - big_buf_index : msglen;
		memcpy(big_buf + big_buf_index, msg, copy_len);
		big_buf_index += copy_len;
		if(big_buf_index < big_buf_len) return;
	}else {
		copy_len = RECV_BUF_LEN - recv_index < (int)msglen? RECV_BUF_LEN - recv_index : msglen;
		memcpy(recv_buf + recv_index, msg, copy_len);
		if((recv_index + copy_len) > PACKET_LEN_BIT) {
			unsigned int content_len = recv_buf[PACKET_LEN_BIT] << 8 | recv_buf[PACKET_LEN_BIT - 1];
			if(content_len != MSG_NORMAL_LEN){
				big_buf_index = 0;
				big_buf_len = content_len + MSG_EXTRA_LEN;
				big_buf = malloc(big_buf_len);
				printf("uart malloc buflen %d\n", big_buf_len);
				memset(big_buf,0x00 ,big_buf_len );
				memcpy(big_buf, recv_buf, recv_index);
				big_buf_index += recv_index;
				recv_index = 0;
				return uart_rec(msg, msglen, user_data);
			}
		}
		recv_index += copy_len;
		if(recv_index < RECV_BUF_LEN) return;
	}

	//已接收完一条完整消息
	if(big_buf != NULL){
		process_recv((unsigned char*)big_buf, big_buf_len);   //接受消息处理
		big_buf_len = 0;
		big_buf_index = 0;
		printf("uart free buf-----\n");
		free(big_buf);
		big_buf = NULL;
		recv_index = 0;
	}else{
		process_recv(recv_buf, RECV_BUF_LEN);  //接受消息处理
		recv_index = 0;
	}

	//读取的数据中包含下一条消息的开头部分
	if((unsigned int)copy_len < msglen){
		printf("multi msg in one stream left %d byte\n", msglen - copy_len);
		uart_rec(msg + copy_len, msglen - copy_len, user_data);
	}
}
*/
/*******************************************************************
* 名称：            UART_Set
* 功能：            设置串口数据位，停止位和效验位
* 入口参数：        fd         串口文件描述符
*                   speed      串口速度
*                   flow_ctrl  数据流控制
*                   databits   数据位   取值为 7 或者8
*                   stopbits   停止位   取值为 1 或者2
*                   parity     效验类型 取值为N,E,O,,S
*出口参数：         正确返回为1，错误返回为0
*******************************************************************/
int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
     unsigned int   i;
//      int   status;
      int   speed_arr[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
                            B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
      int   name_arr[] = {115200, 38400,  19200,  9600,  4800,  2400,  1200,  300,
                          115200, 38400, 19200,  9600, 4800, 2400, 1200,  300 };
         
    struct termios options;
   
/*
	tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options。
	该函数,还可以测试配置是否正确，该串口是否可用等。
	若调用成功，函数返回值为0，若调用失败，函数返回值为1.
*/

//获取设备属性信息
    if  ( tcgetattr( fd, &options )  !=  0)
       {
          perror("SetupSerial 1");    
          return FALSE;          
       }
  
    //设置串口输入波特率和输出波特率 /i o 入和出
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
        if  (speed == name_arr[i])
        {       
            cfsetispeed(&options, speed_arr[i]); 
            cfsetospeed(&options, speed_arr[i]);  
        }
    }     
   
    //修改控制模式，保证程序不会占用串口
    options.c_cflag |= CLOCAL;
    //修改控制模式，使得能够从串口中读取输入数据
    options.c_cflag |= CREAD;
  
    //设置数据流控制
    switch(flow_ctrl)
    {
      
       case 0 ://不使用流控制
              options.c_cflag &= ~CRTSCTS;
              break;   
      
       case 1 ://使用硬件流控制
              options.c_cflag |= CRTSCTS;
              break;
       case 2 ://使用软件流控制
              options.c_cflag |= IXON | IXOFF | IXANY;
              break;
    }
    //设置数据位
    options.c_cflag &= ~CSIZE; //屏蔽其他标志位
    switch (databits)
    {  
       case 5 :
              options.c_cflag |= CS5;
              break;
       case 6 :
              options.c_cflag |= CS6;
              break;
       case 7 :    
              options.c_cflag |= CS7;
              break;
       case 8 :    
              options.c_cflag |= CS8;
              break;  
       default :   
              fprintf(stderr,"Unsupported data size/n");
              return FALSE; 
    }
    //设置校验位
    switch (parity)
    {  
       case 'n':
       case 'N': //无奇偶校验位。
                 options.c_cflag &= ~PARENB; 
                 options.c_iflag &= ~INPCK;    
                 break; 
       case 'o':  
       case 'O'://设置为奇校验
                 options.c_cflag |= (PARODD | PARENB); 
                 options.c_iflag |= INPCK;             
                 break; 
       case 'e': 
       case 'E'://设置为偶校验
                 options.c_cflag |= PARENB;       
                 options.c_cflag &= ~PARODD;       
                 options.c_iflag |= INPCK;       
                 break;
       case 's':
       case 'S': //设置为空格
                 options.c_cflag &= ~PARENB;
                 options.c_cflag &= ~CSTOPB;
                 break; 
        default:  
                 fprintf(stderr,"Unsupported parity/n");   
                 return FALSE; 
    } 
    // 设置停止位
    switch (stopbits)
    {  
       case 1:   
                 options.c_cflag &= ~CSTOPB; 
                 break; 
       case 2:   
                 options.c_cflag |= CSTOPB; 
                 break;
       default:   
                 fprintf(stderr,"Unsupported stop bits/n"); 
                 return FALSE;
    }
   
    //修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;

    options.c_lflag |= ~(ICANON|ECHOE|ECHO|ISIG);
		//(ICANON | ECHO | ECHOE);    

   
    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */
   
    //如果发生数据溢出，接收数据，但是不再读取
    tcflush(fd,TCIFLUSH);
   
    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)  
    {
       perror("com set error!/n");  
       return FALSE; 
    }
    return TRUE; 
}

//uart初始化
int uart_init(UART_HANDLE *uart_hd, const char *device,int rate, uart_rec_fn uart_rec_cb, void *user_data)
{
	int ret = 0;
	uartData *uart = NULL;
	void * unused;
	pthread_attr_t attr;
	//句柄检查
	if (NULL == device || NULL == uart_rec_cb || NULL == uart_hd){
		return -1;
	}
	//申请结构体
	uart = (uartData*)malloc(sizeof(uartData));
	if (NULL == uart){
		ret = -1;
		goto error;
	}
	memset(uart, 0, sizeof(uartData));
	uart->uart_rec_cb = uart_rec_cb;
	uart->user_data = user_data;
	//打开设备
	uart->fd = open(device,O_RDWR);
	if(uart->fd < 0){
		ret = -1;
		printf("open error\n");
		goto error;
	}
	//设定属性
	if (UART_Set(uart->fd, rate, 0, 8, 1, 'N') == FALSE){   
		ret = -1;  
		printf("set port error\n");
        goto error;
    }
	uart->running = 1;
	
	//创建线程，接收数据
	pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//分离属性
	ret = pthread_create(&(uart->thread_recv), &attr,uart_recv, uart);
	if(0 != ret){
		ret = -1;
		goto error;
	}
	*uart_hd = uart;

	goto exit;
error:
	if (NULL != uart){
		uart->running = 0;
		if (uart->fd > 0){
			close(uart->fd);
		}
		if (0 != uart->thread_recv){
			pthread_join(uart->thread_recv, &unused);
		}
		free(uart);
	}

exit:
	return ret;
}

int uart_send(UART_HANDLE uart_hd, const char *msg, int msglen)
{
	int ret = 0;
	int sended_len = 0;
	uartData *uart = (uartData *)uart_hd;
	if (NULL == uart_hd){
		return -1;
	}
	if(0){
		int i = 0;
		for(i=0;i<msglen;i++)
		{
			printf("%x ",msg[i]);
		}
	}
	//反复发送，直到全部发完
	while(sended_len != msglen){
		int retlen = 0;
		retlen = write(uart->fd, msg + sended_len, msglen - sended_len);
		if (retlen < 0){
			return -1;
		}
		sended_len += retlen;
	}
	//DBG_LOG_TRACE("uart_send()%d[out]", ret);
	return ret;
}

void uart_uninit(UART_HANDLE uart_hd)
{
//	void * unused;
	if (NULL != uart_hd){
		uartData *uart = (uartData *)uart_hd;
		//关闭运行状态
		uart->running = 0;
		if (uart->fd > 0){
			//关闭设备
			close(uart->fd);
		}
		//关闭接收线程
		printf("before join\n");
//		if (NULL != uart->thread_recv){
//			pthread_cancel(uart->thread_recv);
//		}
		printf("after join\n");
		//释放结构体
		free(uart);
	}
}

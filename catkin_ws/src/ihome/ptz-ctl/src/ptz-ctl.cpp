#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>  
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <termios.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <net/if.h>

#include <ros/ros.h> 
#include <robot_msgs/Head.h>

#define	    _PC_DEBUG_
#define	    _PRINT_DEBUG_

#ifdef 		_PRINT_DEBUG_
#define		PRINT(stuff, arg...)	printf(stuff, ##arg)
#else
#define 	PRINT(stuff, arg...)
#endif

#define	CHIP_MAX				4
#define	OUTPUT_BUF_SIZE			0x200
#define	FPGA_CARD_SIZE			0x04000
#define	FLASH_BLOCK_SIZE		768 //串口传输使用
#define	RETRY_TIME				3
#define	IPNUM					8
#define	RX_BUF					100
#define	CMD_BUF					200
#define	UP_FILE					0xb7 //升级使用还是其它
#define	CMD_TIME_1805			(5000000*60)//5 minutes
#define	CMD_TIME_OTH			(5000000)
	
/////////////////////////  TABS帧协议与操作相关信息 //////////////////////////
#define	TABS_ACK_LEN_MAX		1024
#define	CMD_FRM_SIZE			3072
#define	ACK_FRM_SIZE			1024
#define	DATA_BUF_SIZE			2048

#define FRAM_DATA_SIZE			2048

#define	TABS_FM_HEAD			4
#define	TABS_CMD_HEAD_SIZE		8
#define	TABS_CMD_CHCKSUM_SIZE	4
#define	TABS_DATA_POS			(2 + TABS_FM_HEAD)

#define	TABS_CMD_HEAD			0xdb  //发送帧头
#define	TABS_ACK_HEAD			0xe7  //应答帧头

#define	CMD_QUERY_INFO			0x1002//查询命令 add 0x1001


#define	CARD_NAME_SIZE		20  
#define	PARAM_SIZE_MAX		100

#define REQUSET		1
#define ACTION    	2

typedef enum {NONE = 0, QUERY, RECONFIG, UPGRADE, BOOT,EPCS_READ} CMD_TYPE;
typedef	enum {CPU = 0, FPGA, KERNEL,FPGA1,FPGA2,MAX_TYPE} t_img_type;

typedef	enum {NORMAL = 0, CHECK_SUM_ERR, BLK_NUM_ERR, BLK_TOTAL_ERR\
			 , CARD_ID_ERR, IMG_TYPE_ERR, CHIP_NUM_ERR, ERASE_ERR, TIME_OUT} t_ack_state;

int time_count;


///////////////////////////  input param  //////////////////////
typedef enum {FALSE = 0, SUCCESS} t_upg_state;
///////////////////////全局变量//////////////////////////
const char *port = "/dev/ttyUSB0";

int				g_uart_fd;					// 记录串口的句柄

unsigned short	g_tabs_cmd = 0;
unsigned int	g_time_out = 0 ;	    


///////////////////////////函数声明/////////////////////////////
int	query_card_info(void);

unsigned long 	checksum(unsigned char *pframe, unsigned short length);

int time_cmp_usec( struct timeval * start, struct timeval * stop, uint usec_val )
{  
	uint  	 usec;

	if( stop->tv_sec < start->tv_sec )
		return 1;
		
	if( stop->tv_sec == start->tv_sec )
	{
		if( stop->tv_usec <= start->tv_usec )
		{
			return 0;
		}
	}

	usec	= (stop->tv_sec - start->tv_sec) * 1000 * 1000 + (stop->tv_usec - start->tv_usec);

	return (usec > usec_val);
}

void delay_usec( unsigned int usec )
{
	struct timeval	start, stop;

	gettimeofday( &start, NULL );

	do
	{
		gettimeofday( &stop, NULL );
	}	while( !(time_cmp_usec(&start, &stop, usec)) );
}

/***************************************************
	 向串口发送数据 (命令）
***************************************************/
void send_data(unsigned char *buf, int len)
{
	int rc, send_byte;

	rc = 0;
#ifdef	_PRINT_DEBUG_    
	int i;
	printf("\n\nsend data :\n");
	for(i = 0; i < len ; i++)
	{
	if(!(i%16))
	printf("\n");
	printf("%02x ", buf[i]);	
	}
	printf("\n\n");

#endif
	send_byte = len;

	while(send_byte)	
	{
		rc = write(g_uart_fd, (buf), send_byte);
		if(rc > 0)
		{
			send_byte -= rc;
			buf += rc;
		}	
	}
}
/******************************************************************* 
* 名称：				  UART0_Open 
* 功能：				打开串口并返回串口设备文件描述 
* 入口参数：		fd	  :文件描述符	  port :串口号(ttyS0,ttyS1,ttyS2) 
* 出口参数：		正确返回为1，错误返回为0 
*******************************************************************/  
int UART0_Open(int fd,const char* port)  
{  
	 
	fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);  
	if (-1 == fd)  
	{  
		   perror("Can't Open Serial Port");  
		   return(-1);  
	}  
	//恢复串口为阻塞状态								  
	if(fcntl(fd, F_SETFL, 0) < 0)	
	{  
		   printf("fcntl failed!\n");  
		 return(-1);  
	}		
	else  
	{  
	  //printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));  
	}  
	//测试是否为终端设备		
	if(0 == isatty(STDIN_FILENO))  
	{  
		printf("standard input is not a terminal device\n");  
	  return(-1);  
	}  
	else	
	{  
		 printf("isatty success!\n");  
	}				 
	//printf("fd->open=%d\n",fd);  
	return fd;  
}  
/******************************************************************* 
* 名称：				UART0_Close 
* 功能：				关闭串口并返回串口设备文件描述 
* 入口参数：		fd	  :文件描述符	  port :串口号(ttyS0,ttyS1,ttyS2) 
* 出口参数：		void 
*******************************************************************/     
void UART0_Close(int fd)  
{  
	close(fd);	
}    
/******************************************************************* 
* 名称：				UART0_Set 
* 功能：				设置串口数据位，停止位和效验位 
* 入口参数：		fd		  串口文件描述符 
*							   speed	 串口速度 
*							   flow_ctrl   数据流控制 
*							databits   数据位	取值为 7 或者8 
*							stopbits   停止位	取值为 1 或者2 
*							parity	   效验类型 取值为N,E,O,,S 
*出口参数： 		 正确返回为1，错误返回为0 
*******************************************************************/  
int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)	
{  
	 
	  int	i;	
		 int   status;	
		 int   speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};	
	 int   name_arr[] = {115200,  19200,  9600,  4800,	2400,  1200,  300};  
		   
	struct termios options;  
	 
	/*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1. 
	*/	
	if	( tcgetattr( fd,&options)  !=  0)  
	   {  
		  perror("SetupSerial 1");		
		  return(-1);   
	   }  
	
	//设置串口输入波特率和输出波特率  
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
	//屏蔽其他标志位  
	options.c_cflag &= ~CSIZE;	
	switch (databits)  
	{	 
	   case 5	 :	
					 options.c_cflag |= CS5;  
					 break;  
	   case 6	 :	
					 options.c_cflag |= CS6;  
					 break;  
	   case 7	 :		
				 options.c_cflag |= CS7;  
				 break;  
	   case 8:		
				 options.c_cflag |= CS8;  
				 break;    
	   default: 	
				 fprintf(stderr,"Unsupported data size\n");  
				 return (-1);   
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
				 fprintf(stderr,"Unsupported parity\n");	  
				 return (-1);   
	}	
	// 设置停止位	
	switch (stopbits)  
	{	 
	   case 1:	   
				 options.c_cflag &= ~CSTOPB; break;   
	   case 2:	   
				 options.c_cflag |= CSTOPB; break;	
	   default: 	
					   fprintf(stderr,"Unsupported stop bits\n");	
					   return (-1);	
	}  
	 
  //修改输出模式，原始数据输出	
  options.c_oflag &= ~OPOST;  
	
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//我加的  
//options.c_lflag &= ~(ISIG | ICANON);	
	 
	//设置等待时间和最小接收字符  
	options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */	
	options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */  
	 
	//如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读  
	tcflush(fd,TCIFLUSH);  
	 
	//激活配置 (将修改后的termios数据设置到串口中）  
	if (tcsetattr(fd,TCSANOW,&options) != 0)	
		   {  
			   perror("com set error!\n");	  
			  return (-1);	
		   }  
	return (0);	 
}  
/******************************************************************* 
* 名称：				UART0_Init() 
* 功能：				串口初始化 
* 入口参数：		fd		 :	文件描述符	  
*				speed  :  串口速度 
*							   flow_ctrl  数据流控制 
*				databits   数据位	取值为 7 或者8 
*							stopbits   停止位	取值为 1 或者2 
*							parity	   效验类型 取值为N,E,O,,S 
*						
* 出口参数：		正确返回为1，错误返回为0 
*******************************************************************/  
int UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)  
{  
	int err;  
	//设置串口数据帧格式  
	if (UART0_Set(fd,speed,0,8,1,'N') == -1)  
	   {														   
		return -1;  
	   }  
	else  
	   {  
			   return  0;  
	   }  
}  

/***************************************************
	    关闭串口
***************************************************/
void uart_close(int fd)
{
	close(fd);
}

/***************************************************
    打印相关命令的状态应答字相应的信息
****************************************************/
int display_ack_err(unsigned short cmd, unsigned char state)
{
	int rc = -1;
	const char *opstr  = NULL;
	const char *errstr = NULL;

	switch(cmd)
	{		
		default:
			opstr = "UNKNOWN OPRATION";
			break;
	}
	  
	switch(state)
	{
		case NORMAL:
			errstr = "NORMAL";
			rc = 0;
			return rc;
			break;
			
		case CHECK_SUM_ERR:
			errstr = "CHECK_SUM_ERR";
			rc = 1;
			break;
			
		case BLK_TOTAL_ERR:
			errstr = "BLK_TOTAL_ERR";
			break;
			
		case BLK_NUM_ERR:
			errstr = "BLK_NUM_ERR";
			rc = 1;
			break;			
		case TIME_OUT:
			errstr = "TIME_OUT";
			break;
			
		default:
			errstr = "UNKNOWN ERR";
			break;
	}// end of switch

	fflush(stdout);

	return rc; 
}

/***************************************************
    发送组帧(非标tabs帧)
****************************************************/
unsigned short frm_pack(unsigned char *buf, unsigned short len)
{
	unsigned short tx_len = 0;
	int i;
	unsigned int check_sum = 0;

	buf[tx_len++] = 0xdb;
	buf[tx_len++] = 1;
	buf[tx_len++] = (unsigned char)((len & 0xff00)  >>  8);
	buf[tx_len++] = (unsigned char)((len & 0xff));

	for(i = 0; i < (len + TABS_FM_HEAD); i++)
	{
		check_sum += buf[i];	
	}

	buf[tx_len + len] = (unsigned char)((check_sum&0xff000000)  >>  24);
	buf[tx_len + len + 1] = (unsigned char)((check_sum&0x00ff0000)  >>  16);
	buf[tx_len + len + 2] = (unsigned char)((check_sum&0x0000ff00)  >>  8);
	buf[tx_len + len + 3] = (unsigned char)((check_sum&0x000000ff));

	return (len + TABS_CMD_HEAD_SIZE);
}

void init_signal(void)
{
}
//校验和
unsigned long checksum(unsigned char *pframe, unsigned short length)
{
	unsigned short		i;
	unsigned long		sum	= 0;

	for(i = 0; i < length; ++i)
	{
		sum += pframe[i];
	}

	return sum;
}

int Init_Uart(void)
{
	int err;                           //返回调用函数的状态 
	g_uart_fd = UART0_Open(g_uart_fd, port); //打开串口，返回文件描述符  
	 
	err = UART0_Init(g_uart_fd,115200,0,8,1,'N');  
	printf("Set Port Exactly!\n");  
	 
	return g_uart_fd;
}
/***************************************************
	    从串口接收数据（应答帧）
 ***************************************************/
int recv_data(unsigned char *buf, int *return_len)
{
    struct timeval	time_last;
    struct timeval	time_curr;
    unsigned long	sum = 0;
    int				fd	= g_uart_fd;
    int				rc;
    int				i = 0, j	= 0;
    unsigned short	usCostLen 	= 0, usFrmLen = 0;
    unsigned short	rx_len 		= 0;
    unsigned char	rx_tmp_ar[DATA_BUF_SIZE]   = {0};
    unsigned char	frm_buff_ar[DATA_BUF_SIZE] = {0};
    unsigned char	*rx_tmp		= rx_tmp_ar;
    unsigned char	*frm_buff	= frm_buff_ar;

   gettimeofday(&time_last, NULL);
#ifdef _PRINT_DEBUG_
	printf("\nrecv data : \n");
#endif
    while(1)
    {
		rc	= read(fd, rx_tmp, DATA_BUF_SIZE);
		if(rc > 0)
		{
#ifdef _PRINT_DEBUG_
printf("%s\n",rx_tmp);
for(i = 0; i < rc; i++)
{
if(!(i%16))
printf("\n");
printf("%02x ", rx_tmp[i]);
}
printf("\n");
#endif
		
			usleep(1000);
		
			gettimeofday(&time_last, NULL);
	    
			if(rx_len >= TABS_ACK_LEN_MAX)
			{
				rx_len	= 0;
				return -1;
			}
			else if(rc > (DATA_BUF_SIZE - rx_len))
			{
				rc = TABS_ACK_LEN_MAX - rx_len;
			}
	    
			memcpy((frm_buff + rx_len), rx_tmp, rc);
			rx_len += rc;
	    
			for(i = 0; i < rx_len;)
			{
				if(*(frm_buff + i) != TABS_ACK_HEAD)
				{
					i++;
					continue;
				}
		
				frm_buff	= frm_buff + i;
		
				usCostLen	= 4;				

				if(rx_len - i < usCostLen)
				{
					break;
				}

				usFrmLen = (((unsigned short)frm_buff[2])  <<  8) | frm_buff[3];				

				if(0 == usFrmLen)
				{
					i += usCostLen;
					continue;
				}

				usFrmLen += usCostLen;

				if(usFrmLen > ACK_FRM_SIZE)
				{
					i++;
					continue;
				}

				usFrmLen += 4;

				if(rx_len - i < usFrmLen)
				{
					memmove(frm_buff, (frm_buff + i), (rx_len - i));
					rx_len = rx_len - i;
					break;
				}

				for(j = 0; j < (usFrmLen - 4); j++)
				{
					sum += frm_buff[j];
				}
				
				if((frm_buff[usFrmLen - 4] == (unsigned char)(sum  >>  24))
					&& (frm_buff[usFrmLen - 3] == (unsigned char)(sum  >>  16))
					&& (frm_buff[usFrmLen - 2] == (unsigned char)(sum  >>   8))
					&& (frm_buff[usFrmLen - 1] == (unsigned char)(sum)))
				{
					memcpy(buf, frm_buff, usFrmLen);
					*return_len	= usFrmLen;
					return 1;
				}
				else
				{
#ifdef _PRINT_DEBUG_
printf("check sum error, frm_sum : 0x%02x%02x%02x%02x, result : 0x%lx\n"
		, frm_buff[usFrmLen - 4], frm_buff[usFrmLen - 3]
		, frm_buff[usFrmLen - 2], frm_buff[usFrmLen - 1]
		, sum);
#endif
				}
				i++ ;//= usFrmLen;
			}// end of for(i = 0; i < rx_len;)
		}//end of if(rc>0)

		gettimeofday(&time_curr, NULL);
		if(time_cmp_usec(&time_last, &time_curr, g_time_out))
		{
			return -1;
		}
    }//end  of while(1)

    return -1;
}
/***************************************************
    接收应答帧处理
****************************************************/
int deal_ack( )
{
	int rc = 0;
	int buf_len = 0, i = 0;
	unsigned char frm_buf[DATA_BUF_SIZE + 1] = {0};
	unsigned short ret_cmd,count;
	unsigned char card_id = 0;
	unsigned short pos = 0;
	unsigned short j = 0;
	short flag 	= 0;

	g_time_out = 5 * 1000 * 1000;
	
	flag = recv_data(frm_buf, &buf_len);

	if( flag > 0 ) 
	{
		ret_cmd = ((frm_buf[4]  <<  8) | frm_buf[5]);

		switch(ret_cmd)
		{	
			case CMD_QUERY_INFO:
				pos = TABS_DATA_POS;
				printf("\n get data from stm32 :0x%x \n",ret_cmd);
				break;
			default:
				rc = -1;
				break;
		}//end of switch(ret_cmd)
	}//end of if(flag > 0) 
	else
	{
		printf("\n ****   ERR   :receive data timeout !!\n");
	}// end of else

	fflush(stdout);

	return rc;
}

/***************************************************
    测试    
 ***************************************************/
int test_cmd(void)
{
	int rc = 0;
	unsigned char	tx_buf[CMD_FRM_SIZE];
	unsigned char	rx_buf[RX_BUF];
	unsigned int	pos = 0;
	unsigned short	tx_len = 0;

	memset(tx_buf, 0x00, sizeof(tx_buf));
	memset(rx_buf, 0x00, sizeof(rx_buf));
	pos = TABS_FM_HEAD;
	
	tx_buf[pos++] = 0x10;
	tx_buf[pos++] = 0x02;
		
	tx_len = frm_pack(tx_buf, (pos-TABS_FM_HEAD));
	send_data(tx_buf, tx_len);
	
	rc = deal_ack( );
	fflush(stdout);
	return rc;
}

//(0,0)      (640,480)
//    (320,240)
//(x-320, y-240)

int set_motor(unsigned char ch, unsigned char dir, unsigned short pulse)
{
	int rc = 0;
	unsigned char	tx_buf[CMD_FRM_SIZE];
	unsigned char	rx_buf[RX_BUF];
	unsigned int pos = 0;
	unsigned short tx_len = 0;

	memset(tx_buf, 0x00, sizeof(tx_buf));
	memset(rx_buf, 0x00, sizeof(rx_buf));
	pos = TABS_FM_HEAD;
		
	tx_buf[pos++] = 0x20;
	tx_buf[pos++] = 0x01;
	tx_buf[pos++] = ch;//channle
	tx_buf[pos++] = dir;//dir
	tx_buf[pos++] = (unsigned char)pulse;//pulse low bit
	tx_buf[pos++] = (unsigned char)(pulse >> 8);//high bit
	tx_len =frm_pack(tx_buf, (pos-TABS_FM_HEAD));
	send_data(tx_buf, tx_len);

	rc = deal_ack();
    fflush(stdout);
	return rc;
}
int reset_motor_pose()
{
	set_motor(1, 2, 7140);
	set_motor(1, 2, 510);
	set_motor(2, 1, 2050);
	set_motor(2, 2, 510);
}

void head_move(unsigned short x, unsigned short y)
{
	unsigned short dx = 0,dy = 0;
  	unsigned char dir1 = 0,dir2 = 0;
	if((x - 320) < 0)
	{
		dir1 = 1;
		dx = 320 - x;
	}else{
		dir1 = 2;
		dx = x - 320;
	}
	if((y - 240) < 0)
	{
		dir2 = 1;
		dy = 240 - y;
	}else{
		dir2 = 2;
		dy = y - 240;
	}
	set_motor(1, dir1, dx);
	set_motor(2, dir2, dy);
}

void head_action(unsigned char type, unsigned short x, unsigned short y)
{

	switch(type)
	{
		case 1:
			reset_motor_pose();
			break;
		case 2:
			head_move(x,y);
			break;
		default:break;
	}
}

bool head_service(robot_msgs::Head::Request  &req,robot_msgs::Head::Response &res)  
{  
  unsigned int err = 0;
  switch(req.id)
  {
  		case REQUSET:
			//err = get_aiui_status(req.type);
  		break;
  		case ACTION:
			head_action(req.type, req.x, req.y);
  		break;
  		default:break;
  }
  res.x = 1;
  res.y = 2;
  res.error = 0;
  ROS_INFO("request: id=%d, type=%d, x = %d y= %d", req.id, req.type, req.x, req.y); 
 // ROS_INFO("sending back response: [%d]",res.code);  
  return true;  
} 

/***************************************************
					主函数
****************************************************/	 
int main(int argc, char *argv[])
{
	ros::init(argc, argv, "head");  
    ros::NodeHandle nh_; 
	ros::ServiceServer service;

	Init_Uart();

	service = nh_.advertiseService("/head", head_service);
    set_motor(1, 2, 7140);
	//set_motor(2, 1, 2050);
	//reset_motor_pose();
	
	ros::Rate loop_rate(50);
	while (nh_.ok()) 
	{  
		ros::spinOnce();  
		loop_rate.sleep();
		break;
	}  
	uart_close(g_uart_fd);
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <termios.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include "ros/ros.h"
#include <ros/time.h>
#include <robot_msgs/Ear.h>
#include <robot_msgs/Speek.h>
#include "cJSON.h"
#include "aiui.h"
#include "uart.h"

using namespace std;

extern UART_HANDLE uart_hd;

#define UART_DEVICE "/dev/ttyUSB0"

ros::Publisher pub_ear;
ros::ServiceServer service;

bool speek_service(robot_msgs::Speek::Request  &req,robot_msgs::Speek::Response &res)  
{  
  static int code = 0;
  int err = 0;
  res.code = code++;
  switch(req.id)
  {
  		case REQUSET:
  		break;
  		case ACTION:
			err = ask_aiui_todo(req.type,req.arg1.c_str(),req.arg2.c_str());
  		break;
  		default:break;
  }
  ROS_INFO("request: id=%d, type=%d,arg1=%s arg2=%s", req.id, req.type, req.arg1.c_str(),req.arg2.c_str());
  AiuiCtrl_tts_start((char*)req.arg1.c_str());  
  ROS_INFO("sending back response: [%d]",res.code);  
  return true;  
} 

int main(int argc, char* argv[])
{
	int ret = 0;
	ros::init(argc, argv, "aiui");
	ros::NodeHandle nh_;
	pub_ear = nh_.advertise<robot_msgs::Ear>("/aiui_ear", 1);
	service = nh_.advertiseService("/speek", speek_service);

	char bugff[4096] = {"start system!"};

	ret = uart_init(&uart_hd, UART_DEVICE,115200, uart_rec, NULL);
	if (0 != ret)
	{
		ROS_INFO("uart_init error ret = %d\n", ret);
	}
	
	ros::Rate loop_rate(0.2);   //指定一个循环频率
	int count = 0;

	while (ros::ok())
	{
		//ros::ok()在以下情况下会返回false：
		//1 收到SIGINT信号（ctrl-c）；
		//2 节点网络断开；
		//3 ros::shutdown()函数被调用；
		//4 所有的ros::NodeHandles都已经消失.

		// Aiui_config("renxiaoliang","passwd-ren","main",true);
		ros::spinOnce();

		loop_rate.sleep();   //休眠一段时间以达到循环频率10Hz

		++count;
	}
}

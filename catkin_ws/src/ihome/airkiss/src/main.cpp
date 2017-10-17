#include <stdio.h>
#include <math.h>
#include "ros/ros.h"  
#include <robot_msgs/Speak.h> 
#include <cstdlib>  
#include "aiui.h"
#include "app.h"

int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "airkiss");    
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<robot_msgs::Speak>("/speak");  

  if (argc == 3)  
  {  
	std::stringstream arg1;
  	std::stringstream arg2;
    robot_msgs::Speak srv;  
    srv.request.id = ACTION;  
    srv.request.type = TODO_CONFIG_WIFI;	
    arg1 << argv[1];
	arg2 << argv[2];
    srv.request.arg1 = arg1.str();
	srv.request.arg2 = arg2.str();
    if (client.call(srv))  
    {  
      ROS_INFO("code: %d", srv.response.code);
    }  
    else  
    {  
      ROS_ERROR("Failed to call service to config wifi");  
    }	
  }
  else if(argc == 2)
  {
  	  while(ros::ok())
	  {
		if(app(argv[1]) == 1){
			ROS_INFO("get wifi pass ok !\n");
			return 0;
		}else{
		    ROS_ERROR("get wifi pass error !\n");
			return 0;
		}
	  }
  }else{
	printf("usesge: 1. rosrun airkiss airkiss ssid passwd\n        2. rosrun airkiss airkiss wlan0\n");
	return 0; 
  }	  
  return 0;  
} 

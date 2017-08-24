#include <stdio.h>
#include <math.h>
#include "ros/ros.h"  
#include <robot_msgs/Speek.h> 
#include <cstdlib>  
#include "aiui.h"

int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "say");
  unsigned char id = 0;
  unsigned char type = 0;
  if (argc != 5)  
  {  
	ROS_INFO("/say id type name test\n");
  }  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<robot_msgs::Speek>("/speek");  

  while(ros::ok())
  {
  	std::stringstream arg1;
  	std::stringstream arg2;
    robot_msgs::Speek srv;  
    srv.request.id = atoi(argv[1]);  
    srv.request.type = atoi(argv[2]);	
    arg1 << argv[3];
	arg2 << argv[4];
    srv.request.arg1 = arg1.str();
	srv.request.arg2 = arg2.str();
    if (client.call(srv))  
    {  
      ROS_INFO("code: %d", srv.response.code);
	  break;
    }  
    else  
    {  
      ROS_ERROR("Failed to call service add_two_ints");  
    }
  }
  return 0;  
} 
#include "ros/ros.h"  
#include <robot_msgs/Speek.h> 
#include <cstdlib>  

#define WIFI_STATUS 1
#define TTS_STATUS  2

int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "say");
  int debug = 0;
  if (argc == 2)  
  {  
    ROS_INFO("debug mode");
    debug = 1;
  }  
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<robot_msgs::Speek>("/speek");  

  while(ros::ok())
  {
    robot_msgs::Speek srv;  
    srv.request.id = 1;  
    srv.request.type = 2;
    std::stringstream ss;
    if(debug ==1)
        ss << argv[1];
    else
        ss << "test msg";
    srv.request.arg1 = ss.str();
    if (client.call(srv))  
    {  
      ROS_INFO("code: %d", srv.response.code);  
    }  
    else  
    {  
      ROS_ERROR("Failed to call service add_two_ints");  
      return 1;  
    }
    if(debug == 1)
      break; 
  }
  
  
  return 0;  
} 
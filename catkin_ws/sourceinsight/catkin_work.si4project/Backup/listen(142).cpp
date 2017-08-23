#include "ros/ros.h"
#include "robot_msgs/Ear.h"

void chatterCallback(const robot_msgs::Ear::ConstPtr& msg)   //callback函数，收到msg时调用
{
  printf("get data :%s\n",msg->data.c_str());
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/aiui_ear", 10, chatterCallback);

  ros::spin();

  return 0;
}

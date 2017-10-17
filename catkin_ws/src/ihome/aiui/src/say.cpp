#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>   
#include <iostream> 

#include "ros/ros.h"  
#include <robot_msgs/Speak.h> 
#include <cstdlib>  
#include "aiui.h"

using namespace std;

void SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

//rosrun aiui say 2 11 xiaoyan 任晓亮,你好欢迎乘坐地平线自动驾驶汽车
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "say");
  unsigned char id = 0;
  unsigned char type = 0;

  ROS_INFO("rosrun aiui say id type name test\n");
  
  ros::NodeHandle n;  
  ros::ServiceClient client = n.serviceClient<robot_msgs::Speak>("/speak"); 
  
  if(argc == 2 && access("./data/say_data.txt",0) == 0)
  {
	    ifstream in("./data/say_data.txt");
		string line;
		while (getline (in, line))
		{
			vector<string> v;
			SplitString(line, v,","); //可按多个字符来分隔;
			printf(" size: %d = %s - %s - %s - %s - %s\n",(int)v.size(),v[0].c_str(),v[1].c_str(),v[2].c_str(),v[3].c_str(),v[4].c_str());
			const char *ptr = v[0].c_str();
			if(ptr[0] == '#')
				continue;
			robot_msgs::Speak srv;  
			srv.request.id = atoi(v[1].c_str());  
			srv.request.type = atoi(v[2].c_str());	
			srv.request.arg1 = v[3];
			srv.request.arg2 = v[4];
		    if (client.call(srv))  
		    {  
		      ROS_INFO("code: %d", srv.response.code);
		    }  
		    else  
		    {  
		      ROS_ERROR("Failed to call service add_two_ints");  
		    }
			sleep(1); 
		}
		return 0;
  }

  if (argc == 5)
  {
	  while(ros::ok())
	  {
	  	std::stringstream arg1;
	  	std::stringstream arg2;
	    robot_msgs::Speak srv;  
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
  }
  return 0;  
} 
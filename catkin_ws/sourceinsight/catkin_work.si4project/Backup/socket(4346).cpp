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

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "ros/ros.h"
#include <sensor_msgs/TimeReference.h>
#include <sensor_msgs/LaserScan.h>
#include <math.h>
#include <ros/time.h>
#include <autodrive_msgs/Obstacle.h>
#include <autodrive_msgs/Obstacles.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Vector3.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>
#include <visualization_msgs/Marker.h>  
#include <pcl/io/pcd_io.h>  
#include <pcl/impl/point_types.hpp>  
#include <pcl_ros/point_cloud.h>  
#include <vector>  
#include <pcl_conversions/pcl_conversions.h> 
#include <cmath>

using namespace std;

#define SOCK_PORT 	10001
#define BUFFER_LENGTH 	1024
#define SERVER_IP	"127.0.0.1"
//#define SERVER_IP	"10.31.32.114"
#define PORT_NUM	12
#define FRAME_LEN 	50 
#define FRAME_HEAD_LEN 2
unsigned short distence[12] = {0,0,0,0};

ros::Time time_ref;
int seq_ref = 0;
ros::Time time_ros;
int sequence = 0;
ros::Publisher pub_ultrasonic_point2cloud;
ros::Publisher pub_ultrasonic_Object;

ros::Publisher marker_pub;

void timeCallback(const sensor_msgs::TimeReferenceConstPtr& msg) 
{  
	time_ref = msg->time_ref;  
	seq_ref = msg->header.seq;  
	time_ros = ros::Time::now();  
}

struct ult_position
{
	double x;
	double y;
	double z;
	double post_x;
	double post_y;
	double post_z;
	double post_w;
	double angle;
	unsigned short distence;
};
ult_position ultrasonic_post[6] = {{-8,-3,0,0.707,0,0.707,0,165},{-6,-1,0,0.653,-0.270,0.653,0.270,120},{-4,0,0,0.707,0,0,0.707,75},{4,0,0,0.707,0,0,0.707,75},{6,-1,0,0.653,0.270,0.653,-0.270,30},{8,-3,0,0.707,0,0.707,0,-15}};

void ultrasonic_point2cloud(double distence,int id) 
{   
  pcl::PointCloud<pcl::PointXYZ> cloud;  
  sensor_msgs::PointCloud2 output;  
  // Fill in the cloud data  
  cloud.width = 100;  
  cloud.height = 1;  
  cloud.points.resize(cloud.width * cloud.height);  
  for (size_t i = 0; i < cloud.points.size(); ++i)  
  {  
    cloud.points[i].x = distence/100.0 + 1024 * rand () / (RAND_MAX + 1.0f);  
    cloud.points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);  
    cloud.points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);  
  }  
  //Convert the cloud to ROS message  
  pcl::toROSMsg(cloud, output);
  char frame_name[20];
  sprintf(frame_name,"ultrasonic%d",id);  
  output.header.frame_id = frame_name;//this has been done in order to be able to visualize our PointCloud2 message on the RViz visualizer  
  pub_ultrasonic_point2cloud.publish(output);  
}

void ultrasonic_Object() 
{   
		autodrive_msgs::Obstacles objArray;
		for(int i = 0; i < PORT_NUM; i++)
		{
			if(distence[i] < 200)
				continue;
			string frame_name = "ultrasonic" + i;
			unsigned short boxWidth = 500;
			unsigned short boxLength = (unsigned short)( distence[i]*sin(15.0)*2 );
			short boxOrientation = 0;
			autodrive_msgs::Obstacle msg_obj;
			msg_obj.header.frame_id = frame_name;
			msg_obj.header.stamp = ros::Time::now();
			msg_obj.Classification = -1;
			msg_obj.ObsId = i ;
			msg_obj.Life = 0.1 ;

			msg_obj.Width =  boxWidth / 100.; 
			msg_obj.Length = boxLength  / 100.;
            if(msg_obj.Length > 2) 
                continue;
			msg_obj.Height = 0;
			msg_obj.ObsTheta = (boxOrientation / 100.) / 180. * M_PI;
			msg_obj.Orientation = tf::createQuaternionMsgFromYaw((double)msg_obj.ObsTheta);
			
			geometry_msgs::Point p; //center point
			p.x = distence[i]*sin(15.0) + 0.25;
			p.y = 0.0;
			msg_obj.ObsPosition = p;

			geometry_msgs::Vector3 v;
			v.x = 0;
			v.y = 0;
			msg_obj.Velocity = v;

			geometry_msgs::Point tp1, tp2, tp3, tp4;
			tp1.x =  p.x -msg_obj.Length / 2;
			tp1.y =  p.y -msg_obj.Width / 2;
			tp2.x =  p.x +msg_obj.Length / 2;
			tp2.y =  p.y -msg_obj.Width / 2;
			tp3.x =  p.x +msg_obj.Length / 2;
			tp3.y =  p.y +msg_obj.Width / 2;
			tp4.x =  p.x -msg_obj.Length / 2;
			tp4.y =  p.y +msg_obj.Width / 2;
			msg_obj.PolygonPoints.push_back(tp1);
			msg_obj.PolygonPoints.push_back(tp2);
			msg_obj.PolygonPoints.push_back(tp3);
			msg_obj.PolygonPoints.push_back(tp4);
			objArray.obs.push_back(msg_obj);
		}
		objArray.header.frame_id = "gnss";
		objArray.header.stamp = ros::Time::now();
		pub_ultrasonic_Object.publish( objArray );
        cout << "end process objs" << endl;
}  

int main(int argc,char **argv)
{
    int sockfd = 0;
    int tempfd = 0;
    struct sockaddr_in s_addr_in;
    unsigned char data_send[BUFFER_LENGTH];
    unsigned char data_recv[BUFFER_LENGTH]; 
    
    ros::init(argc, argv, "ultrasonic");
	ros::NodeHandle nh_;
	
	pub_ultrasonic_point2cloud = nh_.advertise<sensor_msgs::PointCloud2>("/sensor/ultrasonic12/pointscloud2", 1);
	pub_ultrasonic_Object = nh_.advertise<autodrive_msgs::Obstacles>("/sensor/ultrasonic12/objects", 1);

    printf("Get connect to server :%s \n", SERVER_IP);

    memset(data_send,0,BUFFER_LENGTH);
    memset(data_recv,0,BUFFER_LENGTH);

    sockfd = socket(AF_INET,SOCK_STREAM,0);       //ipv4,TCP
    if(sockfd == -1)
    {
        fprintf(stderr,"socket error!\n");
        exit(1);
    }

    //before func connect, set the attr of structure sockaddr.
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_addr.s_addr = inet_addr(SERVER_IP);      //trans char * to in_addr_t
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_port = htons(SOCK_PORT);
    
    while(1)
    {
	    tempfd = connect(sockfd,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
	    if(tempfd == -1)
	    {
	        fprintf(stderr,"Connect error! \n");
	    }else{
	    	break;
	    }
	    sleep(1);
    }

    while (ros::ok())
	{
		memset(data_recv,0,BUFFER_LENGTH);
		memset(distence,0,PORT_NUM*sizeof(short));	
		int recv_length = read(sockfd,data_recv,BUFFER_LENGTH);
		assert(recv_length != -1);
		//printf("\nrecv length ===:%d\n",recv_length);
		if(recv_length == FRAME_LEN)
		{
			// for(int i = FRAME_HEAD_LEN; i < recv_length; i++)
			// {
			// 	printf("%02x ",data_recv[i]);
			// 	if((i+1-FRAME_HEAD_LEN)%4 == 0)
			// 		printf("\n");	
			// }
			if(data_recv[0] == 0x55 && data_recv[1] == 0xaa)
			{
				for(int i = 0; i < PORT_NUM; i++)
				{
					if( data_recv[FRAME_HEAD_LEN+i*4] == 0xff )
					{
						unsigned int sum = data_recv[FRAME_HEAD_LEN+i*4+0] + data_recv[FRAME_HEAD_LEN+i*4+1] + data_recv[FRAME_HEAD_LEN+i*4+2];
						sum = sum & 0xff;
						if(data_recv[FRAME_HEAD_LEN+i*4+3] = (unsigned char)sum)
						{
							distence[i] = (unsigned short)data_recv[FRAME_HEAD_LEN+i*4+1];
							distence[i] = distence[i] << 8 | (unsigned short)data_recv[FRAME_HEAD_LEN+i*4+2];
							printf("sensor%d : %d mm = %.2f\n",i,distence[i],distence[i]/100.0);
        					ultrasonic_point2cloud(distence[i],i);
						}else{
							distence[i] = 0;
						}             
					}
				}
				ultrasonic_Object(); 
			}			
		}
    }
    close(sockfd);
    return 0;
}
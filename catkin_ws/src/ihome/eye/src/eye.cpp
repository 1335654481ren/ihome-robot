#include <ros/ros.h>  
#include <image_transport/image_transport.h>  
#include <opencv2/highgui/highgui.hpp>  
#include <cv_bridge/cv_bridge.h>  
#include <sstream> // for converting the command line parameter to integer  
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
using namespace std;
using namespace cv;


#define  VIDEO   "/dev/video0"

int main(int argc, char** argv)  
{  
  // Check if video source has been passed as a parameter  
  ros::init(argc, argv, "eye");  
  ros::NodeHandle nh;  
  image_transport::ImageTransport it(nh);  
  image_transport::Publisher pub = it.advertise("camera/image", 1);  
  
  cv::VideoCapture cap(0);  
  // Check if video device can be opened with the given index  
  if(!cap.isOpened())   
  {  
      ROS_INFO("can not opencv video device\n");  
      return 1;  
  }else{
    cap.set(CV_CAP_PROP_FPS, 30);
    //cvSetCaptureProperty(cap,CV_CAP_PROP_FPS,30);
  }  
  cv::Mat frame;  
  sensor_msgs::ImagePtr msg;  
  
  ros::Rate loop_rate(50);
  while (nh.ok()) 
  {  
    cap >> frame;
    // Check if grabbed frame is actually full with some content  
    if(!frame.empty())
    {  
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();  
      pub.publish(msg);  
      //cv::Wait(1);  
    }  
  
    ros::spinOnce();  
    loop_rate.sleep();  
  }  
}

/*
#include <ros/ros.h>  
#include <image_transport/image_transport.h>  
#include <opencv2/highgui/highgui.hpp>  
#include <cv_bridge/cv_bridge.h>  
  
void imageCallback(const sensor_msgs::ImageConstPtr& msg)  
{  
  try  
  {  
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);  
   // cv::WaitKey(30);  
  }  
  catch (cv_bridge::Exception& e)  
  {  
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());  
  }  
}  
  
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "eye");  
  ros::NodeHandle nh;  
  cv::namedWindow("view");  
  cv::startWindowThread();  
  image_transport::ImageTransport it(nh);  
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);  
  ros::spin();  
  cv::destroyWindow("view");  
}  
*/
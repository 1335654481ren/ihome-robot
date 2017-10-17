#include <ros/ros.h>  
#include <image_transport/image_transport.h>  
#include <opencv2/highgui/highgui.hpp>  
#include <cv_bridge/cv_bridge.h>  
#include <sstream> // for converting the command line parameter to integer  
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/objdetect/objdetect.hpp"  
#include "opencv2/highgui/highgui.hpp" 

#include <robot_msgs/Head.h>

using namespace std;
using namespace cv;

const string cascadeName = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
const string nestedCascadeName = "/usr/share/opencv/haarcascades/haarcascade_eye.xml";
const double scale = 1.3;

HOGDescriptor hog;  

ros::ServiceClient client;

void detectFace(Mat& img, CascadeClassifier& cascade,CascadeClassifier& nestedCascade) 
{
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] = {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;
    Point center;
    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
    equalizeHist(smallImg, smallImg);
    t = (double)getTickCount();
    cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            |CASCADE_SCALE_IMAGE,
            Size(30, 30));
    t = (double)getTickCount() - t;
//    printf("detection time = %g ms\n", t*1000/getTickFrequency());    
    for (size_t i = 0; i < faces.size(); ++i) {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Scalar color = colors[i%8];
        int radius;
        double aspect_ratio = (double)r.width/r.height;
        if(0.75 < aspect_ratio && aspect_ratio < 1.3) {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
            radius = cvRound((r.width + r.height)*0.25*scale);
            circle(img, center, radius, color, 3, 8, 0);
        }
        // } else {
        //     rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
        //             cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
        //             color, 3, 8, 0);
        // }
        // smallImgROI = smallImg(r);
        // nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
        //         1.1, 2, 0
        //         |CASCADE_SCALE_IMAGE,
        //         Size(30, 30) );
        // for (size_t j = 0; j < nestedObjects.size(); ++j) {
        //     Rect nr = nestedObjects[j];
        //     center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
        //     center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
        //     radius = cvRound((nr.width + nr.height)*0.25*scale);
        //     circle(img, center, radius, color, 3, 8, 0);
        // }
    }
    robot_msgs::Head srv;  
    srv.request.id = 1;  
    srv.request.type = 2; 

    srv.request.x = center.x;
    srv.request.y = center.y;
    if (client.call(srv))  
    {  
      ROS_INFO("code: %d", srv.response.error);
    }  
    else  
    {  
      ROS_ERROR("Failed to call service head");  
    }
    printf("center: x = %d  y = %d \n",center.x, center.y);
    imshow("webcamimg", img);
    waitKey(1);
}

void img_Callback(const sensor_msgs::ImageConstPtr& msg) {
    Mat src ,dst;
    CascadeClassifier cascade, nestedCascade;
    try {
        src = cv_bridge::toCvShare(msg, "bgr8")->image;
        dst.create( src.size(), src.type());
        Mat map_x;
        Mat map_y;
        map_x.create( src.size(), CV_32FC1);
        map_y.create( src.size(), CV_32FC1);
        for( int i = 0; i < src.rows; ++i)
        {
          for( int j = 0; j < src.cols; ++j)
          {
            map_x.at<float>(i, j) = (float) (src.cols - j) ;
            map_y.at<float>(i, j) = (float) i;//(src.rows - i) ;
          }
        }
        remap(src, dst, map_x, map_y, CV_INTER_LINEAR);
        //CascadeClassifier cascade, nestedCascade;
        nestedCascade.load(nestedCascadeName);
        cascade.load(cascadeName);
        
        // vector<Rect> found, found_filtered;  
        // double t = (double)getTickCount();
        // hog.detectMultiScale(dst, found, 0, Size(8,8), Size(32,32), 1.05, 2); 
        // t = (double)getTickCount() - t;  
        // printf("tdetection time = %gms\n", t*1000./cv::getTickFrequency());  
        // size_t i, j;  
        // for( i = 0; i < found.size(); i++ )//去掉空间中具有内外包含关系的区域，保留大的  
        // {  
        //     Rect r = found[i];  
        //     for( j = 0; j < found.size(); j++ )  
        //         if( j != i && (r & found[j]) == r)  
        //             break;  
        //     if( j == found.size() )  
        //         found_filtered.push_back(r);  
        // }  
        // for( i = 0; i < found_filtered.size(); i++ )  
        // {  
        //     Rect r = found_filtered[i];  
        //     // the HOG detector returns slightly larger rectangles than the real objects.  
        //     // so we slightly shrink the rectangles to get a nicer output.  
        //     r.x += cvRound(r.width*0.1);  
        //     r.width = cvRound(r.width*0.8);  
        //     r.y += cvRound(r.height*0.07);  
        //     r.height = cvRound(r.height*0.8);  
        //     rectangle(dst, r.tl(), r.br(), cv::Scalar(0,255,0), 3);// tl:the top-left corner,br: the bottom-right corner  
        // }  
        // imshow("people detector", dst);  

        detectFace(dst, cascade, nestedCascade);
        
        //Convert an immutable sensor_msgs::Image message to an OpenCV-compatible CvImage, 
        //sharing the image data if possible. 
        //imshow("img", cv_bridge::toCvShare(msg, "bgr8")->image);//IplImage 类型的 mat
        //waitKey(1);
    } catch (cv_bridge::Exception &e) {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
    if (!src.empty()) detectFace(dst, cascade, nestedCascade);
    else printf("no image!");
}
int main(int argc, char *argv[]) { 
    ros::init(argc, argv, "face_tracker");
    ros::NodeHandle nh_; 
    client = nh_.serviceClient<robot_msgs::Head>("/head");
    
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    namedWindow("webcamimg");
    startWindowThread();
    // CascadeClassifier cascade, nestedCascade;
    // if (!nestedCascade.load(nestedCascadeName))
    //     cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    // if (!cascade.load(cascadeName)) {
    //     cerr << "ERROR: Could not load classifier cascade" << endl;
    //     return -1;
    //    }

    image_transport::ImageTransport it(nh_);
    image_transport::Subscriber img_sub = it.subscribe("camera/image", 1, &(img_Callback));

    destroyWindow("webcamimg");
    ros::spin();
    return 0;
}
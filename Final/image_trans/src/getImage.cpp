#include <ros/ros.h>
#include <string>
#include <image_transport/image_transport.h>
#include<opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
using namespace std;


class getImage{
private:
  ros::NodeHandle nh;
  image_transport::ImageTransport it;
  image_transport::Publisher pub_image;
  //ros::Publisher pub_url;
public:
  getImage():it(nh){
    imagePublish();
  }
  void imagePublish(){
    //publish image
    pub_image = it.advertise("image", 1);
    cv::Mat image = cv::imread("/home/laiwd3/image_transport_ws/src/image_trans/src/roger.jpg");
    cv::waitKey(30);  
    sensor_msgs::ImagePtr msg_ptr = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

    ros::Rate loop_rate(5);
    while (nh.ok()){
      pub_image.publish(msg_ptr);      
      ros::spinOnce();
      loop_rate.sleep();
    }
  }

};
int main(int argc, char** argv)
{
  ros::init(argc, argv, "getImage");
  getImage im;
  ros::spin();
}


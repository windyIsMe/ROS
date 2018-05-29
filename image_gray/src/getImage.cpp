#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

class getImage{
private:
  ros::NodeHandle nh;
  image_transport::ImageTransport it;
  image_transport::Publisher pub;
public:
  getImage():it(nh){
    imagePublish();
  }
  void imagePublish(){
    pub = it.advertise("camera/image", 1);
    cv::Mat image = cv::imread("/home/laiwd3/下载/roger.jpg", CV_LOAD_IMAGE_COLOR);
    cv::waitKey(30);  
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

    ros::Rate loop_rate(5);
    while (nh.ok()){
      pub.publish(msg);      
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


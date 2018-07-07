    #include<ros/ros.h> //ros标准库头文件  
    #include<iostream> //C++标准输入输出库 
    #include<cstdlib> 
    #include<unistd.h>
    #include "std_msgs/String.h"
    #include<stdlib.h>
    /* 
      cv_bridge中包含CvBridge库 
    */  
    #include<cv_bridge/cv_bridge.h>   
    /* 
      ROS图象类型的编码函数 
    */  
    #include<sensor_msgs/image_encodings.h>   
    /* 
       image_transport 头文件用来在ROS系统中的话题上发布和订阅图象消息 
    */  
    #include<image_transport/image_transport.h>   
      
    //OpenCV2标准头文件  
    #include<opencv2/core/core.hpp>  
    #include<opencv2/highgui/highgui.hpp>  
    #include<opencv2/imgproc/imgproc.hpp>  
    using namespace std;
    using namespace cv;
      
    const string INPUT = "Input"; //定义输入窗口名称  
    const string OUTPUT = "Output"; //定义输出窗口名称  
    cv::Mat src;
    cv::Mat dst;
    int morph_elem = 0;
    int morph_size = 0;
    int morph_operator = 0;
    int max_operator = 4;  
    int max_elem = 2;
    int max_kernel_size = 21;
    string window_name = "Morphology Transformation";

    void Morphology_Operations(int, void*);
    //定义一个转换的类  
    class imageServer
    {  
    private:  
        ros::NodeHandle nh_; //定义ROS句柄  
        image_transport::ImageTransport it_; //定义一个image_transport实例  
        image_transport::Subscriber image_sub_; //定义ROS图象接收器  
    public:  
        imageServer():it_(nh_)
        {  
            //定义图象接受器，订阅话题是“image”   
            image_sub_ = it_.subscribe("image", 1, &imageServer::image_callback, this); 
        }  
        /* 
          这是一个ROS和OpenCV的格式转换回调函数，将图象格式从sensor_msgs/Image  --->  cv::Mat 
        */  
        void image_callback(const sensor_msgs::ImageConstPtr& msg_ptr)   
        {  
            cv_bridge::CvImagePtr cv_ptr; // 声明一个CvImage指针的实例  
            try  
            {  
                cv_ptr =  cv_bridge::toCvCopy(msg_ptr, sensor_msgs::image_encodings::RGB8); //将ROS消息中的图象信息提取，生成新cv类型的图象，复制给CvImage指针  
            }  
            catch(cv_bridge::Exception& e)  //异常处理  
            {  
                ROS_ERROR("cv_bridge exception: %s", e.what());  
                return;  
            } 
            int num;
            cout << "Please choose the function you want to do with the image:" << endl;
            cout << "1. gray" << endl;
            cout << "2. morphology" << endl;
            cout << "Input number: ";
            cin >> num;
            if (num == 1){
                //初始化输入输出窗口 
                image_gray(cv_ptr->image);
                exit(0);
            }
            else if (num == 2){
                morphology(cv_ptr->image);
                exit(0);
            }
            else{
                cout << "Input error!\n";
            }
            
        }  

        void morphology(cv::Mat image){
          if (image.empty())
          {
            std::cout << "Could not open or find the image!\n" << std::endl;
            return;
          }
          src =  image;

          //![window]
          namedWindow( window_name, WINDOW_NORMAL ); // Create window
          resizeWindow(window_name, 800, 600); //resize window
          //![window]

          //![create_trackbar1]
          /// Create Trackbar to select Morphology operation
          createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, 
                          &morph_operator, max_operator, Morphology_Operations );
          //![create_trackbar1]

          //![create_trackbar2]
          /// Create Trackbar to select kernel type
          createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
                          &morph_elem, max_elem, Morphology_Operations );
          //![create_trackbar2]

          //![create_trackbar3]
          /// Create Trackbar to choose kernel size
          createTrackbar( "Kernel size:\n 2n +1", window_name,
                          &morph_size, max_kernel_size, Morphology_Operations );
          //![create_trackbar3]

          /// Default start
          Morphology_Operations(0, 0);  
        
          waitKey(30000);

          destroyWindow(window_name);
        }

        /* 
           这是图象处理的主要函数，一般会把图像处理的主要程序写在这个函数中。这里的例子只是一个彩色图象到灰度图象的转化 
        */  
        void image_gray(cv::Mat img)   
        {  

            namedWindow(INPUT, WINDOW_NORMAL);
            namedWindow(OUTPUT, WINDOW_NORMAL);
            resizeWindow(INPUT, 800, 600);
            resizeWindow(OUTPUT, 800, 600);
            cv::Mat img_out;  
            cv::cvtColor(img, img_out, CV_RGB2GRAY);  //转换成灰度图象  
           
            cv::imshow(INPUT, img);  
            cv::imshow(OUTPUT, img_out);
            cv::waitKey(8000);  
            destroyWindow(INPUT);
            destroyWindow(OUTPUT);
        }  
    };  

    /** Function Headers */
    void Morphology_Operations(int, void*){
      // Since MORPH_X : 2,3,4,5 and 6
      //![operation]
      int operation = morph_operator + 2;
      //![operation]

      Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size + 1 ), Point( morph_size, morph_size ) );

      /// Apply the specified morphology operation
      morphologyEx( src, dst, operation, element );
      imshow( window_name, dst );
    }
    //主函数  
    int main(int argc, char** argv)  
    {  
        ros::init(argc, argv, "imageServer");  
        imageServer obj;  
        ros::spin();  
    }  

# 使用说明
**1.打开FINAL文件夹下web文件夹**  
**2.命令行运行python server.py, 即可访问http://0.0.0.0:8080/看到前端界面**  
**3.FINAL文件夹下image_trans文件夹为ROS下的package,可由catkin_make, rosrun编译运行, src中包括一个发布器和一个订阅器**  
**4.编译前必须将web文件夹下server.py中的IMAGE_PATH和image_trans/src文件夹下getImage.cpp中的IMAGE_PATH改为相同的本地绝对路径**  

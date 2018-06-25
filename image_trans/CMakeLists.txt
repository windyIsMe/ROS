cmake_minimum_required(VERSION 2.8.12.2)
project(image_trans)

find_package(OpenCV REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  OpenCV
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
)

catkin_package(
)

include_directories(
# include
  /include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(getImage src/getImage.cpp)
add_dependencies(getImage ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(getImage ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(imageServer src/imageServer.cpp)
add_dependencies(imageServer ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(imageServer ${catkin_LIBRARIES} ${OpenCV_LIBS})

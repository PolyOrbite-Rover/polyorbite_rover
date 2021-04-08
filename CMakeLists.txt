cmake_minimum_required(VERSION 3.0.2)
project(polyorbite_rover)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  cv_bridge
  image_transport
  nav_msgs
  tf
  pcl_conversions
  pcl_ros
  sensor_msgs
)

find_package(OpenCV REQUIRED)

if(CATKIN_ENABLE_TESTING)
  find_package(roslaunch REQUIRED)
  roslaunch_add_file_check(launch)
endif()

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(odom src/odom.cpp)
add_executable(collisions_detector src/collisions_detector/collisions_detector.cpp)

target_link_libraries(odom ${catkin_LIBRARIES})
target_link_libraries(collisions_detector ${catkin_LIBRARIES})

install(DIRECTORY launch rviz urdf DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
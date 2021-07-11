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
)

find_package(OpenCV REQUIRED)

if(CATKIN_ENABLE_TESTING)
  find_package(roslaunch REQUIRED)
  roslaunch_add_file_check(launch)
endif()

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES void
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(odom src/odom.cpp)
add_executable(hardware_interface
  src/hardware_interface_node.cpp
  src/new_hardware_interface.cpp
)

target_link_libraries(odom ${catkin_LIBRARIES})
target_link_libraries(hardware_interface ${catkin_LIBRARIES})

#############
## Install ##
#############

install(
  DIRECTORY launch rviz urdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
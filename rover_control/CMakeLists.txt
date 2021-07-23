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
  controller_manager
  hardware_interface
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
  ${Boost_INCLUDE_DIRS}
)

add_executable(odom src/odom.cpp)
add_executable(rover_node
  src/rover_base.cpp
  src/rover_hardware.cpp
)

target_link_libraries(odom ${catkin_LIBRARIES})
target_link_libraries(rover_node ${catkin_LIBRARIES} ${Boost_LIBRARIES})

#############
## Install ##
#############

install(
  DIRECTORY launch rviz urdf rover_node
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
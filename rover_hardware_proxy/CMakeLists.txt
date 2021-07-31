cmake_minimum_required(VERSION 3.0.2)
project(rover_hardware_proxy)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rover_hardware_proxy
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(
  PROGRAMS scripts/rover_drive_train_proxy.py
  PROGRAMS scripts/rover_speed_encoder.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

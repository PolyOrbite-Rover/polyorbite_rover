cmake_minimum_required(VERSION 3.0.2)
project(polyorbite_rover_interest_points)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

add_message_files(
  FILES
  InterestPoint.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rover_interest_points
  CATKIN_DEPENDS roscpp std_msgs message_runtime
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(interest_point_service src/interest_point_service.cpp)
target_link_libraries(interest_point_service ${catkin_LIBRARIES})

install(
  DIRECTORY interest_point_service
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
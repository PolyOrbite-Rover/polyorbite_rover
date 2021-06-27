cmake_minimum_required(VERSION 3.0.2)
project(interest_points)

add_compile_options(-std=c++11)

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
#  LIBRARIES interest_points
   CATKIN_DEPENDS roscpp std_msgs message_runtime
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(collector src/collector_node.cpp src/collector.cpp)
target_link_libraries(collector ${catkin_LIBRARIES})

install(
  DIRECTORY collector
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
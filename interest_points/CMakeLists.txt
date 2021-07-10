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

add_service_files(
  FILES
  InterestPointsQuery.srv
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
  /usr/include/mysql
)

link_directories(/usr/lib/aarch64-linux-gnu)

add_executable(
  collector
  src/collector/collector_node.cpp
  src/collector/collector.cpp
  src/collector/persistor.cpp
)
target_link_libraries(collector ${catkin_LIBRARIES} libmysqlclient.so libpthread.so libz.so libm.so librt.so libatomic.so libssl.so libcrypto.so libdl.so)

install(
  DIRECTORY collector
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

add_executable(
  server
  src/server/server_node.cpp
  src/server/server.cpp
)
target_link_libraries(server ${catkin_LIBRARIES} libmysqlclient.so libpthread.so libz.so libm.so librt.so libatomic.so libssl.so libcrypto.so libdl.so)

install(
  DIRECTORY server
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
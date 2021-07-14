#include <boost/chrono.hpp>

#include "controller_manager/controller_manager.h"
#include "new_hardware_interface.h"

typedef boost::chrono::steady_clock time_source;

int main(int argc, char **argv)
{
  // Initialize the node
  ros::init(argc, argv, "hardware_interface_node");
  ros::NodeHandle nh;
  ros::Rate rate(2);  // 2 Hz for fun

  // Initialize the hardware interface class and the controller manager
  MyRobot myHWInterface(nh);
  controller_manager::ControllerManager cm(&myHWInterface);

  time_source::time_point lastTime = time_source::now();

  // While this hardware interface is active
  while (ros::ok())
  {
    time_source::time_point currentTime = time_source::now();
    boost::chrono::duration<double> elapsedTime = currentTime - lastTime;
    lastTime = currentTime;

    myHWInterface.readMotorData();  //TBD
    cm.update(ros::Time::now(), (ros::Duration)(elapsedTime.count()));
    myHWInterface.writeMotorData();

    rate.sleep();
  }
}

#include "controller_manager/controller_manager.h"

#include "new_hardware_interface.h"

int main(int argc, char **argv)
{
  // Initialize the node
  ros::init(argc, argv, "hardware_interface_node");
  ros::NodeHandle nh;
  ros::Rate rate(2);  // 2 Hz for fun

  // Initialize the hardware interface class and the controller manager
  MyRobot myHWInterface(nh);
  controller_manager::ControllerManager cm(&myHWInterface);

  // While this hardware interface is active
  while (ros::ok())
  {
    myHWInterface.readMotorData();  //TBD
    cm.update(myHWInterface.getTime(), myHWInterface.getPeriod());
    myHWInterface.writeMotorData();

    rate.sleep();
  }
}

#ifndef HARDWARE_INTERFACE
#define HARDWARE_INTERFACE

#include <ros/ros.h>
#include <hardware_interface/robot_hw.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <std_msgs/Int32MultiArray.h>

#define DEFAULT_FREQUENCY 16

class MyRobot : public hardware_interface::RobotHW
{
public:
  MyRobot(ros::NodeHandle nh, double frequency = DEFAULT_FREQUENCY);

  void readMotorData();

  void writeMotorData();

private:
  ros::NodeHandle nh_;

  // Publishers
  ros::Publisher velocity_pub;

  // Control interfaces
  hardware_interface::JointStateInterface joint_state_interface_;
  hardware_interface::VelocityJointInterface velocity_joint_interface_;

  // Variables for the interface
  double cmd[6];
  double pos[6];
  double vel[6];
  double eff[6];

  // // Structure for storing rover wheels' information
  // struct Joint
  // {
  //   double position;
  //   double velocity;
  //   double effort;
  //   double command;
  // } joint[6];
};

#endif // HARDWARE_INTERFACE ///

#include "new_hardware_interface.h"

MyRobot::MyRobot(ros::NodeHandle nh, double frequency) : nh_(nh)
{
  // Create publisher
  velocity_pub = nh.advertise<std_msgs::Int32MultiArray>("command", 1000);  // https://github.com/dheera/ros-pwm-pca9685

  // connect and register the JointStateInterface (hard coded)
  hardware_interface::JointStateHandle state_handle_front_left("left_front_wheel_joint", &pos[0], &vel[0], &eff[0]);
  joint_state_interface_.registerHandle(state_handle_front_left);
  hardware_interface::JointStateHandle state_handle_front_right("right_front_wheel_joint", &pos[1], &vel[1], &eff[1]);
  joint_state_interface_.registerHandle(state_handle_front_right);
  hardware_interface::JointStateHandle state_handle_middle_left("left_middle_wheel_joint", &pos[2], &vel[2], &eff[2]);
  joint_state_interface_.registerHandle(state_handle_middle_left);
  hardware_interface::JointStateHandle state_handle_middle_right("right_middle_wheel_joint", &pos[3], &vel[3], &eff[3]);
  joint_state_interface_.registerHandle(state_handle_middle_right);
  hardware_interface::JointStateHandle state_handle_back_left("left_back_wheel_joint", &pos[4], &vel[4], &eff[4]);
  joint_state_interface_.registerHandle(state_handle_back_left);
  hardware_interface::JointStateHandle state_handle_back_right("right_back_wheel_joint", &pos[5], &vel[5], &eff[5]);
  joint_state_interface_.registerHandle(state_handle_back_right);

  registerInterface(&joint_state_interface_);

  // connect and register the velocity joint interface
  hardware_interface::JointHandle vel_handle_front_left(joint_state_interface_.getHandle("left_front_wheel_joint"), &cmd[0]);
  velocity_joint_interface_.registerHandle(vel_handle_front_left);
  hardware_interface::JointHandle vel_handle_front_right(joint_state_interface_.getHandle("right_front_wheel_joint"), &cmd[1]);
  velocity_joint_interface_.registerHandle(vel_handle_front_right);
  hardware_interface::JointHandle vel_handle_middle_left(joint_state_interface_.getHandle("left_middle_wheel_joint"), &cmd[2]);
  velocity_joint_interface_.registerHandle(vel_handle_middle_left);
  hardware_interface::JointHandle vel_handle_middle_right(joint_state_interface_.getHandle("right_middle_wheel_joint"), &cmd[3]);
  velocity_joint_interface_.registerHandle(vel_handle_middle_right);
  hardware_interface::JointHandle vel_handle_back_left(joint_state_interface_.getHandle("left_back_wheel_joint"), &cmd[4]);
  velocity_joint_interface_.registerHandle(vel_handle_back_left);
  hardware_interface::JointHandle vel_handle_back_right(joint_state_interface_.getHandle("right_back_wheel_joint"), &cmd[5]);
  velocity_joint_interface_.registerHandle(vel_handle_back_right);

  registerInterface(&velocity_joint_interface_);
}

void MyRobot::readMotorData()  // This function connects to the node that will subscribe to the correct topic for obtaining the velocity or voltage values
{

  // En attente de connaitre a quoi se subscribe
}

void MyRobot::writeMotorData()
{
  // Publish the velocity commands while this node is active
  // Methode interessante: https://answers.ros.org/question/353861/why-my-hardware-interface-cant-get-command-from-controller/
  
  std_msgs::Int32MultiArray pwmValues;
  
  for(double wantedVelocity : cmd)
  {
    pwmValues.data.push_back(velocityToPwm(wantedVelocity));
  }
  
  velocity_pub.publish(pwmValues);

  ROS_INFO("pos: [%f, %f, %f, %f, %f, %f]", pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);
  ROS_INFO("vec: [%f, %f, %f, %f, %f, %f]", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
  ROS_INFO("eff: [%f, %f, %f, %f, %f, %f]", eff[0], eff[1], eff[2], eff[3], eff[4], eff[5]);
  ROS_INFO("cmd: [%f, %f, %f, %f, %f, %f]", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
}

double MyRobot::velocityToPwm(const double& velocity)
{
  return velocity;
}
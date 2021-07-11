#include "new_hardware_interface.h"

MyRobot::MyRobot(ros::NodeHandle nh, double frequency) : nh_(nh)
{
  // Create publisher
  velocity_pub = nh.advertise<std_msgs::Int32MultiArray>("command", 1000);  // https://github.com/dheera/ros-pwm-pca9685

  // connect and register the JointStateInterface (hard coded)
  hardware_interface::JointStateHandle state_handle_front_left("FL", &pos[0], &vel[0], &eff[0]);
  joint_state_interface_.registerHandle(state_handle_front_left);
  hardware_interface::JointStateHandle state_handle_front_right("FR", &pos[1], &vel[1], &eff[1]);
  joint_state_interface_.registerHandle(state_handle_front_right);
  hardware_interface::JointStateHandle state_handle_middle_left("ML", &pos[2], &vel[2], &eff[2]);
  joint_state_interface_.registerHandle(state_handle_middle_left);
  hardware_interface::JointStateHandle state_handle_middle_right("MR", &pos[3], &vel[3], &eff[3]);
  joint_state_interface_.registerHandle(state_handle_middle_right);
  hardware_interface::JointStateHandle state_handle_back_left("BL", &pos[4], &vel[4], &eff[4]);
  joint_state_interface_.registerHandle(state_handle_back_left);
  hardware_interface::JointStateHandle state_handle_back_right("BR", &pos[5], &vel[5], &eff[5]);
  joint_state_interface_.registerHandle(state_handle_back_right);

  registerInterface(&joint_state_interface_);

  // connect and register the velocity joint interface
  hardware_interface::JointHandle vel_handle_front_left(joint_state_interface_.getHandle("FL"), &cmd[0]);
  velocity_joint_interface_.registerHandle(vel_handle_front_left);
  hardware_interface::JointHandle vel_handle_front_right(joint_state_interface_.getHandle("FR"), &cmd[1]);
  velocity_joint_interface_.registerHandle(vel_handle_front_right);
  hardware_interface::JointHandle vel_handle_middle_left(joint_state_interface_.getHandle("ML"), &cmd[2]);
  velocity_joint_interface_.registerHandle(vel_handle_middle_left);
  hardware_interface::JointHandle vel_handle_middle_right(joint_state_interface_.getHandle("MR"), &cmd[3]);
  velocity_joint_interface_.registerHandle(vel_handle_middle_right);
  hardware_interface::JointHandle vel_handle_back_left(joint_state_interface_.getHandle("BL"), &cmd[4]);
  velocity_joint_interface_.registerHandle(vel_handle_back_left);
  hardware_interface::JointHandle vel_handle_back_right(joint_state_interface_.getHandle("BR"), &cmd[5]);
  velocity_joint_interface_.registerHandle(vel_handle_back_right);

  registerInterface(&velocity_joint_interface_);
}

void MyRobot::readMotorData()  // This function connects to the node that will subscribe to the correct topic for obtaining the velocity or voltage values
{

  //ROS_INFO_STREAM("Received velocity information from Robot!");  // en attente de connaitre a quoi se subscribe
}

void MyRobot::writeMotorData()
{
  // Publish the velocity commands while this node is active
  // Methode interessante: https://answers.ros.org/question/353861/why-my-hardware-interface-cant-get-command-from-controller/
  
  std_msgs::Int32MultiArray pwmValues;
  pwmValues.data.push_back(velocityToPwm(vel[0]));
  pwmValues.data.push_back(velocityToPwm(vel[1]));
  pwmValues.data.push_back(velocityToPwm(vel[2]));
  pwmValues.data.push_back(velocityToPwm(vel[3]));
  pwmValues.data.push_back(velocityToPwm(vel[4]));
  pwmValues.data.push_back(velocityToPwm(vel[5]));
  velocity_pub.publish(pwmValues);
  ROS_INFO("Velocity: %f", vel[0]);
  //ROS_INFO_STREAM("Velocity information sent from Robot!");
}


double MyRobot::velocityToPwm(const double& velocity)
{
  return velocity;
}
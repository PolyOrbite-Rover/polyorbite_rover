#pragma once

#include "hardware_interface/joint_state_interface.h"
#include "hardware_interface/joint_command_interface.h"
#include "hardware_interface/robot_hw.h"
#include "ros/ros.h"

#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float64.h"

#include <string>
#include <chrono>

#define DISTANCE_PER_PULSE 1.570796 // mm

namespace polyorbite_rover
{
    struct Joint
    {
        std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

        double position;
        double positionOffset;
        double velocity;
        double effort;
        double velocityCommand;

        Joint() :
            position(0), velocity(0), effort(0), velocityCommand(0)
        {}
    };

    class RoverHardware : public hardware_interface::RobotHW
    {
    public:
        RoverHardware(ros::NodeHandle handle, ros::NodeHandle privateHandle, double target_control_frequency);

        void updateJointsFromHardware();
        void writeCommandsToHardware();

    private:
        void initializeDiagnostics();
        void registerControlInterfaces();
        void encoderSignalCallback(const std_msgs::Float32MultiArray::ConstPtr& message);

        void encoderCallback(Joint& joint, const std_msgs::Float64::ConstPtr& message);

        void encoderFLCallback(const std_msgs::Float64::ConstPtr& message);
        void encoderFRCallback(const std_msgs::Float64::ConstPtr& message);

        void encoderCLCallback(const std_msgs::Float64::ConstPtr& message);
        void encoderCRCallback(const std_msgs::Float64::ConstPtr& message);

        void encoderRLCallback(const std_msgs::Float64::ConstPtr& message);
        void encoderRRCallback(const std_msgs::Float64::ConstPtr& message);

        double angularToPercent(const double& travel) const;

    private:
        ros::NodeHandle handle, privateHandle;

        ros::Publisher motorFLPublisher;
        ros::Publisher motorFRPublisher;
        
        ros::Publisher motorCLPublisher;
        ros::Publisher motorCRPublisher;

        ros::Publisher motorRLPublisher;
        ros::Publisher motorRRPublisher;

        ros::Subscriber motorFLSubscriber;
        ros::Subscriber motorFRSubscriber;

        ros::Subscriber motorCLSubscriber;
        ros::Subscriber motorCRSubscriber;

        ros::Subscriber motorRLSubscriber;
        ros::Subscriber motorRRSubscriber;

        ros::Publisher driveTrainStatePublisher;
        ros::Subscriber driveTrainStateSubscriber;

        hardware_interface::JointStateInterface jointStateInterface;
        hardware_interface::VelocityJointInterface velocityJointInterface;

        double wheelDiameter;
        double maximumAcceleration;
        double maximumVelocity;
        double pollingTimeout;

        Joint joints[6];
    };
}

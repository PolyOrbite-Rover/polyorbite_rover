#pragma once

#include "hardware_interface/joint_state_interface.h"
#include "hardware_interface/joint_command_interface.h"
#include "hardware_interface/robot_hw.h"
#include "ros/ros.h"

#include <string>

namespace polyorbite_rover
{

    class RoverHardware : public hardware_interface::RobotHW
    {
    public:
        RoverHardware(ros::NodeHandle handle, ros::NodeHandle privateHandle, double target_control_frequency);

        void updateJointsFromHardware();
        void writeCommandsToHardware();

    private:
        void initializeDiagnostics();
        void registerControlInterfaces();

        double angularToPercent(const double& travel) const;

    private:
        ros::NodeHandle handle, privateHandle;

        hardware_interface::JointStateInterface jointStateInterface;
        hardware_interface::VelocityJointInterface velocityJointInterface;

        double wheelDiameter;
        double maximumAcceleration;
        double maximumVelocity;
        double pollingTimeout;

        struct Joint
        {
            double position;
            double positionOffset;
            double velocity;
            double effort;
            double velocityCommand;

            Joint() :
                position(0), velocity(0), effort(0), velocityCommand(0)
            {}
        } joints[6];
    };

}
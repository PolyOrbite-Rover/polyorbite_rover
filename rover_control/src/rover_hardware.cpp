#include "rover_base/rover_hardware.h"
#include <boost/assign/list_of.hpp>

namespace
{
    const uint8_t LEFT  = 0;
    const uint8_t RIGHT = 3;
};

namespace polyorbite_rover
{
    RoverHardware::RoverHardware(ros::NodeHandle handle, ros::NodeHandle privateHandle, double target_control_frequency)
        : handle(handle),
          privateHandle(privateHandle)
    {
        privateHandle.param<double>("wheel_diameter", wheelDiameter, 0.3302);
        privateHandle.param<double>("max_acceleration", maximumAcceleration, 5.0);
        privateHandle.param<double>("max_velocity", maximumVelocity, 10.0);

        registerControlInterfaces();
    }

    void RoverHardware::registerControlInterfaces()
    {
        ros::V_string jointNames = boost::assign::list_of
            ("left_front_wheel_joint")
            ("left_middle_wheel_joint")
            ("left_back_wheel_joint")
            ("right_front_wheel_joint")
            ("right_middle_wheel_joint")
            ("right_back_wheel_joint");

        for(unsigned int i = 0; i < jointNames.size(); ++i)
        {
            hardware_interface::JointStateHandle jointStateHandle(
                jointNames[i],
                &joints[i].position,
                &joints[i].velocity,
                &joints[i].effort
            );

            jointStateInterface.registerHandle(jointStateHandle);

            hardware_interface::JointHandle jointHandle(
                jointStateHandle, &joints[i].velocityCommand
            );

            velocityJointInterface.registerHandle(jointHandle);
        }

        registerInterface(&jointStateInterface);
        registerInterface(&velocityJointInterface);
    }

    void RoverHardware::updateJointsFromHardware()
    {
        // TODO: Read joint feedback from hardware
    }

    void RoverHardware::writeCommandsToHardware()
    {
        double leftVelocityPercent = angularToPercent(joints[LEFT].velocityCommand);
        double rightVelocityPercent = angularToPercent(joints[RIGHT].velocityCommand);

        ROS_INFO(
            "Left: %f | Right: %f",
            joints[LEFT].velocityCommand,
            joints[RIGHT].velocityCommand
        );
    }

    double RoverHardware::angularToPercent(const double& travel) const
    {
        double result = travel / maximumVelocity;
        return result > 1.0 ? 1.0 : result;
    }
}
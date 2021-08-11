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

        driveTrainStatePublisher = handle.advertise<std_msgs::Float32MultiArray>("drive_train_state", 10);
        driveTrainStateSubscriber = handle.subscribe("/velocity", 1000, &RoverHardware::encoderSignalCallback, this);

        registerControlInterfaces();
    }

    void RoverHardware::encoderSignalCallback(const std_msgs::Float32MultiArray::ConstPtr& message)
    {
        // Stores the velocity values in a Variable
        for (int n : message->data)
        {
            joints[n].velocity = message->data[n];
        }

        // Print out the velocity values
        ROS_INFO(
            "Left front: %f | Right front: %f",
            joints[0].velocity,
            joints[3].velocity
        );

        ROS_INFO(
            "Left middle: %f | Right middle: %f",
            joints[1].velocity,
            joints[4].velocity
        );

	      ROS_INFO(
            "Left rear: %f | Right rear: %f",
            joints[2].velocity,
            joints[5].velocity
        );

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

    void RoverHardware::updateJointsFromHardware()  // callback function?
    {
        // TODO: Read joint feedback from hardware

    }

    void RoverHardware::writeCommandsToHardware()
    {
        double leftVelocityPercent = angularToPercent(joints[LEFT].velocityCommand);
        double rightVelocityPercent = angularToPercent(joints[RIGHT].velocityCommand);

        ROS_INFO(
            "Left front: %f | Right front: %f",
            joints[0].velocityCommand,
            joints[3].velocityCommand
        );

        ROS_INFO(
            "Left middle: %f | Right middle: %f",
            joints[1].velocityCommand,
            joints[4].velocityCommand
        );

	      ROS_INFO(
            "Left rear: %f | Right rear: %f",
            joints[2].velocityCommand,
            joints[5].velocityCommand
        );

        std_msgs::Float32MultiArray message;

        for(auto joint : joints)
        {
            message.data.push_back(joint.velocityCommand);
        }

        driveTrainStatePublisher.publish(message);
    }

    double RoverHardware::angularToPercent(const double& travel) const
    {
        double result = travel / maximumVelocity;
        return result > 1.0 ? 1.0 : result;
    }
}

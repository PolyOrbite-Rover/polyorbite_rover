#include "rover_base/rover_hardware.h"
#include <boost/assign/list_of.hpp>

namespace
{
    const uint8_t FRONT_LEFT   = 0;
    const uint8_t FRONT_RIGHT  = 3;
    const uint8_t CENTER_LEFT  = 1;
    const uint8_t CENTER_RIGHT = 4;
    const uint8_t REAR_LEFT    = 2;
    const uint8_t REAR_RIGHT   = 5;
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

        motorFLPublisher = handle.advertise<std_msgs::Float64>("FL_velocity", 10);
        motorFRPublisher = handle.advertise<std_msgs::Float64>("FR_velocity", 10);

        motorCLPublisher = handle.advertise<std_msgs::Float64>("CL_velocity", 10);
        motorCRPublisher = handle.advertise<std_msgs::Float64>("CR_velocity", 10);

        motorRLPublisher = handle.advertise<std_msgs::Float64>("RL_velocity", 10);
        motorRRPublisher = handle.advertise<std_msgs::Float64>("RR_velocity", 10);

        motorFLSubscriber = handle.subscribe("/FL_distance", 1000, &RoverHardware::encoderFLCallback, this);
        motorFLSubscriber = handle.subscribe("/FR_distance", 1000, &RoverHardware::encoderFRCallback, this);

        motorFLSubscriber = handle.subscribe("/CL_distance", 1000, &RoverHardware::encoderCLCallback, this);
        motorFLSubscriber = handle.subscribe("/CR_distance", 1000, &RoverHardware::encoderCRCallback, this);

        motorFLSubscriber = handle.subscribe("/RL_distance", 1000, &RoverHardware::encoderRLCallback, this);
        motorFLSubscriber = handle.subscribe("/RR_distance", 1000, &RoverHardware::encoderRRCallback, this);

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

    void RoverHardware::encoderCallback(Joint& joint, const std_msgs::Float64::ConstPtr& message)
    {
        std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto deltaTime = (timestamp - joint.timestamp).count();

        if(deltaTime > 0)
        {
            float position = message->data * DISTANCE_PER_PULSE;
            float deltaPosition = position - joint.position;
            float velocity = deltaPosition / deltaTime;

            joint.timestamp = timestamp;
            joint.position = position;
            joint.velocity = velocity;
        }
    }

    void RoverHardware::encoderFLCallback(const std_msgs::Float64::ConstPtr& message)
    {
        encoderCallback(joints[FRONT_LEFT], message);
    }

    void RoverHardware::encoderFRCallback(const std_msgs::Float64::ConstPtr& message)
    {
        encoderCallback(joints[FRONT_RIGHT], message);
    }

    void RoverHardware::encoderCLCallback(const std_msgs::Float64::ConstPtr& message)
    {
        encoderCallback(joints[CENTER_LEFT], message);
    }

    void RoverHardware::encoderCRCallback(const std_msgs::Float64::ConstPtr& message)
    {
        encoderCallback(joints[CENTER_RIGHT], message);
    }

    void RoverHardware::encoderRLCallback(const std_msgs::Float64::ConstPtr& message)
    {
        encoderCallback(joints[REAR_LEFT], message);
    }

    void RoverHardware::encoderRRCallback(const std_msgs::Float64::ConstPtr& message)
    {
        encoderCallback(joints[REAR_RIGHT], message);
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

        std_msgs::Float64 velocityFLmessage;
        velocityFLmessage.data = (float)joints[FRONT_LEFT].velocityCommand;
        motorFLPublisher.publish(velocityFLmessage);

        std_msgs::Float64 velocityFRmessage;
        velocityFRmessage.data = (float)joints[FRONT_RIGHT].velocityCommand;
        motorFRPublisher.publish(velocityFRmessage);

        std_msgs::Float64 velocityCLmessage;
        velocityCLmessage.data = (float)joints[CENTER_LEFT].velocityCommand;
        motorCLPublisher.publish(velocityCLmessage);

        std_msgs::Float64 velocityCRmessage;
        velocityCRmessage.data = (float)joints[CENTER_RIGHT].velocityCommand;
        motorCRPublisher.publish(velocityCRmessage);

        std_msgs::Float64 velocityRLmessage;
        velocityRLmessage.data = (float)joints[CENTER_LEFT].velocityCommand;
        motorRLPublisher.publish(velocityRLmessage);

        std_msgs::Float64 velocityRRmessage;
        velocityRRmessage.data = (float)joints[CENTER_RIGHT].velocityCommand;
        motorRRPublisher.publish(velocityRRmessage);
    }

    double RoverHardware::angularToPercent(const double& travel) const
    {
        double result = travel / maximumVelocity;
        return result > 1.0 ? 1.0 : result;
    }
}

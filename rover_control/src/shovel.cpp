#include "ros/ros.h"

#include "std_msgs/Float32.h"

class Shovel {
    ros::NodeHandle handle;
    ros::Publisher shovelStatePublisher;
    ros::Subscriber shovelStateSubscriber;

    public:
    Shovel() {
        shovelStatePublisher = handle.advertise<std_msgs::Float32>("shovel_state", 10);
        shovelStateSubscriber = handle.subscribe<std_msgs::Float32>("/shovel_cmds", 1000, &Shovel::callback, this);
    }

    void callback(const std_msgs::Float32::ConstPtr& message)
    {
        shovelStatePublisher.publish(message);
    }
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "shovel_node");

    Shovel shovel;

    ros::spin();

    return 0;
}
#include "ros/ros.h"
#include "interest_points/server.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "interest_points_server");
    ros::NodeHandle node;

    Server server(node);

    ros::spin();
    return 0;
}

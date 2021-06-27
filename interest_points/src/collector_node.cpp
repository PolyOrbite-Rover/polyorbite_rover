#include "ros/ros.h"
#include "interest_points/collector.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "interest_points_collector");
    ros::NodeHandle node;

    Collector collector(node);

    ros::spin();
    return 0;
}

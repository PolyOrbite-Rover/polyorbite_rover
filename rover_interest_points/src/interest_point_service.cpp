#include "ros/ros.h"
#include "rover_interest_points/interest_point_collector.h"

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "interest_point_service");
    ros::NodeHandle node;

    InterestPointCollector interestPointCollector(node);

    ros::spin();

    return 0;
}
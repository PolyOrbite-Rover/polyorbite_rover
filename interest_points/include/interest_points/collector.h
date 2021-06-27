#pragma once

#include "ros/ros.h"
#include "interest_points/InterestPoint.h"

class Collector
{
public:
    Collector(const ros::NodeHandle& node);

private:
    ros::NodeHandle node;
    ros::Subscriber interest_points_subscriber;

    void initialize();

    void onMessageReceived(const interest_points::InterestPoint::ConstPtr& message);
};
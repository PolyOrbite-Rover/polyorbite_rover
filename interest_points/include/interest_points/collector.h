#pragma once

#include "ros/ros.h"
#include "interest_points/InterestPoint.h"
#include "interest_points/persistor.h"

class Collector
{
public:
    Collector(const ros::NodeHandle& node);

private:
    ros::NodeHandle node;
    ros::Subscriber interest_points_add_subscriber;
    ros::Subscriber interest_points_update_subscriber;

    Persistor persistor;

    void initialize();

    void onAddMessageReceived(const interest_points::InterestPoint::ConstPtr& message);
    void onUpdateMessageReceived(const interest_points::InterestPoint::ConstPtr& message);
};
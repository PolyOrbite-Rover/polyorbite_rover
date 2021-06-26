#pragma once

#include "ros/ros.h"

#include "polyorbite_rover_interest_points/InterestPoint.h"

class InterestPointCollector
{
public:
    InterestPointCollector(ros::NodeHandle& node)
        : node(node) { initialize(); }

private:
    ros::NodeHandle& node;
    ros::Subscriber collector;

    void onInterestPointReceived(
        const polyorbite_rover_interest_points::InterestPoint::ConstPtr& message)
    {
        
    }

    void initialize()
    {
        collector = node.subscribe(
            "interest_point",
            10,
            &InterestPointCollector::onInterestPointReceived
        );
    }
};
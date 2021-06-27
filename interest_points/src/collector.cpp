#include "interest_points/collector.h"

Collector::Collector(const ros::NodeHandle& node)
    : node(node)
{
    initialize();
}

void Collector::initialize()
{
    interest_points_subscriber = node.subscribe(
        "interest_points",
        10,
        &Collector::onMessageReceived,
        this
    );
}

void Collector::onMessageReceived(const interest_points::InterestPoint::ConstPtr& message)
{
    
}
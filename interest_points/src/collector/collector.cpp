#include "interest_points/collector.h"

Collector::Collector(const ros::NodeHandle& node)
    : node(node)
{
    initialize();
}

void Collector::initialize()
{
    interest_points_add_subscriber = node.subscribe(
        "interest_points/add",
        10,
        &Collector::onAddMessageReceived,
        this
    );
    interest_points_update_subscriber = node.subscribe(
        "interest_points/update",
        10,
        &Collector::onUpdateMessageReceived,
        this
    );
}

void Collector::onAddMessageReceived(const interest_points::InterestPoint::ConstPtr& message)
{
    persistor.addInterestPoint(*message);
}

void Collector::onUpdateMessageReceived(const interest_points::InterestPoint::ConstPtr& message)
{
    persistor.updateInterestPoint(*message);
}
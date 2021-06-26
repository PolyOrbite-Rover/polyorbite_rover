#pragma once

#include <string.h>

#include "ros/ros.h"

class InterestPointHandler
{
public:
    InterestPointHandler(ros::NodeHandle& node)
        : node(node) { initialize(); }
protected:
    ros::Subscriber collector;

    virtual std::string getServiceName();
    virtual void handle();

private:
    ros::NodeHandle& node;

    void handle()
    {

    }

    void initialize()
    {
    }
};
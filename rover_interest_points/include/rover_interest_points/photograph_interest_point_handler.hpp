#pragma once

#include "interest_point_handler.hpp"

#include "ros/ros.h"

class PhotographInterestPointHandler
{
public:
    //PhotographInterestPointHandler(ros::NodeHandler& node)
    //    : node(node) {}

    std::string getServiceName()
    {
        return "interest-point-photograph";
    }

    void handle()
    {

    }
};
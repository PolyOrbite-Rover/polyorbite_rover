#pragma once

#include "ros/ros.h"
#include "interest_points/InterestPoint.h"
#include "interest_points/InterestPointsQuery.h"

class Server
{
public:
    Server(const ros::NodeHandle& node);

private:
    ros::NodeHandle node;
    ros::ServiceServer service;

    void initialize();

    bool handleQuery(
        interest_points::InterestPointsQueryRequest& request,
        interest_points::InterestPointsQueryResponse& response
    );
};
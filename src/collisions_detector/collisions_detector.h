#pragma once

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/filter.h>

#include <sstream>

class CollisionsDetector
{
public:

    void onCloudMessage(const sensor_msgs::PointCloud2ConstPtr& message);

    CollisionsDetector(ros::NodeHandle nodeHandle);

private:
    ros::Subscriber mPointCloudSubscriber;

    pcl::PointCloud<pcl::PointXYZ>::Ptr getCleanCloudFromMessage(const sensor_msgs::PointCloud2ConstPtr& message);
};
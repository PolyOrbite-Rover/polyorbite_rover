#include "collisions_detector.h"

CollisionsDetector::CollisionsDetector(ros::NodeHandle nodeHandle)
    : mPointCloudSubscriber(nodeHandle.subscribe("cloud", 1, &CollisionsDetector::onCloudMessage, this))
{}

void CollisionsDetector::onCloudMessage(const sensor_msgs::PointCloud2ConstPtr& message)
{
    auto cloud = getCleanCloudFromMessage(message);
    auto isCloudEmpty = cloud->size() == 0;
    
    if(!isCloudEmpty)
    {
        for(auto point : cloud->points)
        {
            ROS_INFO("x: %f, y: %f, z: %f", point.x, point.y, point.z);
            ROS_ERROR("x: %f, y: %f, z: %f", point.x, point.y, point.z);
        }
    }
}

pcl::PointCloud<pcl::PointXYZ>::Ptr CollisionsDetector::getCleanCloudFromMessage(const sensor_msgs::PointCloud2ConstPtr& message)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg(*message, *cloud);
    
    if(cloud->isOrganized())
    {
        std::vector<int> indices;
        pcl::removeNaNFromPointCloud(*cloud, *cloud, indices);
    }
    else if(!cloud->is_dense && cloud->height == 1)
    {
        cloud->is_dense = true;
    }

    return cloud;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "collisions_detector");
    ros::NodeHandle nodeHandle;

    CollisionsDetector CollisionsDetector(nodeHandle);
}
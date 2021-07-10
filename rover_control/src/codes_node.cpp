#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include "codes/aruco_code_reading.hpp"
#include "codes/qr_code_reading.hpp"

#define NODE_NAME    "codes_listener"
#define ARUCO_TOPIC  "aruco"
#define QR_TOPIC     "qr"
#define CAMERA_TOPIC "stereo_camera/right/image_raw/compressed"
#define RATE         5000   // 5 sec

ros::Subscriber sub;
ros::Publisher pub_a;
ros::Publisher pub_q;


cv_bridge::CvImageConstPtr transformROSImgToCVImg(const sensor_msgs::CompressedImage::ConstPtr& msg)
{
  cv_bridge::CvImageConstPtr cv_img_ptr;
  try
  {
    img_ptr = cv_bridge::toCvShare(msg, "bgr8")
    return img_ptr;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Codes node - Transform ROS img to CV img - Could not convert from '%s' to 'bgr8' : %s", msg->encoding.c_str(), e.what());
  }
  return null;
}

void lookForCodes(const cv_bridge::CvImage camImg)
{
  std_msgs::String msg1;    // TODO : changer ça pour un type de message qui pourrait être équivalent à ArucoResult
  std_msgs::String msg2;

  ArucoResult a = arucoRead(camImg);
  QRResult q = qrRead(camImg);

  if (a != nullptr) {
    pub_a.publish(a->img_ptr->toImageMsg());
  }
  if (q != nullptr) {
    pub_q.publish(q->img_ptr->toImageMsg());
  }
}

/*
 * FRAME CALLBACK
 * Converts frame to OpenCV image and looks for Aruco or QR codes in it, publishes on the publisher topics only if codes are found
 * Called when : a frame is captured by the Zed camera
 * Returns : nothing
 */
void frameCallback(const sensor_msgs::CompressedImage::ConstPtr& msg)
{
  cv_bridge::CvImageConstPtr cv_img_ptr;
  cv_img_ptr = transformROSImgToCVImg(msg);
  lookForCodes(cv_img_ptr->image);
}

/*
 * MAIN
 * Node execution flow
 * Returns : 0 when the node ends
 */
int main(int argc, char **argv)
{
  // Node initialisation
  ros::init(argc, argv, NODE_NAME);
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe(CAMERA_TOPIC, 1000, frameCallback); // subscribes to the Zed camera topic
  ros::Publisher pub_a = n.advertise<std_msgs::String>(ARUCO_TOPIC, 1000); // publishes Aruco codes in aruco topic
  ros::Publisher pub_q = n.advertise<std_msgs::String>(QR_TOPIC, 1000); // publishes QR codes in qr topic
  arucoInit();
  qrInit();
  ros::Rate r(RATE);

  // Node loop
  while (ros::ok()) {
    ros::spinOnce();
    r.sleep();
  }

  // Node end
  return 0;
}

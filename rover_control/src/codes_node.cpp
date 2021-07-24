#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/aruco.hpp>
#include "../../../../devel/include/polyorbite_rover/Code.h"

#define NODE_NAME        "codes_listener"
#define ARUCO_TOPIC      "aruco"
#define ARUCO_IMG_SIZE   50
#define CAMERA_TOPIC     "stereo_camera/right/image_raw/compressed"
#define RATE             5000   // 5 sec

ros::Subscriber sub;
ros::Publisher pub_a;
cv::Ptr<cv::aruco::Dictionary> dictionary;

struct ArucoResult {
   int value;                              // the value of the code detected (null if no code detected)
   cv_bridge::CvImageConstPtr img_ptr;         // image of the code detected (null if no code detected)
};

/*
 * ARUCO INIT
 * Initialize aruco detection
 */
void arucoInit()
{
    // Dictionary of aruco codes used at the competition
    // https://circ.cstag.ca/2021/rules/#autonomy-guidelines (7.2.)
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
}

/*
 * ARUCO READ
 * Extract aruco codes from an image taken with the camera
 * Returns : the value and image of the code detected, or a null struct
 */
ArucoResult arucoRead(cv_bridge::CvImageConstPtr cv_ptr)
{
    ArucoResult code;
    cv_bridge::CvImagePtr tempImg;
    std::vector<int> detectedCodes;
    std::vector<std::vector<cv::Point2f>> corners;

    // launch detection algorithm on frame
    cv::aruco::detectMarkers(cv_ptr->image, dictionary, corners, detectedCodes);

    if (detectedCodes.size() > 0) {
        // only 0 to 1 code should be seen at all times, therefore, only the first item of detectedCodes will be saved
        code.value = detectedCodes[0];
        cv::aruco::drawMarker(dictionary, code.value, ARUCO_IMG_SIZE, tempImg->image, 1);
        code.img_ptr = tempImg;
        return code;
    }

    code.value = NULL;
    code.img_ptr = NULL;
    return code;
}

cv_bridge::CvImageConstPtr transformROSImgToCVImg(const sensor_msgs::CompressedImage::ConstPtr& msg)
{
  const sensor_msgs::ImageConstPtr& const_img = (const sensor_msgs::ImageConstPtr&)msg;
  cv_bridge::CvImageConstPtr cv_img_ptr;
  try
  {
    cv_img_ptr = cv_bridge::toCvShare(const_img, "bgr8");
    return cv_img_ptr;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Codes node - Transform ROS img to CV img - Could not convert from '%s' to 'bgr8' : %s", const_img->encoding.c_str(), e.what());
  }
  return nullptr;
}

void lookForCodes(cv_bridge::CvImageConstPtr cam_img_ptr)
{
  ArucoResult a = arucoRead(cam_img_ptr);

  if (a.value != NULL) {
    polyorbite_rover::Code c;
    c.value = a.value;
    c.image = *a.img_ptr->toImageMsg();
    pub_a.publish(c);
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
  lookForCodes(cv_img_ptr);
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
  ros::Publisher pub_a = n.advertise<polyorbite_rover::Code>(ARUCO_TOPIC, 1000); // publishes Aruco codes in aruco topic
  arucoInit();
  ros::Rate r(RATE);

  // Node loop
  while (ros::ok()) {
    ros::spinOnce();
    r.sleep();
  }

  // Node end
  return 0;
}

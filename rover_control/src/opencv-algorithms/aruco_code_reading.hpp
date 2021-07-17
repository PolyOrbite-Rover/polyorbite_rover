#include <opencv2/aruco.hpp>
#include <cv_bridge/cv_bridge.h>

cv::Ptr<cv::aruco::Dictionary> dictionary;

struct ArucoResult {
   int value;                         // the value of the code detected (null if no code detected)
   cv_bridge::CvImageConstPtr img_ptr;         // image of the code detected (null if no code detected)
};

void arucoInit();

ArucoResult arucoRead(cv_bridge::CvImageConstPtr cv_ptr);
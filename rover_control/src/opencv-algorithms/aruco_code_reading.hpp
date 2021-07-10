#include <opencv2/aruco.hpp>

cv::Ptr<cv::aruco::Dictionary> dictionary;

struct ArucoResult {
   int value;                         // the value of the code detected (null if no code detected)
   cv_bridge::CvImagePtr img_ptr;         // image of the code detected (null if no code detected)
};

void arucoInit();

ArucoResult arucoRead(cv_bridge::CvImagePtr cv_ptr);
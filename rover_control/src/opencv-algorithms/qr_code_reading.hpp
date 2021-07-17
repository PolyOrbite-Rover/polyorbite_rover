#include <opencv2/objdetect.hpp>
#include <cv_bridge/cv_bridge.h>

//cv::QRCodeDetector decoder;   // oops, requires OpenCV4

struct QRResult {
   std::string value;                      // the value of the code detected (null if no code detected)
   cv_bridge::CvImageConstPtr img_ptr;         // image of the code detected (null if no code detected)
};

void qrInit();

QRResult qrRead(cv_bridge::CvImageConstPtr cv_ptr);
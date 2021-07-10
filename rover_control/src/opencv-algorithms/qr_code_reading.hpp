#include <opencv2/objdetect.hpp>

QRCodeDetector decoder;

struct QRResult {
   string value;                      // the value of the code detected (null if no code detected)
   cv_bridge::CvImagePtr img_ptr;         // image of the code detected (null if no code detected)
};

void qrInit();

QRResult qrRead(cv_bridge::CvImagePtr cv_ptr);
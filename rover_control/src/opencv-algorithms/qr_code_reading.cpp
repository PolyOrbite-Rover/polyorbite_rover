/**************************************
 *   QR code reading with OpenCV
 *   File written by : KatherineZT
 *   Code based on : https://learnopencv.com/opencv-qr-code-scanner-c-and-python/
 *************************************/

#include "qr_code_reading.hpp"

/*
 * QR INIT
 * Initialize QR code detection
 */
void qrInit()
{
    // QRCodeDetector object
    //decoder = cv::QRCodeDetector::QRCodeDetector();      // oops, requires OpenCV4
}

/*
 * QR READ
 * Extract QR codes from an image taken with the camera
 * Returns : the value and image of the code detected, or a null struct
 */
QRResult qrRead(cv_bridge::CvImageConstPtr cv_ptr)
{
    QRResult code;
    /*
    cv_bridge::CvImagePtr tempImg;
    std::string data;

    // launch detection algorithm on frame
    data = decoder.detectAndDecode(cv_ptr->image, nullptr, tempImg->image);

    if (data.length() > 0)
    {
        // the algorithm already assumes there is only one QR code visible if one is found
        code.value = data;
        code.img_ptr = tempImg;
        return code;
    }
    */
    
    code.value = nullptr;
    code.img_ptr = NULL;
    return code;
}

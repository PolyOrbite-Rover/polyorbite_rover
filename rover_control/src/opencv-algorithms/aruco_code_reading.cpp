/**************************************
 *   Aruco code reading with OpenCV
 *   File written by : KatherineZT
 *   Code based on : https://docs.opencv.org/master/d5/dae/tutorial_aruco_detection.html
 *************************************/

#include "aruco_code_reading.hpp"

#define ARUCO_IMG_SIZE 50

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

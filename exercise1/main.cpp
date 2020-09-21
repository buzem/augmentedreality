#include <iostream>

#include <opencv/cv.h> /// include most of core headers
#include <opencv/cv.hpp>

void trackbarHandler(int pos, void *slider_value) {
    *((int *) slider_value) = pos;
}

void initVideoStream(cv::VideoCapture &cap) {
    if (cap.isOpened())
        cap.release();

    //cap.open(0); // open the default camera
    if (!cap.isOpened()) {
        std::cout << "No webcam found, using a video file" << std::endl;
        cap.open("/home/berkay/MEGAsync/TUM/Augmented Reality/MarkerMovie.MP4");
        if (!cap.isOpened()) {
            std::cout << "No video file found. Exiting." << std::endl;
            exit(0);
        }
    }
}

int main(int ac, char **av) {
    cv::VideoCapture cap;
    const std::string kWinName1 = "Exercise 1 - Original Image";
    const std::string kWinName2 = "Exercise 1 - Thresholded Image";

    std::cout << "Startup (Press ESC to quit)" << std::endl;
    cv::namedWindow(kWinName1, CV_WINDOW_AUTOSIZE);
    cv::namedWindow(kWinName2, CV_WINDOW_NORMAL);
    initVideoStream(cap);

    int slider_value = 100;
    cv::createTrackbar("Threshold", "Exercise 1 - Thresholded Image", &slider_value, 255, trackbarHandler,
                       &slider_value);

    cv::Mat img_bgr;
    cv::Mat img_gray;
    cv::Mat img_filtered;
    while (true) {
        cap >> img_bgr;

        if (img_bgr.empty()) {
            printf("Could not query frame. Trying to reinitialize.\n");
            initVideoStream(cap);
            cv::waitKey(1000); /// Wait for one sec.
            continue;
        }
        cv::cvtColor(img_bgr, img_gray, CV_BGR2GRAY);

        cv::threshold(img_gray, img_filtered, slider_value, 255, CV_THRESH_BINARY);
        //cvAdaptiveThreshold(iplConverted, iplThreshold, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 33, 5);

        cv::imshow(kWinName1, img_bgr);
        cv::imshow(kWinName2, img_filtered);

        int key = cv::waitKey(10);
        if (key == 27) break;
    }

    cv::destroyWindow(kWinName1);
    cv::destroyWindow(kWinName2);
    std::cout << "Finished\n";
}
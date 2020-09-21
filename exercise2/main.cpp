#include <iostream>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

void trackbarHandler(int pos, void *slider_value) {
    *((int *) slider_value) = pos;
}

void initVideoStream(VideoCapture &cap) {
    if (cap.isOpened())
        cap.release();

    //cap.open(0); // open the default camera
    if (!cap.isOpened()) {
        cout << "No webcam found, using a video file" << endl;
        cap.open("/home/berkay/MEGAsync/TUM/Augmented Reality/MarkerMovie.MP4");
        if (!cap.isOpened()) {
            cout << "No video file found. Exiting." << endl;
            exit(0);
        }
    }
}

void process_corners(Mat &frame, Point const *points, int n_points) {

    for (int i = 0; i < n_points; ++i) {
        circle(frame, points[i], 3, CV_RGB(0, 255, 0), -1);

        double dx = (double) (points[(i + 1) % 4].x - points[i].x) / 7.0;
        double dy = (double) (points[(i + 1) % 4].y - points[i].y) / 7.0;

        for (int j = 1; j < 7; ++j) {
            double px = (double) points[i].x + (double) j * dx;
            double py = (double) points[i].y + (double) j * dy;

            cv::Point p;
            p.x = (int) px;
            p.y = (int) py;
            circle(frame, p, 2, CV_RGB(0, 0, 255), -1);
        }
    }
}

void process_contour(Mat &frame, contour_t const &contour) {
    contour_t approx_contour;

    approxPolyDP(contour, approx_contour, arcLength(contour, true) * 0.02, true);

    Scalar QUADRILATERAL_COLOR(0, 0, 255);

    Scalar colour;
    Rect r = boundingRect(approx_contour);
    if (approx_contour.size() == 4) {
        colour = QUADRILATERAL_COLOR;
    } else {
        return;
    }

    if (r.height < 20 || r.width < 20 || r.width > frame.cols - 10 || r.height > frame.rows - 10) {
        return;
    }

    Point const *points(&approx_contour[0]);
    auto n_points(static_cast<int>(approx_contour.size()));
    polylines(frame, &points, &n_points, 1, true, colour, 4);

    process_corners(frame, points, n_points);
}

void process_frame(Mat &frame, Mat &result_frame, int slider_value) {
    result_frame = frame.clone();

    Mat feedGrayScale;
    cvtColor(result_frame, feedGrayScale, cv::COLOR_BGR2GRAY);

    //thresholding the grayscale image to get better results
    threshold(feedGrayScale, feedGrayScale, slider_value, 255, cv::THRESH_BINARY);

    contour_vector_t contours;

    cv::findContours(feedGrayScale, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    for (const auto &contour : contours) {
        process_contour(result_frame, contour);
    }
}

int main(int ac, char **av) {
    VideoCapture cap;
    const string kWinName1 = "Exercise 2 - Original Image";
    const string kWinName2 = "Exercise 2 - Contours Image";

    cout << "Startup (Press ESC to quit)" << endl;
    namedWindow(kWinName1, CV_WINDOW_AUTOSIZE);
    namedWindow(kWinName2, CV_WINDOW_NORMAL);

    initVideoStream(cap);

    int slider_value = 100;
    createTrackbar("Threshold", kWinName2, &slider_value, 255, trackbarHandler, &slider_value);

    Mat img_bgr;
    Mat img_filtered;

    while (true) {
        cap >> img_bgr;

        if (img_bgr.empty()) {
            printf("Could not query frame. Trying to reinitialize.\n");
            initVideoStream(cap);
            waitKey(1000); /// Wait for one sec.
            continue;
        }

        process_frame(img_bgr, img_filtered, slider_value);

        imshow(kWinName1, img_bgr);
        imshow(kWinName2, img_filtered);

        int key = waitKey(20);
        if (key == 27) break;
    }

    destroyWindow(kWinName1);
    destroyWindow(kWinName2);
    cout << "Finished\n";
}
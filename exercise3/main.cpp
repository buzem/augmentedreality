#include <iostream>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

struct MyStrip {
    int stripeLength{};
    int nStop{};
    int nStart{};
    Point2f stripeVecX;
    Point2f stripeVecY;
};

//Const windows Names
const std::string kWinName1 = "Exercise 3 - Original Image";
const std::string kWinName2 = "Exercise 3 - Converted Image";
const std::string kWinName3 = "Exercise 3 - Stripe Image";

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

int subpixSampleSafe(const Mat &pSrc, const Point2f &p) {

    auto x = int(floorf(p.x));
    auto y = int(floorf(p.y));

    if (x < 0 || x >= pSrc.cols - 1 ||
        y < 0 || y >= pSrc.rows - 1)
        return 127;

    auto dx = int(256 * (p.x - floorf(p.x)));
    auto dy = int(256 * (p.y - floorf(p.y)));

    auto *i = (unsigned char *) ((pSrc.data + y * pSrc.step) + x);

    int a = i[0] + ((dx * (i[1] - i[0])) >> 8);
    i += pSrc.step;
    int b = i[0] + ((dx * (i[1] - i[0])) >> 8);
    return a + ((dy * (b - a)) >> 8);
}

void calculate_Sobel(Mat &iplStripe, Point p, bool &isFirstStrip, MyStrip &st, Mat &frame) {
    //use sobel operator on stripe
    // ( -1 , -2, -1 )
    // (  0 ,  0,  0 )
    // (  1 ,  2,  1 )

    vector<double> sobelValues(static_cast<unsigned long>(st.stripeLength - 2));
    /// double* sobelValues = new double[stripeLength-2];

    for (int n = 1; n < (st.stripeLength - 1); n++) {

        unsigned char *stripePtr = &(iplStripe.at<uchar>(n - 1, 0));
        /// unsigned char* stripePtr = ( unsigned char* )( iplStripe->imageData + (n-1) * iplStripe->widthStep );

        double r1 = -stripePtr[0] - 2 * stripePtr[1] - stripePtr[2];
        stripePtr += 2 * iplStripe.step;
        /// stripePtr += 2*iplStripe->widthStep;
        double r3 = stripePtr[0] + 2 * stripePtr[1] + stripePtr[2];

        sobelValues[n - 1] = r1 + r3;
    }

    double maxVal = -1;
    int maxIndex = 0;

    for (int n = 0; n < st.stripeLength - 2; ++n) {

        if (sobelValues[n] > maxVal) {
            maxVal = sobelValues[n];
            maxIndex = n;
        }
    }
    // Added in Sheet 3 - Ex7 (b) End *****************************************************************

    // Added in Sheet 3 - Ex7 (d) Start *****************************************************************

    double y0, y1, y2; // y0 .. y1 .. y2
    y0 = (maxIndex <= 0) ? 0 : sobelValues[maxIndex - 1];
    y1 = sobelValues[maxIndex];
    y2 = (maxIndex >= st.stripeLength - 3) ? 0 : sobelValues[maxIndex + 1];

    //formula for calculating the x-coordinate of the vertex of a parabola, given 3 points with equal distances
    //(xv means the x value of the vertex, d the distance between the points):
    //xv = x1 + (d / 2) * (y2 - y0)/(2*y1 - y0 - y2)

    double pos = (y2 - y0) / (4 * y1 - 2 * y0 - 2 * y2); //d = 1 because of the normalization and x1 will be added later

    // This would be a valid check, too
    if (pos != pos) {
        // value is not a number
        return;
    }

    Point2d edgeCenter; //exact point with subpixel accuracy
    int maxIndexShift = maxIndex - (st.stripeLength >> 1);

    //shift the original edgepoint accordingly
    edgeCenter.x = (double) p.x + (((double) maxIndexShift + pos) * st.stripeVecY.x);
    edgeCenter.y = (double) p.y + (((double) maxIndexShift + pos) * st.stripeVecY.y);
    circle(frame, edgeCenter, 2, CV_RGB(0, 0, 255), -1);

    // Added in Sheet 3 - Ex7 (d) End *****************************************************************

    // Added in Sheet 3 - Ex7 (c) Start *****************************************************************
    if (isFirstStrip) {

        Mat iplTmp;

        resize(iplStripe, iplTmp, Size(100, 300));

        imshow(kWinName3, iplStripe);

        isFirstStrip = false;
    }
    // Added in Sheet 3 - Ex7 (c) End *****************************************************************
}

// Added in Sheet 3 - Ex7 (a) Start *****************************************************************
Mat calculate_Stripe(double dx, double dy, MyStrip &st) {

    double diffLength = sqrt(dx * dx + dy * dy);

    //length proportional to the marker size
    st.stripeLength = (int) (0.8 * diffLength);
    if (st.stripeLength < 5)
        st.stripeLength = 5;

    //make stripeLength odd (because of the shift in nStop)
    st.stripeLength |= 1;

    //e.g. stripeLength = 5 --> from -2 to 2
    st.nStop = st.stripeLength >> 1;
    st.nStart = -st.nStop;

    Size stripeSize;
    //sample a strip of width 3 pixels
    stripeSize.width = 3;
    stripeSize.height = st.stripeLength;

    //normalize vectors
    st.stripeVecX.x = static_cast<float>(dx / diffLength);
    st.stripeVecX.y = static_cast<float>(dy / diffLength);

    st.stripeVecY.x = st.stripeVecX.y;
    st.stripeVecY.y = -st.stripeVecX.x;

    return Mat(stripeSize, CV_8UC1);
}

void define_Stripe_pixel(Mat &frame, Point p, Mat &iplStripe, Mat &gray, bool &isFirstStrip, MyStrip &st) {

    for (int m = -1; m <= 1; ++m) {
        for (int n = st.nStart; n <= st.nStop; ++n) {

            Point2f subPixel;
            subPixel.x = static_cast<float>((double) p.x + ((double) m * st.stripeVecX.x) +
                                            ((double) n * st.stripeVecY.x));
            subPixel.y = static_cast<float>((double) p.y + ((double) m * st.stripeVecX.y) +
                                            ((double) n * st.stripeVecY.y));

            Point p2;
            p2.x = (int) subPixel.x;
            p2.y = (int) subPixel.y;

            if (isFirstStrip)
                circle(frame, p2, 1, CV_RGB(255, 0, 255), -1);
            else
                circle(frame, p2, 1, CV_RGB(0, 255, 255), -1);

            int pixel = subpixSampleSafe(gray, subPixel);
            int w = m + 1; //add 1 to shift to 0..2
            int h = n + (st.stripeLength >> 1); //add stripelenght>>1 to shift to 0..stripeLength
            iplStripe.at<uchar>(h, w) = (uchar) pixel; //set pointer to correct position and safe subpixel intensity

            // Added in Sheet 3 - Ex7 (a) End *****************************************************************
        }
    }
    // Added in Sheet 3 - Ex7 (b) Start *****************************************************************
    calculate_Sobel(iplStripe, p, isFirstStrip, st, frame);
}

void process_corners(Mat &frame, Point const *points, int n_points, Mat &gray, bool &isFirstStrip) {

    for (int i = 0; i < n_points; ++i) {
        circle(frame, points[i], 3, CV_RGB(0, 255, 0), -1);

        double dx = (double) (points[(i + 1) % 4].x - points[i].x) / 7.0;
        double dy = (double) (points[(i + 1) % 4].y - points[i].y) / 7.0;

        // Added in Sheet 3 - Start *****************************************************************
        MyStrip strip;

        //a simple array of unsigned char cv::Mat
        Mat iplStripe = calculate_Stripe(dx, dy, strip);

        // Added in Sheet 3 - End *******************************************************************

        for (int j = 1; j < 7; ++j) {
            double px = (double) points[i].x + (double) j * dx;
            double py = (double) points[i].y + (double) j * dy;

            Point p;
            p.x = (int) px;
            p.y = (int) py;
            circle(frame, p, 2, CV_RGB(0, 0, 255), -1);
            define_Stripe_pixel(frame, p, iplStripe, gray, isFirstStrip, strip);
        }
    }
}

void process_contour(Mat &frame, contour_t const &contour, Mat &gray, bool &isFirstStrip) {
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

    process_corners(frame, points, n_points, gray, isFirstStrip);
}

void process_frame(Mat &frame, Mat &result_frame, int slider_value, bool isFistStrip) {
    result_frame = frame.clone();

    Mat feedGrayScale;

    cvtColor(result_frame, feedGrayScale, cv::COLOR_BGR2GRAY);

    //thresholding the grayscale image to get better results
    threshold(feedGrayScale, feedGrayScale, slider_value, 255, THRESH_BINARY);

    contour_vector_t contours;

    cv::findContours(feedGrayScale, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    for (const auto &contour : contours) {
        process_contour(result_frame, contour, feedGrayScale, isFistStrip);
    }
}

int main(int ac, char **av) {
    VideoCapture cap;

    cout << "Startup (Press ESC to quit)" << endl;

    cv::namedWindow(kWinName1, CV_WINDOW_AUTOSIZE);
    cv::namedWindow(kWinName2, CV_WINDOW_NORMAL);
    cv::namedWindow(kWinName3, CV_WINDOW_NORMAL);

    initVideoStream(cap);

    int max = 255;
    int slider_value = 186;
    createTrackbar("Threshold", kWinName2, &slider_value, 255, trackbarHandler, &slider_value);

    // Added in Sheet 3 - Start *****************************************************************

    bool isFirstStripe = true;

    // Added in Sheet 3 - End *******************************************************************

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

        imshow(kWinName1, img_bgr);

        process_frame(img_bgr, img_filtered, slider_value, isFirstStripe);

        imshow(kWinName2, img_filtered);

        int key = waitKey(20);
        if (key == 27) break;
    }

    destroyWindow(kWinName1);
    destroyWindow(kWinName2);
    destroyWindow(kWinName3);
    cout << "Finished\n";
}
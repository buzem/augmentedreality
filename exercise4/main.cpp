#include <iostream>

#include <opencv/cv.h>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;

struct MyStrip {
    int stripeLength;
    int nStop;
    int nStart;
    Point2f stripeVecX;
    Point2f stripeVecY;
};

//Const windows Names
const std::string kWinName1 = "Exercise 4 - Original Image";
const std::string kWinName2 = "Exercise 4 - Converted Image";
const std::string kWinName3 = "Exercise 4 - Stripe Image";
const std::string kWinName4 = "Exercise 4 - Marker";

// Added in sheet 4 - Start *****************************************************************

int bw_thresh = 55;

// Added in sheet 4 - End *******************************************************************

void trackbarHandler(int pos, void *slider_value) {
    *((int *) slider_value) = pos;
}

// Added in sheet 4 - Start *****************************************************************

void bw_trackbarHandler(int pos, void *slider_value) {
    *((int *) slider_value) = pos;
}

// Added in sheet 4 - End *******************************************************************

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

int main(int ac, char **av) {
    VideoCapture cap;

    cout << "Startup (Press ESC to quit)" << endl;
    namedWindow(kWinName1, CV_WINDOW_AUTOSIZE);
    namedWindow(kWinName2, CV_WINDOW_NORMAL);
    namedWindow(kWinName3, CV_WINDOW_NORMAL);

    // Added in sheet 4 - Start *****************************************************************

    namedWindow(kWinName4, CV_WINDOW_NORMAL);

    resizeWindow(kWinName4, 120, 120);

    // Added in sheet 4 - End *******************************************************************

    initVideoStream(cap);

    int max = 255;
    int slider_value = 100;
    createTrackbar("Threshold", kWinName2, &slider_value, 255, trackbarHandler, &slider_value);

    // Added in sheet 4 - Start *****************************************************************

    int bw_sileder_value = bw_thresh;

    createTrackbar("BW Threshold", kWinName2, &bw_sileder_value, 255, bw_trackbarHandler, &bw_sileder_value);

    // Added in sheet 4 - End *******************************************************************

    bool isFirstStripe = true;

    // Added in sheet 4 - Start *****************************************************************

    bool isFirstMarker = true;

    // Added in sheet 4 - End *******************************************************************

    Mat img_bgr;

    Mat img_gray;

    Mat img_mono;

    while (true) {
        cap >> img_bgr;

        if (img_bgr.empty()) {
            printf("Could not query frame. Trying to reinitialize.\n");
            initVideoStream(cap);
            waitKey(1000); /// Wait for one sec.
            continue;
        }

        cvtColor(img_bgr, img_gray, COLOR_BGR2GRAY);

        //thresholding the grayscale image to get better results
        threshold(img_gray, img_mono, slider_value, 255, THRESH_BINARY);

        imshow(kWinName2, img_mono);

        contour_vector_t contours;

        findContours(img_mono, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        for (const auto &contour : contours) {

            contour_t approx_contour;

            approxPolyDP(contour, approx_contour, arcLength(contour, true) * 0.02, true);

            Scalar QUADRILATERAL_COLOR(0, 0, 255);

            Scalar colour;
            Rect r = boundingRect(approx_contour);
            if (approx_contour.size() == 4) {
                colour = QUADRILATERAL_COLOR;
            } else {
                continue;
            }

            if (r.height < 20 || r.width < 20 || r.width > img_mono.cols - 10 || r.height > img_mono.rows - 10) {
                continue;
            }

            Point const *points(&approx_contour[0]);
            auto n_points(static_cast<int>(approx_contour.size()));

            polylines(img_bgr, &points, &n_points, 1, true, colour, 4);

            float lineParams[16];

            Mat lineParamsMat(Size(4, 4), CV_32F, lineParams); // lineParams is shared

            for (int i = 0; i < n_points; ++i) {
                circle(img_bgr, points[i], 3, CV_RGB(0, 255, 0), -1);

                double dx = (double) (points[(i + 1) % 4].x - points[i].x) / 7.0;
                double dy = (double) (points[(i + 1) % 4].y - points[i].y) / 7.0;

                MyStrip strip;

                //a simple array of unsigned char cv::Mat

                // Added in Sheet 3 - Ex7 (a) Start *****************************************************************

                Mat iplStripe = calculate_Stripe(dx, dy, strip);

                // Added in Sheet 3 - Ex7 (a) End *****************************************************************
                // Array for edge point centers
                Point2f edgepointcenters[6];

                for (int j = 1; j < 7; ++j) {
                    double px = (double) points[i].x + (double) j * dx;
                    double py = (double) points[i].y + (double) j * dy;

                    Point p;
                    p.x = (int) px;
                    p.y = (int) py;
                    circle(img_bgr, p, 2, CV_RGB(0, 0, 255), -1);

                    // Added in Sheet 3 - Ex7 (a) Start *****************************************************************

                    for (int m = -1; m <= 1; ++m) {
                        for (int n = strip.nStart; n <= strip.nStop; ++n) {

                            Point2f subPixel;

                            subPixel.x = static_cast<float>((double) p.x + ((double) m * strip.stripeVecX.x) +
                                                            ((double) n * strip.stripeVecY.x));

                            subPixel.y = static_cast<float>((double) p.y + ((double) m * strip.stripeVecX.y) +
                                                            ((double) n * strip.stripeVecY.y));

                            Point p2;
                            p2.x = (int) subPixel.x;
                            p2.y = (int) subPixel.y;

                            if (isFirstStripe)
                                circle(img_bgr, p2, 1, CV_RGB(255, 0, 255), -1);
                            else
                                circle(img_bgr, p2, 1, CV_RGB(0, 255, 255), -1);

                            int pixel = subpixSampleSafe(img_gray, subPixel);
                            int w = m + 1; //add 1 to shift to 0..2
                            int h = n + (strip.stripeLength >> 1); //add stripelenght>>1 to shift to 0..stripeLength
                            iplStripe.at<uchar>(h,
                                                w) = (uchar) pixel; //set pointer to correct position and safe subpixel intensity
                        }
                    }
                    // Added in Sheet 3 - Ex7 (a) End *****************************************************************

                    // Added in Sheet 3 - Ex7 (b) Start *****************************************************************

                    //use sobel operator on stripe
                    // ( -1 , -2, -1 )
                    // (  0 ,  0,  0 )
                    // (  1 ,  2,  1 )

                    vector<double> sobelValues(strip.stripeLength - 2);
                    /// double* sobelValues = new double[stripeLength-2];

                    for (int n = 1; n < (strip.stripeLength - 1); n++) {

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

                    for (int n = 0; n < strip.stripeLength - 2; ++n) {

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
                    y2 = (maxIndex >= strip.stripeLength - 3) ? 0 : sobelValues[maxIndex + 1];

                    //formula for calculating the x-coordinate of the vertex of a parabola, given 3 points with equal distances
                    //(xv means the x value of the vertex, d the distance between the points):
                    //xv = x1 + (d / 2) * (y2 - y0)/(2*y1 - y0 - y2)

                    double pos = (y2 - y0) / (4 * y1 - 2 * y0 -
                                              2 * y2); //d = 1 because of the normalization and x1 will be added later

                    // This would be a valid check, too
                    if (pos != pos) {
                        // value is not a number
                        continue;
                    }

                    Point2d edgeCenter; //exact point with subpixel accuracy

                    int maxIndexShift = maxIndex - (strip.stripeLength >> 1);

                    //shift the original edgepoint accordingly
                    edgeCenter.x = (double) p.x + (((double) maxIndexShift + pos) * strip.stripeVecY.x);
                    edgeCenter.y = (double) p.y + (((double) maxIndexShift + pos) * strip.stripeVecY.y);

                    // Added in Sheet 3 - Ex7 (d) End *****************************************************************

                    // Added in sheet 4 Ex8 (a)- Start *****************************************************************

                    Point p_tmp;
                    p_tmp.x = (int) edgeCenter.x;
                    p_tmp.y = (int) edgeCenter.y;

                    circle(img_bgr, p_tmp, 1, CV_RGB(0, 0, 255), -1);

                    edgepointcenters[j - 1].x = static_cast<float>(edgeCenter.x);

                    edgepointcenters[j - 1].y = static_cast<float>(edgeCenter.y);

                    // Added in sheet 4 Ex8 (a) - End *******************************************************************

                    // Added in Sheet 3 - Ex7 (c) Start *****************************************************************

                    if (isFirstStripe) {

                        Mat iplTmp;

                        resize(iplStripe, iplTmp, Size(100, 300));

                        imshow(kWinName3, iplStripe);

                        isFirstStripe = false;
                    }

                    // Added in Sheet 3 - Ex7 (c) End *****************************************************************

                }// end of loop over edge points of one edge

                // Added in sheet 4 Ex8 (a)- Start *****************************************************************

                // we now have the array of exact edge centers stored in "points"

                Mat mat(Size(1, 6), CV_32FC2, edgepointcenters);

                fitLine(mat, lineParamsMat.col(i), CV_DIST_L2, 0, 0.01, 0.01);

                // cvFitLine stores the calculated line in lineParams in the following way:
                // vec.x, vec.y, point.x, point.y

                Point p;
                p.x = (int) lineParams[8 + i] - (int) (50.0 * lineParams[i]);
                p.y = (int) lineParams[12 + i] - (int) (50.0 * lineParams[4 + i]);

                Point p2;
                p2.x = (int) lineParams[8 + i] + (int) (50.0 * lineParams[i]);
                p2.y = (int) lineParams[12 + i] + (int) (50.0 * lineParams[4 + i]);

                line(img_bgr, p, p2, CV_RGB(0, 255, 255), 1, 8, 0);

                // Added in sheet 4 Ex8 (a)- End *******************************************************************

            }// end of loop over the 4 edges
            // Added in sheet 4 Ex8 (b)- Start *****************************************************************

            // so far we stored the exact line parameters and show the lines in the image
            // now we have to calculate the exact corners

            Point2f corners[4];

            for (int i = 0; i < 4; ++i) {

                int j = (i + 1) % 4;

                double x0, x1, y0, y1, u0, u1, v0, v1;

                x0 = lineParams[i + 8];
                y0 = lineParams[i + 12];

                x1 = lineParams[j + 8];
                y1 = lineParams[j + 12];

                u0 = lineParams[i];
                v0 = lineParams[i + 4];

                u1 = lineParams[j];
                v1 = lineParams[j + 4];

                // (x|y) = p + s * vec

                // s = Ds / D (see cramer's rule)

                // (x|y) = p + (Ds / D) * vec

                // (x|y) = (p * D / D) + (Ds * vec / D)

                // (x|y) = (p * D + Ds * vec) / D

                // (x|y) = a / c;

                double a = x1 * u0 * v1 - y1 * u0 * u1 - x0 * u1 * v0 + y0 * u0 * u1;

                double b = -x0 * v0 * v1 + y0 * u0 * v1 + x1 * v0 * v1 - y1 * v0 * u1;

                double c = v1 * u0 - v0 * u1;

                if (fabs(c) < 0.001) //lines parallel?
                {
                    std::cout << "lines parallel" << std::endl;

                    continue;
                }

                a /= c;
                b /= c;

                //exact corner
                corners[i].x = static_cast<float>(a);
                corners[i].y = static_cast<float>(b);

                // Added in sheet 4 Ex8 (b)- End *******************************************************************
                Point p;
                p.x = (int) corners[i].x;
                p.y = (int) corners[i].y;

                // Added in sheet 4 Ex8 (c)- Start *****************************************************************
                circle(img_bgr, p, 5, CV_RGB(255, 255, 0), -1);
                // Added in sheet 4 Ex8 (c)- End *******************************************************************
            } //finished the calculation of the exact corners

            // Added in sheet 4 Ex9 (a)- Start *****************************************************************
            Point2f targetCorners[4];

            targetCorners[0].x = static_cast<float>(-0.5);
            targetCorners[0].y = static_cast<float>(-0.5);

            targetCorners[1].x = 5.5;
            targetCorners[1].y = static_cast<float>(-0.5);

            targetCorners[2].x = 5.5;
            targetCorners[2].y = 5.5;

            targetCorners[3].x = static_cast<float>(-0.5);
            targetCorners[3].y = 5.5;

            //create and calculate the matrix of perspective transform

            Mat projMat(Size(3, 3), CV_32FC1);

            projMat = getPerspectiveTransform(corners, targetCorners);

            // Added in sheet 4 Ex9 (a)- End *******************************************************************

            //create image for the marker

            // Added in sheet 4 Ex9 (b)- Start *****************************************************************

            Mat iplMarker(Size(6, 6), CV_8UC1);

            //change the perspective in the marker image using the previously calculated matrix

            warpPerspective(img_gray, iplMarker, projMat, Size(6, 6));
            // Added in sheet 4 Ex9 (b)- End *******************************************************************

            threshold(iplMarker, iplMarker, bw_thresh, 255, CV_THRESH_BINARY);

            //now we have a B/W image of a supposed Marker

            // check if border is black
            // Added in sheet 4 Ex9 (c)- Start *****************************************************************
            int code = 0;

            for (int i = 0; i < 6; ++i) {

                //int pixel1 = ((unsigned char*)(iplMarker->imageData + 0*iplMarker->widthStep + i))[0]; //top

                //int pixel2 = ((unsigned char*)(iplMarker->imageData + 5*iplMarker->widthStep + i))[0]; //bottom

                //int pixel3 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep))[0]; //left

                //int pixel4 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep + 5))[0]; //right

                int pixel1 = iplMarker.at<uchar>(0, i);
                int pixel2 = iplMarker.at<uchar>(5, i);
                int pixel3 = iplMarker.at<uchar>(i, 0);
                int pixel4 = iplMarker.at<uchar>(i, 5);

                if ((pixel1 > 0) || (pixel2 > 0) || (pixel3 > 0) || (pixel4 > 0)) {
                    code = -1;
                    break;
                }
            }

            if (code < 0) {
                continue;
            }
            //copy the BW values into cP

            int cP[4][4];

            for (int i = 0; i < 4; ++i) {

                for (int j = 0; j < 4; ++j) {

                    cP[i][j] = iplMarker.at<uchar>(i + 1, j + 1);

                    cP[i][j] = (cP[i][j] == 0) ? 1 : 0; //if black then 1 else 0
                }
            }

            //save the ID of the marker

            int codes[4];

            codes[0] = codes[1] = codes[2] = codes[3] = 0;

            for (int i = 0; i < 16; i++) {

                int row = i >> 2;
                int col = i % 4;

                codes[0] <<= 1;
                codes[0] |= cP[row][col]; // 0°

                codes[1] <<= 1;
                codes[1] |= cP[3 - col][row]; // 90°

                codes[2] <<= 1;
                codes[2] |= cP[3 - row][3 - col]; // 180°

                codes[3] <<= 1;
                codes[3] |= cP[col][3 - row]; // 270°
            }

            if ((codes[0] == 0) || (codes[0] == 0xffff)) {
                continue;
            }
            //account for symmetry

            code = codes[0];

            for (int i = 1; i < 4; ++i) {

                if (codes[i] < code) {

                    code = codes[i];
                }
            }

            printf("Found: %04x\n", code);

            if (isFirstMarker) {
                imshow(kWinName4, iplMarker);

                isFirstMarker = false;

            }
            // Added in sheet 4 Ex9 (c)- End *****************************************************************

            // Added in sheet 4 - End *******************************************************************

        }// end of loop over contours

        imshow(kWinName1, img_bgr);

        isFirstStripe = true;

        // Added in sheet 4 - Start *****************************************************************

        isFirstMarker = true;

        // Added in sheet 4 - End *******************************************************************

        int key = waitKey(10);
        if (key == 27) break;
    }

    destroyWindow(kWinName1);
    destroyWindow(kWinName2);
    destroyWindow(kWinName3);
    destroyWindow(kWinName4);
    cout << "Finished\n";
}
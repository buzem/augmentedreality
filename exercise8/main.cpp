#include <GLFW/glfw3.h>

#include "DrawPrimitives.h"
#include <iostream>
#include <iomanip>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "PoseEstimation.h"
#include "MarkerTracker.h"

using namespace std;

cv::VideoCapture cap;

//camera settings
const int camera_width = 640;
const int camera_height = 480;
const int virtual_camera_angle = 30;
unsigned char bkgnd[camera_width * camera_height * 3];

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

/* program & OpenGL initialization */
void initGL(int argc, char *argv[]) {
    // initialize the GL library

// Added in Exercise 8 - End *****************************************************************
    // pixel storage/packing stuff
    glPixelStorei(GL_PACK_ALIGNMENT, 1); // for glReadPixels​
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // for glTexImage2D​
    glPixelZoom(1.0, -1.0);
// Added in Exercise 8 - End *****************************************************************

    // enable and set colors
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0, 0, 0, 1.0);

    // enable and set depth parameters
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    // light parameters
    GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_dif[] = {0.7, 0.7, 0.7, 1.0};

    // enable lighting
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void display(GLFWwindow *window, const cv::Mat &img_bgr, float resultMatrix[16]) {
// Added in Exercise 8 - Start *****************************************************************
    memcpy(bkgnd, img_bgr.data, sizeof(bkgnd));
// Added in Exercise 8 - End *****************************************************************

    int width0, height0;
    glfwGetFramebufferSize(window, &width0, &height0);
//	reshape(window, width, height);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

// Added in Exercise 8 - Start *****************************************************************
    // draw background image
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
//    gluOrtho2D( 0.0, camera_width, 0.0, camera_height );
    glOrtho(0.0, camera_width, 0.0, camera_height, -1, 1);
    glRasterPos2i(0, camera_height - 1);
    glDrawPixels(camera_width, camera_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd);

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);

// Added in Exercise 8 - End *****************************************************************

    // move to marker-position
    glMatrixMode(GL_MODELVIEW);

    float resultTransposedMatrix[16];
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            resultTransposedMatrix[x * 4 + y] = resultMatrix[y * 4 + x];
        }
    }

    //glLoadTransposeMatrixf( resultMatrix );
    glLoadMatrixf(resultTransposedMatrix);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.03, 0.03, 0.03);

    // draw 3 white spheres
    glColor4f(1.0, 1.0, 1.0, 1.0);
    drawSphere(0.8, 10, 10);
    glTranslatef(0.0, 0.8, 0.0);
    drawSphere(0.6, 10, 10);
    glTranslatef(0.0, 0.6, 0.0);
    drawSphere(0.4, 10, 10);

    // draw the eyes
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.2, 0.2);
    drawSphere(0.066, 10, 10);
    glTranslatef(0, 0, -0.4);
    drawSphere(0.066, 10, 10);
    glPopMatrix();

    // draw a nose
    glColor4f(1.0, 0.5, 0.0, 1.0);
    glTranslatef(0.3, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    drawCone(0.1, 0.3, 10, 10);
}

void reshape(GLFWwindow *window, int width, int height) {

    // set a whole-window viewport
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Note: Just setting the Perspective is an easy hack. In fact, the camera should be calibrated.
    // With such a calibration we would get the projection matrix. This matrix could then be loaded
    // to GL_PROJECTION.
    // If you are using another camera (which you'll do in most cases), you'll have to adjust the FOV
    // value. How? Fiddle around: Move Marker to edge of display and check if you have to increase or
    // decrease.
    //gluPerspective( virtual_camera_angle, ((GLfloat)width/(GLfloat)height), 0.01, 100 );
    float ratio = (GLfloat) width / (GLfloat) height;

    float near = 0.01f, far = 100.f;
    auto top = static_cast<float>(tan(virtual_camera_angle * M_PI / 360.0f) * near);
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);

    // invalidate display
    //glutPostRedisplay();
}

int main(int argc, char *argv[]) {

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // initialize the window system
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(camera_width, camera_height, "Exercise 8 - Combine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    reshape(window, window_width, window_height);

    glViewport(0, 0, window_width, window_height);

    // initialize the GL library
    initGL(argc, argv);

    // setup OpenCV
    cv::Mat img_bgr;
    initVideoStream(cap);
    const double kMarkerSize = 0.048; // [m]
    MarkerTracker markerTracker(kMarkerSize);

    float resultMatrix[16];
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Capture here */
        cap >> img_bgr;

        if (img_bgr.empty()) {
            std::cout << "Could not query frame. Trying to reinitialize." << std::endl;
            initVideoStream(cap);
            cv::waitKey(1000); /// Wait for one sec.
            continue;
        }

        /* Track a marker */
        markerTracker.findMarker(img_bgr, resultMatrix);

        /* Render here */
        display(window, img_bgr, resultMatrix);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
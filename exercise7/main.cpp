#include <GLFW/glfw3.h>

#include "DrawPrimitives.h"
#include <iostream>

GLfloat angle = 0;

void display(GLFWwindow *window) {

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // move to origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move the object backwards
    glTranslatef(0.0, -0.8, -10.0);

    const float degreePerSec = 90.0f; // [degree/sec]
    const float angle = (float) glfwGetTime() *
                        degreePerSec; /// glfwGetTime returns elapsed time counted from the program start in second
    glRotatef(angle, 0, 1, 0);

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
//	gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );
    float ratio = (GLfloat) width / (GLfloat) height;
    int fov = 30;
    float near = 0.01f, far = 100.f;
    float top = tan((double) (fov * M_PI / 360.0f)) * near;
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
}

/* program & OpenGL initialization */
void init(int argc, char *argv[]) {
    // initialize the GL library

    // enable and set colors
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.5, 0.5, 1.0, 1.0);

    // enable and set depth parameters
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    // light parameters
    GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_dif[] = {0.9, 0.9, 0.9, 1.0};

    // enable lighting
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[]) {

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // initialize the window system
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Exercise 7 - Open GL Snowman", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

    glViewport(0, 0, width, height);

    // initialize the GL library
    init(argc, argv);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        display(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
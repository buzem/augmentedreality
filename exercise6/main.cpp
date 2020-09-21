#include <GLFW/glfw3.h>

#include "DrawPrimitives.h"

/* program & OpenGL initialization */
void init(int argc, char *argv[]) {
    // enable and set colors
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // enable and set depth parameters
    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);
}

void display(GLFWwindow *window) {

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//	gluPerspective( 30, ((double)width/(double)height), 0.01, 100 );
    int fov = 30;
    float near = 0.01f, far = 100.f;
    auto top = static_cast<float>(tan(fov * M_PI / 360.0f) * near);
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
    // move to origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move the object backwards
    glTranslatef(0.0f, 0.0f, -10.0f);

    // move the object in a fancy way
    //const float t = (float) glfwGetTime() * 2.0f;
    //const float n = 0.5f;
    //glTranslatef( 1.5f*sin(n*t), 1.5f*cos(n*t), 0.0f );

    // rotate the object
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

    // draw a triangle and a red sphere
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();

    glColor4f(1.0, 0.0, 0.0, 1.0);
    drawSphere(0.5, 10, 10);
}

void reshape(GLFWwindow *window, int width, int height) {

    // set a whole-window viewport
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );
    float ratio = (GLfloat) width / (GLfloat) height;
    int fov = 30;
    float near = 0.01f, far = 100.f;
    auto top = static_cast<float>(tan(fov * M_PI / 360.0f) * near);
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
}

int main(int argc, char *argv[]) {

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // initialize the window system
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Exercise 6 - Open GL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

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
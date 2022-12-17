#include <utils.h>
const int WIDTH = 720;
const int HEIGHT = 720;
GLFWwindow *window;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
} 


int main() {
    WindowGuard windowguard(window, WIDTH, HEIGHT, "CS171 Final");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}
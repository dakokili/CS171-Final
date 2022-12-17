#include <utils.h>
#include <mesh.h>
#include <shader.h>
#include <camera.h>

const int WIDTH = 720;
const int HEIGHT = 720;
GLFWwindow *window;

//camera


Camera camera;
float lastX = 360, lastY = 360;
bool firstMouse=true;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(0);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(1);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(4);
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        camera.ProcessKeyboard(5);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(2);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(3);
} 

int main() {
  WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 hw1");
  Shader  myshader("shader\\object.vshader","shader\\object.fshader"),
          furshader("shader\\object.vshader","shader\\fur.fshader","shader\\fur.gshader"),
          lightshader("shader\\lightsource.vshader","shader\\lightsource.fshader"),
          modelshader("shader\\object.vshader","shader\\model.fshader","shader\\model.gshader");
  Mesh amesh("assets\\bunny.obj"), bmesh("assets\\plane.obj"),cmesh("assets\\sphere.obj");
  vec3 lightColor(1.0f,1.0f,1.0f);
  vec3 lightPos(1.0,1.0,1.0);
  amesh.drawprepare();bmesh.drawprepare();cmesh.drawprepare();

  mat4 model(1.0f);
  glm::mat4 proj = glm::perspective(glm::radians(camera.fov), 1.0f, 0.1f, 100.0f);
  glm::mat4 view = camera.getView();
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = camera.getView();
    proj = glm::perspective(glm::radians(camera.fov), 1.0f, 0.1f, 100.0f);

    myshader.use();
    myshader.setVec3("lightPos",lightPos);
    myshader.setVec3("lightColor",lightColor);
    myshader.setMat4("view",view);
    myshader.setMat4("projection",proj);
    myshader.setVec3("viewPos",camera.Pos);

    model = glm::scale(mat4(1.0f), glm::vec3(3, 3, 3));
    model = glm::rotate(model,(float)glfwGetTime(),glm::vec3(0.0,1.0,0.0));
    myshader.setMat4("model",model);
    amesh.draw();
    furshader.use();
    furshader.setMat4("view",view);
    furshader.setMat4("projection",proj);
    furshader.setMat4("model",model);
    amesh.draw();
    modelshader.use();
    modelshader.setMat4("view",view);
    modelshader.setMat4("projection",proj);
    modelshader.setMat4("model",model);
    amesh.draw();

    myshader.use();
    model = glm::mat4(1.0f);
    myshader.setMat4("model",model);
    bmesh.draw();

    lightshader.use();
    lightshader.setVec3("lightColor",lightColor);
    lightshader.setMat4("view",view);
    lightshader.setMat4("projection",proj);
    model = glm::translate(glm::mat4(1.0f), lightPos);
    model = glm::scale(model, glm::vec3(0.01f,0.01f,0.01f));
    lightshader.setMat4("model",model);
    cmesh.draw();

    processInput(window);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  return 0;
}
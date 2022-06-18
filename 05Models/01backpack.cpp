#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shaders.h"
#include "camera.h"
#include "model.h"


//2)
#define SCREEN_H 800
#define SCREEN_W 600

//3)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    return glViewport(0,0,width,height);
}

//23)
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
Camera camera(cameraPos);

//24)
float lastX = SCREEN_H/2.0f;
float lastY = SCREEN_W/2.0f;
bool firstMouse = true;

//25)
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//26)
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
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//27)
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main()
{
    //1)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //2)
    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "SAD FISH", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create a window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //3)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    //4)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    //5)
    glEnable(GL_DEPTH_TEST);

    //6)
    Shader ourShader("05Models/vertex.vs","05Models/fragment.fs");

    Model ourModel("models/fish/fish.obj");
    
    while (!glfwWindowShouldClose(window)) 
    {
        //a)
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame-lastFrame;
        lastFrame = currentFrame;

        //b)
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //c)
        glm::mat4 view(1.0f);
        view =  camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_W/SCREEN_H, 0.1f, 100.0f);

        //d)
        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        //e)
        processInput(window);
        glfwPollEvents();

        //h)
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(glm::vec3(0.0f,0.0f,5.0f)));
        model = glm::scale(model, glm::vec3(glm::vec3(0.3f)));
        float angle = glfwGetTime()*50.0f;
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f,0.0f,1.0f));
        ourShader.setMat4("model", model);

        ourModel.Draw(ourShader);

        //i)
        glfwSwapBuffers(window);   
    }

    //29)
    glfwTerminate();
    return 0;
}

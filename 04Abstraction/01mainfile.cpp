#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shaders.h"
#include "camera.h"

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
    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "YOU CANNOT RUN!", NULL, NULL);
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
    Shader ourShader("04Abstraction/vertex.vs","04Abstraction/fragment.fs");

    //7)
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  1.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };

    //10)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //11)
    glBindVertexArray(VAO);

    //12)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //14)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);    

    //15)
    unsigned int texture1;
    glGenTextures(1, &texture1);

    //16)
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //17)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    stbi_set_flip_vertically_on_load(1);

    //18)
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/illum.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    //21)
    ourShader.use();
    ourShader.setInt("texture1", 0);

    //22)
    glm::vec3 cubePositions[] = 
    {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(-1.5f, 0.2f, -1.5f),
    glm::vec3(0.5f, 1.0f, -10.5f),
    };

    //28)
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

        //f)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        //g)
        glBindVertexArray(VAO);

        //h)
        for (unsigned int i = 0; i < 10; ++i)
        {
            //i)
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(cubePositions[i]));
            float angle = glfwGetTime()*(70.0f)*(i+1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,1.0f,1.0f));

            //ii)
            ourShader.use();
            ourShader.setMat4("model", model);

            //iii)            
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        //i)
        glfwSwapBuffers(window);   
    }

    //29)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

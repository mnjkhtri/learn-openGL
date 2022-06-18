#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

#define SCREEN_W 800
#define SCREEN_H 600

/*
When a object is made in the local space and want the same copies of it in world space (just different orientation), only change the model matrix for each
*/

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection*view*model*vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture1, TexCoord)*vec4(ourColor, 1.0);\n"
    "}\n\0";
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "YOU CANNOT RUN!", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create a window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Enable the depth test globally and also clear the depth buffer in game loop
    glEnable(GL_DEPTH_TEST);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n",infoLog);
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n",infoLog);
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n",infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f

    };
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    stbi_set_flip_vertically_on_load(1);
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

    glm::vec3 cubePositions[] = {
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

    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        //Clear the depth buffer also
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W/SCREEN_H, 0.1f, 100.0f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; ++i)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = glfwGetTime()*50.0f*(i+1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        glfwSwapBuffers(window);   
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    return glViewport(0,0,width,height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}
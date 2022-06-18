#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define SCREEN_W 800
#define SCREEN_H 600

int main()
{
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Enable the combined shader programs
        glUseProgram(shaderProgram);

        //Bind the VAO to suppy the vertex data
        glBindVertexArray(VAO);
        //The order of binding and activating the shader programs does not matter

        //The drawarrays asks the data from the bound VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);   
        glfwPollEvents();
    }

    //Free the allocated resources
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
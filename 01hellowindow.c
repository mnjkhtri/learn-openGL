#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define SCREEN_W 800
#define SCREEN_H 600

/**
 
1) OpenGL communicates with the openGL drivers provided by the graphics manufacturers.
2) It is basically a large state machine whose current state is a collection of variables called a 'context'.
2) To create a context and an application window to draw in, is OS specific so such functionalities are abstracted to GLFW
3) GLAD retrievevs the functions of openGL drivers and stores them in function pointers for later use

 **/

//Ignore the prototypes until mentioned;

//Proto1:
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Proto2:
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();

    //To configure openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Use core-profile, i.e access only a small features of openGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a window object of width*height
    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create a window");
        glfwTerminate();
        return -1;
    }
    //"Make the context of our window the main context on the current thread"
    glfwMakeContextCurrent(window);

    //GLAD does it's stuffs
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    //Set the size of rendering window as (0,0)*(800,600). Could cause problems when window is resized?
    glViewport(0,0,SCREEN_W,SCREEN_H);
    //Yes, so tell GLFW to call <Proto1> everytime a window is resized by filling in proper arguments

    //Register the function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Keep the window until told to stop:
    while (!glfwWindowShouldClose(window))  //Checks whether GLFW has been instructed to close
    {
        //Input functionality of GLFW that when pressed ESCAPE closes the window <Proto2>
        processInput(window);

        //To clear the screen with a particular color, to avoid seeing the results of previous screens
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);   //Set the color to clear the screen with (state-setting)
        glClear(GL_COLOR_BUFFER_BIT);        //Set the COLOR BUFFER BIT (state-using)

        //The window has two faces (to avoid flickering): the front (to show the word) and the back (to draw stuffs), this swaps them
        glfwSwapBuffers(window);   

        //Communicates with the external window system: processes events and calls the corresponding callback functions (if any)
        glfwPollEvents();
    }

    //Return the resources
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

/*
What just happened?

>Initialze the GLFW
>Create an window object
>Make it current context
>Initialize GLAD
>Register the traceback functions
>Create an rendering loop:
    >Input functionalities
    >Clear screen
    >Swap buffers
    >Communicate with window system (define tracebacks)

Note: The only input functionalities of GLFW is the ESCAPE, everything else is from the window system
*/
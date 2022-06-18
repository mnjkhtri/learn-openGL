#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define SCREEN_W 800
#define SCREEN_H 600

/*
1) OpenGL takes in 3D coordinates and transfroms them into 2D pixels through the process of pipelining.
2) The pipelining consists of small parallel programs called shaders:
    a) Vertex Shader: a vertex (attributes) -> a processed vertex
    b) Shape Assembly: 
    c) Geometry Shader:
    d) Rasterization:
    e) Fragement Shader: 
    f) Tests and Blending:
Go to main now:
*/
//GLSL has data types called vec2, vec3 and vec4 which are 2, 3, and 4-component vector equivalent where each component is a GL_FLOAT
//Version 330 corresponds to OpenGL 3.3

//vertexShader will expect vertex attributes as input (in this case position; a vec3) and set up the predefined output variable (a vec4)
//Set the layout of vec3 input variable 'aPos' at location 0
//Set the predefined gl_Position using the input aPos
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
//vertexShader will expect at the location 0, a vec3 attribute

//fragmentShader will create an output variable (a vec4) and initialize it with a color
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
    "}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "LearnOpenGL", NULL, NULL);
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

    //Program the required shaders and link them:

    //Create a vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Attach the source code to it
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //Compile it
    glCompileShader(vertexShader);

    //Check for compilation errors:
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n",infoLog);
    }

    //Create a fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Attach the source code to it
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //Compile it
    glCompileShader(fragmentShader);

    //Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n",infoLog);
    }

    //Combine shaders to link the inputs and outputs of one another
    unsigned int shaderProgram = glCreateProgram();
    //Attach the vertex shader
    glAttachShader(shaderProgram, vertexShader);
    //Attach the fragement shader
    glAttachShader(shaderProgram, fragmentShader);
    //Link them
    glLinkProgram(shaderProgram);
    
    //Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n",infoLog);
    }
    //Now longer need the original shader objects after linking them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //Resulting is the shaderProgram final linked version of multiple shaders combined, later will activate it

    //Send vertices to video memory at once
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f 
    };

    //OpenGL objects are construct that contains some state, when are bound to the context, they state they contain is mapped into context's state
    //>Buffer objects store an array of unformatted memory allocated by the OpenGL context; is bound with a target that specifies its use
        //>GL_ARRAY_BUFFER target implies that buffer is to be used for vertex attribute data
    //>Vertex array objects store all of the state needed to supply vertex data; references the VBOs bound together with it
    
    unsigned int VBO, VAO;
    //Generate the buffer ID
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    //Bind the VAO first
    glBindVertexArray(VAO);
    //Bind the VBO as GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Now any calls on GL_ARRAY_BUFFER target will be used to configure VBO

    //To copy the vertex data into buffer's memory:
    //First Arg: the type of buffer we want to copy data into
    //Second Arg: size of the data
    //Third Arg: actual array of data
    //Fourth Arg: how graphics card manage the data; 
        //GL_STATIC_DRAW: likely not change or very rarely
        //GL_DYNAMIC_DRAW: likely to change a lot
        //GL_STREAM_DRAW: change every time it is drawn
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //NOTE: Now that we have our shaders setup and our data into the memory: lets make a path to send that data into the shaders

    //Give knowledge on how to interpret the data when getting it from the currently bounded VAO:

    //First Arg: that the data will go to location 0
    //Second Arg: that the location 0 is to receive 3 values of ...
    //Third Arg: type GL_FLOAT
    //Forth Arg: not requring it to normalize
    //Fifth Arg: such that the next attribute will come after 3*sizeof(float)
    //Sixth Arg: starting from the offset (void*)0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    //Enable the vertex attribute giving the location as its argument
    glEnableVertexAttribArray(0);

    //NOTE: The VAO followed by VBO must be bound before we specify the format of the locations

    //Unbind the VBO for later use
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Unbind the VAO for later use
    glBindVertexArray(0);

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

/*
What just happened?

>Initialze the GLFW
>Create an window object
>Make it current context
>Initialize GLAD
>Register the traceback functions

>Create the vertex shader expecting position attribute at some location and fragment shaders delivering the output own its own
>Compile them
>Link them into a shader program

>Initialize the vertices
>Create VBO(s) to store the vertices and VAO(s) to supply them
>Bind VAO(s) followed by VBO(s)
>Copy the vertices into VBO(s)
>Tell OpenGL how we to interpret the data on current buffer when it enters the vertex shader
......

>Unbind VBO and VAO (optional)

>Create an rendering loop:
    >Input functionalities
    >Clear screen

    >Activate the shader program
    >Bind the VAO
    >Draw the triangle

    >Swap buffers
    >Communicate with window system (define tracebacks)
*/
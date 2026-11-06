#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>



// Function declaration
void framebuffer_size_callback (GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main()
{

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #endif    

    // create a window
    GLFWwindow *window = glfwCreateWindow(640, 480, "First Window", NULL, NULL);

    if (!window) {
        std::cout << " Window Error!";
        glfwTerminate();
        return 1;
    }


    glfwMakeContextCurrent(window);

    // Initialize GLAD AFTER creating and activating the OpenGL context
    // Glad and rendering will not work if this code is not here.
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Tell GLFW to call callback function on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // **********************************************************
    // Vertex Data
    // **********************************************************

    // OpenGL doesn’t simply transform all your 3D coordinates to 2D pixels on your screen; OpenGL
    // only processes 3D coordinates when they’re in a specific range between -1.0 and 1.0 on all 3
    // axes (x, y and z). All

    // The vertex shader can only take values between -1 and 1. if any other numbers are put in the vertex 
    // shader will clip those numbers

    float vertexData[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // we can send this data to the vertex shader by creating memory in the gpu, first we
    // can create a Vertex Buffer Object.

    // Sending data from cpu to gpu is slow
    // By sending it to the gpu in its memory it can access it very fast.
    // Via the VBO
    // This object has a unique ID, which can be accessed by glGenBuffers function.

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //We can bind the newly created buffer to the GL_ARRAY_BUFFER target with
    //the glBindBuffer function:
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // glBufferData is a function specifically targeted to copy user-defined data into the currently
    // bound buffer. Its first argument is the type of the buffer we want to copy data into: the vertex buffer
    // object currently bound to the GL_ARRAY_BUFFER target. The second argument specifies the size
    // of the data (in bytes) we want to pass to the buffer; a simple sizeof of the vertex data suffices.
    // The third parameter is the actual data we want to send.

    // The fourth parameter specifies how we want the graphics card to manage the given data. This
    // can take 3 forms:
    // • GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // • GL_STATIC_DRAW: the data is set only once and used many times.
    // • GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

    // The position data of the triangle does not change, is used a lot, and stays the same for every
    // render call so its usage type should best be GL_STATIC_DRAW. If, for instance, one would have a
    // buffer with data that is likely to change frequently, a usage type of GL_DYNAMIC_DRAW ensures
    // the graphics card will place the data in memory that allows for faster writes.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Create our Vertex Shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";

    // Compile the shader
    // The first thing we need to do is create a shader object, again referenced by an ID.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Converte string literal to c string
    const char* vertexShaderSourceC_Str = vertexShaderSource.c_str();

    // Next we attach the shader source code to the shader object and compile the shader:
    glShaderSource(vertexShader, 1, &vertexShaderSourceC_Str, NULL);
    glCompileShader(vertexShader);


    // Next test if the shader was compiled
    int success;
    char infoLog [512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // First we define an integer to indicate success and a storage container for the error messages
    // (if any). Then we check if compilation was successful with glGetShaderiv. If
    // compilation failed, we should retrieve the error message with glGetShaderInfoLog
    // and print the error message.

    if(!success)
    {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
    infoLog << std::endl;

    }

    // **************************************************************
    // Fragment Shader
    // **************************************************************

    // Shader
    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )";

    // The process for compiling a fragment shader is similar to the vertex shader, although this time
    // we use the GL_FRAGMENT_SHADER constant as the shader type:
    unsigned int fragmentShader;
    const char* fragmentShaderSourceC_Str = fragmentShaderSource.c_str();
    fragmentShader = (GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC_Str, NULL);
    glCompileShader(fragmentShader);

    // Both the shaders are now compiled and the only thing left to do is link both shader objects into a
    // shader program that we can use for rendering. Make sure to check for compile errors here as well!









    // LOOP
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);   

        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}




// We can create a callback function to automatically resizes the window.
    void framebuffer_size_callback (GLFWwindow* window, int width, int height)
    {
        glViewport(0,0,width,height);
    }

// Create input for window to close on ESC button being pressed
void processInput(GLFWwindow *window)
{
if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
glfwSetWindowShouldClose(window, true);
}    
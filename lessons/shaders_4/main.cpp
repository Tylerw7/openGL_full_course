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

    

    float vertexData[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    

    unsigned int VBO;
    glGenBuffers(1, &VBO);

   
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);


    // Shaders are written in the C-like language GLSL. GLSL is tailored for use with graphics and contains
    // useful features specifically targeted at vector and matrix manipulation.

    // Shaders always begin with a version declaration, followed by a list of input and output variables,
    // uniforms and its main function. Each shader’s entry point is at its main function where we process
    // any input variables and output the results in its output variables. Don’t worry if you don’t know
    // what uniforms are, we’ll get to those shortly.

    // A shader typically has the following structure:


    // #version version_number
    // in type in_variable_name;
    // in type in_variable_name;
    // out type out_variable_name;
    // uniform type uniform_name;
    // void main()
    // {
    // // process input(s) and do some weird graphics stuff
    // ...
    // // output processed stuff to output variable
    // out_variable_name = weird_stuff_we_processed;


    // GLSL also features two container types that we’ll be using a
    // lot, namely vectors and matrices.



    // A vector in GLSL is a 1,2,3 or 4 component container for any of the basic types just mentioned.
    // They can take the following form (n represents the number of components):

    // • vecn: the default vector of n floats.
    // • bvecn: a vector of n booleans.
    // • ivecn: a vector of n integers.
    // • uvecn: a vector of n unsigned integers.
    // • dvecn: a vector of n double components.

    // Most of the time we will be using the basic vecn since floats are sufficient for most of our
    // purposes.




    // Components of a vector can be accessed via vec.x where x is the first component of the vector.
    // You can use .x, .y, .z and .w to access their first, second, third and fourth component respectively.
    // GLSL also allows you to use rgba for colors or stpq for texture coordinates, accessing the same
    // components.

    // The vector datatype allows for some interesting and flexible component selection called swiz-
    // zling. Swizzling allows us to use syntax like this:

    // vec2 someVec;
    // vec4 differentVec = someVec.xyxx;
    // vec3 anotherVec = differentVec.zyw;
    // vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

    // You can use any combination of up to 4 letters to create a new vector (of the same type) as long
    // as the original vector has those components; it is not allowed to access the .z component of a vec2
    // for example. We can also pass vectors as arguments to different vector constructor calls, reducing
    // the number of arguments required:

    // To
    // define how the vertex data is organized we specify the input variables with location metadata so we
    // can configure the vertex attributes on the CPU. We’ve seen this in the previous chapter as layout
    // (location = 0). The vertex shader thus requires an extra layout specification for its inputs so
    // we can link it with the vertex data.


    // Create our Vertex Shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
         
        out vec4 vertexColor; 

        void main()
        {
        gl_Position = vec4(aPos, 1.0); 
        vertexColor = vec4(0.5, 0.5, 0.0, 1.0);
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

        uniform vec4 ourColor;

        void main()
        {
        FragColor = ourColor;
        }
    )";

    // The process for compiling a fragment shader is similar to the vertex shader, although this time
    // we use the GL_FRAGMENT_SHADER constant as the shader type:
    unsigned int fragmentShader;
    const char* fragmentShaderSourceC_Str = fragmentShaderSource.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC_Str, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
    infoLog << std::endl;

    }

    
    // Create program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

  

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR: LINKING SHADER PROGRAM ERROR" << std::endl;
    }

    glUseProgram(shaderProgram);

   

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Technically this code could be omitted because we already called it earlier
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    



    // LOOP
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);  

        glUseProgram(shaderProgram); 

        // Uniform
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include <iostream>






int main()
{
    // --------------------------------
    // INITIALIZE GLFW
    // --------------------------------

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "OpenGL Triangle",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // --------------------------------
    // INITIALIZE GLAD
    // --------------------------------

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // --------------------------------
    // VERTEX DATA
    // --------------------------------

    float vertices[] = {

        // X       Y       Z

         0.0f,   0.5f,   0.0f,  // Top
        -0.5f,  -0.5f,   0.0f,  // Bottom left
         0.5f,  -0.5f,   0.0f   // Bottom right
    };

    float textCoords[] =
    {
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        0.5f, 1.0f
    };

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    

    // --------------------------------
    // CREATE VAO AND VBO
    // --------------------------------

    unsigned int VAO;
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertex data into GPU memory
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Configure vertex attribute
    glVertexAttribPointer(
        0,                  // Attribute location
        3,                  // Number of components
        GL_FLOAT,           // Data type
        GL_FALSE,           // Normalize?
        3 * sizeof(float),  // Stride
        (void*)0            // Offset
    );

    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --------------------------------
    // LOAD SHADERS
    // --------------------------------

    Shader triangleShader(
        "shaders/vertex.vert",
        "shaders/fragment.frag"
    );

    // --------------------------------
    // RENDER LOOP
    // --------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader
        triangleShader.use();

        // Draw triangle
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------------------
    // CLEANUP
    // --------------------------------

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

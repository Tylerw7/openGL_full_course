#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    // A quad made of 4 unique vertices. Each vertex packs:
    //   position (3 floats) | color (3 floats) | texture coords (2 floats)
    // The two triangles that form the quad are described by the index
    // array below so the shared corners aren't duplicated.

    float vertices[] = {
        // positions          //     colors      //  texture coords
         0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    // Indices into the vertex array, two triangles making one quad.
    unsigned int indices[] = {
        0, 1, 3, // first triangle  (top right, bottom right, top left)
        1, 2, 3  // second triangle (bottom right, bottom left, top left)
    };

    // --------------------------------
    // LOAD TEXTURE
    // --------------------------------
    // Paths are resolved relative to the working directory the program is
    // launched from (the project root, same as the shader paths below).

    int width, height, nrChannels;
    unsigned char* data = stbi_load(
        "lessons/textures/brick.png",
        &width,
        &height,
        &nrChannels,
        0
    );

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // other option: GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        // Match the pixel format to the number of channels stb reported so
        // that an image with an alpha channel (4 channels) isn't uploaded
        // as if it were RGB.
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }

    stbi_image_free(data);

    // --------------------------------
    // CREATE VAO, VBO AND EBO
    // --------------------------------

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO first; it records the buffer bindings and attribute
    // layout set up below.
    glBindVertexArray(VAO);

    // Upload vertex data into the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Upload index data into the EBO. The VAO remembers this binding, so the
    // EBO must stay bound while the VAO is bound (do not unbind it early).
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // Configure the vertex attributes. Each vertex is 8 floats wide.

    // Position attribute (location = 0)
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // Unbind the VBO and VAO. The EBO is intentionally left bound because the
    // VAO stores the element-buffer binding; unbinding it here would clear
    // that record. Unbinding the VAO first makes this safe.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --------------------------------
    // LOAD SHADERS
    // --------------------------------

    Shader triangleShader(
        "shaders/vertex.vert",
        "shaders/fragment.frag"
    );

    // Tell the sampler uniform which texture unit to read from. The texture
    // is bound to unit 0 in the render loop below.
    triangleShader.use();
    glUniform1i(glGetUniformLocation(triangleShader.ID, "outTexture"), 0);

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

        // Bind the texture to unit 0.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Activate shader and draw the quad.
        triangleShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------------------
    // CLEANUP
    // --------------------------------

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

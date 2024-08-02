#include "shader.hpp"
#include "gui.hpp"
#include "text.hpp"

#include <cmath>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define HEIGHT 720
#define WIDTH 1280

unsigned int VAO, VBO;
int currFontSize = 24;

TextRenderer *Text;

int main(int argc, char **argv)
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

    // create a glfw window
    GLFWwindow *window = glfwCreateWindow(WIDTH,HEIGHT,"Term", NULL, NULL);
    if(window==NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("/home/mohit/github/mosh/src/shaders/shader.vs", "/home/mohit/github/mosh/src/shaders/shader.fs");
    // glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    GLenum error;
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << error << std::endl;
    }

    float vertices[] = {
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3
    };


    // Create a vertex buffer object and vertex array object
    // Create a memory on the GPU, configure how opengl interprets it and specify how to send data to the graphics card
    // unsigned int EBO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO); // generate 1 buffer and assign it to VBO
    // glGenBuffers(1, &EBO);
    //
    // // Bind vertex array object
    // glBindVertexArray(VAO);
    // // copy vertices to a vertex buffer
    // glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO to GL_ARRAY_BUFFER target
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // send data to GL_ARRAY_BUFFER target
    // // copy indices to element buffer
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // // copy color to fragment shader
    //
    // // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    //
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //
    // // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0); 
    //
    //
    //
    Text = new TextRenderer(WIDTH, HEIGHT);
    Text->LoadFont("/home/mohit/.local/share/fonts/Go Mono Bold for Powerline.ttf", 24);

    double xCursorPos, yCursorPos;
    glm::vec2 offset, offsetPrev;
    ourShader.use();
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Text->RenderText("Hello", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.2f, 0.3f));
        // float timeValue = glfwGetTime();
        //
        // RenderText(textShader, "Hello Hello Hello", 25.0f, 25.0f, 1.0f, glm::vec3(0.2f, 0.1f, 0.2f));
        // RenderText(textShader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));

    //     glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
    //     offset = glm::vec2(xCursorPos, yCursorPos) / glm::vec2(WIDTH, HEIGHT);
    //     offset -= 0.5;
    //     offset.y = -offset.y;
    // 
    //     float angle = ((sin(timeValue))/2.0f + 0.5f)*360;
    //     float scaling = 1.0f;
    //
    //     glm::mat4 rot = glm::mat4(1.0f);
    //     glm::mat4 scale = glm::mat4(1.0f);
    //     glm::mat4 trans = glm::mat4(1.0f);
    //     scale = glm::scale(scale, glm::vec3(scaling, scaling, scaling));
    //     rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    //     trans = glm::translate(trans, glm::vec3(offset, 0.0f));
    //
    //     // std::cout << angle <<  " " << scaling << " " << offset.y << std::endl;
    //
    //     ourShader.setMatrix4("transform.rotate", rot);
    //     ourShader.setMatrix4("transform.scale", scale);
    //     ourShader.setMatrix4("transform.translate", trans);
    //
    //     int rotateLocation = glGetUniformLocation(ourShader.ID, "transform.rotate");
    //     int scaleLocation = glGetUniformLocation(ourShader.ID, "transform.scale");
    //     int translateLocation = glGetUniformLocation(ourShader.ID, "transform.translate");
    //     // std::cout << rotateLocation << scaleLocation << translateLocation << std::endl;
    //     // glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(transf.rotate));
    //     // glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, glm::value_ptr(transf.scale));
    //     // glUniformMatrix4fv(translateLocation, 1, GL_FALSE, glm::value_ptr(transf.translate));
    //
    //     glBindVertexArray(VAO);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    //     // glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    //     // float* data = (float*)malloc(sizeof(vertices));
    //     // memcpy(data, vertices, sizeof(vertices));
    //     // for(int i = 0; i < sizeof(vertices)/sizeof(float); i++) {
    //     //     std::cout << data[i] << " ";
    //     // }
    //     // std::cout << std::endl;
    //     // free(data);
    //     // glDrawArrays(GL_TRIANGLES, 0, 3);
    //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "Resizing window to " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS | glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "q/Esc key pressed: Closing window" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (currFontSize <=60)
        {
            std::cout << "Changing font size" << std::endl;
            currFontSize += 2;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (currFontSize >= 18)
        {
            std::cout << "Changing font size" << std::endl;
            currFontSize -= 2;
        }
    }
}

// void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color)
// {
//     s.Use();
//     glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
//     glActiveTexture(GL_TEXTURE0);
//     glBindVertexArray(VAO);
//
//     std::string::const_iterator c;
//     for(c = text.begin(); c !=text.end(); c++)
//     {
//         Character ch = Characters[*c]
//     }
// }

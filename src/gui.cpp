#include "shader.hpp"

#include <cmath>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include FT_FREETYPE_H

#define HEIGHT 720
#define WIDTH 1280

struct transformation {
    glm::mat4 rotate;
    glm::mat4 scale;
    glm::mat4 translate;
};

unsigned int VAO, VBO;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);

// const char *vertexShaderSource = "#version 460 core\n"
//     "struct transformation {\n"
//         "mat4 rotate;\n"
//         "mat4 scale;\n"
//         "mat4 translate;\n"
//     "};\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "layout (location = 1) in vec3 aColor;\n"
//     "uniform transformation transform;\n"
//     "out vec3 vertexColor;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = transform.scale*transform.translate*transform.rotate*vec4(aPos, 1.0f);\n"
//     "   vertexColor = aColor;\n"
//     "}\0";
//
// const char *fragmentShaderSource = "#version 460 core\n"
//     "in vec3 vertexColor;\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(vertexColor, 1.0f);\n"
//     "}\n\0";
//

struct Character {
    unsigned int TextureID; // ID handle of the glypth texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top glyph
    long Advance; // Offset to advance to next glyph
};

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

    GLenum error;

    Shader ourShader("/home/mohit/github/mosh/src/shaders/shader.vs", "/home/mohit/github/mosh/src/shaders/shader.fs");

    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout << "1. OpenGL Error: " << error << std::endl;
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
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // generate 1 buffer and assign it to VBO
    glGenBuffers(1, &EBO);

    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout << "2. OpenGL Error: " << error << std::endl;
    }

    // Bind vertex array object
    glBindVertexArray(VAO);
    // copy vertices to a vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO to GL_ARRAY_BUFFER target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // send data to GL_ARRAY_BUFFER target
    // copy indices to element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // copy color to fragment shader

    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout << "3. OpenGL Error: " << error << std::endl;
    }

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout << "4. OpenGL Error: " << error << std::endl;
    }


    // // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //
    // // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0); 

    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR:FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if(FT_New_Face(ft, "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 0, &face))
    {
        std::cout << "ERROR:FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    std::map<char, Character> Characters;
    FT_Set_Pixel_Sizes(face, 0, 10);
    for(unsigned char c=0; c < 128; c++)
    {
        // std::cout << c << std::endl;
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << c << " ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    double xCursorPos, yCursorPos;
    glm::vec2 offset, offsetPrev;
    ourShader.use();
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();

        ourShader.use();

        glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
        offset = glm::vec2(xCursorPos, yCursorPos) / glm::vec2(WIDTH, HEIGHT);
        offset -= 0.5;
        offset.y = -offset.y;
    
        float angle = ((sin(timeValue))/2.0f + 0.5f)*360;
        float scaling = 1.0f;

        glm::mat4 rot = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        scale = glm::scale(scale, glm::vec3(scaling, scaling, scaling));
        rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(offset, 0.0f));

        // std::cout << angle <<  " " << scaling << " " << offset.y << std::endl;

        ourShader.setMatrix4("transform.rotate", rot);
        ourShader.setMatrix4("transform.scale", scale);
        ourShader.setMatrix4("transform.translate", trans);

        int rotateLocation = glGetUniformLocation(ourShader.ID, "transform.rotate");
        int scaleLocation = glGetUniformLocation(ourShader.ID, "transform.scale");
        int translateLocation = glGetUniformLocation(ourShader.ID, "transform.translate");
        // std::cout << rotateLocation << scaleLocation << translateLocation << std::endl;
        // glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(transf.rotate));
        // glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, glm::value_ptr(transf.scale));
        // glUniformMatrix4fv(translateLocation, 1, GL_FALSE, glm::value_ptr(transf.translate));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // float* data = (float*)malloc(sizeof(vertices));
        // memcpy(data, vertices, sizeof(vertices));
        // for(int i = 0; i < sizeof(vertices)/sizeof(float); i++) {
        //     std::cout << data[i] << " ";
        // }
        // std::cout << std::endl;
        // free(data);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

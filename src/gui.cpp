#include "shader.hpp"
#include "gui.hpp"
#include "text.hpp"
#include "commands.hpp"
#include "configReader.hpp"

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
#include <string>

#include FT_FREETYPE_H

int HEIGHT = 1216;
int WIDTH = 1126;

unsigned int VAO, VBO;
int currFontSize = 24;
std::string inputText;
int lineHeight = 30;

TextRenderer *Text;
ConfigReader *Config;

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
    glfwSetKeyCallback(window, key_callback);

    GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    glfwSetCursor(window, cursor);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Text = new TextRenderer(WIDTH, HEIGHT);
    Config = new ConfigReader("/home/mohit/github/mosh/src/config.yml");

    Text->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", currFontSize);
    Text->SetColors(Config->colorMap);

    while(!glfwWindowShouldClose(window))
    {
        float LineX = 10.0f;
        float LineY = HEIGHT - 30.0f;
        process_input(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Text->RenderText(inputText, LineX, LineY, 1.0f, "blue");
        Text->RenderCursor('|', Text->currPos, 1.0f, "red"); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    std::cout << "Resizing window to " << width << "x" << height << std::endl;
    Text->UpdateWindowSize(WIDTH, HEIGHT);
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const char* key_name = glfwGetKeyName(key, scancode);
    if(action != 0)
    {
        if(key == GLFW_KEY_SPACE)
            inputText += " ";
        else if (key == GLFW_KEY_ENTER)
        {
            inputText += "\n";
        }
        else if (key == GLFW_KEY_BACKSPACE)
        {
            if(!inputText.empty())
                inputText.pop_back();
        }
        else if (key == GLFW_KEY_C && mods == GLFW_MOD_CONTROL)
        {
            if(!inputText.empty())
                inputText += "^C";
        }
        else
        {
            if(key_name != NULL)
                inputText += key_name;

        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // std::cout << "Cursor position: " << xpos << ", " << ypos << std::endl;
}

void process_input(GLFWwindow *window)
{
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    // {
    //     std::cout << "Esc key pressed: Closing window" << std::endl;
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (currFontSize <=60)
        {
            Text->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", currFontSize);
            std::cout << "Changing font size" << std::endl;
            currFontSize += 2;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (currFontSize >= 18)
        {
            Text->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", currFontSize);
            std::cout << "Changing font size" << std::endl;
            currFontSize -= 2;
        }
    }

}

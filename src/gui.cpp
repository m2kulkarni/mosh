#include "SDL.h"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "shader.hpp"
#include "gui.hpp"
#include "text.hpp"

#include <SDL.h>

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

#define HEIGHT 1216
#define WIDTH 1126

unsigned int VAO, VBO;
int currFontSize = 24;
std::string inputText;

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

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "ERROR::SDL::Failed Initialization" << std::endl;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Text = new TextRenderer(WIDTH, HEIGHT);
    Text->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", currFontSize);

    SDL_StartTextInput();
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        handle_sdl_input();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        std::cout << inputText<< std::endl;
        Text->RenderText(inputText, 25.0f, 5.0f, 1.0f, glm::vec3(0.1f, 0.5f, 0.9f));

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

void handle_sdl_input()
{
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_TEXTINPUT)
            inputText += e.text.text;
        else if (e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == SDLK_BACKSPACE && inputText.empty())
                inputText.pop_back();
            else if (e.key.keysym.sym == SDLK_RETURN)
                inputText += "\n";
        }
    }
}

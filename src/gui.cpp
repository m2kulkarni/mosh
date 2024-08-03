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
#include FT_FREETYPE_H

#define HEIGHT 1216
#define WIDTH 1126

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

    Text = new TextRenderer(WIDTH, HEIGHT);
    Text->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", currFontSize);

    std::cout << Text->Characters.at(10).Size.x << std::endl;

    std::string in;
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // in = TextInputEvent();
        Text->RenderText("HELLLLLL", 5.0f, 10.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

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

std::string TextInputEvent()
{
    SDL_StartTextInput();
    std::string in;
    bool running = true;

    while(running)
    {
        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            if(ev.type == SDL_TEXTINPUT)
            {
                in += ev.text.text;
                // std::cout << " > " << in << std::endl;
            }
            else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && in.size())
            {
                in.pop_back();
                // std::cout << " > " << in << std::endl;
            }
            else if (ev.type == SDL_QUIT)
            {
                running = false;
            }

        }
    }
    SDL_StopTextInput();
    return in;
}

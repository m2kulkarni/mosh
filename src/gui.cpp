#include "shader.hpp"
#include "gui.hpp"
#include "text.hpp"
#include "configReader.hpp"

#include <cmath>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <string>
#include <vector>
#include <array>
#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <algorithm>

#include FT_FREETYPE_H

// Window dimensions
int HEIGHT = 1216;
int WIDTH = 1126;

// Text propoerties
int fontSize = 28;
size_t cursorIndex = 0;
int lineSpacing = 30;
float scrollOffset = 0.0f;
TextRenderer *textRenderer;
struct Line {
    std::string text;
    std::string cname;
    std::string source;
};
std::vector<Line> textBuffer;

ConfigReader *Config;
unsigned int VAO, VBO;

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

    // Set viewports and callbacks
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        WIDTH = width;
        HEIGHT = height;
        textRenderer->UpdateWindowSize(WIDTH, HEIGHT);
        glViewport(0, 0, width, height);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            processKeyboardInput(key, mods);
        }
    });

    GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    glfwSetCursor(window, cursor);

    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textRenderer = new TextRenderer(WIDTH, HEIGHT);
    Config = new ConfigReader("/home/mohit/github/mosh/src/config.yml");

    textRenderer->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", fontSize);
    textRenderer->SetColors(Config->colorMap);

    textBuffer.push_back({"$ ", "blue", "user_input"});

    while(!glfwWindowShouldClose(window))
    {

        handleKeyInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderTextBuffer(10.0f, HEIGHT - lineSpacing, 1.0f);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void handleKeyInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (fontSize<=60)
        {
            textRenderer->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", fontSize);
            std::cout << "Changing font size" << std::endl;
            fontSize += 2;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (fontSize>= 18)
        {
            textRenderer->LoadFont("/home/mohit/.local/share/fonts/Noto Mono for Powerline.ttf", fontSize);
            std::cout << "Changing font size" << std::endl;
            fontSize -= 2;
        }
    }
}

void processKeyboardInput(int key, int mods)
{
    if (key == GLFW_KEY_ENTER) {
        // Execute the command
        std::string command = textBuffer.back().text.substr(2);
        execCommands(command);

        // Add a new line to the text buffer
        textBuffer.push_back({"$ ", "foreground", "user_input"});
    }
    else if (key == GLFW_KEY_BACKSPACE) {
        // remove last character
        if (!textBuffer.back().text.empty() && textBuffer.back().text.size() > 2)
        {
            textBuffer.back().text.pop_back();
        }
    }
    else if (key == GLFW_KEY_SPACE) {
        // Add a space at the cursor position
        textBuffer.back().text += " ";
    } 
    else 
    {
        const char *keyName = glfwGetKeyName(key, 0);
        if (keyName && strlen(keyName) == 1)
        {
            textBuffer.back().text += keyName;
        }
    }
}

void execCommands(const std::string &command) {
    std::string output = runShellCommand(command);
    textBuffer.push_back({output, "green", "output"});
}

std::string runShellCommand(const std::string& command)
{
    std::array<char, 128> buffer;
    std::string result;
    std::string fish_command = "fish -c \"" + command + "\" | cat -v";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(fish_command.c_str(), "r"), pclose);
    if(!pipe)
        throw std::runtime_error("popen() failed");

    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    std::cout << result << std::endl;
    return result;
}

// FIXME: Auto Scrolling when text fills the screen
void renderTextBuffer(float x, float startY, float scale) {
    float maxVisibleHeight = HEIGHT;
    float totalBufferHeight = textBuffer.size() * lineSpacing * scale;

    if (totalBufferHeight > maxVisibleHeight) {
        scrollOffset = totalBufferHeight - maxVisibleHeight;
    }
    float y = startY - scrollOffset;
    std::cout << "Scroll offset: " << maxVisibleHeight << " " << totalBufferHeight << std::endl;
    for (const auto &line : textBuffer) 
    {
        if (y < -lineSpacing * scale || y > HEIGHT * scale) {
            y -= lineSpacing * scale * (1 + std::count(line.text.begin(), line.text.end(), '\n'));
            std::cout << "Skipping line " << y << std::endl;
            continue;
        }
        textRenderer->RenderText(line.text, x, y, scale, line.cname);

        y -= lineSpacing * scale * (1 + std::count(line.text.begin(), line.text.end(), '\n'));
    }

    renderCursor('|', textRenderer->currPos, scale);
}

void renderCursor(unsigned char c, glm::vec2 currPos, float scale)
{
    textRenderer->RenderCursor(c, currPos, scale, "green");
}


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    const float scrollStep = 30.0f;
    // scrollOffset += yoffset * scrollStep;

    // Clamp scrollOffset to valid range
    if (scrollOffset < 0.0f) {
        scrollOffset = 0.0f;
    } else if (scrollOffset > (textBuffer.size() * lineSpacing - HEIGHT)) {
        scrollOffset = textBuffer.size() * lineSpacing - HEIGHT;
    }
}

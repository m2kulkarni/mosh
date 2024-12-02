#ifndef GUI_HPP
#define GUI_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string>

struct transformation {
    glm::mat4 rotate;
    glm::mat4 scale;
    glm::mat4 translate;
};

void handleKeyInput(GLFWwindow *window);
void processKeyboardInput(int key, int mods);
void execCommands(const std::string &command);
std::string runShellCommand(const std::string& command);
void renderTextBuffer(float x, float y, float scale);
void renderCursor(unsigned char c, glm::vec2 currPos, float scale);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

#endif

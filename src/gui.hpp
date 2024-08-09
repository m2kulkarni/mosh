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

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
// std::string TextInputEvent();
void handle_sdl_input();
void process_input(GLFWwindow *window);

#endif

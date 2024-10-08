#ifndef GUI_HPP
#define GUI_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>

struct transformation {
    glm::mat4 rotate;
    glm::mat4 scale;
    glm::mat4 translate;
};

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_input(GLFWwindow *window);


#endif

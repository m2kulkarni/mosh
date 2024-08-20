#ifndef CURSOR_HPP
#define CURSOR_HPP
#include "shader.hpp"

class CursorRenderer 
{
public:
    Shader CursorShader;
    CursorRenderer(unsigned int width, unsigned int height);
    void RenderCursor(float x, float y, float scale, glm::vec3 color);
private:
    unsigned int VAO, VBO;
};
#endif

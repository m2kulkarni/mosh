#include "cursor.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


CursorRenderer::CursorRenderer(unsigned int width, unsigned int height)
{
    this->CursorShader.LoadShader(
        "/home/mohit/github/mosh/src/shaders/cursorShaderVertex.glsl", "/home/mohit/github/mosh/src/shaders/cursorSHaderFragment.glsl");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(width));

    this->CursorShader.use();
    this->CursorShader.setMatrix4("projection", projection);
    this->CursorShader.setInt("cursor_type", GLFW_IBEAM_CURSOR);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CursorRenderer::RenderCursor(float x, float y, float scale, glm::vec3 color)
{
    this->CursorShader.use();
    glUniform3f(glGetUniformLocation(this->CursorShader.ID, "cursor_type"), color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    // std::cout << "ehhrere" << std::endl;
    std::string::const_iterator c;
    Character ch = this->Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    // float xpos = x + ch.Bearing.x * scale;
    // float ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    float vertices[6][4] = {
        {xpos,      ypos+h,     0.0f, 0.0f},
        {xpos,      ypos,       0.0f, 1.0f},
        {xpos+w,    ypos,       1.0f, 1.0f},

        {xpos,      ypos+h,     0.0f, 0.0f},
        {xpos+w,    ypos,       1.0f, 1.0f},
        {xpos+w,    ypos+h,     1.0f, 0.0f}
    };

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

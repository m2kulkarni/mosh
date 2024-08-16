#include "shader.hpp"
#include "text.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
    this->TextShader.LoadShader("/home/mohit/github/mosh/src/shaders/textShaderVertex.glsl", "/home/mohit/github/mosh/src/shaders/textShaderFragment.glsl");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    this->TextShader.use();

    this->TextShader.setMatrix4("projection", projection);
    this->TextShader.setInt("text", 0);

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

void TextRenderer::LoadFont(std::string font, unsigned int fontSize)
{
    this->Characters.clear();
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR:FREETYPE: Could not Init Freetype Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "/home/mohit/.local/share/fonts/Go Mono Bold for Powerline.ttf", 0, &face))
        std::cout << "ERROR:FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char c = 0; c <= 127; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
            std::cout << "ERROR::FREETYPE: Failed to Load Glyph" << std::endl;

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character currchar = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        this->Characters.insert(std::pair<char, Character>(c, currchar));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return;
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    this->TextShader.use();
    glUniform3f(glGetUniformLocation(this->TextShader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    // std::cout << "ehhrere" << std::endl;
    std::string::const_iterator c;
    for(c=text.begin(); c != text.end(); c++)
    {
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

        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

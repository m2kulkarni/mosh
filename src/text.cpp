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

#define WIDTH 600
#define HEIGHT 800

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
    this->TextShader = Shader("/home/mohit/github/mosh/src/shaders/textShader.vs", "/home/mohit/github/mosh/src/shaders/textShader.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT));
    this->TextShader.setMatrix4("projection", projection);
    this->TextShader.setInt("text", 0);
    this->TextShader.use();
}

void TextRenderer::LoadFont(std::string font, unsigned int fontSize)
{
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
        Characters.insert(std::pair<char, Character>(c, currchar));
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
    glBindVertexArray(VAO);


    std::string::const_iterator c;
    for(c=text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

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
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

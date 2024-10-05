#include "freetype/fttypes.h"
#include "shader.hpp"
#include "text.hpp"
#include "commands.hpp"

#include <cstddef>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>
#include <regex>
#include <GLFW/glfw3.h>
#include <string>
#include <utility>
#include <vector>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
    this->TextShader.LoadShader("/home/mohit/github/mosh/src/shaders/textShaderVertex.glsl", "/home/mohit/github/mosh/src/shaders/textShaderFragment.glsl");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    this->TextShader.use();
    this->TextShader.setMatrix4("projection", projection);

    // ----------------- Text Rendering -----------------
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenVertexArrays(1, &this->VAO);   // Generate VAO for text
    glGenBuffers(1, &this->VBO);        // Generate VBO for text
    glBindVertexArray(this->VAO);       // Bind VAO
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);   // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 8, NULL, GL_DYNAMIC_DRAW); // Allocate memory for VBO

    glEnableVertexAttribArray(0);       // Enable Vertex Attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);       // Enable Vertex Attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(6 * 4 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::UpdateWindowSize(unsigned int width, unsigned int height)
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    this->TextShader.setMatrix4("projection", projection);
}

void TextRenderer::LoadFont(std::string font, unsigned int fontSize)
{
    this->Characters.clear();
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR:FREETYPE: Could not Init Freetype Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "/home/mohit/.local/share/fonts/MesloLGLNerdFont-Regular.ttf", 0, &face))
        std::cout << "ERROR:FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_UInt glyph_index;
    // FT_ULong character = FT_Get_First_Char(FT_Face face, FT_UInt *agindex);

    for(unsigned char c = 0; c <= 200; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
            std::cout << "ERROR::FREETYPE: Failed to Load Glyph" << std::endl;
        // std::cout << int(c) << " " << c << std::endl;

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

void TextRenderer::SetTextColor(std::string cname)
{
    glm::vec3 color = this->colorMap.find(cname)->second;
    glUniform3f(glGetUniformLocation(this->TextShader.ID, "textColor"), color.x, color.y, color.z);
}

void TextRenderer::SetBackgroundColor(std::string cname)
{
    glm::vec3 color = this->colorMap.find(cname)->second;
    glUniform3f(glGetUniformLocation(this->TextShader.ID, "backgroundColor"), color.x, color.y, color.z);
}

void TextRenderer::SetBackgroundBounds(float left, float right, float bottom, float top)
{
    glUniform1f(glGetUniformLocation(this->TextShader.ID, "left"), left);
    glUniform1f(glGetUniformLocation(this->TextShader.ID, "right"), right);
    glUniform1f(glGetUniformLocation(this->TextShader.ID, "bottom"), bottom);
    glUniform1f(glGetUniformLocation(this->TextShader.ID, "top"), top);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, std::string cname)
{
    this->SetTextColor(cname);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    float Hpos = this->Characters['H'].Bearing.y*scale;
    float Hneg = (this->Characters['j'].Size.y - this->Characters['j'].Bearing.y)*scale;

    // std::cout << max_Hneg << " "<< max_Hpos << std::endl;

    this->currPos = glm::vec2(x, y);
    auto segments = this->ParseText(text);

    std::string::const_iterator c;
    for(auto s : segments)
    {
        // std::cout << s.first << s.second << std::endl;
        for(c = s.second.begin(); c != s.second.end(); c++)
        {
            // std::cout << *c << " " << int(*c) << std::endl;
            for (int n : s.first)
                this->ParseANSICodes(n);
            
            if(*c == '\n')
            {
                this->currPos.x = 10;
                this->currPos.y -= 30*scale;
            }

            Character ch = this->Characters[*c];
            float xpos = this->currPos.x + ch.Bearing.x * scale;
            float ypos = this->currPos.y - (ch.Size.y - ch.Bearing.y) * scale;
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float bg_left = this->currPos.x;
            float bg_right = this->currPos.x + (ch.Advance >> 6) * scale;
            float bg_bottom = this->currPos.y - Hneg;
            float bg_top = this->currPos.y + Hpos;

            float BackgroundVertices[6][4] = {
                {bg_left,  bg_top,    0.0f, 0.0f, },
                {bg_left,  bg_bottom, 0.0f, 1.0f, },
                {bg_right, bg_bottom, 1.0f, 1.0f, },
                                                    
                {bg_left,  bg_top,    0.0f, 0.0f, },
                {bg_right, bg_bottom, 1.0f, 1.0f, },
                {bg_right, bg_top,    1.0f, 0.0f, }
            };
            
            float vertices[6][4] = {
                {xpos,      ypos+h,     0.0f, 0.0f, },
                {xpos,      ypos,       0.0f, 1.0f, },
                {xpos+w,    ypos,       1.0f, 1.0f, },

                {xpos,      ypos+h,     0.0f, 0.0f, },
                {xpos+w,    ypos,       1.0f, 1.0f, },
                {xpos+w,    ypos+h,     1.0f, 0.0f, }
            };
            // std::cout << "Bottoms: " << ypos << " " << bg_bottom << std::endl;
            // std::cout << "Tops: " << ypos + h << " " << bg_top << std::endl;
            // std::cout << "Left: " << xpos << " " << bg_left << std::endl;
            // std::cout << "Right: " << xpos + w << " " << bg_right << std::endl;
            // std::cout << "Background Vertices: " << bg_left << " " << bg_right << " " << bg_bottom << " " << bg_top << std::endl;
            // std::cout << "Vertices: " << xpos << " " << ypos << " " << xpos+w << " " << ypos+h << std::endl;

            this->TextShader.setInt("isTextUniform", 0);
            glBindBuffer(GL_ARRAY_BUFFER, this->bgVBO);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, sizeof(BackgroundVertices), BackgroundVertices);
            glDrawArrays(GL_TRIANGLES, 6, 6);

            this->TextShader.setInt("isTextUniform", 1);
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            this->currPos.x += (ch.Advance >> 6) * scale;
        }
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::RenderCursor(unsigned char c, glm::vec2 currPos, float scale, std::string cname)
{
    this->TextShader.use();
    this->SetTextColor(cname);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    Character ch = this->Characters[c]; // ASCII code for the cursor. | is 124, I is 73

    float xpos = currPos.x + ch.Bearing.x * scale;
    float ypos = currPos.y - (ch.Size.y - ch.Bearing.y) * scale;

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

std::vector<std::pair<std::vector<int>, std::string>> TextRenderer::ParseText(std::string& text)
{
    // text = execCommands("echo -e '\\033[32mRed text\\033[0m Bold blue text\\033[0m'");
    text = execCommands("echo -e '\033[1;31mBold Red\033[0m \033[4;32mUnderlined Green\033[0m \033[3;33mItalic Yellow\033[0m \033[1;4;35mBold Underlined Magenta\n\033[0m \033[7;36mInverted Cyan\033[0m \033[1;5;37mBold Blinking White\033[0m \033[38;5;208mOrange (256 color)\033[0m \033[46mWhite on Blue Background\033[0m'");
    // text = "aaa";
    // text = "\\033[41mRed text\\033[0m \\033[1;34mBold blue text\\033[0m";
    // text = "\033[2;37;41mHello";

    std::string normalized_input = text;
    normalized_input = std::regex_replace(normalized_input, std::regex("\\^\\["), "\033");
    normalized_input = std::regex_replace(normalized_input, std::regex("\\x1B"), "\033");

    std::vector<std::pair<std::vector<int>, std::string>> result;
    std::regex ansi_escape("\033\\[(\\d+(?:;\\d+)*)m([^\033]*)");

    auto begin = std::sregex_iterator(normalized_input.begin(), normalized_input.end(), ansi_escape);
    auto end = std::sregex_iterator();

    for(std::sregex_iterator i=begin; i != end; i++)
    {
        std::smatch match = *i;
        std::string codesStr= match[1].str();
        std::string t = match[2].str();
        std::regex codesRegex("\\d+");

        auto codesBegin = std::sregex_iterator(codesStr.begin(), codesStr.end(), codesRegex);
        auto codesEnd = std::sregex_iterator();
        std::vector<int> codes;

        for(std::sregex_iterator j=codesBegin; j != codesEnd; j++)
        {
            std::smatch codesMatch = *j;
            codes.push_back(std::stoi(codesMatch.str()));
        }
        result.push_back(std::make_pair(codes, t));
    }

    std::string remaining_text = std::regex_replace(normalized_input, ansi_escape, "");
    if(remaining_text.size() > 0)
        result.push_back({std::vector<int>(37), remaining_text});
    return result;
}

void TextRenderer::SetColors(const std::map<std::string, glm::vec3>& colorMap)
{
    this->colorMap = colorMap;
}


void TextRenderer::ParseANSICodes(int code)
{
    switch (code) {
        //--------- Foreground Color codes ------------
        case 30: this->SetTextColor("black"); break; // black
        case 31: this->SetTextColor("red"); break; // red
        case 32: this->SetTextColor("green"); break; // green
        case 33: this->SetTextColor("yellow"); break; // yellow
        case 34: this->SetTextColor("blue"); break; // blue
        case 35: this->SetTextColor("magenta"); break; // magenta
        case 36: this->SetTextColor("cyan"); break; // cyan
        case 37: this->SetTextColor("white"); break; // white
        case 39: this->SetTextColor("foreground"); // default color

        //--------- Background Color codes ------------
        case 40: this->SetBackgroundColor("black"); break; // black
        case 41: this->SetBackgroundColor("red"); break; // red
        case 42: this->SetBackgroundColor("green"); break; // green
        case 43: this->SetBackgroundColor("yellow"); break; // yellow
        case 44: this->SetBackgroundColor("blue"); break; // blue
        case 45: this->SetBackgroundColor("magenta"); break; // magenta
        case 46: this->SetBackgroundColor("cyan"); break; // cyan
        case 47: this->SetBackgroundColor("white"); break; // white
        case 49: this->SetBackgroundColor("background"); // default color
        //
        // --------- Reset Codes ------------
        case 0: 
            this->SetTextColor("foreground"); 
            this->SetBackgroundColor("background");
            break; // reset all attributes

        // case 1:
        //     this.SetFontStyle("bold")

    }
}

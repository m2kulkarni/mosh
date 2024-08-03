#ifndef TEXT_HPP
#define TEXT_HPP
#include "shader.hpp"
#include <map>

struct Character {
    unsigned int TextureID; // ID handle of the glypth texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top glyph
    long Advance; // Offset to advance to next glyph
};

class TextRenderer
{
public:
    Shader TextShader;
    TextRenderer(unsigned int width, unsigned int height);
    std::map<char, Character> Characters;
    void LoadFont(std::string font, unsigned int fontSize);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    unsigned int VAO, VBO;
};
#endif

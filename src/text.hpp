#ifndef TEXT_HPP
#define TEXT_HPP
#include "shader.hpp"
#include <map>
#include <vector>

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;
};

class TextRenderer
{
public:
    Shader TextShader;
    TextRenderer(unsigned int width, unsigned int height);
    void SetTextColor(glm::vec3 color);
    void UpdateWindowSize(unsigned int width, unsigned int height);
    std::map<char, Character> Characters;
    glm::vec2 currPos;
    void RenderCursor(unsigned char c, glm::vec2 currPos, float scale, glm::vec3 color);
    void LoadFont(std::string font, unsigned int fontSize);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
    void ParseANSICodes(int code);
    std::vector<std::pair<std::vector<int>, std::string>> ParseText(std::string& text);
    // glm::vec3 ANSItoColor(int colorCode);
private:
    unsigned int VAO, VBO;
};
#endif

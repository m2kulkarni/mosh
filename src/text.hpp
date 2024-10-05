#ifndef TEXT_HPP
#define TEXT_HPP
#include "shader.hpp"
#include "configReader.hpp"
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

    void UpdateWindowSize(unsigned int width, unsigned int height);

    glm::vec2 currPos;

    void SetColors(const std::map<std::string, glm::vec3>& colorMap);

    void RenderText(std::string text, float x, float y, float scale, std::string cname);
    void RenderCursor(unsigned char c, glm::vec2 currPos, float scale, std::string cname);

    void LoadFont(std::string font, unsigned int fontSize);
    void ParseANSICodes(int code);
    std::vector<std::pair<std::vector<int>, std::string>> ParseText(std::string& text);
private:
    unsigned int VAO, VBO;
    unsigned int bgVAO, bgVBO;
    std::map<std::string, glm::vec3> colorMap;
    std::map<char, Character> Characters;
    void SetTextColor(std::string cname);
    void SetBackgroundColor(std::string cname);
    void SetBackgroundBounds(float left, float right, float bottom, float top);
};
#endif

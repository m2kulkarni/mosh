#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP
#include <string>
#include <map>
#include <glm/glm.hpp>

class ConfigReader
{
public:
    ConfigReader(std::string path);
    void Defaults();
    std::map<std::string, glm::vec3> colorMap;
    glm::vec3 HexToRGB(int hex);
};
#endif

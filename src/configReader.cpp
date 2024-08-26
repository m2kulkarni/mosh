#include "configReader.hpp"
#include <map>
#include <string>
#include <yaml-cpp/yaml.h>

ConfigReader::ConfigReader(std::string path)
{
    this->Defaults();
    YAML::Node config = YAML::LoadFile(path);
    YAML::Node config_colors = config["colors"]["normal"];

    for(YAML::const_iterator j = config_colors.begin(); j != config_colors.end(); j++)
    {
        const std::string &color = j->first.as<std::string>();
        glm::vec3 rgb = this->HexToRGB(j->second.as<int>());
        this->colorMap[color] = rgb;
    }
}

void ConfigReader::Defaults()
{
    std::map<std::string, int> default_hex;
    default_hex["black"] = 0x282a2e;
    default_hex["red"] = 0xa54242;
    default_hex["green"] = 0x8c9440;
    default_hex["yellow"] = 0xde935f;
    default_hex["blue"] = 0x5f819d;
    default_hex["magenta"] = 0x85678f;
    default_hex["cyan"] = 0x5e8d87;
    default_hex["white"] = 0x707880;

    for(const auto& n : default_hex)
    {
        glm::vec3 rgb = this->HexToRGB(n.second);
        this->colorMap.insert(std::make_pair(n.first, rgb));
    }
}

glm::vec3 ConfigReader::HexToRGB(int hex)
{
    float r = ((hex >> 16) & 0xFF)/255.0;
    float g = ((hex >> 8) & 0xFF)/255.0;
    float b = ((hex) & 0xFF)/255.0;
    return glm::vec3(r, g, b);
}


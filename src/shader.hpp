#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader 
{
public:
    unsigned int ID;

    // constructor
    Shader() { }
    Shader use();

    void LoadShader(const char* vertexPath, const char* fragmentPath);

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, glm::vec2 &value) const;
    void setVec3(const std::string &name, glm::vec2 &value) const;
    void setVec4(const std::string &name, glm::vec2 &value) const;

    void setMatrix2(const std::string &name, const glm::mat2 &mat) const;
    void setMatrix3(const std::string &name, const glm::mat3 &mat) const;
    void setMatrix4(const std::string &name, const glm::mat4 &mat) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // !SHADER_HPP


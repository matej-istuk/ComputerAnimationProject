
//
// Created by babinsinko on 29/03/24.
//

#ifndef IRGLAB_SHADER_H
#define IRGLAB_SHADER_H


#include <glad/glad.h>

#include <string>

#include <fstream>
#include <sstream>
#include "glm/fwd.hpp"

class Shader
{
private:
    void checkCompilerErrors(unsigned int shader, std::string type);
    unsigned int ID;

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

    ~Shader();

    unsigned int getId() const;

    void use() const;
    void setUniform(const std::string &name, bool value)const;
    void setUniform(const std::string &name, int value)const;
    void setUniform(const std::string &name, float value)const;
    void setUniform(const std::string &name, glm::mat4 value)const;
    void setUniform(const std::string &name, glm::vec3 value)const;
    void setUniform(const std::string &name, glm::vec4 value)const;


};



#endif //IRGLAB_SHADER_H

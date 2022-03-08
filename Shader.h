
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader{

    public:
    unsigned int shaderProgram;

    //constructor
    Shader(const char* vertexPath, const char* fragPath);

    void use();
};

#endif
#include "Headers/Shader.h"
#include "Headers/Renderer.h"
#include <glm/gtc/type_ptr.hpp>

/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.cpp
Purpose: <Shader loader and access>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

Shader::Shader(const std::string& filepath) 
	: m_FilePath(filepath), m_RendererID(0), hasGeometry(false)
{
    //std::cout << "custom shader constructor" << std::endl;
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };

    std::string line;
    std::stringstream ss[2]; //one for vertex and one for fragment
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
            {
                type = ShaderType::GEOMETRY;
                hasGeometry = true;
            }
        }
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    //Debug
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char)); //allocate on the stack
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        //choose shader text
        std::string tmpErr = "";
        if (type == GL_VERTEX_SHADER)
            tmpErr = "vertex";
        else if (type == GL_FRAGMENT_SHADER)
            tmpErr = "fragment";
        else
            tmpErr = "geometry";

        std::cout << "Failed to compile " << tmpErr << " shader." << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(ShaderProgramSource shader)
{
    if (!hasGeometry)
    {
        GLCall(unsigned int program = glCreateProgram());
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, shader.VertexSource);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shader.FragmentSource);    

        GLCall(glAttachShader(program, vs));
        GLCall(glAttachShader(program, fs));

        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));

        GLCall(
            glDeleteShader(vs);
            glDeleteShader(fs);
        );

        return program;
    }

    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, shader.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shader.FragmentSource);
    unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, shader.GeometrySource);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glAttachShader(program, gs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteShader(gs);
    );

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
        
}

void Shader::SetUniform1i(const std::string& name, int val)
{
    GLCall(glUniform1i(GetUniformLocation(name), val));
}

void Shader::SetUniform1f(const std::string& name, float val)
{
    GLCall(glUniform1f(GetUniformLocation(name), val));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
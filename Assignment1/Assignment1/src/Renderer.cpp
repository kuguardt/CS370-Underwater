#include "Headers/Renderer.h"
#include <iostream>

/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Renderer.cpp
Purpose: <call GL draw call>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

Renderer* Renderer::s_Instance = nullptr;

void GLCLearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            "\n" << file << ": line " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum mode) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    GLCall(glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
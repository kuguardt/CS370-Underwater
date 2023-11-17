#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Renderer.h
Purpose: <call GL draw call>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

#include "GL/glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define SCR_WIDTH 1720
#define SCR_HEIGHT 720

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLCLearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLCLearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
    static Renderer* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Renderer(); }
    ~Renderer() {}
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum mode) const;

private:
    static Renderer* s_Instance;
    Renderer() {}
};


#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: <Shader loader and access>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

#include <string>
#include <unordered_map>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	//caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
	//methods
	int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(ShaderProgramSource shader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);

	bool hasGeometry;

public:
	Shader() : m_RendererID(0) { 
		m_FilePath = "";
		std::cout << "default shader consturctor" << std::endl; 
	}
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void getID() const
	{
		std::cout << "Shader::m_RendereID: " << m_RendererID << std::endl;
	}

	void SetUniform1i(const std::string& name, int val);
	void SetUniform1f(const std::string& name, float val);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, glm::mat4 matrix);
};


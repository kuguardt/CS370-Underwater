#pragma once
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include <vector>

class Plane
{
public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	Plane() {}

	void Init();
	void Draw(Shader&);

	void SetScale(glm::vec2 scale);
	void SetScale(float scale);
	void SetPosition(glm::vec2 pos);

	glm::mat4 GetScale() const { return scale; }
	glm::vec4 GetPosition() const { return pos; }
	glm::mat4 UpdateModelMatrix();

	void ResetScale();

private:
	VertexArray VAO;
	VertexBuffer VBO;
	IndexBuffer EBO;
	VertexBufferLayout layout;

	glm::mat4 scale		= glm::mat4(1.0f);
	glm::vec4 pos		= glm::vec4(1.0f);
	glm::mat4 modelMat	= glm::mat4(1.0f);

};


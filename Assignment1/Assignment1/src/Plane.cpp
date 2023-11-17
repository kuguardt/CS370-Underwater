#include "Headers/Plane.h"

void Plane::Init()
{
	vertices =
	{
		-0.5f,	-0.5f,	0.0f, 0.0f,	//0
		 0.5f,	-0.5f,	1.0f, 0.0f,	//1
		 0.5f,	 0.5f,	1.0f, 1.0f,	//2
		-0.5f,	 0.5f,	0.0f, 1.0f	//3
	};

	indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	//Gen VAO buffers
	VBO.Init(&vertices[0], vertices.size() * sizeof(float));

	//Attributes
	layout.Push<float>(2); //positions
	//VAO.AddBuffer(VBO, layout);
	layout.Push<float>(2); //tex coords
	VAO.AddBuffer(VBO, layout);

	EBO.Init(&indices[0], indices.size());

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Plane::Draw(Shader& shader)
{

	//update model matrix
	//modelMat = glm::translate(glm::mat4(1.0f), pos) * rot * scale * glm::translate(glm::mat4(1.0f), -center);
	//shader.SetUniformMat4f("model", modelMat);

	//set up shaders uniform
	Renderer::GetInstance()->Draw(VAO, EBO, shader, GL_TRIANGLES);
}

glm::mat4 Plane::UpdateModelMatrix() 
{
	modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z)) * scale;
	
	return modelMat;
}

void Plane::SetScale(glm::vec2 scale)
{
	this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, -scale.y, 1.0f));
}

void Plane::SetScale(float scale)
{
	this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
}

void Plane::SetPosition(glm::vec2 pos)
{
	this->pos = glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
}

void Plane::ResetScale()
{
	scale = glm::mat4(1.0f);
}

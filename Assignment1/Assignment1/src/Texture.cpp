#include "Headers/Texture.h"
#include "vendor/stb_image/stb_image.h"

void Texture::Init(Image* const img)
{
	this->img = img;
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//Essentials
	//Assignment PDF said: nearest neightbor and bilinear interpolation
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	if (img->image)
	{
		//This glPixelStorei line is for fixing reading access violation in the next function call
		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			img->GetWidth(), img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->image));
		
		Unbind();
	}
	else
	{
		std::cout << "Error: Image has no data" << std::endl;
	}
}

Texture::Texture() :m_RendererID(0), m_BPP(0){
	img = nullptr;
}

Texture::Texture(Image* const img) :m_RendererID(0), m_BPP()
{
	Init(img);
}

void Texture::Clean()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

Texture::~Texture()
{
	Clean();
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

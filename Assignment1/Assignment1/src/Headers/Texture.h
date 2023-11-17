#pragma once
#include <string>
#include "Renderer.h"
#include "Image.h"

class Texture
{
private:
	unsigned int m_RendererID;
	Image* img;
	int m_BPP;

public:
	Texture();
	Texture(Image* const img);
	~Texture();

	void Clean();

	void Init(Image* const img);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return img->GetWidth(); }
	inline int GetHeight() const { return img->GetHeight(); }
};


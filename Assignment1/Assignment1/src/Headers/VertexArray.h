#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexArray.h
Purpose: <Act as a vertex array manager>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

#include "Headers/VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
	private:
		unsigned int m_RendererID;
		unsigned int m_Offset = 0;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;
};


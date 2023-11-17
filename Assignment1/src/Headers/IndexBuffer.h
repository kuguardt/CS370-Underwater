#pragma once

/*
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: IndexBuffer.h
Purpose: <Act as a index buffer manager>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

class IndexBuffer
{
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	public:
		IndexBuffer();
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
		void Init(const unsigned int* data, unsigned int count);
		
		inline unsigned int GetCount() const { return m_Count; }
};


#pragma once

/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: UI.h
Purpose: <Manages UI>
Language: C++
Platform: Windows 32-bit, Microsoft C++ compiler (VS2019)
Project: <n.chungwatana_CS300_1>
Author: Nattamon Chungwatana, n.chungwatana, 0053759
Creation date: 29 Sep, 20222
*/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UI
{
public:
	static UI* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new UI(); }
	~UI() {}

	void Init(GLFWwindow* window);
	void Display();
	void Render();
	void CleanUp();

private:
	UI() {}
	static UI* s_Instance;
	GLFWwindow* window = nullptr;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};


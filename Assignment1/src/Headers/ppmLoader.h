#pragma once
#include <string>
#include "Image.h"

class ppmLoader
{
public:
	Image LoadPPM(std::string path);
	void writePPM(std::string name, Image* im);

	static ppmLoader* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new ppmLoader(); }
	~ppmLoader() {}
private:
	static ppmLoader* s_Instance;
};


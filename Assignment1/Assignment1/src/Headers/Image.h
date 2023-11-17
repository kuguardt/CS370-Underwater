#pragma once
#include <string>
#include <iostream>
#include "Pixel.h"

struct Image {
private:
	std::string path;
	int header;
	int band;
	int width, height;
	int maxRGB;

public:
	unsigned char* image;
	//unsigned char* histrogram;
	int totalPixels = 0;

	/// <summary>
	/// Create image object
	/// </summary>
	/// <param name="Path"></param>
	/// <param name="Header (Pn)"></param>
	/// <param name="Band"></param>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	/// <param name="Max RGB"></param>
	Image(std::string p, int head, int b, int w, int h, int m) :
		path(p), header(head), band(b), width(w), height(h), maxRGB(m) {
		totalPixels = 0;
		image = nullptr;
		//histrogram = nullptr;
	}

	Image() : Image("", 3, 3, 0, 0, 255) {}

	//Deep copy
	Image& operator=(const Image& rhs)
	{
		//if there is existing data
		if (totalPixels != 0) {
			delete[] image;
			image = nullptr;
		}

		path = rhs.path;
		header = rhs.header;
		band = rhs.band;
		width = rhs.width;
		height = rhs.height;
		maxRGB = rhs.maxRGB;

		totalPixels = rhs.totalPixels;

		image = new unsigned char[totalPixels];
		for (int i = 0; i < totalPixels; i++)
			image[i] = rhs.image[i];

		//histrogram = new unsigned char(*rhs.histrogram);

		return *this;
	}

	Image(Image& i)
	{
		this->operator=(i);
	}

	~Image()
	{
		delete[] image;
		image = nullptr;
	}

	//Getters
	int GetHeader() const { return header; }
	int GetBand() const { return band; }
	std::string GetPath() const { return path; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetMaxRGB() const { return maxRGB; }

	//Setters
	void SetHeader(int t) { header = t; }
	void SetBand(int band) { this->band = band; }
	void SetPath(std::string path) { this->path = path; };
	void SetWidth(int w) { width = w;}
	void SetHeight(int h) { height = h; }
	void SetMaxRGB(int m) { maxRGB = m; }

	//Calc
	void CalcTotalPixels()
	{
		totalPixels = width * height * band;
	}

	//Print info
	void PrintInfo()
	{
		std::cout << "Path:\t\t" << path << std::endl;
		std::cout << "Header:\t\tP" << header << std::endl;
		std::cout << "Band:\t\t" << band << std::endl;
		std::cout << "Size:\t\t" << width << " x " << height << std::endl;
		std::cout << "Max RGB:\t" << maxRGB << std::endl;
		std::cout << "===========================================================" << std::endl;
	}

	void PrintData()
	{
		for (int i = 0; i < totalPixels; i+=3)
		{
			std::cout << "(" << (int)image[i] << ", " << (int)image[i+1] << ", " << (int)image[i+2] << ") ";
		}

		std::cout << std::endl;
	}
};
#pragma once
#include "Image.h"
#include <vector>
#include <math.h>
#include <algorithm>

HSV* RGBtoHSV(RGB*);
float* HSVtoRGB(HSV HSV);

std::vector<RGB*> ImageToVector(Image* inputImg)
{
	const int imgSize = inputImg->pixelCount;
	std::vector<RGB*> out;
	RGB* var = new RGB();
	RGB* tmp;

	for (int i = 0; i < imgSize; i++)
	{
		inputImg->GetRGB(i, var);
		tmp = new RGB(var->R, var->G, var->B);
		out.push_back(tmp);
	}

	std::cout << "Image to Vector" << std::endl;

	delete var;
	return out;
}

void SortPixels(std::vector<std::vector<RGB*>>& outLookupTable, int(&outColCount)[766], 
	std::vector<RGB*> imageCols, Image* inputImg)
{
	RGB col;
	const int imgSize = inputImg->pixelCount;

	for (int i = 0; i < imgSize; i++)
	{
		int index = imageCols[i]->R + imageCols[i]->G + imageCols[i]->B;
		outLookupTable[index].push_back(imageCols[i]);
		outColCount[index]++;
	}

	std::cout << "Sort Pixels" << std::endl;

	return;
}

//Out white colors is in HSV
void FindWhiteColor(float* outWhiteColor,
	std::vector<std::vector<RGB*>>& sortedColorsLookupTable, Image* inputImg)
{
	const int imgSize = inputImg->pixelCount;
	const int whiteCount = std::round(0.8f/ 100.0f * (float)imgSize);
	int currentCount = 0;

	for (int i = 765; i > -1; i--)
	{
		if (currentCount < whiteCount)
		{
			int size = sortedColorsLookupTable[i].size();
			if (size > 0)
			{
				for (int j = 0; j < size; j++)
				{
					if (currentCount < whiteCount)
					{
						HSV* clr = RGBtoHSV(sortedColorsLookupTable[i][j]);
						outWhiteColor[0] += clr->H;
						outWhiteColor[1] += clr->S;
						outWhiteColor[2] += clr->V;
						currentCount++;
					}
					else { break; }
				}
			}
		}
	}

	outWhiteColor[0] /= whiteCount;
	outWhiteColor[1] /= whiteCount;
	outWhiteColor[2] /= whiteCount;

	std::cout << "Find White Color" << std::endl;

	return;
}

//whiteRef is in HSV
Image* ApplyChanges(Image* originalImg, float* whiteRef)
{
	Image* result = new Image;
	result = originalImg;
	int imgSize = originalImg->pixelCount;

	float* whiteRGB = HSVtoRGB(HSV(whiteRef[0], whiteRef[1], whiteRef[2]));
	std::cout << "(Apply Changes) White: ";
	std::cout << std::endl;

	whiteRGB[0] = 1.0f / (whiteRGB[0] / 255.0f);
	whiteRGB[1] = 1.0f / (whiteRGB[1] / 255.0f);
	whiteRGB[2] = 1.0f / (whiteRGB[2] / 255.0f);

	if (whiteRGB[0] > 3)
		whiteRGB[0] = 3;
				
	if (whiteRGB[1] > 3)
		whiteRGB[1] = 3;
				
	if (whiteRGB[2] > 3)
		whiteRGB[2] = 3;

	float _R_ = 1.0f * whiteRGB[0];
	float _G_ = 1.0f * whiteRGB[1];
	float _B_ = 1.0f * whiteRGB[2];

	RGB* tmpGet = new RGB();

	for (int i = 0; i < imgSize; i++)
	{
		result->GetRGB(i, tmpGet);
		RGB tmpRes(
			(int)std::floor(((float)tmpGet->R * _R_) + 0.5f),
			(int)std::floor(((float)tmpGet->G * _G_) + 0.5f),
			(int)std::floor(((float)tmpGet->B * _B_) + 0.5f)
		);
		
		result->SetPixel(i, tmpRes);
	}

	std::cout << "Finish apply changes" << std::endl;

	delete tmpGet;
	return result;
}

Image* WhiteBalance(Image* imgIn)
{
	//Image colors in vector format
	std::vector<RGB*> colors = ImageToVector(imgIn);

	//Sort colors using lookup table where the index is R+G+B
	std::vector<std::vector<RGB*>> sortedColorsLookupTable(766);
	int colCount[766] = { 0 };

	SortPixels(sortedColorsLookupTable, colCount, colors, imgIn);

	//Image white pixels
	float* referenceWhite = new float[3]{ 0.0f, 0.0f, 0.0f };
	FindWhiteColor(referenceWhite, sortedColorsLookupTable, imgIn);

	//Apply changes
	Image* res = ApplyChanges(imgIn, referenceWhite);
	delete [] referenceWhite;

	return res;
}

HSV* RGBtoHSV(RGB* clrIn) {

	HSV* clr = new HSV();

	float fRed = ((float)(clrIn->R)) / 255.0f;
	float fGreen = ((float)(clrIn->G)) / 255.0f;
	float fBlue = ((float)(clrIn->B)) / 255.0f;

	float fHue = 0.0f;
	float fSaturation = 0.0f;
	float fValue = 0.0f;

	float fCMax = std::max(std::max(fRed, fGreen), fBlue);
	float fCMin = std::min(std::min(fRed, fGreen), fBlue);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0.0f) {
		if (fCMax == fRed) {
			fHue = 60.0f * (fmod(((fGreen - fBlue) / fDelta), 6.0f));
		}
		else if (fCMax == fGreen) {
			fHue = 60.0f * (((fBlue - fRed) / fDelta) + 2.0f);
		}
		else if (fCMax == fBlue) {
			fHue = 60.0f * (((fRed - fGreen) / fDelta) + 4.0f);
		}

		if (fCMax > 0.0f) {
			fSaturation = fDelta / fCMax;
		}
		else {
			fSaturation = 0.0f;
		}

		fValue = fCMax;
	}
	else {
		fHue = 0.0f;
		fSaturation = 0.0f;
		fValue = fCMax;
	}

	if (fHue < 0.0f) {
		fHue = 360.0f + fHue;
	}

	clr->H = fHue;
	clr->S = fSaturation *100.0f;
	clr->V = fValue * 100.0f;

	return clr;
}

float* HSVtoRGB(HSV hsv) {
	float H = hsv.H;
	float S = hsv.S;
	float V = hsv.V;

	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	float R = (r + m) * 255;
	float G = (g + m) * 255;
	float B = (b + m) * 255;

	float* res = new float[3]{ R, G, B };

	return res;
}
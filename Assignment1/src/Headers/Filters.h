#pragma once
#include "Image.h"
#include <vector>

std::vector<std::vector<float>>  GaussianFilter(int N, float signma = 1.0f);

std::vector<std::vector<float>>  SobelFilterHorizontal(int N, float scalingFactor = 1.0f);
std::vector<std::vector<float>>  SobelFilterVertical(int N, float scalingFactor = 1.0f);

Image ApplySobel(Image* im, float scale, int num);

Image UnsharpMasking(Image* im, float sigma, int N);

Image ApplyFilter(Image*, const std::vector<std::vector<float>>);

/// <summary>
/// Unsharp-masking operation
/// </summary>
/// <param name="Input Image"></param>
/// <param name="Scaling Factor (K)"></param>

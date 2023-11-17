#pragma once
#include "Image.h"
#define INTENSITYLEVEL 256

Image HistogramEqualization(Image*, int* getSk = nullptr);
Image HistogramMatching(Image*, Image*);

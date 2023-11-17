#include "Headers/Filters.h"
#include "Headers/ImageOperations.h"
#include <math.h>
#define PI 3.141592653589793238462643383279502884L

void PrintKernel(std::vector<std::vector<float>> kernel);

std::vector<std::vector<float>> GaussianFilter(int N, float signma)
{
	float r, s = 2.0f * signma * signma;
	float sum = 0.0f;

	int symmetry = std::floor((N-1) / 2.0f);
	int newN = (symmetry * 2) + 1;
	//create vector of size N
	std::vector<std::vector<float>> kernel(newN, std::vector<float>(newN, 0.0f));

	for (int x = -symmetry; x <= symmetry; x++)
	{
		for (int y = -symmetry; y <= symmetry; y++)
		{
			r = std::sqrtf(x * x + y * y);
			kernel[x + symmetry][y + symmetry] = (std::exp(-(r * r) / s)) / (PI * s);
			sum += kernel[x + symmetry][y + symmetry];
		}
	}

	//normalising the kernal
	for (int i = 0; i < newN; i++)
		for (int j = 0; j < newN; j++)
			kernel[i][j] /= sum;

	//PrintKernel(kernel);

	return kernel;
}

std::vector<std::vector<float>> SobelFilterHorizontal(int N, float scalingFactor)
{
	int symmetry = std::floor((N - 1) / 2.0f);
	int newN = (symmetry * 2) + 1;
	int middle = symmetry + 1;
	//create vector of size N
	std::vector<std::vector<float>> kernelHorizontal(newN, std::vector<float>(newN, 0.0f));

	int num = symmetry;

	for (int y = -symmetry; y <= symmetry; y++)
	{
		for (int x = -symmetry; x <= symmetry; x++)
		{
			//horizontal (Gx) filter
			if (y == 0)
				break;

			kernelHorizontal[y + symmetry][x + symmetry] = num;
			if (x == 0)
				kernelHorizontal[y + symmetry][x + symmetry] = num * 2;
		}

		num--;
	}

	//scaling factor
	for (int i = 0; i < kernelHorizontal.size(); i++)
		for (int j = 0; j < kernelHorizontal.size(); j++)
		{
			kernelHorizontal[i][j] *= scalingFactor;
		}

	//PrintKernel(kernelHorizontal);
	return kernelHorizontal;
}


std::vector<std::vector<float>> SobelFilterVertical(int N, float scalingFactor)
{
	int symmetry = std::floor((N - 1) / 2.0f);
	int newN = (symmetry * 2) + 1;
	int middle = symmetry + 1;
	//create vector of size N
	std::vector<std::vector<float>> kernelVertical(newN, std::vector<float>(newN, 0.0f));

	int num = symmetry;

	for (int x = -symmetry; x <= symmetry; x++)
	{
		for (int y = -symmetry; y <= symmetry; y++)
		{
			//vertial (Gy) filter
			if (x == 0)
				break;

			kernelVertical[y + symmetry][x + symmetry] = num;
			if (y == 0)
				kernelVertical[y + symmetry][x + symmetry] = num * 2;
		}

		num--;
	}

	//scaling factor
	for (int i = 0; i < kernelVertical.size(); i++)
		for (int j = 0; j < kernelVertical.size(); j++)
			kernelVertical[i][j] *= scalingFactor;

	//PrintKernel(kernelVertical);
	return kernelVertical;
}

Image UnsharpMasking(Image* im, float sigma, int N)
{
	Image result;

	Image blur;
	blur = ApplyFilter(im, GaussianFilter(N, sigma));

	Image mask;
	mask = ArithmeticOperation(im, &blur, 1);

	result = ArithmeticOperation(im, &mask, 0);

	return result;
}

void PrintKernel(std::vector<std::vector<float>> kernel)
{
	for (int i = 0; i < kernel.size(); i++)
	{
		for (int j = 0; j < kernel.size(); j++)
			std::cout << kernel[i][j] << "\t";
		std::cout << std::endl;
	}
	std::cout << "\n\n";
}

Image ApplyFilter(Image* im, const std::vector<std::vector<float>> Gkernel)
{
	Image result;
	result = *im;

	float sumColor[3];
	RGB* tmpColor = new RGB;
	int symmetry = std::floor((Gkernel.size()-1) / 2.0f);

	int matRow = 0, matCol = 0;

	//loop through rows
	for (int i = 0; i < result.GetWidth(); i++)
	{
		//loop through columns
		for (int j = 0; j < result.GetHeight(); j++)
		{
			//reset sum
			matRow = 0;
			sumColor[0] = 0.0f;
			sumColor[1] = 0.0f;
			sumColor[2] = 0.0f;

			//lopp through the filter
			for (int row = i - symmetry; row <= i + symmetry; row++)
			{
				for (int col = j - symmetry; col <= j + symmetry; col++)
				{
					//zero padding: doesn't add to the sum
					if (!im->GetRGB((row * result.GetWidth()) + col, tmpColor))
					{
						matCol++;
						continue;
					}

					//skip the middle of the kernel
					if (matRow == symmetry + 1 && matCol == symmetry + 1)
					{
						matCol++; 
						continue;
					}

					sumColor[0] += (float)tmpColor->R * Gkernel[matRow][matCol];
					sumColor[1] += (float)tmpColor->G * Gkernel[matRow][matCol];
					sumColor[2] += (float)tmpColor->B * Gkernel[matRow][matCol];

					matCol ++;
				}

				matCol = 0;
				matRow++;
			}

			//std::cout << "sumColor: " << sumColor[0] << " " << sumColor[1] << " " << sumColor[2] << std::endl;

			result.SetPixel((i * result.GetWidth() + j), RGB(
				(unsigned char)std::round(sumColor[0]), 
				(unsigned char)std::round(sumColor[1]), 
				(unsigned char)std::round(sumColor[2])));
		}

	}

	delete tmpColor;
	tmpColor = nullptr;
	
	return result;
}

Image ApplySobel(Image* im, float scale, int num)
{
	Image horizontal;
	horizontal = ApplyFilter(im, SobelFilterHorizontal(num));

	Image vertical;
	vertical = ApplyFilter(im, SobelFilterVertical(num));

	Image result;
	result = ArithmeticOperation(&horizontal, &vertical, 1);
	return result;

}

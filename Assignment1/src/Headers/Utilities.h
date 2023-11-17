#pragma once
#include <math.h>

struct RGB
{
public:
	/*RGBA ranged from 0-255*/
	int R, G, B;

	RGB() : R(0), G(0), B(0) {}

	RGB(int red, int green, int blue) {
		//prevent value from going above 255
		R = (red > 255)		? 255 : red;
		G = (green > 255)	? 255 : green;
		B = (blue > 255)	? 255 : blue;
	}

	RGB(const RGB& original)
	{
		R = original.R;
		G = original.G;
		B = original.B;
	}

	RGB(RGB& clr) : RGB(clr.R, clr.G, clr.B) {}

	//operator overload
	bool operator == (const RGB& comp)
	{
		if (R != comp.R)
			return false;
		if (G != comp.G)
			return false;
		if (B != comp.B)
			return false;
	}

	RGB Red()		{ return RGB(255, 0, 0); }
	RGB Green()		{ return RGB(0, 255, 0); }
	RGB Blue()		{ return RGB(0, 0, 255); }

	void Print()
	{
		std::cout << "(" << R << ",\t" << G << ",\t" << B << ")" << std::endl;
	}
};

struct RGBA : public RGB
{
public:
	/*RGBA ranged from 0-255*/
	int A;

	RGBA(int red, int green, int blue) : RGB(red, green, blue){
		A = 255;
	}

	RGBA(int red, int green, int blue, int alpha) : RGB(red, green, blue)
	{
		//prevent value from going above 255
		A = (alpha > 255) ? 255 : alpha;
	}

	RGBA(const RGBA& i) : RGBA(i.R, i.G, i.B, i.A) {}

	//operator overload
	bool operator == (const RGBA& comp)
	{
		RGB::operator==(comp);
		if (A != comp.A)
			return false;
	}

	RGBA Red() { return RGBA(255, 0, 0); }
	RGBA Green() { return RGBA(0, 255, 0); }
	RGBA Blue() { return RGBA(0, 0, 255); }
};
#pragma once
#define PERSP_CORRECT

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "Types.h"

class PerspectiveCorrectInterpolation
{
public:
	static int32_t MakeImage();
private:
	static float edgeFunction(const PeCoIn::Vec3& a, const PeCoIn::Vec3& b, const PeCoIn::Vec3& c)
	{
		return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
	}
};
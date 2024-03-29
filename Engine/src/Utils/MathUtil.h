#pragma once

class MathUtil
{
public:
	static float Remap(float value, float start1, float stop1, float start2, float stop2);

	static bool IsAlmostEqual(float v1, float v2, float epsilon = 0.0001f);
	static bool IsAlmostEqual(double v1, double v2, double epsilon = 0.0000001);
};

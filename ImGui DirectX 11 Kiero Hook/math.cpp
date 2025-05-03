#include "math.h"
#include <cmath>

const float pi = 3.1415926535;

const float Vec3::Dot(Vec3 v2)
{
	return (x * v2.x) + (y * v2.y) + (z * v2.z);
}

const Vec3 Vec3::Cross(Vec3 v2)
{
	return Vec3{
		y * v2.z - z * v2.y,
		z * v2.x - x * v2.z,
		x * v2.y - y * v2.x
	};
}

const float Vec3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

const Vec3 Vec3::Unit()
{
	float length = Magnitude();

	return Vec3{
		x / length,
		y / length,
		z / length
	};
}

const Vec3 Vec3::ToAngles()
{
	Vec3 angles;

	angles.x = atan2(-z, hypot(x, y)) * (180.0 / pi);
	angles.y = atan2(y, x) * (180.0 / pi);
	angles.z = 0.0f;

	return angles;
}

const bool Vec3::WorldToScreen(Vec2& out, float(*ViewMatrix)[4][4])
{
	const float w = (*ViewMatrix)[3][0] * x + (*ViewMatrix)[3][1] * y + (*ViewMatrix)[3][2] * z + (*ViewMatrix)[3][3];

	if (w <= 0.01f)
		return false;

	const float invW = 1.0f / w;

	const float screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const float screenHeight = GetSystemMetrics(SM_CYSCREEN);

	out.x = (screenWidth / 2) + (((*ViewMatrix)[0][0] * x + (*ViewMatrix)[0][1] * y + (*ViewMatrix)[0][2] * z + (*ViewMatrix)[0][3]) * invW) * (screenWidth / 2);
	out.y = (screenHeight / 2) - (((*ViewMatrix)[1][0] * x + (*ViewMatrix)[1][1] * y + (*ViewMatrix)[1][2] * z + (*ViewMatrix)[1][3]) * invW) * (screenHeight / 2);

	if (out.x < 0 || out.x > screenWidth)
		return false;

	if (out.y < 0 || out.y > screenHeight)
		return false;

	return true;
}

ImVec2 Vec2::imVec2()
{
	return { x, y };
}

const float Vec2::Magnitude()
{
	return sqrt(
		pow(x, 2) + pow(y, 2)
	);
}

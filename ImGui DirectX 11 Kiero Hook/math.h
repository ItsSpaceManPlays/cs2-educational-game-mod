#pragma once
#include <Windows.h>

#include "imgui/imgui.h"

struct Vec2
{
	constexpr Vec2(
		const float x = 0.f,
		const float y = 0.f) noexcept :
		x(x), y(y) { }

	// Operator overrides
	Vec2 operator+(const Vec2& v) const { return { x + v.x, y + v.y }; }
	Vec2 operator-(const Vec2& v) const { return { x - v.x, y - v.y }; }
	Vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
	Vec2 operator/(float scalar) const { return{ x / scalar, y / scalar }; }

	bool operator==(const Vec2& v) const { return { x == v.x && y == v.y }; }
	bool operator!=(const Vec2& v) const { return !(*this == v); }

	ImVec2 imVec2();
	const float Magnitude();

	float x, y;
};

struct Vec3
{
	constexpr Vec3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// Operator overrides
	Vec3 operator+(const Vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vec3 operator-(const Vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vec3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Vec3 operator/(float scalar) const { return{ x / scalar, y / scalar, z / scalar }; }

	bool operator==(const Vec3& v) const { return { x == v.x && y == v.y && z == v.z }; }
	bool operator!=(const Vec3& v) const { return !(*this == v); }

	const float Dot(Vec3 v2);
	const Vec3 Cross(Vec3 v2);
	const float Magnitude();
	const Vec3 Unit();

	const Vec3 ToAngles();

	const bool WorldToScreen(Vec2& out, float(*ViewMatrix)[4][4]);

	float x, y, z;
};
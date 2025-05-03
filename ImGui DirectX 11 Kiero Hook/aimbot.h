#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "math.h"

struct AimbotSettings
{
	bool silentAimEnabled;
	int silentAimFov;
	bool aimbotEnabled;
};

namespace aimbot
{
	extern uintptr_t client;
	extern Vec3 targetViewAngles;

	void SilentAim(AimbotSettings);
	void DrawSilentAimFov(int fov);

	int GetAimbotFov();
	int GetCircleRadius();

	void Aimbot(AimbotSettings);
}
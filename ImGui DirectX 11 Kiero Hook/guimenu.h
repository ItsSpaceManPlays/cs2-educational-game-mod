#pragma once

#include <vector>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "visuals.h"
#include "aimbot.h"

namespace cheat_values
{
	extern VisualSettings settings;
	extern bool enableBhop;
	extern bool obsHide;
	extern AimbotSettings aimbotSettings;
}

namespace ImGui
{
	ImVec2 CalcItemSize(const ImVec2 size, float default_x, float default_y);
}

namespace gui
{
	void SetupGuiStyle(ImGuiStyle& style, ImGuiIO& io);

	void CenterButtons(std::vector<std::string> names, std::vector<int> indexes, int& selected_index);
	void CenterText(const char* format, const float y_padding = 0.0f, ImColor color = ImColor(255, 255, 255));

	void Render();
}
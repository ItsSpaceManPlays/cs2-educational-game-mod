#include "guimenu.h"
#include "othercheats.h"
#include "visuals.h"
#include "fonts.hpp"

extern bool cheat_values::enableBhop = false;
extern bool cheat_values::obsHide = false;

EspParameters espParams = { false, true, ImColor(0, 255, 0, 255), ImColor(255, 0, 0, 255) };
EspParameters tracerParams = { false, true, ImColor(0, 255, 0, 255), ImColor(255, 0, 0, 255) };

extern VisualSettings cheat_values::settings = { true, true, true, false, espParams, tracerParams };
extern AimbotSettings cheat_values::aimbotSettings = { false, 40, false };

const ImColor ActiveButtonColor = ImColor(0, 89, 116, 255);

namespace gui
{
	void SetupGuiStyle(ImGuiStyle& style, ImGuiIO& io)
	{
		auto& colors = style.Colors;

		io.LogFilename = nullptr;
		io.IniFilename = nullptr;

		// Window
		colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 255);

		// Checkbox
		colors[ImGuiCol_FrameBg] = ImColor(22, 22, 22, 255);
		colors[ImGuiCol_FrameBgHovered] = ImColor(30, 30, 30, 255);
		colors[ImGuiCol_FrameBgActive] = ImColor(22, 22, 22, 255);
		colors[ImGuiCol_CheckMark] = ImColor(28, 209, 229, 255);

		io.Fonts->AddFontFromMemoryTTF(
			fonts::ruda_bold,
			sizeof(fonts::ruda_bold),
			16.0f
		);
	}

	void CenterButtons(std::vector<std::string> names, std::vector<int> indexes, int& selected_index)
	{
		std::vector<ImVec2> sizes = {};
		float total_area = 0.0f;

		const auto& style = ImGui::GetStyle();

		for (std::string& name : names)
		{
			const ImVec2 label_size = ImGui::CalcTextSize(name.c_str(), 0, true);
			ImVec2 size = ImGui::CalcItemSize(ImVec2(), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
			size.x += 45.5f;
			size.y += 15.f;

			sizes.push_back(size);
			total_area += size.x;
		}

		ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (total_area / 2));
		for (uint32_t i = 0; i < names.size(); i++)
		{
			//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 78);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

			if (selected_index == indexes[i])
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ActiveButtonColor.Value);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ActiveButtonColor.Value);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ActiveButtonColor.Value);

				if (ImGui::Button(names[i].c_str(), sizes[i]))
					selected_index = indexes[i];
				ImGui::PopStyleColor(3);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0, 0, 0, 255).Value);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0, 0, 0, 255).Value);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(10, 10, 10, 255).Value);
				ImGui::PushStyleColor(ImGuiCol_Border, ImColor(28, 209, 229, 255).Value);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

				if (ImGui::Button(names[i].c_str(), sizes[i]))
					selected_index = indexes[i];
				ImGui::PopStyleVar(1);
				ImGui::PopStyleColor(4);
			}

			ImGui::PopStyleVar();
			if (i != names.size() - 1)
				ImGui::SameLine();
		}
	}

	void CenterText(const char* format, const float y_padding, ImColor color)
	{
		const ImVec2 text_size = ImGui::CalcTextSize(format);
		ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (text_size.x / 2));
		if (y_padding > 0.0f)
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y_padding);
		ImGui::TextColored(color, format);
	}

	void Render()
	{
		static int index = 0;

		ImGui::Begin("Professional gaming chair");

		CenterButtons({ "Visual", "Aimbot", "Misc"}, {0, 1, 2}, index);
		ImGui::Separator();

		if (index == 0)
		{
			ImGui::Checkbox("Enable ESP", &cheat_values::settings.esp);
			if (cheat_values::settings.esp)
			{
				ImGui::Checkbox("ESP: Show team", &cheat_values::settings.espParams.ShowTeam);
				ImGui::Checkbox("ESP: Show enemies", &cheat_values::settings.espParams.ShowEnemies);
			}

			ImGui::Checkbox("Show bones", &cheat_values::settings.bones);

			ImGui::Checkbox("Enable Tracers", &cheat_values::settings.tracers);
			if (cheat_values::settings.tracers)
			{
				ImGui::Checkbox("Tracers: Show team", &cheat_values::settings.tracerParams.ShowTeam);
				ImGui::Checkbox("Tracers: Show enemies", &cheat_values::settings.tracerParams.ShowEnemies);
			}

			ImGui::Checkbox("Enable bomb ESP", &cheat_values::settings.plantedBomb);
		}
		else if (index == 1)
		{
			ImGui::Checkbox("Aimbot###toggle", &cheat_values::aimbotSettings.aimbotEnabled);
			ImGui::Checkbox("Silent aim", &cheat_values::aimbotSettings.silentAimEnabled);
			ImGui::SliderInt("Fov", &cheat_values::aimbotSettings.silentAimFov, 0, 120);
		}
		else if (index == 2)
		{
			ImGui::Checkbox("Enable BunnyHop", &cheat_values::enableBhop);
			ImGui::Checkbox("Discord/Obs hide", &cheat_values::obsHide);
		}

		visuals::DoVisuals(cheat_values::settings);
		if (cheat_values::aimbotSettings.silentAimEnabled)
			aimbot::DrawSilentAimFov(cheat_values::aimbotSettings.silentAimFov);

		ImGui::End();
	}
}
#include "visuals.h"
#include "offsets.hpp"
#include "math.h"

#include <string>

uintptr_t visuals::client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));

std::vector<std::pair<int, int>> boneLines = {
	{6,  5},
	{5, 4},
	{4, 0},
	{4, 8},
	{8, 9},
	{9, 11},
	{4, 13},
	{13, 14},
	{14, 16},
	{4, 2},
	{0, 22},
	{0, 25},
	{22, 23},
	{23, 24},
	{25, 26},
	{26, 27}
};

void visuals::PlayerESP(EspParameters& params)
{
	float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(client + offsets::dwViewMatrix);

	auto localPawn = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayerPawn);

	if (!localPawn)
		return;

	auto localTeam = *reinterpret_cast<int*>(localPawn + offsets::m_iTeamNum);

	auto entityList = *reinterpret_cast<uintptr_t*>(client + offsets::dwEntityList);

	if (!entityList)
		return;

	for (int i = 0; i < 64; i++)
	{
		uintptr_t list_entry1 = *(uintptr_t*)(entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
		if (!list_entry1)
			continue;

		uintptr_t playerController = *(uintptr_t*)(list_entry1 + 120 * (i & 0x1FF));
		if (!playerController)
			continue;

		uint32_t playerPawn = *(uint32_t*)(playerController + offsets::m_hPlayerPawn);
		if (!playerPawn)
			continue;

		uintptr_t list_entry2 = *(uintptr_t*)(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2)
			continue;

		uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
		if (!pCSPlayerPawnPtr)
			continue;

		int health = *(int*)(pCSPlayerPawnPtr + offsets::m_iHealth);

		if (!health || health > 100)
			continue;

		int team = *(int*)(pCSPlayerPawnPtr + offsets::m_iTeamNum);

		bool isFriendly = team == localTeam;

		if (pCSPlayerPawnPtr == localPawn)
			continue;

		Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets::m_vOldOrigin);
		Vec3 headpos = { feetpos.x + 0.0f, feetpos.y + 0.0f, feetpos.z + 65.0f };

		Vec2 feet, head;

		bool feetOnScreen = feetpos.WorldToScreen(feet, ViewMatrix);
		bool headOnScreen = headpos.WorldToScreen(head, ViewMatrix);

		if (feetOnScreen && headOnScreen)
		{
			float height = (feet.y - head.y) * 1.1;
			float width = height / 1.5;

			float x = feet.x - width / 2;

			if (!isFriendly && params.ShowEnemies)
			{
				ImGui::GetForegroundDrawList()->AddRect({ x, head.y }, { x + width, head.y + height }, params.enemyColor);
			}
			else if (isFriendly && params.ShowTeam)
			{
				ImGui::GetForegroundDrawList()->AddRect({ x, head.y }, { x + width, head.y + height }, params.teamColor);
			}
		}
	}
}

void visuals::PlayerTracers(EspParameters& params)
{
	float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(client + offsets::dwViewMatrix);

	auto localPawn = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayerPawn);

	if (!localPawn)
		return;

	auto localTeam = *reinterpret_cast<int*>(localPawn + offsets::m_iTeamNum);

	auto entityList = *reinterpret_cast<uintptr_t*>(client + offsets::dwEntityList);

	if (!entityList)
		return;

	int found = 0;
	int renderedFound = 0;

	const float screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const float screenHeight = GetSystemMetrics(SM_CYSCREEN);

	for (int i = 0; i < 64; i++)
	{
		uintptr_t list_entry1 = *(uintptr_t*)(entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
		if (!list_entry1)
			continue;

		uintptr_t playerController = *(uintptr_t*)(list_entry1 + 120 * (i & 0x1FF));
		if (!playerController)
			continue;

		uint32_t playerPawn = *(uint32_t*)(playerController + offsets::m_hPlayerPawn);
		if (!playerPawn)
			continue;

		uintptr_t list_entry2 = *(uintptr_t*)(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2)
			continue;

		uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
		if (!pCSPlayerPawnPtr)
			continue;

		int health = *(int*)(pCSPlayerPawnPtr + offsets::m_iHealth);

		if (!health || health > 100)
			continue;

		int team = *(int*)(pCSPlayerPawnPtr + offsets::m_iTeamNum);

		bool isFriendly = team == localTeam;

		if (pCSPlayerPawnPtr == localPawn)
			continue;

		Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets::m_vOldOrigin);

		Vec2 feet;

		found += 1;
		bool feetOnScreen = feetpos.WorldToScreen(feet, ViewMatrix);

		if (feetOnScreen)
		{
			if (!isFriendly && params.ShowEnemies)
			{
				ImVec2 winPos = ImGui::GetWindowPos();
				ImVec2 winSize = ImGui::GetWindowSize();

				ImVec2 tracerStart = ImVec2(screenWidth / 2, screenHeight);

				ImGui::GetForegroundDrawList()->AddLine(tracerStart, ImVec2(feet.x, feet.y), params.enemyColor);
			}
			else if (isFriendly && params.ShowTeam)
			{
				ImVec2 winPos = ImGui::GetWindowPos();
				ImVec2 winSize = ImGui::GetWindowSize();

				ImVec2 tracerStart = ImVec2(screenWidth / 2, screenHeight);

				ImGui::GetForegroundDrawList()->AddLine(tracerStart, ImVec2(feet.x, feet.y), params.teamColor);
			}
		}
	}
}

void visuals::PlantedBombEsp(ImColor color = ImColor(255, 99, 71, 255))
{
	bool isPlanted = *reinterpret_cast<bool*>(client + offsets::dwPlantedC4 - 0x8);
	if (!isPlanted)
		return;

	float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(client + offsets::dwViewMatrix);

	uintptr_t c4Base = *reinterpret_cast<uintptr_t*>(client + offsets::dwPlantedC4);
	if (!c4Base)
		return;

	uintptr_t plantedC4 = *reinterpret_cast<uintptr_t*>(c4Base);
	if (!plantedC4)
		return;

	uintptr_t c4node = *reinterpret_cast<uintptr_t*>(plantedC4 + offsets::m_pGameSceneNode);
	if (!c4node)
		return;

	if (!(*reinterpret_cast<uintptr_t*>(c4node + offsets::m_vecAbsOrigin)))
		return;

	Vec3 bombPos = *reinterpret_cast<Vec3*>(c4node + offsets::m_vecAbsOrigin);
	if (bombPos.x == 0.0f && bombPos.y == 0.0f && bombPos.z == 0.0f)
		return;

	Vec2 bombScreenPos;
	bool bombOnScreen = bombPos.WorldToScreen(bombScreenPos, ViewMatrix);

	if (!bombOnScreen)
		return;

	ImFont* font = ImGui::GetFont();

	ImGui::GetForegroundDrawList()->AddText(
		font,
		35.0f,
		{ bombScreenPos.x, bombScreenPos.y },
		color,
		"c4"
	);
}

void visuals::PlayerBoneEsp(uintptr_t pCSPlayerPawn)
{
	/*uintptr_t playerBodyComponent = *reinterpret_cast<uintptr_t*>(pCSPlayerPawn + offsets::m_CBodyComponent);
	if (!playerBodyComponent)
		return;

	uintptr_t playerSkeletonInstance = *reinterpret_cast<uintptr_t*>(playerBodyComponent + offsets::m_skeletonInstance);
	if (!playerSkeletonInstance)
		return;

	uintptr_t playerModelState = *reinterpret_cast<uintptr_t*>(playerSkeletonInstance + offsets::m_modelState);
	if (!playerModelState)
		return;*/

	float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(client + offsets::dwViewMatrix);

	uintptr_t p_gameScene = *reinterpret_cast<uintptr_t*>(pCSPlayerPawn + offsets::m_pGameSceneNode);
	if (!p_gameScene)
		return;

	uintptr_t boneMatrix = *reinterpret_cast<uintptr_t*>(p_gameScene + offsets::m_modelState + 0x80);
	if (!boneMatrix)
		return;

	/*for (int i = 0; i < 24; i++)
	{
		if (!(*reinterpret_cast<uintptr_t*>(boneMatrix + i * 32)))
			continue;

		Vec3 bonePosition = *reinterpret_cast<Vec3*>(boneMatrix + i * 32);

		Vec2 boneScreen;
		bool boneOnScreen = bonePosition.WorldToScreen(boneScreen, ViewMatrix);
		if (!boneOnScreen)
			continue;

		ImGui::GetForegroundDrawList()->AddText(
			{ boneScreen.x, boneScreen.y },
			ImColor(255, 255, 255, 255),
			std::to_string(i).c_str()
		);
	}*/

	for (std::pair<int, int> bonePair : boneLines)
	{
		Vec3 bone1Pos = *reinterpret_cast<Vec3*>(boneMatrix + bonePair.first  * 32);
		Vec3 bone2Pos = *reinterpret_cast<Vec3*>(boneMatrix + bonePair.second * 32);

		Vec2 bone1;
		Vec2 bone2;

		bool bone1Visible = bone1Pos.WorldToScreen(bone1, ViewMatrix);
		bool bone2Visible = bone2Pos.WorldToScreen(bone2, ViewMatrix);

		if (!(bone1Visible || bone2Visible))
			continue;

		ImGui::GetForegroundDrawList()->AddLine(
			bone1.imVec2(),
			bone2.imVec2(),
			ImColor(255, 255, 255)
		);
	}
}

void visuals::DoVisuals(VisualSettings& settings)
{
	float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(client + offsets::dwViewMatrix);

	auto localPawn = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayerPawn);

	if (!localPawn)
		return;

	auto localTeam = *reinterpret_cast<int*>(localPawn + offsets::m_iTeamNum);

	auto entityList = *reinterpret_cast<uintptr_t*>(client + offsets::dwEntityList);

	if (!entityList)
		return;

	const float screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const float screenHeight = GetSystemMetrics(SM_CYSCREEN);

	for (int i = 0; i < 64; i++)
	{
		uintptr_t list_entry1 = *(uintptr_t*)(entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
		if (!list_entry1)
			continue;

		uintptr_t playerController = *(uintptr_t*)(list_entry1 + 120 * (i & 0x1FF));
		if (!playerController)
			continue;

		uint32_t playerPawn = *(uint32_t*)(playerController + offsets::m_hPlayerPawn);
		if (!playerPawn)
			continue;

		uintptr_t list_entry2 = *(uintptr_t*)(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2)
			continue;

		uintptr_t pCSPlayerPawnPtr = *(uintptr_t*)(list_entry2 + 120 * (playerPawn & 0x1FF));
		if (!pCSPlayerPawnPtr)
			continue;

		int health = *(int*)(pCSPlayerPawnPtr + offsets::m_iHealth);

		if (!health || health > 100)
			continue;

		int team = *(int*)(pCSPlayerPawnPtr + offsets::m_iTeamNum);

		bool isFriendly = team == localTeam;

		if (pCSPlayerPawnPtr == localPawn)
			continue;

		if (settings.bones) PlayerBoneEsp(pCSPlayerPawnPtr);

		Vec3 feetpos = *(Vec3*)(pCSPlayerPawnPtr + offsets::m_vOldOrigin);
		Vec3 headpos = { feetpos.x + 0.0f, feetpos.y + 0.0f, feetpos.z + 65.0f };

		Vec2 feet, head;

		bool feetOnScreen = feetpos.WorldToScreen(feet, ViewMatrix);
		bool headOnScreen = headpos.WorldToScreen(head, ViewMatrix);

		if (settings.tracers)
		{
			EspParameters params = settings.tracerParams;
			if (feetOnScreen)
			{
				if (!isFriendly && params.ShowEnemies)
				{
					ImVec2 winPos = ImGui::GetWindowPos();
					ImVec2 winSize = ImGui::GetWindowSize();

					ImVec2 tracerStart = ImVec2(screenWidth / 2, screenHeight);

					ImGui::GetForegroundDrawList()->AddLine(tracerStart, ImVec2(feet.x, feet.y), params.enemyColor);
				}
				else if (isFriendly && params.ShowTeam)
				{
					ImVec2 winPos = ImGui::GetWindowPos();
					ImVec2 winSize = ImGui::GetWindowSize();

					ImVec2 tracerStart = ImVec2(screenWidth / 2, screenHeight);

					ImGui::GetForegroundDrawList()->AddLine(tracerStart, ImVec2(feet.x, feet.y), params.teamColor);
				}
			}
		}

		if (settings.esp)
		{
			EspParameters params = settings.espParams;
			if (feetOnScreen || headOnScreen)
			{
				float height = (feet.y - head.y) * 1.1;
				float width = height / 1.5;

				float x = feet.x - width / 2;

				if (!isFriendly && params.ShowEnemies)
				{
					ImGui::GetForegroundDrawList()->AddRect({ x, head.y }, { x + width, head.y + height }, params.enemyColor);
				}
				else if (isFriendly && params.ShowTeam)
				{
					ImGui::GetForegroundDrawList()->AddRect({ x, head.y }, { x + width, head.y + height }, params.teamColor);
				}
			}
		}
	}

	if (settings.plantedBomb)
	{
		PlantedBombEsp();
	}
}
#include "aimbot.h"
#include "offsets.hpp"
#include "visuals.h"
#include "math.h"
#include "guimenu.h"

uintptr_t aimbot::client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
Vec3 aimbot::targetViewAngles = {};

uintptr_t GetBoneMatrix(uintptr_t pCSPlayerPawn)
{
	uintptr_t p_gameScene = *reinterpret_cast<uintptr_t*>(pCSPlayerPawn + offsets::m_pGameSceneNode);
	if (!p_gameScene)
		return 0;

	uintptr_t boneMatrix = *reinterpret_cast<uintptr_t*>(p_gameScene + offsets::m_modelState + 0x80);
	if (!boneMatrix)
		return 0;

	return boneMatrix;
}

void aimbot::SilentAim(AimbotSettings settings)
{

}

void aimbot::Aimbot(AimbotSettings settings)
{
	float(*ViewMatrix)[4][4] = reinterpret_cast<float(*)[4][4]>(client + offsets::dwViewMatrix);

	// loop entity list
	uintptr_t localPlayerPawn = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayerPawn);
	if (!localPlayerPawn)
		return;

	int localHealth = *reinterpret_cast<int*>(localPlayerPawn + offsets::m_iHealth);
	if (!localHealth)
		return;

	int localTeam = *reinterpret_cast<int*>(localPlayerPawn + offsets::m_iTeamNum);

	// Vec3 localPos = *reinterpret_cast<Vec3*>(localPlayerPawn + offsets::m_vOldOrigin);
	// Vec3 localEyeOffset = *reinterpret_cast<Vec3*>(localPlayerPawn + offsets::m_vecViewOffset);
	// Vec3 localEyePosition = localPos + localEyeOffset;

	uintptr_t p_lBoneMatrix = GetBoneMatrix(localPlayerPawn);
	if (!p_lBoneMatrix)
		return;

	Vec3 localEyePosition = *reinterpret_cast<Vec3*>(p_lBoneMatrix + (unsigned long long)BoneId::Head * 32);

	uintptr_t entityList = *reinterpret_cast<uintptr_t*>(client + offsets::dwEntityList);
	if (!entityList)
		return;

	ImVec2 mousePos = ImGui::GetMousePos();
	Vec2 vMousePos = { mousePos.x, mousePos.y };

	float closest_distance = -1;
	Vec3 enemyPos;

	int fovSize = cheat_values::aimbotSettings.silentAimFov;

	for (int i = 0; i < 32; i++)
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

		int eTeam = *reinterpret_cast<int*>(pCSPlayerPawnPtr + offsets::m_iTeamNum);
		if (eTeam == localTeam)
			continue;

		int eHealth = *reinterpret_cast<int*>(pCSPlayerPawnPtr + offsets::m_iHealth);
		if (eHealth <= 0)
			continue;

		// Vec3 ePos = *reinterpret_cast<Vec3*>(pCSPlayerPawnPtr + offsets::m_vOldOrigin);
		// Vec3 eEyeOffset = *reinterpret_cast<Vec3*>(pCSPlayerPawnPtr + offsets::m_vecViewOffset);
		// Vec3 eEyePosition = ePos + eEyeOffset;

		uintptr_t eBoneMatrix = GetBoneMatrix(pCSPlayerPawnPtr);
		if (!eBoneMatrix)
			continue;

		Vec3 eEyePosition = *reinterpret_cast<Vec3*>(eBoneMatrix + (int)BoneId::Head * 32);
		Vec2 screenEyePos;
		bool isOnScreen = eEyePosition.WorldToScreen(screenEyePos, ViewMatrix);

		float eDistance = (eEyePosition - localEyePosition).Magnitude();

		if (!isOnScreen)
			continue;

		if ((screenEyePos - vMousePos).Magnitude() > fovSize)
			continue;

		if (closest_distance < 0 || eDistance < closest_distance)
		{
			closest_distance = eDistance;
			enemyPos = eEyePosition;
		}
	}

	if (closest_distance == -1)
		return;

	Vec3 lookAngles = (enemyPos - localEyePosition).ToAngles();
	*reinterpret_cast<Vec3*>(client + offsets::dwViewAngles) = lookAngles;
}

int aimbot::GetAimbotFov()
{
	uintptr_t localPlayer = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayerPawn);
	if (!localPlayer)
		return -1;
}

int aimbot::GetCircleRadius()
{
	return -1;
}

void aimbot::DrawSilentAimFov(int fov)
{
	ImVec2 mousePos = ImGui::GetMousePos();

	ImGui::GetForegroundDrawList()->AddCircle(mousePos, (float)fov, ImColor(255, 255, 255, 255), 90);
}
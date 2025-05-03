#pragma once

#include "scanner.h"
#include "cs2backend.h"
#include "../visuals.h"
#include "../aimbot.h"
#include "../kiero/minhook/include/MinHook.h"

#include "../imgui/imgui.h"

#define G_CREATEHOOK_SUCCESS 0;

using CreateMoveFn = bool(__fastcall*)(CCSGOInput*, unsigned int, __int64);

CreateMoveFn oCreateMove = nullptr;
bool prevJumpFlag = false;

bool __fastcall hkCreateMove(CCSGOInput* thisptr, unsigned int a2, __int64 a3)
{
	printf("Thisptr: %p\n", thisptr);

	uintptr_t pCSPlayerPawn = *reinterpret_cast<uintptr_t*>(visuals::client + offsets::dwLocalPlayerPawn);
	if (!pCSPlayerPawn)
		return oCreateMove(thisptr, a2, a3);

	int32_t fFlags = *reinterpret_cast<int32_t*>(pCSPlayerPawn + offsets::m_fFlags);

	// Do stuff here
	CUserCmd* cmd = reinterpret_cast<CUserCmd*>(a3);
	if (cmd)
	{
		if (fFlags & FL_ONGROUND && cmd->buttons & IN_JUMP && cheat_values::enableBhop)
		{
			cmd->buttons &= ~IN_JUMP;
		}

		printf("CUserCmd* cmd = %p\n", cmd);
		printf("Buttons %d\n", (cmd->buttons & IN_JUMP) == 2);
	}

	return oCreateMove(thisptr, a2, a3);
}

MH_STATUS HookCreateMove()
{
	CreateMoveFn createMoveLoc = reinterpret_cast<CreateMoveFn>(PatternScan(L"client.dll", "48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 57"));
	printf("Found CreateMove at %p\n", createMoveLoc);

	MH_STATUS createStatus = MH_CreateHook((LPVOID)createMoveLoc, (LPVOID)hkCreateMove, reinterpret_cast<LPVOID*>(&oCreateMove));
	if (createStatus != MH_OK)
		return createStatus;

	MH_STATUS enableStatus = MH_EnableHook(MH_ALL_HOOKS);
	if (enableStatus != MH_OK)
		return enableStatus;

	return MH_OK;
}

MH_STATUS RegisterHooks()
{
	MH_STATUS initStatus = MH_Initialize();
	if (initStatus != MH_OK && initStatus != MH_ERROR_ALREADY_INITIALIZED)
		return initStatus;

	MH_STATUS createMoveStatus = HookCreateMove();
	if (createMoveStatus != MH_OK)
		return createMoveStatus;

	return MH_OK;
}
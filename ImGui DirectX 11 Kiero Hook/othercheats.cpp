#include "othercheats.h"

uintptr_t movement::client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
bool movement::bhopEnabled = false;

void movement::BunnyHop()
{
	if (!bhopEnabled)
		return;

	const uintptr_t localPawn = *reinterpret_cast<uintptr_t*>(client + offsets::dwLocalPlayerPawn);
	if (!localPawn)
		return;

	const uintptr_t health = *reinterpret_cast<uintptr_t*>(localPawn + offsets::m_iHealth);
	if (!health)
		return;

	int32_t fFlags = *reinterpret_cast<int32_t*>(localPawn + offsets::m_fFlags);

	if (GetAsyncKeyState(VK_SPACE) && fFlags & (1 << 0))
	{
		*reinterpret_cast<int*>(client + offsets::dwForceJump) = 65537;
		Sleep(1);
		*reinterpret_cast<int*>(client + offsets::dwForceJump) = 256;
	}
}

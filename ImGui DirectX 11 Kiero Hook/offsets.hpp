#pragma once
#include <thread>

namespace offsets
{
	constexpr std::ptrdiff_t dwEntityList = 0x19F6F00;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x184B0D0;
	constexpr std::ptrdiff_t dwViewMatrix = 0x1A5FED0;
	constexpr std::ptrdiff_t dwForceJump = 0x1884C40; // buttons.hpp cs2 dumper
	constexpr std::ptrdiff_t dwPlantedC4 = 0x19F9260;
	constexpr std::ptrdiff_t dwViewAngles = 0x1A6A2C0;

	constexpr std::ptrdiff_t m_pGameSceneNode = 0x328;
	constexpr std::ptrdiff_t m_vecAbsOrigin = 0xD0;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x824;

	constexpr std::ptrdiff_t m_modelState = 0x170;
	constexpr std::ptrdiff_t m_skeletonInstance = 0x50;
	constexpr std::ptrdiff_t m_CBodyComponent = 0x38;
	constexpr std::ptrdiff_t m_iHealth = 0x344;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1324;
	constexpr std::ptrdiff_t m_fFlags = 0x3EC;
	constexpr std::ptrdiff_t m_vecViewOffset = 0xCB0;
}

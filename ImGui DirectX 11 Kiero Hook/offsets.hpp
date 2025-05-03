#pragma once
#include <thread>

namespace offsets
{
	constexpr std::ptrdiff_t dwEntityList = 0x1A1F670;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1874040;
	constexpr std::ptrdiff_t dwViewMatrix = 0x1A89070;
	constexpr std::ptrdiff_t dwForceJump = 0x1884C40; // buttons.hpp cs2 dumper
	constexpr std::ptrdiff_t dwPlantedC4 = 0x1A22570;
	constexpr std::ptrdiff_t dwViewAngles = 0x1A93300;

	constexpr std::ptrdiff_t m_pGameSceneNode = 0x328;
	constexpr std::ptrdiff_t m_vecAbsOrigin = 0xD0;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x814;

	constexpr std::ptrdiff_t m_modelState = 0x170;
	constexpr std::ptrdiff_t m_skeletonInstance = 0x50;
	constexpr std::ptrdiff_t m_CBodyComponent = 0x38;
	constexpr std::ptrdiff_t m_iHealth = 0x344;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1324;
	constexpr std::ptrdiff_t m_fFlags = 0x3EC;
	constexpr std::ptrdiff_t m_vecViewOffset = 0xCB0;
}
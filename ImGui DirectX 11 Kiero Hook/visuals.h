#pragma once
#include <Windows.h>
#include <thread>
#include <vector>

#include "./ImGui/imgui.h"

enum class BoneId : int
{
	Head = 6,
	Neck = 5,
	Spine = 4,
	Spine1 = 2,
	LeftShoulder = 8,
	LeftArm = 9,
	LeftHand = 11,
	Waist = 0,
	RightShoulder = 13,
	RightArm = 14,
	RightHand = 16,
	LeftHip = 22,
	LeftKnee = 23,
	LeftFeet = 24,
	RightHip = 25,
	RightKnee = 26,
	RightFeet = 27
};

struct EspParameters
{
	bool ShowTeam;
	bool ShowEnemies;

	ImColor teamColor;
	ImColor enemyColor;
};

struct VisualSettings
{
	bool esp;
	bool bones;
	bool tracers;
	bool plantedBomb;

	EspParameters espParams;
	EspParameters tracerParams;
};

namespace visuals
{
	extern uintptr_t client;

	void DoVisuals(VisualSettings& settings);

	void PlayerESP(EspParameters& params);
	void PlayerTracers(EspParameters& params);
	void PlantedBombEsp(ImColor color);
	void PlayerBoneEsp(uintptr_t pCSPlayerPawn);
}
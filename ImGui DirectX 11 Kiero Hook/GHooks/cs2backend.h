#pragma once

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define PADDING(size) char TOKENPASTE2(padding_, __LINE__) [size]

#include "../math.h"

// Input codes
//#define IN_ATTACK        (1 << 0)   // Primary attack (left click)
//#define IN_JUMP          (1 << 1)   // Jump (spacebar)
//#define IN_DUCK          (1 << 2)   // Crouch (ctrl)
//#define IN_FORWARD       (1 << 3)   // Move forward (W)
//#define IN_BACK          (1 << 4)   // Move backward (S)
//#define IN_USE           (1 << 5)   // Use (E key)
//#define IN_CANCEL        (1 << 6)   // ??? (Unused mostly)
//#define IN_LEFT          (1 << 7)   // Move left (A)
//#define IN_RIGHT         (1 << 8)   // Move right (D)
//#define IN_MOVELEFT      (1 << 9)   // Move left (A)
//#define IN_MOVERIGHT     (1 << 10)  // Move right (D)
//#define IN_ATTACK2       (1 << 11)  // Secondary attack (right click)
//#define IN_RUN           (1 << 12)  // Sprint (not used in CS)
//#define IN_RELOAD        (1 << 13)  // Reload (R key)
//#define IN_ALT1          (1 << 14)  // Alternate action (not common)
//#define IN_ALT2          (1 << 15)  // Another alternate action
//#define IN_SCORE         (1 << 16)  // Shows scoreboard (TAB key)
//#define IN_SPEED         (1 << 17)  // Walk (SHIFT key)
//#define IN_WALK          (1 << 18)  // Same as IN_SPEED (SHIFT key)
//#define IN_ZOOM          (1 << 19)  // Zoom (AWP / Scout scope)
//#define IN_WEAPON1       (1 << 20)  // Switch to primary weapon
//#define IN_WEAPON2       (1 << 21)  // Switch to secondary weapon
//#define IN_BULLRUSH      (1 << 22)  // Used by NPCs in HL2 (not in CS)
//#define IN_GRENADE1      (1 << 23)  // Throw first grenade (bindable)
//#define IN_GRENADE2      (1 << 24)  // Throw second grenade (bindable)
//#define IN_LOOKSPIN      (1 << 25)  // Quick 180-degree spin (unused)

enum ECommandButtons : std::uint64_t
{
	IN_ATTACK = 1 << 0,
	IN_JUMP = 1 << 1,
	IN_DUCK = 1 << 2,
	IN_FORWARD = 1 << 3,
	IN_BACK = 1 << 4,
	IN_USE = 1 << 5,
	IN_LEFT = 1 << 7,
	IN_RIGHT = 1 << 8,
	IN_MOVELEFT = 1 << 9,
	IN_MOVERIGHT = 1 << 10,
	IN_SECOND_ATTACK = 1 << 11,
	IN_RELOAD = 1 << 13,
	IN_SPRINT = 1 << 16,
	IN_JOYAUTOSPRINT = 1 << 17,
	IN_SHOWSCORES = 1ULL << 33,
	IN_ZOOM = 1ULL << 34,
	IN_LOOKATWEAPON = 1ULL << 35
};

enum EntityFlags : int {
	FL_ONGROUND = 1 << 0,  // Player is on the ground
	FL_DUCKING = 1 << 1,  // Player is crouching (ducking)
	FL_WATERJUMP = 1 << 2,  // Player is jumping while in water
	FL_INWATER = 1 << 3,  // Player is underwater
	FL_FROZEN = 1 << 4,  // Player is frozen
	FL_ATCONTROLS = 1 << 5,  // Player is at the controls (interacting with a device, etc.)
	FL_CLIENT = 1 << 6,  // Entity is a client (player)
	FL_FAKECLIENT = 1 << 7,  // Entity is a fake client (non-real player for certain purposes)
	FL_GRAPHED = 1 << 8,  // Used in graphing, possibly for debugging
	FL_INACTIVE = 1 << 9,  // Entity is inactive
	FL_STATICPROP = 1 << 10, // Entity is a static prop (not moving)
	FL_DEAD = 1 << 11, // Player or entity is dead
	FL_DORMANT = 1 << 12, // Entity is dormant (not being simulated)
	FL_FIGHTER = 1 << 13, // Specific to certain entities or combatants
	FL_IMMUNE = 1 << 14, // Entity is immune to damage (perhaps for invincibility)
	FL_BOTS = 1 << 15  // Entity is a bot (AI controlled)
};

//class CUserCmd
//{
//public:
//	PADDING(40);
//	int64_t buttons;
//	PADDING(344);
//	Vec3 viewAngles;
//};

class CInButtonState
{

};

class CMsgQAngle
{
public:
	PADDING(0x18);
	Vec3 viewAngles;
};

class CBaseUserCmd
{
public:
	PADDING(0x40);
	CMsgQAngle* m_pQAngles;
};

class CUserCmd
{
public:
	void* vtable;
	int64_t m_iCmdNum;
	PADDING(48);
	CBaseUserCmd* m_pBaseCmd;
	PADDING(16);
	CInButtonState* m_pButtonState;
	int32_t buttons;
	int32_t changedButtons;
	int32_t previousButtons;
};

class CCSGOInput
{
public:
	void* v_table;
	PADDING(584); // 0x248
	CUserCmd m_commands[150];
	PADDING(1);
	bool in_third_person;
	PADDING(34); // 0x22
	int32_t m_sequence_number;

	auto get_user_cmd()
	{
		return &m_commands[m_sequence_number % 150];
	}
};
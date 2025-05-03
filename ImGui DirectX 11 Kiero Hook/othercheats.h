#pragma once
#include <Windows.h>
#include <stdint.h>
#include "offsets.hpp"

namespace movement
{
	extern uintptr_t client;
	extern bool bhopEnabled;

	void BunnyHop();
}
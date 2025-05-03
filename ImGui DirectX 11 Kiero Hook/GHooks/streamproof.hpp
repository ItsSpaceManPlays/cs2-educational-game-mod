#pragma once

#include <Windows.h>
#include <cstddef>
#include <d3d11.h>

#include "scanner.h"

template <typename T>
T SwapPointer(T* target, T newValue)
{
	if (!target) return nullptr;

	DWORD oldProtect;
	VirtualProtect(target, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);

	T oldValue = *target;
	*target = newValue;

	VirtualProtect(target, sizeof(T), oldProtect, &oldProtect);
	return oldValue;
}

constexpr std::ptrdiff_t obsPresentOffset = 0x40EC0;
constexpr std::ptrdiff_t obsPresentOffset1 = 0x40EC8;
using pPresent = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT);
using pPresent1 = HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT, void*);

static pPresent oObsPresent = nullptr;

bool ReplaceObsHook(LPVOID targetMethod)
{
	uintptr_t obsBase = (uintptr_t)GetModuleHandleA("graphics-hook64.dll");
	if (!obsBase)
		return false;

	uintptr_t* regHookLoc = reinterpret_cast<uintptr_t*>(
		PatternScan(
			L"graphics-hook64.dll",
			"41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 45 33 F6"
		)
	);

	printf("Found register obs hook at %p\n", regHookLoc);

	pPresent* renderObsPresent = reinterpret_cast<pPresent*>(obsBase + obsPresentOffset);
	if (!renderObsPresent || renderObsPresent == nullptr) return false;
	printf("Found function at %p\n", renderObsPresent);

	oObsPresent = SwapPointer<pPresent>(renderObsPresent, (pPresent)targetMethod);

	return true;
}

bool UndoObsHook()
{
	uintptr_t obsBase = (uintptr_t)GetModuleHandleA("graphics-hook32.dll");
	if (!obsBase)
		return false;

	pPresent* renderObsPresent = reinterpret_cast<pPresent*>(obsBase + obsPresentOffset);
	if (!renderObsPresent || renderObsPresent == nullptr) return false;

	SwapPointer<pPresent>(renderObsPresent, oObsPresent);
	oObsPresent = nullptr;

	return true;
}

// next challenge is discords shitty hook
#include "includes.h"

#include "visuals.h"
#include "othercheats.h"
#include "guimenu.h"
#include "aimbot.h"
#include "GHooks/g_hooks.hpp"
#include "GHooks/streamproof.hpp"

//#define OVERLAY_HOOK
//#define SHOW_CONSOLE

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	gui::SetupGuiStyle(ImGui::GetStyle(), io);
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureKeyboard || io.WantCaptureMouse)
	{
		return TRUE;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	MEMORY_BASIC_INFORMATION mbInfo;
	VirtualQuery(_ReturnAddress(), &mbInfo, sizeof(MEMORY_BASIC_INFORMATION));

	char mod[MAX_PATH];
	GetModuleFileNameA(
		reinterpret_cast<HMODULE>(mbInfo.AllocationBase),
		mod,
		MAX_PATH
	);

	printf("Called by %s\n", mod);

	if ( (strstr(mod, "DiscordHook64.dll") || strstr(mod, "graphics-hook64.dll")) && cheat_values::obsHide)
		return oPresent(
			pSwapChain,
			SyncInterval,
			Flags
		);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	gui::Render();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
#ifdef SHOW_CONSOLE
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONIN$", "r", stdin);
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif

	bool init_hook = false;
	do
	{
#ifdef OVERLAY_HOOK
		MH_Initialize();

		printf("Scanning for overlay present\n");
		Present overlayPresent = reinterpret_cast<uintptr_t>(PatternScan(L"gameoverlayrenderer64.dll", "68 ? ? ? ? 68 ? ? ? ? FF 76 44") + 0x1);
		printf("Found overlay present at %p\n", overlayPresent);

		MH_CreateHook((LPVOID)overlayPresent, (LPVOID)hkPresent, (LPVOID*)&oPresent);
		MH_EnableHook((LPVOID)overlayPresent);
		init_hook = true;
#else
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
#endif
	} while (!init_hook);

	MH_STATUS registerStatus = RegisterHooks();
	if (registerStatus != MH_OK)
		MessageBoxA(0, "Error", MH_StatusToString(registerStatus), MB_ICONERROR | MB_OK);

	while (!oPresent || oPresent == nullptr)
		Sleep(2);

	if (!ReplaceObsHook(hkPresent))
		MessageBoxA(0, "Error", "Failed obs hook", MB_ICONERROR | MB_OK);
	else
		MessageBoxA(0, "Success", "did obs hook", MB_ICONINFORMATION | MB_OK);

	return TRUE;
}

void CheatThread(LPVOID lpReserved)
{
	while (true)
	{
		if (cheat_values::aimbotSettings.aimbotEnabled && GetAsyncKeyState(VK_XBUTTON1))
		{
			aimbot::Aimbot(cheat_values::aimbotSettings);
		}
		Sleep(2);
	}
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, (PTHREAD_START_ROUTINE)CheatThread, NULL, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
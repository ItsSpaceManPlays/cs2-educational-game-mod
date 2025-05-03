#include "scanner.h"

auto patternToByte = [](const char* pattern)
	{
		std::vector<int> bytes = std::vector<int>();
		char* start = const_cast<char*>(pattern);
		char* end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else
			{
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}

		return bytes;
	};

uint8_t* PatternScan(const wchar_t* szModName, const char* cSignature)
{
	const HMODULE hModule = GetModuleHandleW(szModName);

	if (!hModule)
		return 0;

	PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
	PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
		reinterpret_cast<uint8_t*>(hModule) + pDosHeader->e_lfanew
		);

	DWORD dwImageSize = pNtHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = patternToByte(cSignature);
	auto scanBytes = reinterpret_cast<uint8_t*>(hModule);

	auto patternBytesSize = patternBytes.size();
	auto patternBytesData = patternBytes.data();

	for (auto i = 0ul; i < dwImageSize - patternBytesSize; ++i)
	{
		bool found = true;

		for (auto j = 0ul; j < patternBytesSize; ++j)
		{
			if (scanBytes[i + j] != patternBytesData[j] && patternBytesData[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
			return &scanBytes[i];
	}
}

#include "pch.h"

// Defines

#define DEBUG(...) fprintf(stdout, __VA_ARGS__) 
#define WINDOW FindWindowA(0, "Europa Universalis IV")
#define BASE (uintptr_t)GetModuleHandleA(NULL)

typedef uintptr_t* (__fastcall* tFetchProvinceID)(void* a1, uintptr_t provinceID);

typedef uintptr_t(__fastcall* tMoveArmy)(uintptr_t* armyObject, uintptr_t* provinceObject, int appendToPath);

// Created with ReClass.NET 1.2 by KN4CK3R

class CWorld
{
public:
	class CMap* pMap; //0x0000
}; //Size: 0x0008

class CMap
{
public:
	char pad_0000[7648]; //0x0000
	class CProvince* pProvinces; //0x1DE0
}; //Size: 0x1DE8

class CProvince
{
public:
	char pad_0000[6320]; //0x0000
	class CSelection* pSelection; //0x18B0
}; //Size: 0x18B8

class CSelection
{
public:
	char pad_0000[8]; //0x0000
	class CNotSure1* pNotSure1; //0x0008
	char pad_0010[24]; //0x0010
	void* currentArmy; //0x0028
}; //Size: 0x0030

class CNotSure1
{
public:
	char pad_0000[456]; //0x0000
	class CNotSure2* pNotSure2; //0x01C8
}; //Size: 0x01D0

class N000008D1
{
public:
	char pad_0000[8]; //0x0000
}; //Size: 0x0008

class CNotSure2
{
public:
	char pad_0000[864]; //0x0000
	class CNotSure3* pNotSure3; //0x0360
}; //Size: 0x0368

class CNotSure3
{
public:
	char pad_0000[48]; //0x0000
	void* pAdjacancies; //0x0030
}; //Size: 0x0038


WNDPROC oWndProc = nullptr;
LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RIGHT:
			DEBUG("[INFO] You pressed a key bruv.\n");
			break;
		case VK_LEFT:
			DEBUG("[INFO] Hopefully moving selected army to provinceID 1753.\n");

			// IMPORTANT BIT
			CWorld* pWorld = (CWorld*)(BASE + 0x223D380);

			DEBUG("[INFO] Fetching province object for province with ID: 1753\n");
			uintptr_t *pProvince = tFetchProvinceID(BASE + 0xed0d0)(pWorld->pMap->pProvinces->pSelection->pNotSure1->pNotSure2->pNotSure3->pAdjacancies, 1753);
			DEBUG("[INFO] Fetched province object = %p\n", (void*)pProvince);

			DEBUG("[INFO] Moving currently selected army\n");
			
			uintptr_t *currentArmy = (uintptr_t*)((uintptr_t) pWorld->pMap->pProvinces->pSelection + 0x28);
			DEBUG("[INFO] Current army selected: %p\n", (void*) currentArmy);
			tMoveArmy(BASE + 0xe7f4bb)(currentArmy, pProvince, 0);

			DEBUG("[INFO] Army en route\n");

			break;
		}
	}

	return oWndProc(hwnd, uMsg, wParam, lParam);
}

void Initialize(HMODULE hModule) {
	if (!AttachConsole(GetCurrentProcessId()))
		if (!AllocConsole())
			MessageBoxA(GetConsoleWindow(), "Failed to create debug console.", NULL, MB_OKCANCEL | MB_ICONEXCLAMATION);

	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_DISABLED);
	SetConsoleTitleA("Console Window");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	DEBUG("[INFO] Console initialized.\n");

	oWndProc = (WNDPROC)SetWindowLongPtr(WINDOW, GWLP_WNDPROC, (LONG_PTR)hWndProc);
	DEBUG("[INFO] WndProc hooked.\n");



	// When we press F2 the dll unloads
	while (true)
	{
		Sleep(100);
		if (GetAsyncKeyState(VK_F2))
		{
			Sleep(500);
			
			SetWindowLongPtr(WINDOW,
				GWLP_WNDPROC, (LONG_PTR)oWndProc);
			DEBUG("[INFO] Input hook removed");

			DEBUG("Destroying console...Bye\n");
			fclose(reinterpret_cast<FILE*>(stdout));
			FreeConsole();

			FreeLibraryAndExitThread(hModule, 0);
		}
	}
}

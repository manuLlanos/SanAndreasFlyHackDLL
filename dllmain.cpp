#include "pch.h"
#include "vector3.h"
#include <cmath>
#include <array>
#include <Windows.h>


float clamp(float v, float min, float max)
{
    const float t = v < min ? min : v;
    return t > max ? max : t;
}


//INJECTED THREAD
DWORD WINAPI MainThread(LPVOID param)
{
	uintptr_t modBase = (uintptr_t)GetModuleHandle(NULL);

    float* CAMERA_PITCH_PTR = (float*)0x00B6F248;
    float* CAMERA_ROLL_PTR = (float*)0x00B6F258;
	float camera_roll = 0;
	float camera_pitch = 0;
	float* HEALTH_PTR = (float*)(*(DWORD*)(modBase + 0x0076F980) + 0x540);

	float* SPEED_X_PTR = (float*)(*(DWORD*)(modBase + 0x0076F3B8) + 0x44);
	float* SPEED_Y_PTR = (float*)(*(DWORD*)(modBase + 0x0076F3B8) + 0x48);
	float* SPEED_Z_PTR = (float*)(*(DWORD*)(modBase + 0x0076F3B8) + 0x4C);
	
	const float PI = 3.14159265359;
	const float JUMP_SPEED = 1.0;
	Vector3 speed;
	float modifier = 1.0;

	bool fly_enabled = false;

	while (true) {

		//write health
		*HEALTH_PTR = 200.0;


		if (GetAsyncKeyState('B') & 0x8000) {
			fly_enabled = !fly_enabled;
		}

		//quit
		if (GetKeyState(VK_F4) && 0x8000) {
			break;
		}


		if (!fly_enabled) {
			continue;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			modifier += 0.1;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			modifier -= 0.1;
		}

		modifier = clamp(modifier, 0.1, 5.0);

		speed.x = 0, speed.y = 0, speed.z = 0;
		camera_roll = *CAMERA_ROLL_PTR;
		camera_pitch = *CAMERA_PITCH_PTR;

		//higher order bit is 1 when pressed
		if (GetAsyncKeyState(VK_SHIFT) && 0x8000) {
			speed.z += 2.0 * JUMP_SPEED;
		}
		if (GetAsyncKeyState(VK_LCONTROL) && 0x8000) {
			speed.z -= JUMP_SPEED;
		}
		//go towards camera target
		if (GetAsyncKeyState('W') && 0x8000) {
			speed = speed + Vector3(-cos(camera_roll), -sin(camera_roll), 0.1 + sin(camera_pitch));
		}
		//back
		if (GetAsyncKeyState('S') && 0x8000) {
			speed = speed + Vector3(cos(camera_roll), sin(camera_roll), -0.1 - sin(camera_pitch));
		}
		//left
		if (GetAsyncKeyState('A') && 0x8000) {
			speed = speed + Vector3(cos(camera_roll - PI / 2), sin(camera_roll - PI / 2), 0);
		}
		//right
		if (GetAsyncKeyState('D') && 0x8000) {
			speed = speed + Vector3(cos(camera_roll + PI / 2), sin(camera_roll + PI / 2), 0);
		}

		//write speed
		*SPEED_X_PTR = speed.x * modifier;
		*SPEED_Y_PTR = speed.y * modifier;
		*SPEED_Z_PTR = speed.z * modifier;
	}

    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


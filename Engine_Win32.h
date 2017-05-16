#pragma once

#define DLL_API __declspec(dllimport)
#pragma comment(lib, "Engine_Win32.lib")

#include <Windows.h>
#include "variable.h"
#include "Object.h"

DLL_API void Start(HINSTANCE hInst, LPWSTR name, int& CmdShow, void (*cf)(), UINT x = 1280, UINT y = 720);

DLL_API void CreateLayer(UINT num);
DLL_API void DestroyLayer();

DLL_API void RegistObject(void* target, UINT layer);
DLL_API void RemoveObject(void* target, UINT layer);

DLL_API void* RegistTexture(IMAGE& i, void*& texture);
DLL_API bool ScreenShot(char* name);

DLL_API bool RegistSound(SOUND s, UINT loop = 0);
DLL_API bool AllRemoveSound();	//not exist

DLL_API void Move2DCamera(FLOAT3 v);

DLL_API UINT GetTick();
DLL_API KEYBOARD* GetKeyboard();
DLL_API MOUSE* GetMouse();
DLL_API FLOAT2 ClickOrtho();

DLL_API bool CheckCollision2D(COL2D& base, COL2D& target);
DLL_API bool CheckCollision2D(FLOAT2 mouse_pos, COL2D& target);

DLL_API bool LoadBMP(char* name, IMAGE& g);
DLL_API bool SaveBMP(char* name, IMAGE& g);
DLL_API bool LoadPNG(char* name, IMAGE& g);
DLL_API bool LoadWAV(char* name, SOUND& s);
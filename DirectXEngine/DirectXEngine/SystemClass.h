#pragma once


#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "FpsClass.h"
#include "CpuClass.h"
#include "TimerClass.h"
#include "PositionClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_input;
	GraphicsClass* m_graphics;
	SoundClass* m_sound;
	FpsClass* m_fps;
	CpuClass* m_cpu;
	TimerClass* m_timer;
	PositionClass* m_position;

};

//Function prototypes
static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//Globals
static SystemClass* ApplicationHandle = 0;

#endif


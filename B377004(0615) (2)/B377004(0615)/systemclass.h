#pragma once
#ifndef _SYSTEMCLASS_H_ 
#define _SYSTEMCLASS_H_
#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include "inputclass.h"
#include "graphicsclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&); ~SystemClass();

	bool Initialize();

	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	int change;
private:
	bool Frame();
	void InitializeWindows(int&, int&);

	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;
};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;
#endif

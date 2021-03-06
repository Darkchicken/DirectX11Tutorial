#pragma once

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

//Pre-processing directives
#define DIRECTINPUT_VERSION 0x0800

//Linking
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//Includes
#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsQPressed();
	bool IsEPressed();
	int IsForwardBackward();
	int IsLeftRight();
	void GetMouseLocation(int&, int&);
	
private:
	bool readKeyboard();
	bool readMouse();
	void processInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif


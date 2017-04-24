//DInput.h: 

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class CDInputer
{
public:
	CDInputer();
	~CDInputer();

public:
	bool CreateDevice(HINSTANCE hInstance, HWND hWnd);
	bool GetKeyInput();
	bool GetMouseInput();
	bool IsKeyDown(char key) { return (m_keyBuffer[key] & 0x80) != 0; }
	bool IsMouseDown(char button) { return (m_MouseState.rgbButtons[button] & 0x80) != 0; }
	LONG MouseX() { return m_MouseState.lX; }
	LONG MouseY() { return m_MouseState.lY; }
	LONG MouseZ() { return m_MouseState.lZ; }

protected:

private:
	IDirectInputDevice8* m_pMouseDev;
	IDirectInputDevice8* m_pKeyDev;

	char m_keyBuffer[256];
	DIMOUSESTATE m_MouseState;
};
//DInput.cpp:

#include "stdafx.h"
#include "InputDev.h"

CDInputer::CDInputer()
: m_pKeyDev(NULL)
, m_pMouseDev(NULL)
{

}

CDInputer::~CDInputer()
{
	if (m_pKeyDev)
	{
		m_pKeyDev->Unacquire();
		m_pKeyDev->Release();
		m_pKeyDev = NULL;
	}

	if (m_pMouseDev)
	{
		m_pMouseDev->Unacquire();
		m_pMouseDev->Release();
		m_pMouseDev = NULL;
	}
}

bool CDInputer::CreateDevice(HINSTANCE hInstance, HWND hWnd)
{
	// 创建DInput接口
	CComPtr<IDirectInput8> pDInput;
	if ( FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL)) )
		return false;

	// 创建键盘设备
	if ( FAILED(pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyDev, NULL)) )
		return false;

	m_pKeyDev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyDev->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyDev->Acquire();
	m_pKeyDev->Poll();

	// 创建鼠标设备
	if ( FAILED(pDInput->CreateDevice(GUID_SysMouse, &m_pMouseDev, NULL)) )
		return false;

	m_pMouseDev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pMouseDev->SetDataFormat(&c_dfDIMouse);
	m_pMouseDev->Acquire();
	m_pMouseDev->Poll();

	return true;
}

bool CDInputer::GetKeyInput()
{
	HRESULT hr;
	while (1)
	{
		hr = m_pKeyDev->GetDeviceState(sizeof(m_keyBuffer), (void**)&m_keyBuffer);
		if (hr == S_OK)
			break;
		else if (hr == DIERR_INPUTLOST || DIERR_NOTACQUIRED)
		    m_pKeyDev->Acquire();
		else
			return false;
	}

	return true;
}

bool CDInputer::GetMouseInput()
{
	HRESULT hr;
	while (1)
	{
		hr = m_pMouseDev->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState);
		if (hr == DI_OK)
			break;
		else if (hr == DIERR_INPUTLOST || DIERR_NOTACQUIRED)
			m_pMouseDev->Acquire();
		else
			return false;
	}

	return true;
}
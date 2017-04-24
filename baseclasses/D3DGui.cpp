#include "stdafx.h"
#include "D3DGui.h"

CGUIControl::CGUIControl(IDirect3DDevice9* pD3DDev, EControlType type)
: m_type(type)
, m_pD3DDev(pD3DDev)
, m_pFont(NULL)
, m_pVB(NULL)
, m_pTexNormal(NULL)
, m_pTexFocused(NULL)
, m_pTexPushed(NULL)
, m_pTexDisabled(NULL)
{
	m_status = ControlStatus_NORMAL;
	memset(m_szText, 0, sizeof(m_szText));
}

CGUIControl::~CGUIControl()
{
	SAFE_RELEASE(m_pFont)
	SAFE_RELEASE(m_pVB)
	SAFE_RELEASE(m_pTexNormal)
	SAFE_RELEASE(m_pTexFocused)
	SAFE_RELEASE(m_pTexPushed)
	SAFE_RELEASE(m_pTexDisabled)
}

void CGUIControl::SetPos(RECT& rc)
{
	memcpy(&m_rcPos, &rc, sizeof(RECT));

	VERTEX_RDT vertice[] = 
	{		
		{rc.left, rc.bottom,	0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f},
		{rc.left, rc.top,		0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f},
		{rc.right, rc.bottom,	0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f},
		{rc.right, rc.top,		0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f},
	};

	m_pD3DDev->CreateVertexBuffer(sizeof(vertice), 0, D3DFVF_VERTEX_RDT, D3DPOOL_MANAGED, &m_pVB, NULL);
	void* pData = NULL;
	m_pVB->Lock(0, sizeof(vertice), (void**)&pData, 0);
	memcpy(pData, vertice, sizeof(vertice));
	m_pVB->Unlock();
}

void CGUIControl::SetText(const char* szText, int nHeight, const char* fontname)
{
	strcpy(m_szText, szText);

	D3DXCreateFontA(m_pD3DDev, nHeight, 0, 0, 1000, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontname, &m_pFont);
}

bool CGUIControl::SetTexture(const char* szNoraml, const char* szFocused, const char* szPushed, const char* szDisabled)
{
	if (szNoraml)
	{
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, szNoraml, &m_pTexNormal)))
			return false;
	}

	if (szFocused)
	{
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, szFocused, &m_pTexFocused)))
			return false;
	}

	if (szPushed)
	{
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, szPushed, &m_pTexPushed)))
			return false;
	}

	if (szDisabled)
	{
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, szDisabled, &m_pTexDisabled)))
			return false;
	}

	return true;
}

void CGUIControl::DrawControl()
{
	// »­±³¾°
	IDirect3DTexture9* m_pTex = NULL;
	switch(m_status)
	{
	case ControlStatus_NORMAL:
		if (m_pTexNormal)
			m_pTex = m_pTexNormal;
		break;
	case ControlStatus_FOCUSED:
		if (m_pTexFocused)
			m_pTex = m_pTexFocused;
		break;
	case ControlStatus_PUSHED:
		if (m_pTexPushed)
			m_pTex = m_pTexPushed;
		break;
	case ControlStatus_DISABLED:
		if (m_pTexDisabled)
			m_pTex = m_pTexDisabled;
		break;
	default:
		break;
	}
	if (m_pTex)
	{
		m_pD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_RDT));
		m_pD3DDev->SetFVF(D3DFVF_VERTEX_RDT);
		m_pD3DDev->SetTexture(0, m_pTex);
		m_pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// Ð´ÎÄ×Ö
	if (strlen(m_szText) != 0)
	{
		m_pFont->DrawTextA(NULL, m_szText, -1, &m_rcPos, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 0));
	}
}

//////////////////////////////////////////////////////////////////////////////
CD3DGui::CD3DGui(IDirect3DDevice9* pD3DDev)
: m_pD3DDev(pD3DDev)
, m_pCtrlBG(NULL)
{

}

CD3DGui::~CD3DGui()
{
	SAFE_DELETE(m_pCtrlBG)
	for (int i = 0; i < m_arrCtrls.GetSize(); i++)
		SAFE_DELETE(m_arrCtrls[i])
}

bool CD3DGui::AddImage(RECT& rc, const char* szNoraml)
{
	m_pCtrlBG = new CGUIControl(m_pD3DDev, ControlType_IMAGE);
	m_pCtrlBG->SetPos(rc);
	return m_pCtrlBG->SetTexture(szNoraml);
}

bool CD3DGui::AddStatic(RECT& rc, const char* szText, int nHeight, const char* szNoraml)
{
	CGUIControl* pCtrl = new CGUIControl(m_pD3DDev, ControlType_STATIC);
	pCtrl->SetPos(rc);
	pCtrl->SetText(szText, nHeight);
	if (!pCtrl->SetTexture(szNoraml))
		return false;
	m_arrCtrls.Add(pCtrl);

	return true;
}

bool CD3DGui::AddButton(RECT& rc, const char* szNoraml, const char* szFocused, const char* szPushed, const char* szDisabled, 
						const char* szText, int nHeight)
{
	CGUIControl* pCtrl = new CGUIControl(m_pD3DDev, ControlType_BUTTON);
	pCtrl->SetPos(rc);
	if (!pCtrl->SetTexture(szNoraml, szFocused, szPushed, szDisabled))
		return false;
	if (szText)
	{
		pCtrl->SetText(szText);
	}
	m_arrCtrls.Add(pCtrl);

	return true;
}

void CD3DGui::DrawGui()
{
	m_pCtrlBG->DrawControl();
	for (int i = 0; i < m_arrCtrls.GetSize(); i++)
	{
		m_arrCtrls[i]->DrawControl();
	}
}
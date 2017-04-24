#include "stdafx.h"
#include "D3DGui.h"

CGUIControl::CGUIControl(IDirect3DDevice9* pD3DDev, EControlType type, const char* szName)
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
	if (szName)
	{
		strcpy(m_szName, szName);
	}
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

	D3DXCreateFontA(m_pD3DDev, nHeight, 0, 0, 1000, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontname, &m_pFont);
}

bool CGUIControl::SetTexture(const char* szNormal, const char* szFocused, const char* szPushed, const char* szDisabled)
{
	if (szNormal)
	{
		CStdString strNormal = CGlobalManager::GetManager()->GetResPath() + szNormal;
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, strNormal.GetData(), &m_pTexNormal)))
			return false;
	}

	if (szFocused)
	{
		CStdString strFocused = CGlobalManager::GetManager()->GetResPath() + szFocused;
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, strFocused.GetData(), &m_pTexFocused)))
			return false;
	}

	if (szPushed)
	{
		CStdString strPushed = CGlobalManager::GetManager()->GetResPath() + szPushed;
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, strPushed.GetData(), &m_pTexPushed)))
			return false;
	}

	if (szDisabled)
	{
		CStdString strDisabled = CGlobalManager::GetManager()->GetResPath() + szDisabled;
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, strDisabled.GetData(), &m_pTexDisabled)))
			return false;
	}

	return true;
}

void CGUIControl::DrawControl()
{
	// »­±³¾°
	IDirect3DTexture9* m_pTex = NULL;
	if (m_pTexNormal)
		m_pTex = m_pTexNormal;
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
		m_pD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);  
		m_pD3DDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );  
		m_pD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		m_pD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_RDT));
		m_pD3DDev->SetFVF(D3DFVF_VERTEX_RDT);
		m_pD3DDev->SetTexture(0, m_pTex);
		m_pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		m_pD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	}

	// Ð´ÎÄ×Ö
	if (strlen(m_szText) != 0)
	{
		m_pFont->DrawTextA(NULL, m_szText, -1, &m_rcPos, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(47,79,79));
	}
}

//////////////////////////////////////////////////////////////////////////////
CD3DGui::CD3DGui(IDirect3DDevice9* pD3DDev, PNOTIFY pfn)
: m_pD3DDev(pD3DDev)
, m_pfn(pfn)
{

}

CD3DGui::~CD3DGui()
{
	for (int i = 0; i < m_arrCtrls.GetSize(); i++)
		SAFE_DELETE(m_arrCtrls[i])
}

bool CD3DGui::AddImage(RECT& rc, const char* szNoraml)
{
	CGUIControl* pCtrl = new CGUIControl(m_pD3DDev, ControlType_IMAGE);
	pCtrl->SetPos(rc);
	if (!pCtrl->SetTexture(szNoraml))
		return false;
	m_arrCtrls.Add(pCtrl);

	return true;
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

bool CD3DGui::AddButton(const char* szName, RECT& rc, const char* szNoraml, const char* szFocused, const char* szPushed, const char* szDisabled, 
						const char* szText, int nHeight)
{
	CGUIControl* pCtrl = new CGUIControl(m_pD3DDev, ControlType_BUTTON, szName);
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
	for (int i = 0; i < m_arrCtrls.GetSize(); i++)
	{
		m_arrCtrls[i]->DrawControl();
	}
}

LRESULT CD3DGui::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			for (int i = 0; i < m_arrCtrls.GetSize(); i++)
			{
				if (m_arrCtrls[i]->m_type == ControlType_BUTTON)
				{
					if (::PtInRect(&m_arrCtrls[i]->m_rcPos, pt))
					{
						m_arrCtrls[i]->SetStatus(ControlStatus_PUSHED);
						if (m_pfn)
						{
							m_pfn(m_arrCtrls[i]->m_szName, EventType_BLICK);
						}
						break;
					}
				}
			}
		}
		break;
	default:
		bHandled = FALSE;
		break;
	}

	return 0;
}
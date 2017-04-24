#ifndef __D3D_GUI_H__
#define __D3D_GUI_H__

enum EControlType
{
	ControlType_UNKNOWN = 0,
	ControlType_STATIC = 1,
	ControlType_BUTTON = 2,
	ControlType_IMAGE  = 3,
};

enum EControlStatus
{
	ControlStatus_NORMAL = 1,
	ControlStatus_FOCUSED = 2,
	ControlStatus_PUSHED = 3,
	ControlStatus_DISABLED = 4,
};

class CGUIControl
{
public:
	CGUIControl(IDirect3DDevice9* pD3DDev, EControlType type);
	~CGUIControl();

public:
	void SetStatus(EControlStatus status) {m_status = status;}
	void SetPos(RECT& rc);
	void SetText(const char* szText, int nHeight = 32, const char* fontname = "ËÎÌו");
	bool SetTexture(const char* szNoraml, const char* szFocused = NULL, const char* szPushed = NULL, const char* szDisabled = NULL);

	void DrawControl();

public:
	IDirect3DDevice9* m_pD3DDev;

	int m_nID;
	EControlType m_type;
	EControlStatus m_status;

	RECT m_rcPos;
	IDirect3DVertexBuffer9* m_pVB;

	char m_szText[128];
	ID3DXFont* m_pFont;
	
	IDirect3DTexture9* m_pTexNormal;
	IDirect3DTexture9* m_pTexFocused;
	IDirect3DTexture9* m_pTexPushed;
	IDirect3DTexture9* m_pTexDisabled;
};

class CD3DGui
{
public:
	CD3DGui(IDirect3DDevice9* pD3DDev);
	~CD3DGui();

public:
	bool AddImage(RECT& rc, const char* szNoraml);

	bool AddStatic(RECT& rc, const char* szText, int nHeight = 32, const char* szNoraml = NULL);
	bool AddButton(RECT& rc, const char* szNoraml, const char* szFocused = NULL, const char* szPushed = NULL, const char* szDisabled = NULL, 
		const char* szText = NULL, int nHeight = 32);

	void DrawGui();

private:
	IDirect3DDevice9* m_pD3DDev;
	CGUIControl* m_pCtrlBG;
	CGrowableArray<CGUIControl*> m_arrCtrls;
};

//void HandleGUIEvent(CD3DGui* gui, MouseStatus mouse, void (*pfun)(int id, MouseStatus));

#endif // __D3D_GUI_H__
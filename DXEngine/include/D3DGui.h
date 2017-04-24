#ifndef __D3D_GUI_H__
#define __D3D_GUI_H__

enum EControlType
{
	ControlType_UNKNOWN = 0,
	ControlType_IMAGE  = 1,
	ControlType_STATIC = 2,
	ControlType_BUTTON = 3,
};

enum EControlStatus
{
	ControlStatus_NORMAL = 1,
	ControlStatus_FOCUSED = 2,
	ControlStatus_PUSHED = 3,
	ControlStatus_DISABLED = 4,
};

enum EEventType
{
	EventType_BLICK = 1,
	EventType_DBBLICK = 1,
};

class CGUIControl
{
public:
	CGUIControl(IDirect3DDevice9* pD3DDev, EControlType type, const char* szName = NULL);
	~CGUIControl();

public:
	void SetStatus(EControlStatus status) {m_status = status;}
	void SetPos(RECT& rc);
	void SetText(const char* szText, int nHeight = 32, const char* fontname = "¡• È");
	bool SetTexture(const char* szNoraml, const char* szFocused = NULL, const char* szPushed = NULL, const char* szDisabled = NULL);

	void DrawControl();

public:
	IDirect3DDevice9* m_pD3DDev;

	char m_szName[128];
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

typedef	LRESULT (*PNOTIFY)(const char* szName, EEventType type); 
class DXENGINE_API CD3DGui
{
public:
	CD3DGui(IDirect3DDevice9* pD3DDev, PNOTIFY pfn);
	~CD3DGui();

public:
	bool AddImage(RECT& rc, const char* szNoraml);
	bool AddStatic(RECT& rc, const char* szText, int nHeight = 32, const char* szNoraml = NULL);
	bool AddButton(const char* szName, RECT& rc, const char* szNoraml, const char* szFocused = NULL, const char* szPushed = NULL, const char* szDisabled = NULL, 
		const char* szText = NULL, int nHeight = 32);

	void DrawGui();

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	IDirect3DDevice9* m_pD3DDev;
	PNOTIFY m_pfn;
	CGrowableArray<CGUIControl*> m_arrCtrls;
};

//void HandleGUIEvent(CD3DGui* gui, MouseStatus mouse, void (*pfun)(int id, MouseStatus));

#endif // __D3D_GUI_H__
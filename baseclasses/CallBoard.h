#ifndef __CALL_BOARD_H__
#define __CALL_BOARD_H__

class CCallBoard
{
public:
	CCallBoard(IDirect3DDevice9* pD3DDev);
	~CCallBoard();

public:
	bool InitBoard(float fX, float fY, const char* pTex);
	void SetWorld(D3DXMATRIX& matWorld);
	void UpdateBoard(D3DXMATRIX& matView);
	void DrawBoard();

protected:
private:
	IDirect3DDevice9* m_pD3DDev;
	D3DXMATRIX m_matWorldOrigin;
	D3DXMATRIX m_matWorldTrans;

	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DTexture9* m_pTex;
};

#endif // __CALL_BOARD_H__
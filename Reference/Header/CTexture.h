#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture();
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();
public:
	HRESULT Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt);
	HRESULT Ready_Texture(const _tchar* pPath);
	//어떤 식으로 텍스쳐를 설정하는 걸까? 
	void Set_Texture(const _uint iIndex = 0);
private:
	vector<IDirect3DBaseTexture9*> m_vecTexture;
public: //texture를 생성할 때 
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		TEXTUREID eID,
		const _tchar* pPath,
		const _uint& iCnt = 1); //기본 텍스쳐 수 1장
	//static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev,
	//	const _tchar* pPath);
	virtual CComponent* Clone();
private:
	LPDIRECT3DTEXTURE9 m_pTexture;
private:
	virtual void Free();
};

END
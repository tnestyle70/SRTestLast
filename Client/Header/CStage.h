#pragma once
#include "CScene.h"
#include "CPlayer.h"

class CStage : public CScene
{
protected:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag) { return S_OK; }

private:
	HRESULT			Ready_Prototype();
	HRESULT Ready_Camera();
	HRESULT Ready_Texture();
private:
	CPlayer* m_pPlayer = nullptr;
public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free();

};


#pragma once
#include "CEnemy.h"

class CWhiteSuit : public CEnemy
{
public:
	explicit CWhiteSuit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWhiteSuit();
public:
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;
protected:
	HRESULT Add_Component() override;
public:
	static CWhiteSuit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual void Free();
};
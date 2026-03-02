#pragma once
#include "CGameObject.h"
#include "CProtoMgr.h"

//개별 enemy가 상속할 추상 클래스

class CEnemy : public CGameObject
{
public:
	explicit CEnemy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnemy();
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
protected:
	virtual HRESULT Add_Component() PURE;
public:
	bool IsDead() { return m_bDead; }
	void SetPlayerTrans(CTransform* pPlayerTrans) { m_pPlayerTrans = pPlayerTrans; }
protected:
	CTransform* m_pPlayerTrans = nullptr;
	CRcTex* m_pRcTexture = nullptr;
	CTransform* m_pTransform = nullptr;
	CCollider* m_pCollider = nullptr;
	CTexture* m_pTexture = nullptr;
	float m_fCurFrame = 0.f;
	float m_fLastFrame = 0.f;
	float m_fAnimSpeed = 0.f;
	float m_fSpeed = 0.f;
	bool m_bDead = false;
protected:
	virtual void Free();
};
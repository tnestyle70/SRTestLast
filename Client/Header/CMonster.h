#pragma once
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"

class CMonster : public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	HRESULT Add_Component();
public:
	CTransform* Get_Transform() { return m_pTransformComponent; }
	void Set_Player(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
private:
	void FireBullet(const _vec3& vDir);
private:
	_float m_fBulletInterval = 2.f;
	_float m_fBulletTimer = 0.f;
private:
	Engine::CTriCol* m_pTriBufferComponent = nullptr;
	Engine::CTransform* m_pTransformComponent = nullptr; //이동과 변환을 위한 컴포넌트
	CCollider* m_pColliderCom = nullptr;
	CGameObject* m_pPlayer = nullptr;
	_int m_iHP = 1;
private:
	float m_fSpeed = 5.f;
private:
	int m_iFrameCounter = 0;
	int m_iMaxFrame = 60;
public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};
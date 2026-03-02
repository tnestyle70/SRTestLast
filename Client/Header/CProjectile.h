#pragma once
#include "CGameObject.h"
#include "CProtoMgr.h"

class CProjectile : public CGameObject
{
public:
	explicit CProjectile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CProjectile(const CProjectile& rhs);
	virtual ~CProjectile();
public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject();
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
private:
	HRESULT Add_Component();
protected:
	virtual HRESULT Ready_Component() { return S_OK; };
	virtual void Fire(const _vec3& vPos, const _vec3& vDir);
protected: //projectile을 상속 받을 클래스들의 공통적인 요소
	_vec3 m_vDir = { 0.f, 0.f, 0.f };
	_float m_fSpeed = 0.f;
	_float m_fDamage = 1.f;
	bool m_bDead = false;
	CTransform* m_pTrans = nullptr;
	CCollider* m_pCollider = nullptr;
	CTexture* m_pTexture = nullptr;
	CVIBuffer* m_pBufferCom = nullptr;
private:
	virtual void Free();
};
#pragma once
#include "CGameObject.h"
#include "CTransform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();
public:
	HRESULT Ready_Collider();
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component();
	void DebugRender();
public: //충돌 여부 판단
	bool CheckCollision(CCollider* pOther);
	void SetSize(const _vec3& vSize) { m_vSize = vSize; }
	void SetOffset(const _vec3& vOffset) { m_vOffset = vOffset; }
	void SetOwner(CGameObject* pOwner) { m_pOwner = pOwner; }
	_vec3& const GetSize() { return m_vSize; }
private:
	_vec3 GetWorldCenter();
private: //Collider를 사용할 owner
	CGameObject* m_pOwner;
private:
	_vec3 m_vSize; //박스 크기 
	_vec3 m_vOffset = {0.f, 0.f, 0.f};
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();
private:
	virtual void Free();
};

END	
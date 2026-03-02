#pragma once
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"

class CWall : public CGameObject
{
private:
	explicit CWall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWall(const CWall& rhs);
	virtual ~CWall();
public: //컴포넌트를 추가
	virtual HRESULT Ready_GameObject(const _tchar* pProtoTexKey, _float fUScale, _float fVScale);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	//Engine::CRcCol* m_pRectCom = nullptr;
	Engine::CRcTex* m_pRectCom = nullptr;
	Engine::CTransform* m_pTransCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	Engine::CCollider* m_pCollider = nullptr;
private:
	HRESULT Add_Component(const _tchar* pProtoTexKey, _float fUScale, _float fVScale);
public:
	static CWall* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pProtoTexKey,
		_vec3 vPos, _vec3 vScale, _vec3 vAngle,
		_float fUScale = 1.f, _float fVScale = 1.f);
private:
	virtual void Free();
};
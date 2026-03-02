#pragma once
//management > scene > layer > gameobject > component에서 gameobject이다.
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CCollisionMgr.h"
#include "CCameraMgr.h"

class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();
public: //컴포넌트를 추가
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	Engine::CTransform* Get_Transform() { return m_pTransCom; }
private:
	//로컬 좌표는 이미 삼각형에서 만들어놨기 때문에 만들지 않아도 상관 없음
	float m_fSpeed = 10.f;
private:
	int m_iFrameCounter = 0;
	int m_iMaxFrame = 60;
private:
	HRESULT Add_Component();
private:
	Engine::CRcCol* m_pRectBufferComponent = nullptr;
	Engine::CTransform* m_pTransCom = nullptr;
	Engine::CCollider* m_pCollider = nullptr;
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};